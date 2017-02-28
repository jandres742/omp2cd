/// \file
/// Wrappers for MPI functions.

#ifndef _PARALLEL_H_
#define _PARALLEL_H_

#include "mytype.h"

/// Structure for use with MPI_MINLOC and MPI_MAXLOC operations.
typedef struct RankReduceDataSt
{
   double val;
   int rank;
} RankReduceData;

/// Return total number of processors.
int getNRanks(void);

/// Return local rank.
int getMyRank(void);

/// Return non-zero if printing occurs from this rank.
int printRank(void);

/// Print a timestamp and message when all tasks arrive.
void timestampBarrier(const char* msg);

/// Wrapper for MPI_Init.
void initParallel(int *argc, char ***argv);

/// Wrapper for MPI_Finalize.
void destroyParallel(void);

/// Wrapper for MPI_Barrier(MPI_COMM_WORLD).
void barrierParallel(void);

/// Wrapper for MPI_Sendrecv.
int sendReceiveParallel(void* sendBuf, int sendLen, int dest,
                        void* recvBuf, int recvLen, int source);
/// Wrapper for MPI_Send.
int sendParallel(void* sendBuf, int sendLen, int dest);

/// Wrapper for MPI_Isend, non-blocking send.
int isendParallel(void* sendBuf, int sendLen, int dest);

/// Wrapper for MPI_Recv from any processor.
int recvAnyParallel(void* recvBuf, int recvLen);

/// Wrapper for MPI_Irecv from any processor, non-blocking receive.
int irecvAnyParallel(void* recvBuf, int recvLen);

/// Wrapper for MPI_Recv.
int recvParallel(void* recvBuf, int recvLen, int source);

/// Wrapper for MPI_Recv, non-blocking receive.
int irecvParallel(void* recvBuf, int recvLen, int source);

/// Wrapper for MPI_Wait on non-blocking receive.
int waitIrecv(int rind);

/// Wrapper for MPI_Wait on non-blocking send.
int waitIsend(int rind);

/// Wrapper for MPI_Test on non-blocking receive.
int testIrecv(int rind);

/// Wrapper for MPI_Test on non-blocking send.
int testIsend(int rind);

/// Wrapper for MPI_Allreduce integer sum.
void addIntReduce2(int* value0, int* value1);
void addIntParallel(int* sendBuf, int* recvBuf, int count);

/// Wrapper for MPI_Allreduce real sum.
void addRealReduce2(real_t* value0, real_t* value1);
void addRealParallel(real_t* sendBuf, real_t* recvBuf, int count);

/// Wrapper for MPI_Allreduce double sum.
void addDoubleParallel(double* sendBuf, double* recvBuf, int count);

/// Wrapper for MPI_Allreduce real min.
void minRealReduce(real_t* value);
void minRealParallel(real_t* sendBuf, real_t* recvBuf, int count);

/// Wrapper for MPI_Allreduce integer max.
void maxIntReduce2(int* value0, int* value1);
void maxIntParallel(int* sendBuf, int* recvBuf, int count);

/// Wrapper for MPI_Allreduce real max.
void maxRealReduce(real_t* value);
void maxRealParallel(real_t* sendBuf, real_t* recvBuf, int count);

/// Wrapper for MPI_Allreduce double min with rank.
void minRankDoubleParallel(RankReduceData* sendBuf, RankReduceData* recvBuf, int count);

/// Wrapper for MPI_Allreduce double max with rank.
void maxRankDoubleParallel(RankReduceData* sendBuf, RankReduceData* recvBuf, int count);

/// Wrapper for MPI_Bcast
void bcastParallel(void* buf, int len, int root);

///  Return non-zero if code was built with MPI active.
int builtWithMpi(void);

#endif

