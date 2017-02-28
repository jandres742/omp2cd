#include "extbuild.output.darts.h"
using namespace darts;
using namespace std;
/*Function: ReadWholeInputData, ID: 24*/
int32 ReadWholeInputData(ADC_VIEW_CNTL* avp, FILE* inpf)
{
    /*ReadWholeInputData:24*/
    /*CompoundStmt:245*/
    uint32 iRec = 0;
    uint32 inpBufferLineSize, inpBufferPace, inpRecSize, ib = 0;
    fseek(inpf, 0L, 0);
    inpRecSize = 8 * avp->nm + 4 * avp->nTopDims;
    inpBufferLineSize = inpRecSize;
    if (inpBufferLineSize % 8)
        inpBufferLineSize += 4;
    inpBufferPace = inpBufferLineSize / 4;
    while (fread(&avp->inpDataBuffer[ib], inpRecSize, 1, inpf)) {
        iRec++;
        ib += inpBufferPace;
    }
    avp->nRowsToRead = iRec;
    fseek(inpf, 0L, 0);
    if (avp->nInputRecs != iRec) {
        fprintf(stderr, " ReadWholeInputData(): wrong input data reading.\n");
        return 2;
    }
    return 0;
}
/*Function: ComputeMemoryFittedView, ID: 25*/
int32 ComputeMemoryFittedView(ADC_VIEW_CNTL* avp)
{
    /*ComputeMemoryFittedView:25*/
    /*CompoundStmt:279*/
    uint32 iRec = 0;
    uint32 viewBuf[28];
    uint32 inpBufferLineSize, inpBufferPace, inpRecSize, ib;
    uint64 ordern = 0;
    fseek(avp->viewFile, 0L, 2);
    inpRecSize = 8 * avp->nm + 4 * avp->nTopDims;
    inpBufferLineSize = inpRecSize;
    if (inpBufferLineSize % 8)
        inpBufferLineSize += 4;
    inpBufferPace = inpBufferLineSize / 4;
    InitializeTree(avp->tree, avp->nv, avp->nm);
    ib = 0;
    for (iRec = 1; iRec <= avp->nRowsToRead; iRec++) {
        SelectToView(&avp->inpDataBuffer[ib], avp->selection, viewBuf, avp->nd, avp->nm, avp->nv);
        ib += inpBufferPace;
        TreeInsert(avp->tree, viewBuf);
        if (avp->tree->memoryIsFull) {
            fprintf(stderr, "ComputeMemoryFittedView(): Not enough memory.\n");
            return 1;
        }
    }
    computeChecksum(avp, avp->tree->root.left, &ordern);
    avp->nViewRows = avp->tree->count;
    avp->totalOfViewRows += avp->nViewRows;
    InitializeTree(avp->tree, avp->nv, avp->nm);
    return 0;
}
/*Function: SharedSortAggregate, ID: 26*/
int32 SharedSortAggregate(ADC_VIEW_CNTL* avp)
{
    /*SharedSortAggregate:26*/
    /*CompoundStmt:345*/
    int32 retCode;
    uint32 iRec = 0;
    uint32 attrs[28];
    uint32 currBuf[28];
    int64 chunkOffset = 0;
    int64 inpfOffset;
    uint32 nPart = 0;
    uint32 prevV;
    uint32 currV;
    uint32 total = 0;
    unsigned char* ib;
    uint32 ibsize = (1024 * 1024);
    uint32 nib;
    uint32 iib;
    uint32 nreg;
    uint32 nlst;
    uint32 nsgs;
    uint32 ncur;
    uint32 ibOffset = 0;
    uint64 ordern = 0;
    ib = (unsigned char*)malloc(ibsize);
    if (!ib) {
        fprintf(stderr, "SharedSortAggregate: memory allocation failed\n");
        return 5;
    }
    nib = ibsize / avp->inpRecSize;
    nsgs = avp->nRowsToRead / nib;
    if (nsgs == 0) {
        nreg = avp->nRowsToRead;
        nlst = nreg;
        nsgs = 1;
    } else {
        nreg = nib;
        if (avp->nRowsToRead % nib) {
            nsgs++;
            nlst = avp->nRowsToRead % nib;
        } else {
            nlst = nreg;
        }
    }
    avp->nViewRows = 0;
    for (iib = 1; iib <= nsgs; iib++) {
        if (iib > 1)
            fseek(avp->viewFile, inpfOffset, 0);
        ;
        if (iib == nsgs)
            ncur = nlst;
        else
            ncur = nreg;
        fread(ib, ncur * avp->inpRecSize, 1, avp->viewFile);
        inpfOffset = ftell(avp->viewFile);
        for (ibOffset = 0, iRec = 1; iRec <= ncur; iRec++) {
            memcpy(attrs, &ib[ibOffset], avp->inpRecSize);
            ibOffset += avp->inpRecSize;
            SelectToView(attrs, avp->selection, currBuf, avp->nd, avp->nm, avp->nv);
            currV = currBuf[2 * avp->nm];
            if (iib == 1 && iRec == 1) {
                prevV = currV;
                nPart = 1;
                InitializeTree(avp->tree, avp->nv, avp->nm);
                TreeInsert(avp->tree, currBuf);
            } else {
                if (currV == prevV) {
                    nPart++;
                    TreeInsert(avp->tree, currBuf);
                    if (avp->tree->memoryIsFull) {
                        avp->chunksParams[avp->numberOfChunks].curChunkNum = avp->tree->count;
                        avp->chunksParams[avp->numberOfChunks].chunkOffset = chunkOffset;
                        (avp->numberOfChunks)++;
                        if (avp->numberOfChunks >= 1024) {
                            fprintf(stderr, "Too many chunks were created.\n");
                            exit(1);
                        }
                        chunkOffset += (uint64)(avp->tree->count * avp->outRecSize);
                        retCode = WriteChunkToDisk(
                            avp->outRecSize, avp->fileOfChunks, avp->tree->root.left, avp->logf);
                        if (retCode != 0) {
                            fprintf(stderr, "SharedSortAggregate: Write error occured.\n");
                            return retCode;
                        }
                        InitializeTree(avp->tree, avp->nv, avp->nm);
                    }
                } else {
                    if (avp->numberOfChunks && avp->tree->count != 0) {
                        avp->chunksParams[avp->numberOfChunks].curChunkNum = avp->tree->count;
                        avp->chunksParams[avp->numberOfChunks].chunkOffset = chunkOffset;
                        (avp->numberOfChunks)++;
                        chunkOffset += (uint64)(avp->tree->count * (4 * avp->nv + 8 * avp->nm));
                        retCode = WriteChunkToDisk(
                            avp->outRecSize, avp->fileOfChunks, avp->tree->root.left, avp->logf);
                        if (retCode != 0) {
                            fprintf(stderr, "SharedSortAggregate: Write error occured.\n");
                            return retCode;
                        }
                    }
                    fseek(avp->viewFile, 0L, 2);
                    ;
                    if (!avp->numberOfChunks) {
                        avp->nViewRows += avp->tree->count;
                        retCode = WriteViewToDiskCS(avp, avp->tree->root.left, &ordern);
                        if (retCode != 0) {
                            fprintf(stderr, "SharedSortAggregate: Write error occured.\n");
                            return retCode;
                        }
                    } else {
                        retCode = MultiWayMerge(avp);
                        if (retCode != 0) {
                            fprintf(stderr, "SharedSortAggregate.MultiWayMerge: failed.\n");
                            return retCode;
                        }
                    }
                    InitializeTree(avp->tree, avp->nv, avp->nm);
                    TreeInsert(avp->tree, currBuf);
                    total += nPart;
                    nPart = 1;
                }
            }
            prevV = currV;
        }
    }
    if (avp->numberOfChunks && avp->tree->count != 0) {
        avp->chunksParams[avp->numberOfChunks].curChunkNum = avp->tree->count;
        avp->chunksParams[avp->numberOfChunks].chunkOffset = chunkOffset;
        (avp->numberOfChunks)++;
        chunkOffset += (uint64)(avp->tree->count * (4 * avp->nv + 8 * avp->nm));
        retCode
            = WriteChunkToDisk(avp->outRecSize, avp->fileOfChunks, avp->tree->root.left, avp->logf);
        if (retCode != 0) {
            fprintf(stderr, "SharedSortAggregate: Write error occured.\n");
            return retCode;
        }
    }
    fseek(avp->viewFile, 0L, 2);
    if (!avp->numberOfChunks) {
        avp->nViewRows += avp->tree->count;
        if ((retCode = WriteViewToDiskCS(avp, avp->tree->root.left, &ordern))) {
            fprintf(stderr, "SharedSortAggregate: Write error occured.\n");
            return retCode;
        }
    } else {
        retCode = MultiWayMerge(avp);
        if (retCode != 0) {
            fprintf(stderr, "SharedSortAggregate.MultiWayMerge failed.\n");
            return retCode;
        }
    }
    fseek(avp->fileOfChunks, 0L, 0);
    total += nPart;
    avp->totalOfViewRows += avp->nViewRows;
    if (ib)
        free(ib);
    return 0;
}
/*Function: PrefixedAggregate, ID: 27*/
int32 PrefixedAggregate(ADC_VIEW_CNTL* avp, FILE* iof)
{
    /*PrefixedAggregate:27*/
    /*CompoundStmt:664*/
    uint32 i;
    uint32 iRec = 0;
    uint32 attrs[28];
    uint32 aggrBuf[28];
    uint32 currBuf[28];
    uint32 prevBuf[28];
    int64* aggrmp;
    int64* currmp;
    int32 compRes;
    uint32 nOut = 0;
    uint32 mpOffset = 0;
    uint32 nOutBufRecs;
    uint32 nViewRows = 0;
    int64 inpfOffset;
    aggrmp = (int64*)&aggrBuf[0];
    currmp = (int64*)&currBuf[0];
    for (i = 0; i < 2 * avp->nm + avp->nv; i++) {
        prevBuf[i] = 0;
        aggrBuf[i] = 0;
    }
    nOutBufRecs = avp->memoryLimit / avp->outRecSize;
    for (iRec = 1; iRec <= avp->nRowsToRead; iRec++) {
        fread(attrs, avp->inpRecSize, 1, iof);
        SelectToView(attrs, avp->selection, currBuf, avp->nd, avp->nm, avp->nv);
        if (iRec == 1)
            memcpy(aggrBuf, currBuf, avp->outRecSize);
        else {
            compRes = KeyComp(&currBuf[2 * avp->nm], &prevBuf[2 * avp->nm], avp->nv);
            switch (compRes) {
            case 1:
                memcpy(&avp->memPool[mpOffset], aggrBuf, avp->outRecSize);
                mpOffset += avp->outRecSize;
                nOut++;
                for (i = 0; i < avp->nm; i++) {
                    avp->mSums[i] += aggrmp[i];
                    avp->checksums[i] += nOut * aggrmp[i] % measbound;
                }
                memcpy(aggrBuf, currBuf, avp->outRecSize);
                break;
            case 0:
                for (i = 0; i < avp->nm; i++)
                    aggrmp[i] += currmp[i];
                break;
            case -1:
                fprintf(stderr, "PrefixedAggregate: wrong parent view order.\n");
                exit(1);
                break;
            default:
                fprintf(stderr, "PrefixedAggregate: wrong KeyComp() result.\n");
                exit(1);
                break;
            }
            if (nOut == nOutBufRecs) {
                inpfOffset = ftell(iof);
                fseek(iof, 0L, 2);
                ;
                if (fwrite(avp->memPool, nOut * avp->outRecSize, 1, iof) != 1) {
                    fprintf(stderr, "\n Write error from WriteToFile()\n");
                    return 1;
                };
                fseek(iof, inpfOffset, 0);
                ;
                mpOffset = 0;
                nViewRows += nOut;
                nOut = 0;
            }
        }
        memcpy(prevBuf, currBuf, avp->outRecSize);
    }
    memcpy(&avp->memPool[mpOffset], aggrBuf, avp->outRecSize);
    nOut++;
    for (i = 0; i < avp->nm; i++) {
        avp->mSums[i] += aggrmp[i];
        avp->checksums[i] += nOut * aggrmp[i] % measbound;
    }
    fseek(iof, 0L, 2);
    if (fwrite(avp->memPool, nOut * avp->outRecSize, 1, iof) != 1) {
        fprintf(stderr, "\n Write error from WriteToFile()\n");
        return 1;
    }
    avp->nViewRows = nViewRows + nOut;
    avp->totalOfViewRows += avp->nViewRows;
    return 0;
}
/*Function: RunFormation, ID: 28*/
int32 RunFormation(ADC_VIEW_CNTL* avp, FILE* inpf)
{
    /*RunFormation:28*/
    /*CompoundStmt:843*/
    uint32 iRec = 0;
    uint32 viewBuf[28];
    uint32 attrs[28];
    int64 chunkOffset = 0;
    InitializeTree(avp->tree, avp->nv, avp->nm);
    for (iRec = 1; iRec <= avp->nRowsToRead; iRec++) {
        fread(attrs, avp->inpRecSize, 1, inpf);
        SelectToView(attrs, avp->selection, viewBuf, avp->nd, avp->nm, avp->nv);
        TreeInsert(avp->tree, viewBuf);
        if (avp->tree->memoryIsFull) {
            avp->chunksParams[avp->numberOfChunks].curChunkNum = avp->tree->count;
            avp->chunksParams[avp->numberOfChunks].chunkOffset = chunkOffset;
            (avp->numberOfChunks)++;
            if (avp->numberOfChunks >= 1024) {
                fprintf(stderr, "RunFormation: Too many chunks were created.\n");
                return 2;
            }
            chunkOffset += (uint64)(avp->tree->count * avp->outRecSize);
            if (WriteChunkToDisk(
                    avp->outRecSize, avp->fileOfChunks, avp->tree->root.left, avp->logf)) {
                fprintf(stderr, "RunFormation.WriteChunkToDisk: Write error is occured.\n");
                return 1;
            }
            InitializeTree(avp->tree, avp->nv, avp->nm);
        }
    }
    if (avp->numberOfChunks && avp->tree->count != 0) {
        avp->chunksParams[avp->numberOfChunks].curChunkNum = avp->tree->count;
        avp->chunksParams[avp->numberOfChunks].chunkOffset = chunkOffset;
        (avp->numberOfChunks)++;
        chunkOffset += (uint64)(avp->tree->count * (4 * avp->nv + 8 * avp->nm));
        if (WriteChunkToDisk(avp->outRecSize, avp->fileOfChunks, avp->tree->root.left, avp->logf)) {
            fprintf(stderr, "RunFormation(.WriteChunkToDisk: Write error is occured.\n");
            return 1;
        }
    }
    fseek(avp->viewFile, 0L, 2);
    return 0;
}
/*Function: SeekAndReadNextSubChunk, ID: 29*/
void SeekAndReadNextSubChunk(uint32 multiChunkBuffer[], uint32 k, FILE* inFile, uint32 chunkRecSize,
    uint64 inFileOffs, uint32 subChunkNum)
{
    /*SeekAndReadNextSubChunk:29*/
    /*CompoundStmt:943*/
    int64 ret;
    ret = fseek(inFile, inFileOffs, 0);
    if (ret < 0) {
        fprintf(stderr, "SeekAndReadNextSubChunk.fseek() < 0 ");
        exit(1);
    }
    fread(&multiChunkBuffer[k], chunkRecSize * subChunkNum, 1, inFile);
}
/*Function: ReadSubChunk, ID: 30*/
void ReadSubChunk(uint32 chunkRecSize, uint32* multiChunkBuffer, uint32 mwBufRecSizeInInt,
    uint32 iChunk, uint32 regSubChunkSize, CHUNKS* chunks, FILE* fileOfChunks)
{
    /*ReadSubChunk:30*/
    /*CompoundStmt:956*/
    if (chunks[iChunk].curChunkNum > 0) {
        if (chunks[iChunk].curChunkNum < regSubChunkSize) {
            SeekAndReadNextSubChunk(multiChunkBuffer,
                (iChunk * regSubChunkSize + (regSubChunkSize - chunks[iChunk].curChunkNum))
                    * mwBufRecSizeInInt,
                fileOfChunks, chunkRecSize, chunks[iChunk].chunkOffset, chunks[iChunk].curChunkNum);
            chunks[iChunk].posSubChunk = regSubChunkSize - chunks[iChunk].curChunkNum;
            chunks[iChunk].curSubChunk = chunks[iChunk].curChunkNum;
            chunks[iChunk].curChunkNum = 0;
            chunks[iChunk].chunkOffset = -1;
        } else {
            SeekAndReadNextSubChunk(multiChunkBuffer, iChunk * regSubChunkSize * mwBufRecSizeInInt,
                fileOfChunks, chunkRecSize, chunks[iChunk].chunkOffset, regSubChunkSize);
            chunks[iChunk].posSubChunk = 0;
            chunks[iChunk].curSubChunk = regSubChunkSize;
            chunks[iChunk].curChunkNum -= regSubChunkSize;
            chunks[iChunk].chunkOffset += regSubChunkSize * chunkRecSize;
        }
    }
}
/*Function: MultiWayMerge, ID: 31*/
int32 MultiWayMerge(ADC_VIEW_CNTL* avp)
{
    /*MultiWayMerge:31*/
    /*CompoundStmt:1011*/
    uint32 outputBuffer[28];
    uint32 r_buf[28];
    uint32 min_r_buf[28];
    uint32 first_one;
    uint32 i;
    uint32 iChunk;
    uint32 min_r_chunk;
    uint32 sPos;
    uint32 iPos;
    uint32 numEmptyBufs;
    uint32 numEmptyRuns;
    uint32 mwBufRecSizeInInt;
    uint32 chunkRecSize;
    uint32* multiChunkBuffer;
    uint32 regSubChunkSize;
    int32 compRes;
    int64* m_min_r_buf;
    int64* m_outputBuffer;
    fseek(avp->fileOfChunks, 0L, 0);
    multiChunkBuffer = (uint32*)&avp->memPool[0];
    first_one = 1;
    avp->nViewRows = 0;
    chunkRecSize = avp->outRecSize;
    mwBufRecSizeInInt = chunkRecSize / 4;
    m_min_r_buf = (int64*)&min_r_buf[0];
    m_outputBuffer = (int64*)&outputBuffer[0];
    mwBufRecSizeInInt = chunkRecSize / 4;
    regSubChunkSize = (avp->memoryLimit / avp->numberOfChunks) / chunkRecSize;
    if (regSubChunkSize == 0) {
        fprintf(stderr, "MultiWayMerge: Not enough memory to run the external sort\n");
        return 2;
    }
    multiChunkBuffer = (uint32*)&avp->memPool[0];
    for (i = 0; i < avp->numberOfChunks; i++) {
        ReadSubChunk(chunkRecSize, multiChunkBuffer, mwBufRecSizeInInt, i, regSubChunkSize,
            avp->chunksParams, avp->fileOfChunks);
    }
    while (1) {
        for (iChunk = 0; iChunk < avp->numberOfChunks; iChunk++) {
            if (avp->chunksParams[iChunk].curSubChunk > 0) {
                sPos = iChunk * regSubChunkSize * mwBufRecSizeInInt;
                iPos = sPos + mwBufRecSizeInInt * avp->chunksParams[iChunk].posSubChunk;
                memcpy(&min_r_buf[0], &multiChunkBuffer[iPos], avp->outRecSize);
                min_r_chunk = iChunk;
                break;
            }
        }
        for (iChunk = min_r_chunk; iChunk < avp->numberOfChunks; iChunk++) {
            uint32 iPos;
            if (avp->chunksParams[iChunk].curSubChunk > 0) {
                iPos = mwBufRecSizeInInt
                    * (iChunk * regSubChunkSize + avp->chunksParams[iChunk].posSubChunk);
                memcpy(&r_buf[0], &multiChunkBuffer[iPos], avp->outRecSize);
                compRes = KeyComp(&r_buf[2 * avp->nm], &min_r_buf[2 * avp->nm], avp->nv);
                if (compRes < 0) {
                    memcpy(&min_r_buf[0], &r_buf[0], avp->outRecSize);
                    min_r_chunk = iChunk;
                }
            }
        }
        if (avp->chunksParams[min_r_chunk].curSubChunk != 0) {
            avp->chunksParams[min_r_chunk].curSubChunk--;
            avp->chunksParams[min_r_chunk].posSubChunk++;
        }
        if (first_one) {
            memcpy(&outputBuffer[0], &min_r_buf[0], avp->outRecSize);
            first_one = 0;
        } else {
            compRes = KeyComp(&outputBuffer[2 * avp->nm], &min_r_buf[2 * avp->nm], avp->nv);
            if (!compRes) {
                for (i = 0; i < avp->nm; i++) {
                    m_outputBuffer[i] += m_min_r_buf[i];
                }
            } else {
                if (fwrite(outputBuffer, avp->outRecSize, 1, avp->viewFile) != 1) {
                    fprintf(stderr, "\n Write error from WriteToFile()\n");
                    return 1;
                };
                avp->nViewRows++;
                for (i = 0; i < avp->nm; i++) {
                    avp->mSums[i] += m_outputBuffer[i];
                    avp->checksums[i] += avp->nViewRows * m_outputBuffer[i] % measbound;
                }
                memcpy(&outputBuffer[0], &min_r_buf[0], avp->outRecSize);
            }
        }
        for (numEmptyBufs = 0, numEmptyRuns = 0, i = 0; i < avp->numberOfChunks; i++) {
            if (avp->chunksParams[i].curSubChunk == 0)
                numEmptyBufs++;
            if (avp->chunksParams[i].curChunkNum == 0)
                numEmptyRuns++;
        }
        if (numEmptyBufs == avp->numberOfChunks && numEmptyRuns == avp->numberOfChunks)
            break;
        if (avp->chunksParams[min_r_chunk].curSubChunk == 0) {
            ReadSubChunk(chunkRecSize, multiChunkBuffer, mwBufRecSizeInInt, min_r_chunk,
                regSubChunkSize, avp->chunksParams, avp->fileOfChunks);
        }
    }
    if (fwrite(outputBuffer, avp->outRecSize, 1, avp->viewFile) != 1) {
        fprintf(stderr, "\n Write error from WriteToFile()\n");
        return 1;
    }
    avp->nViewRows++;
    for (i = 0; i < avp->nm; i++) {
        avp->mSums[i] += m_outputBuffer[i];
        avp->checksums[i] += avp->nViewRows * m_outputBuffer[i] % measbound;
    }
    avp->totalOfViewRows += avp->nViewRows;
    return 0;
}
/*Function: SelectToView, ID: 32*/
void SelectToView(uint32* ib, uint32* ix, uint32* viewBuf, uint32 nd, uint32 nm, uint32 nv)
{
    /*SelectToView:32*/
    /*CompoundStmt:1292*/
    uint32 i, j;
    for (j = 0, i = 0; i < nv; i++)
        viewBuf[2 * nm + j++] = ib[2 * nm + ix[i] - 1];
    memcpy(&viewBuf[0], &ib[0], 8 * nm);
}
/*Function: AdcFileOpen, ID: 33*/
FILE* AdcFileOpen(const char* fileName, const char* mode)
{
    /*AdcFileOpen:33*/
    /*CompoundStmt:1316*/
    FILE* fr;
    if ((fr = (FILE*)fopen(fileName, mode)) == ((void*)0))
        fprintf(stderr, "AdcFileOpen: Cannot open the file %s errno = %d\n", fileName,
            (*__errno_location()));
    return fr;
}
/*Function: AdcFileName, ID: 34*/
void AdcFileName(char* adcFileName, const char* adcName, const char* fileName, uint32 taskNumber)
{
    /*AdcFileName:34*/
    /*CompoundStmt:1325*/
    sprintf(adcFileName, "%s.%s.%d", adcName, fileName, taskNumber);
}
/*Function: NewAdcViewCntl, ID: 35*/
ADC_VIEW_CNTL* NewAdcViewCntl(ADC_VIEW_PARS* adcpp, uint32 pnum)
{
    /*NewAdcViewCntl:35*/
    /*CompoundStmt:1327*/
    ADC_VIEW_CNTL* adccntl;
    uint32 i, j, k;
    char id[9];
    adccntl = (ADC_VIEW_CNTL*)malloc(sizeof(ADC_VIEW_CNTL));
    if (adccntl == ((void*)0))
        return ((void*)0);
    adccntl->ndid = adcpp->ndid;
    adccntl->taskNumber = pnum;
    adccntl->retCode = 0;
    adccntl->swapIt = 0;
    strcpy(adccntl->adcName, adcpp->adcName);
    adccntl->nTopDims = adcpp->nd;
    adccntl->nd = adcpp->nd;
    adccntl->nm = adcpp->nm;
    adccntl->nInputRecs = adcpp->nInputRecs;
    adccntl->inpRecSize = (4 * adccntl->nd + 8 * adccntl->nm);
    adccntl->outRecSize = (4 * adccntl->nv + 8 * adccntl->nm);
    adccntl->accViewFileOffset = 0;
    adccntl->totalViewFileSize = 0;
    adccntl->numberOfMadeViews = 0;
    adccntl->numberOfViewsMadeFromInput = 0;
    adccntl->numberOfPrefixedGroupbys = 0;
    adccntl->numberOfSharedSortGroupbys = 0;
    adccntl->totalOfViewRows = 0;
    adccntl->memoryLimit = adcpp->memoryLimit;
    adccntl->nTasks = adcpp->nTasks;
    strcpy(adccntl->inpFileName, adcpp->adcInpFileName);
    sprintf(id, ".%d", adcpp->ndid);
    AdcFileName(adccntl->adcLogFileName, adccntl->adcName, "logf", adccntl->taskNumber);
    strcat(adccntl->adcLogFileName, id);
    adccntl->logf = AdcFileOpen(adccntl->adcLogFileName, "w");
    AdcFileName(adccntl->inpFileName, adccntl->adcName, "dat", adcpp->ndid);
    adccntl->inpf = AdcFileOpen(adccntl->inpFileName, "rb");
    if (!adccntl->inpf) {
        adccntl->retCode = 4;
        return (adccntl);
    }
    AdcFileName(adccntl->viewFileName, adccntl->adcName, "view.dat", adccntl->taskNumber);
    strcat(adccntl->viewFileName, id);
    adccntl->viewFile = AdcFileOpen(adccntl->viewFileName, "wb+");
    AdcFileName(adccntl->chunksFileName, adccntl->adcName, "chunks.dat", adccntl->taskNumber);
    strcat(adccntl->chunksFileName, id);
    adccntl->fileOfChunks = AdcFileOpen(adccntl->chunksFileName, "wb+");
    AdcFileName(adccntl->groupbyFileName, adccntl->adcName, "groupby.dat", adccntl->taskNumber);
    strcat(adccntl->groupbyFileName, id);
    adccntl->groupbyFile = AdcFileOpen(adccntl->groupbyFileName, "wb+");
    AdcFileName(adccntl->adcViewSizesFileName, adccntl->adcName, "view.sz", adcpp->ndid);
    adccntl->adcViewSizesFile = AdcFileOpen(adccntl->adcViewSizesFileName, "r");
    if (!adccntl->adcViewSizesFile) {
        adccntl->retCode = 4;
        return (adccntl);
    }
    AdcFileName(adccntl->viewSizesFileName, adccntl->adcName, "viewsz.dat", adccntl->taskNumber);
    strcat(adccntl->viewSizesFileName, id);
    adccntl->viewSizesFile = AdcFileOpen(adccntl->viewSizesFileName, "wb+");
    adccntl->chunksParams = (CHUNKS*)malloc(1024 * sizeof(CHUNKS));
    if (adccntl->chunksParams == ((void*)0)) {
        fprintf(adccntl->logf, "NewAdcViewCntl: Cannot allocate 'chunksParsms'\n");
        adccntl->retCode = 5;
        return (adccntl);
    }
    adccntl->memPool = (unsigned char*)malloc(adccntl->memoryLimit);
    if (adccntl->memPool == ((void*)0)) {
        fprintf(adccntl->logf, "NewAdcViewCntl: Cannot allocate 'main memory pool'\n");
        adccntl->retCode = 5;
        return (adccntl);
    }
    adccntl->numberOfChunks = 0;
    for (i = 0; i < adccntl->nm; i++) {
        adccntl->mSums[i] = 0;
        adccntl->checksums[i] = 0;
        adccntl->totchs[i] = 0;
    }
    adccntl->tree
        = CreateEmptyTree(adccntl->nd, adccntl->nm, adccntl->memoryLimit, adccntl->memPool);
    if (!adccntl->tree) {
        fprintf(adccntl->logf, "\nNewAdcViewCntl.CreateEmptyTree failed.\n");
        adccntl->retCode = 5;
        return (adccntl);
    }
    adccntl->nv = adcpp->nd;
    for (i = 0; i < adccntl->nv; i++)
        adccntl->selection[i] = i + 1;
    adccntl->nViewLimit = (1 << adcpp->nd) - 1;
    adccntl->jpp = (JOB_POOL*)malloc((adccntl->nViewLimit + 1) * sizeof(JOB_POOL));
    if (adccntl->jpp == ((void*)0)) {
        fprintf(adccntl->logf, "\n Not enough space to allocate %ld byte for a job pool.",
            (long)(adccntl->nViewLimit + 1) * sizeof(JOB_POOL));
        adccntl->retCode = 5;
        return (adccntl);
    }
    adccntl->lpp = (LAYER*)malloc((adcpp->nd + 1) * sizeof(LAYER));
    if (adccntl->lpp == ((void*)0)) {
        fprintf(adccntl->logf,
            "\n Not enough space to allocate %ld byte for a layer reference array.",
            (long)(adcpp->nd + 1) * sizeof(LAYER));
        adccntl->retCode = 5;
        return (adccntl);
    }
    for (j = 1, i = 1; i <= adcpp->nd; i++) {
        k = NumOfCombsFromNbyK(adcpp->nd, i);
        adccntl->lpp[i].layerIndex = j;
        j += k;
        adccntl->lpp[i].layerQuantityLimit = k;
        adccntl->lpp[i].layerCurrentPopulation = 0;
    }
    JobPoolInit(adccntl->jpp, (adccntl->nViewLimit + 1), adcpp->nd);
    fprintf(adccntl->logf, "\nMeaning of the log file colums is as follows:\n");
    fprintf(adccntl->logf, "Row Number | Groupby | View Size | Measure Sums | Number of Chunks\n");
    adccntl->verificationFailed = 1;
    return adccntl;
}
/*Function: InitAdcViewCntl, ID: 36*/
void InitAdcViewCntl(
    ADC_VIEW_CNTL* adccntl, uint32 nSelectedDims, uint32* selection, uint32 fromParent)
{
    /*InitAdcViewCntl:36*/
    /*CompoundStmt:1636*/
    uint32 i;
    adccntl->nv = nSelectedDims;
    for (i = 0; i < adccntl->nm; i++)
        adccntl->mSums[i] = 0;
    for (i = 0; i < adccntl->nv; i++)
        adccntl->selection[i] = selection[i];
    adccntl->outRecSize = (4 * adccntl->nv + 8 * adccntl->nm);
    adccntl->numberOfChunks = 0;
    adccntl->fromParent = fromParent;
    adccntl->nViewRows = 0;
    if (fromParent) {
        adccntl->nd = adccntl->smallestParentLevel;
        fseek(adccntl->viewFile, adccntl->viewOffset, 0);
        ;
        adccntl->nRowsToRead = adccntl->nParentViewRows;
    } else {
        adccntl->nd = adccntl->nTopDims;
        adccntl->nRowsToRead = adccntl->nInputRecs;
    }
    adccntl->inpRecSize = (4 * adccntl->nd + 8 * adccntl->nm);
    adccntl->outRecSize = (4 * adccntl->nv + 8 * adccntl->nm);
}
/*Function: CloseAdcView, ID: 37*/
int32 CloseAdcView(ADC_VIEW_CNTL* adccntl)
{
    /*CloseAdcView:37*/
    /*CompoundStmt:1703*/
    if (adccntl->inpf)
        fclose(adccntl->inpf);
    if (adccntl->viewFile)
        fclose(adccntl->viewFile);
    if (adccntl->fileOfChunks)
        fclose(adccntl->fileOfChunks);
    if (adccntl->groupbyFile)
        fclose(adccntl->groupbyFile);
    if (adccntl->adcViewSizesFile)
        fclose(adccntl->adcViewSizesFile);
    if (adccntl->viewSizesFile)
        fclose(adccntl->viewSizesFile);
    if (DeleteOneFile(adccntl->chunksFileName))
        return 6;
    if (DeleteOneFile(adccntl->viewSizesFileName))
        return 6;
    if (DeleteOneFile(adccntl->groupbyFileName))
        return 6;
    if (adccntl->chunksParams) {
        free(adccntl->chunksParams);
        adccntl->chunksParams = ((void*)0);
    }
    if (adccntl->memPool) {
        free(adccntl->memPool);
        adccntl->memPool = ((void*)0);
    }
    if (adccntl->jpp) {
        free(adccntl->jpp);
        adccntl->jpp = ((void*)0);
    }
    if (adccntl->lpp) {
        free(adccntl->lpp);
        adccntl->lpp = ((void*)0);
    }
    if (adccntl->logf)
        fclose(adccntl->logf);
    free(adccntl);
    return 0;
}
/*Function: AdcCntlLog, ID: 38*/
void AdcCntlLog(ADC_VIEW_CNTL* adccntlp)
{
    /*AdcCntlLog:38*/
    /*CompoundStmt:1785*/
    fprintf(adccntlp->logf, "    memoryLimit = %20d\n", adccntlp->memoryLimit);
    fprintf(adccntlp->logf, "    treeNodeSize = %20d\n", adccntlp->tree->treeNodeSize);
    fprintf(adccntlp->logf, " treeMemoryLimit = %20d\n", adccntlp->tree->memoryLimit);
    fprintf(adccntlp->logf, "    nNodesLimit = %20d\n", adccntlp->tree->nNodesLimit);
    fprintf(adccntlp->logf, "freeNodeCounter = %20d\n", adccntlp->tree->freeNodeCounter);
    fprintf(adccntlp->logf, "\tnViewRows = %20d\n", adccntlp->nViewRows);
}
/*Function: ViewSizesVerification, ID: 39*/
int32 ViewSizesVerification(ADC_VIEW_CNTL* adccntlp)
{
    /*ViewSizesVerification:39*/
    /*CompoundStmt:1808*/
    char inps[1024];
    char msg[64];
    uint32* viewCounts;
    uint32 selection_viewSize[2];
    uint32 sz;
    uint32 sel[64];
    uint32 i;
    uint32 k;
    uint64 tx;
    uint32 iTx;
    viewCounts = (uint32*)&adccntlp->memPool[0];
    for (i = 0; i <= adccntlp->nViewLimit; i++)
        viewCounts[i] = 0;
    fseek(adccntlp->viewSizesFile, 0L, 0);
    fseek(adccntlp->adcViewSizesFile, 0L, 0);
    while (fread(selection_viewSize, 8, 1, adccntlp->viewSizesFile)) {
        viewCounts[selection_viewSize[0]] = selection_viewSize[1];
    }
    k = 0;
    while (fscanf(adccntlp->adcViewSizesFile, "%s", inps) != (-1)) {
        if (strcmp(inps, "Selection:") == 0) {
            while (fscanf(adccntlp->adcViewSizesFile, "%s", inps)) {
                if (strcmp(inps, "View") == 0)
                    break;
                sel[k++] = atoi(inps);
            }
        }
        if (strcmp(inps, "Size:") == 0) {
            fscanf(adccntlp->adcViewSizesFile, "%s", inps);
            sz = atoi(inps);
            CreateBinTuple(&tx, sel, k);
            iTx = (int32)(tx >> (64 - adccntlp->nTopDims));
            adccntlp->verificationFailed = 0;
            if (!adccntlp->numberOfMadeViews)
                adccntlp->verificationFailed = 1;
            if (viewCounts[iTx] != 0) {
                if (viewCounts[iTx] != sz) {
                    if (viewCounts[iTx] != adccntlp->nInputRecs) {
                        fprintf(adccntlp->logf, "A view size is wrong: genSz=%d calcSz=%d\n", sz,
                            viewCounts[iTx]);
                        adccntlp->verificationFailed = 1;
                        return 7;
                    }
                }
            }
            k = 0;
        }
    }
    fprintf(adccntlp->logf, "\n\nMeaning of the log file colums is as follows:\n");
    fprintf(adccntlp->logf, "Row Number | Groupby | View Size | Measure Sums | Number of Chunks\n");
    if (!adccntlp->verificationFailed)
        strcpy(msg, "Verification=passed");
    else
        strcpy(msg, "Verification=failed");
    fseek(adccntlp->logf, 0L, 0);
    fprintf(adccntlp->logf, "%s", msg);
    fseek(adccntlp->logf, 0L, 2);
    fseek(adccntlp->viewSizesFile, 0L, 0);
    return 0;
}
/*Function: ComputeGivenGroupbys, ID: 40*/
int32 ComputeGivenGroupbys(ADC_VIEW_CNTL* adccntlp)
{
    /*ComputeGivenGroupbys:40*/
    /*CompoundStmt:1921*/
    int32 retCode;
    uint32 i;
    uint64 binRepTuple;
    uint32 ut32;
    uint32 nViews = 0;
    uint32 nSelectedDims;
    uint32 smp;
    uint32 selection_viewsize[2];
    char ttout[16];
    while (fread(&binRepTuple, 8, 1, adccntlp->groupbyFile)) {
        for (i = 0; i < adccntlp->nm; i++)
            adccntlp->checksums[i] = 0;
        nViews++;
        swap8(&binRepTuple);
        GetRegTupleFromBin64(binRepTuple, adccntlp->selection, adccntlp->nTopDims, &nSelectedDims);
        ut32 = (uint32)(binRepTuple >> (64 - adccntlp->nTopDims));
        selection_viewsize[0] = ut32;
        ut32 <<= (32 - adccntlp->nTopDims);
        adccntlp->groupby = ut32;
        smp = GetParent(adccntlp, ut32);
        if (smp != noneParent)
            GetRegTupleFromParent(
                binRepTuple, adccntlp->parBinRepTuple, adccntlp->selection, adccntlp->nTopDims);
        InitAdcViewCntl(adccntlp, nSelectedDims, adccntlp->selection, (smp == noneParent) ? 0 : 1);
        {
            if (smp != noneParent) {
                retCode = RunFormation(adccntlp, adccntlp->viewFile);
                if (retCode != 0) {
                    fprintf(stderr, "ComputrGivenGroupbys.RunFormation failed.\n");
                    return retCode;
                }
            } else {
                if ((retCode = RunFormation(adccntlp, adccntlp->inpf)) != 0) {
                    fprintf(stderr, "ComputrGivenGroupbys.RunFormation failed.\n");
                    return retCode;
                }
                adccntlp->numberOfViewsMadeFromInput++;
            }
            if (!adccntlp->numberOfChunks) {
                uint64 ordern = 0;
                adccntlp->nViewRows = adccntlp->tree->count;
                adccntlp->totalOfViewRows += adccntlp->nViewRows;
                retCode = WriteViewToDiskCS(adccntlp, adccntlp->tree->root.left, &ordern);
                if (retCode != 0) {
                    fprintf(stderr, "ComputeGivenGroupbys.WriteViewToDisk: Write error.\n");
                    return 1;
                }
            } else {
                retCode = MultiWayMerge(adccntlp);
                if (retCode != 0) {
                    fprintf(stderr, "ComputeGivenGroupbys.MultiWayMerge failed.\n");
                    return retCode;
                }
            }
        }
        JobPoolUpdate(adccntlp);
        adccntlp->accViewFileOffset += (int64)(adccntlp->nViewRows * adccntlp->outRecSize);
        fseek(adccntlp->fileOfChunks, 0L, 0);
        ;
        fseek(adccntlp->inpf, 0L, 0);
        ;
        for (i = 0; i < adccntlp->nm; i++)
            adccntlp->totchs[i] += adccntlp->checksums[i];
        selection_viewsize[1] = adccntlp->nViewRows;
        fwrite(selection_viewsize, 8, 1, adccntlp->viewSizesFile);
        adccntlp->totalViewFileSize += adccntlp->outRecSize * adccntlp->nViewRows;
        sprintf(ttout, "%7d ", nViews);
        WriteOne32Tuple(ttout, adccntlp->groupby, adccntlp->nTopDims, adccntlp->logf);
        fprintf(adccntlp->logf, " |  %15d | ", adccntlp->nViewRows);
        for (i = 0; i < adccntlp->nm; i++) {
            fprintf(adccntlp->logf, " %20lld", adccntlp->checksums[i]);
        }
        fprintf(adccntlp->logf, " | %5d", adccntlp->numberOfChunks);
    }
    adccntlp->numberOfMadeViews = nViews;
    if (ViewSizesVerification(adccntlp))
        return 7;
    return 0;
}
