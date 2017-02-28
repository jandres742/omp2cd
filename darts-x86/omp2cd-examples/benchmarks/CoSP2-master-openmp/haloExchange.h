/// \file
/// Communicate halo data such as "ghost" rows with neighboring tasks.

#ifndef __HALO_EXCHANGE_
#define __HALO_EXCHANGE_

#include "haloExchange.h"

#include <assert.h>

#include "sparseMatrix.h"
#include "decomposition.h"
#include "mytype.h"

typedef struct HaloExchangeSt
{
  int maxHalo;     //!< max number of processors for halo
  int bufferSize;  //!< max size in bytes for send/receive buffer
  int haloCount;   //!< number of processors to send/reeive

  int* haloProc;   //!< array of halo procs to send/receive
  int* rlist;      //!< array of request indeces for non-blocking receives

  char* sendBuf;   //!< send buffer
  char** recvBuf;  //!< recv buffer
} 
HaloExchange;

typedef struct NonZeroMsgSt
{
   int irow;
   int icol;
   real_t val;
}
NonZeroMsg;


/// Create a HaloExchange.
HaloExchange* initHaloExchange(struct DomainSt* domain);

/// HaloExchange destructor.
void destroyHaloExchange(struct HaloExchangeSt* haloExchange);

/// Update data to be exchanged.
void updateData(struct HaloExchangeSt* haloExchange, struct SparseMatrixSt* xmatrix, struct DomainSt* domain);

/// Exchange setup - post reads
void exchangeSetup(struct HaloExchangeSt* haloExchange, struct SparseMatrixSt* xmatrix, struct DomainSt* domain);

/// Execute a halo exchange.
void exchangeData(struct HaloExchangeSt* haloExchange, struct SparseMatrixSt* xmatrix, struct DomainSt* domain);

/// Add halo proc if not previously added
void addHaloProc(struct HaloExchangeSt* haloExchange, int rproc);
int isHaloProc(struct HaloExchangeSt* haloExchange, int rproc);

/// Buffer functions
int loadBuffer(char* buf, struct SparseMatrixSt* xmatrix, struct DomainSt* domain);
void unloadBuffer(char* buf, int bufSize, struct SparseMatrixSt* xmatrix, struct DomainSt* domain);

/// Gather functions
void gatherData(struct HaloExchangeSt* haloExchange, struct SparseMatrixSt* xmatrix, struct DomainSt* domain);
void allGatherData(struct HaloExchangeSt* haloExchange, struct SparseMatrixSt* xmatrix, struct DomainSt* domain);
#endif
