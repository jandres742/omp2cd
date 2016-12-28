#include <iostream>
#include <cstdlib>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>
#include "darts.h"

using namespace darts;

double *matrix_a, *matrix_b, *matrix_c;
int blockSize;
int row_size;
double* matrix_a1;
double* matrix_b1;
double* matrix_d;
int num_blocks_p;
int total_blocks;
int work;
int threads, cores;
//Timer timer;
uint64_t tt, t1;
uint64_t ts0[100],ts1[100];
int size;

#define RAN_MAX 1.0
#define RAN_MIN -1.0
#define ERROR_TOL 0.00001
//#define VERIFY

int map_standard_to_block(int row_size, int x)
{
	//std::cout << x << " " << blockSize << std::endl;
	int num_blocks = (row_size)/(blockSize);
	if((row_size)%(blockSize))
		num_blocks++;
	int y;
	int row = (x)/row_size;
	
	int col = (x)%row_size;
	
	int block_row = (row)%blockSize;
	
	int block_col = (col)%blockSize;
	
	y = row/blockSize*num_blocks+col/blockSize;
	
	y = y*blockSize*blockSize+block_col+block_row*blockSize;
	
	return y;
}

void perform_block_matmul_fin(int requested_matrix_size, int z, int x, int c, double* matrix_a, double* matrix_b, double* matrix_c)
{
	int i, j, k;
	
	for(i = 0; i<z; i++)
	{
		register double *a_temp = matrix_a + i * blockSize;
		for(j = 0; j<x; j++)
		{
			register double *b_temp = matrix_b + j * blockSize;
			register double c_ij = *(matrix_c + j + i * blockSize);
			for(k = 0; k<c; k++)
			{
				 c_ij += *(a_temp + k) * *(b_temp+k);
			}
			*(matrix_c + j + i * blockSize) = c_ij;
		}
		
	}
}

void perform_block_matmul_unroll_4(int requested_matrix_size,double* matrix_a, double* matrix_b, double* matrix_c, int i, int j, int k, int bl_i, int bl_j, int bl_k, int num_blocks_p)
{
	//z - number of C elements across to calculate
	//x - number of C elements down to calculate
	//c - number of elements from A, B to use for C element.
	int z, x, c;
	
	//If any of these are true our block is bigger than elements to use
	//in that dimension: i,j,k
	if(i+blockSize > requested_matrix_size)
		z = requested_matrix_size-i;
	else
		z = blockSize;
		
	if(j+blockSize > requested_matrix_size)
		x = requested_matrix_size-j;
	else
		x = blockSize;
		
	if(k+blockSize > requested_matrix_size)
		c = requested_matrix_size-k;
	else
		c = blockSize;
	
	//Offset a,b,c into the correct blocks
	//matrix_a += bl_k * blockSize*blockSize + bl_i * blockSize*blockSize*num_blocks_p;
	//matrix_b += bl_k * blockSize*blockSize + bl_j * blockSize*blockSize*num_blocks_p;
	matrix_c += bl_j * blockSize*blockSize + bl_i * blockSize*blockSize*num_blocks_p;
	
	//Loop movement
	int u_i=i;
	int u_j=j;
	
	//Stride of our tiles
	int j_stride = 2;
	int i_stride = 2;
	
	//Number of elements in i and j dimensions
	int num_i = z/i_stride;
	int num_j = x/j_stride;
	
	//Move one C down
	for(u_i = 0; u_i<num_i; u_i++)
	{
		i=u_i*i_stride;
		register double *a_temp = matrix_a + i * blockSize;
		//if(u_i==0)
		//	std::cout << *a_temp << std::endl;
		//Move one C across
		for(u_j = 0; u_j<num_j; u_j++)
		{
			//Find real j
			j=u_j*j_stride;
			register double *b_temp = matrix_b + j * blockSize;

			//Compute 2 Across and 2 Down thus 2x2 = 4
			register double c_ij1 = *(matrix_c + (j + 0) + (i + 0) * blockSize);
			register double c_ij2 = *(matrix_c + (j + 1) + (i + 0) * blockSize);
			register double c_ij3 = *(matrix_c + (j + 0) + (i + 1) * blockSize);
			register double c_ij4 = *(matrix_c + (j + 1) + (i + 1) * blockSize);
			
			//Accumulate tiles
			for(k = 0; k<c; k++)
			{
				
				c_ij1 += *(a_temp + k + (0) * blockSize) * *(b_temp + k + (0) * blockSize);
				c_ij2 += *(a_temp + k + (0) * blockSize) * *(b_temp + k + (1) * blockSize);
				c_ij3 += *(a_temp + k + (1) * blockSize) * *(b_temp + k + (0) * blockSize);
				c_ij4 += *(a_temp + k + (1) * blockSize) * *(b_temp + k + (1) * blockSize);
			}
			
			//Save results
			*(matrix_c + (j + 0) + (i + 0) * blockSize) = c_ij1;
			*(matrix_c + (j + 1) + (i + 0) * blockSize) = c_ij2;
			*(matrix_c + (j + 0) + (i + 1) * blockSize) = c_ij3;
			*(matrix_c + (j + 1) + (i + 1) * blockSize) = c_ij4;
		}
		
	}
	//We could not compute a elements as they did not fit in our stride
	if(z%i_stride != 0 || x%j_stride !=0 )
	{
			//Finish missing elements wide (j) downward frinces
			perform_block_matmul_fin(requested_matrix_size, z , x - num_j * j_stride, c, matrix_a, matrix_b+num_j*j_stride*blockSize, matrix_c+num_j*j_stride);
			
			//Finish missing elements down (i) rightward fringes
			perform_block_matmul_fin(requested_matrix_size, z - num_i * i_stride, num_j*j_stride, c, matrix_a+num_i*i_stride*blockSize, matrix_b, matrix_c+num_i*i_stride*blockSize);
	}
}

void matmul_simple(int requested_matrix_size, double* matrix_a, double* matrix_b, double* matrix_c)
{
	int i,j,k;
	for(i = 0; i < requested_matrix_size; i++)
	{
		register double *a_temp = matrix_a + i * requested_matrix_size;
		for(j = 0; j < requested_matrix_size; j++)
		{
			register double *b_temp = matrix_b + j * requested_matrix_size;
			register double c_ij = *(matrix_c + j + i * requested_matrix_size);
			for(k = 0; k < requested_matrix_size; k++)
			{		
				c_ij += *(a_temp + k) * *(b_temp+k);
			}
			*(matrix_c + j + i * requested_matrix_size) = c_ij;
		}
	}
}


void matmul_block(int requested_matrix_size, double* matrix_a, double* matrix_b, double* matrix_c, int block_i, int num_blocks_p)
{
	//Local loop block versions are used to indicate i and j for that block
	int i, j, k, block_j, block_k;
	
	
	//block_i = __sync_fetch_and_add(&work, 1);
	
	//bl_i is the col and bl_j is the row
	int bl_i = block_i/(num_blocks_p);
	int bl_j = block_i%(num_blocks_p);
	
	//Element in real full matrix
	i = bl_i*blockSize;
	j = bl_j*blockSize;

	for(block_k = 0; block_k < num_blocks_p; block_k++)
	{	
		//Element in real full matrix
		k = block_k*blockSize;
		perform_block_matmul_unroll_4(requested_matrix_size, matrix_a, matrix_b, matrix_c, i, j, k, bl_i, bl_j, block_k,num_blocks_p);
	}
	
	//block_i = __sync_fetch_and_add(&work, 1);
}

void matmul_block_partial(int requested_matrix_size, double* matrix_a, double* matrix_b, double* matrix_c, int block_i, int block_k, int num_blocks_p)
{
	//Local loop block versions are used to indicate i and j for that block
	int i, j, k, block_j;
	
	
	//block_i = __sync_fetch_and_add(&work, 1);
	
	//bl_i is the col and bl_j is the row
	int bl_i = block_i/(num_blocks_p);
	int bl_j = block_i%(num_blocks_p);
	
	//Element in real full matrix
	i = bl_i*blockSize;
	j = bl_j*blockSize;

	//for(block_k = 0; block_k < num_blocks_p; block_k++)
	//{	
		//Element in real full matrix
		k = block_k*blockSize;
		perform_block_matmul_unroll_4(requested_matrix_size, matrix_a, matrix_b, matrix_c, i, j, k, bl_i, bl_j, block_k,num_blocks_p);
	//}
	
	//block_i = __sync_fetch_and_add(&work, 1);
}

void matmul(int block_i)
{
	matmul_block(row_size, matrix_a, matrix_b, matrix_c, block_i, num_blocks_p);
}

void matmul_partial(int block_i, int block_k, double * myA, double * myB)
{
	matmul_block_partial(row_size, myA, myB, matrix_c, block_i, block_k, num_blocks_p);
}


void prefetch(double * addr, int pos)
{
	addr += pos * blockSize*blockSize;
	__builtin_prefetch(addr, 0, 3);
}

void initMatrix(int size, double * matrix)
{
	int i,j;
	double range = RAN_MAX - RAN_MIN;
	for(i = 0; i < size; i++)
	{
		matrix[i] = rand() * range / RAND_MAX + RAN_MIN;
	}
}

CodeletFinal * finalSignal;

class ResultCodelet : public Codelet   //CodeletFinal
{
public:
  ResultCodelet(){}
  ResultCodelet(uint32_t dep, uint32_t res, ThreadedProcedure * myTP, uint32_t stat):
  Codelet(dep,res,myTP,stat) { }
  void initThis (uint32_t dep, uint32_t res, darts::ThreadedProcedure * myTP, uint32_t stat)
  {
	initCodelet (dep, res, myTP, stat);
  }
  //ResultCodelet(uint64_t dep, uint64_t res): CodeletFinal(dep,res) {}
  
  void fire(void)
  {
	t1 = df_tstamp(ts1);
	//std::cout << "Here" << row_size << std::endl;
	//double timeTaken = timer.get();
	#ifdef VERIFY
	matrix_d = new double[size]();
	matrix_a1 = new double[size];
	matrix_b1 = new double[size];
	
	//std::cout << "Here" << std::endl;
	for(int i=0;i<size;i++)
	{
		int map = map_standard_to_block(row_size,i);
		matrix_a1[i] = matrix_a[map];
		matrix_b1[i] = matrix_b[map];
	}
	
	//std::cout << "Here" << std::endl;
	matmul_simple(row_size, matrix_a1, matrix_b1, matrix_d);
	
	double error;
	int error_count = 0;
	//std::cout << "Here" << std::endl;
	for(int j = 0; j < size; j++)
	{
		int map = map_standard_to_block(row_size,j);
		error = matrix_c[map] - matrix_d[j];
		if(error < -ERROR_TOL || error > ERROR_TOL)
			error_count++;
	}
	
	if(error_count == 0)
			printf("The Matrix C is correct\n");
		else
			printf("The Matrix C has %d errors.\n",error_count);
	delete [] matrix_a1;
	delete [] matrix_b1;
	delete [] matrix_d;	
	#endif
	delete [] matrix_a;
	delete [] matrix_b;
	delete [] matrix_c;
	std::cout <<" Done" << std::endl;
	std::cout <<" Time:" << t1- tt /*timeTaken*/ << " ns" << std::endl;
	Runtime::finalSignal.decDep();
	//delete myTP_;
  }
};

class Block : public darts::Codelet
{
public:
	int i;
	int k;
	double *myA, * myB; 
	Codelet * next;
	Block(){}
	Block (uint32_t dep, uint32_t res, ThreadedProcedure * myTP, uint32_t st, int id, int kid, Codelet * next_);
	void initThis (uint32_t dep, uint32_t res, darts::ThreadedProcedure * myTP, uint32_t stat,
										int id,  int kid, Codelet * next_);
	virtual void fire(void);
};

class Prefetch  : public darts::Codelet
{
public:
	int i;
	double * addr;
	bool type;
	Prefetch(){}
	Prefetch (uint32_t dep, uint32_t res, ThreadedProcedure * myTP, uint32_t st, double * addr_, int id, bool type_);
	void initThis (uint32_t dep, uint32_t res, darts::ThreadedProcedure * myTP, uint32_t stat, double * addr_,
										int id, bool type_);
	virtual void fire(void);
	~Prefetch(){/*std::cout << "Her1e" << std::endl;*/ delete [] addr;}
};

class End : public darts::Codelet
{
public:
	End (){};
	End (uint32_t dep, uint32_t res, ThreadedProcedure * myTP, uint32_t st);

	void initThis (uint32_t dep, uint32_t res, darts::ThreadedProcedure * myTP, uint32_t stat)
	{
		initCodelet (dep, res, myTP, stat);
	}
	
	virtual void fire(void);
};
class Master: public darts::ThreadedProcedure
{
public:
	ResultCodelet end;	
	Block * blocks;
	Prefetch * prefA;
	Prefetch * prefB;
	Codelet * toSignal;
	Master();
	~Master (){ /*std::cout << "Here" << std::endl;*/ delete [] blocks; delete [] prefA; delete [] prefB;};
};

Block::Block (uint32_t dep, uint32_t res, ThreadedProcedure * myTP, uint32_t st, int id, int kid, Codelet * next_):
Codelet(dep,res,myTP,st),
i(id),
k(kid),
next(next_)
{

}
void Block::initThis (uint32_t dep, uint32_t res, darts::ThreadedProcedure * myTP, uint32_t stat,
									int id, int kid, Codelet * next_)
{
	initCodelet (dep, res, myTP, stat);
	i = id;
	k = kid;
	next = next_;
}

void Block::fire(void)
{
	//std::cout << myA[0] << " "  << myB[0] << std::endl;
	matmul_partial(i,k, myA, myB);
	Master *tp = static_cast<Master*>(myTP_);
	if(next!=0)
		next->decDep();
	else
		tp->end.decDep ();
}

Prefetch::Prefetch (uint32_t dep, uint32_t res, ThreadedProcedure * myTP, uint32_t st, double * addr_, int id, bool type_):
Codelet(dep,res,myTP,st),
addr(addr_),
i(id),
type(type_)
{

}
void Prefetch::initThis (uint32_t dep, uint32_t res, darts::ThreadedProcedure * myTP, uint32_t stat, double* addr_,
									int id, bool type_)
{
	initCodelet (dep, res, myTP, stat);
	i = id;
	addr = addr_;
	type = type_;
}
void Prefetch::fire(void)
{
	//prefetch(addr,i);
	Master *tp = static_cast<Master*>(myTP_);
	double * matrix = new double[blockSize*blockSize];
	std::copy(addr + i * blockSize*blockSize,addr + i * blockSize*blockSize+ blockSize*blockSize, matrix);
	addr = matrix;
	if(type == false)
	{
		//std::cout << "A" << i << " " <<  matrix[0] << " " << *(addr + i * blockSize*blockSize+0) << std::endl;
		for(int j = 0; j < num_blocks_p; j++)
		{
			//std::cout << "R" << j+(i/num_blocks_p)*num_blocks_p << std::endl;
			//tp->blocks[j+(i/num_blocks_p)*num_blocks_p].decDep();
			//std::cout << "I " << i << " R" << j*num_blocks_p+(i/num_blocks_p)*num_blocks_p*num_blocks_p+i-(i/num_blocks_p)*num_blocks_p << std::endl;
			tp->blocks[j*num_blocks_p+(i/num_blocks_p)*num_blocks_p*num_blocks_p+i-(i/num_blocks_p)*num_blocks_p].myA = matrix;
			tp->blocks[j*num_blocks_p+(i/num_blocks_p)*num_blocks_p*num_blocks_p+i-(i/num_blocks_p)*num_blocks_p].decDep();
		}
	}
	else
	{
		//std::cout << "B" << i << " " <<  matrix[0] << " " << *(addr + i * blockSize*blockSize+0) << std::endl;
		for(int j = 0; j < num_blocks_p; j++)
		{
			//std::cout << "C" << j*num_blocks_p+(i%num_blocks_p) << std::endl;
			//tp->blocks[j*num_blocks_p+(i%num_blocks_p)].decDep();
			//std::cout << "I " << i << " C" << j*num_blocks_p*num_blocks_p+(i%num_blocks_p)*num_blocks_p+(i/num_blocks_p) << std::endl;
			tp->blocks[j*num_blocks_p*num_blocks_p+(i/num_blocks_p)*num_blocks_p+(i%num_blocks_p)].myB = matrix;
			tp->blocks[j*num_blocks_p*num_blocks_p+(i/num_blocks_p)*num_blocks_p+(i%num_blocks_p)].decDep();
			//tp->blocks[j*num_blocks_p+(i/num_blocks_p)*num_blocks_p*num_blocks_p+i-(i/num_blocks_p)*num_blocks_p].myB = matrix;
			//tp->blocks[j*num_blocks_p+(i/num_blocks_p)*num_blocks_p*num_blocks_p+i-(i/num_blocks_p)*num_blocks_p].decDep();
		}
	}
}

Master::Master(): toSignal(finalSignal)
{
	//timer.start();
	tt = df_tstamp(ts0);
	blocks = new Block [total_blocks*num_blocks_p];
	prefA = new Prefetch [total_blocks];
	prefB = new Prefetch [total_blocks];
	end.initCodelet(total_blocks, total_blocks, this, SHORTWAIT);
	//std::cout << total_blocks << " " << num_blocks_p << std::endl;
	for(int i = 0; i < total_blocks; i++)
	{
		blocks[i*num_blocks_p].initThis (2, 2, this, SHORTWAIT, i,0, &blocks[i*num_blocks_p+1]);
		for(int k = 1; k < num_blocks_p-1; k++)
		{
			//std::cout << i*num_blocks_p+k << std::endl;
			blocks[i*num_blocks_p+k].initThis (3, 3, this, SHORTWAIT, i,k, &blocks[i*num_blocks_p+k+1]);
		}
		blocks[i*num_blocks_p+num_blocks_p-1].initThis (3, 3, this, SHORTWAIT, i, num_blocks_p-1, 0);
	}
	for(int i = 0; i < total_blocks; i++)
	{
		prefA[i].initThis (0, 0, this, SHORTWAIT, matrix_a, i, false);
		prefB[i].initThis (0, 0, this, SHORTWAIT, matrix_b, i, true);
		add(&prefA[i]);
		add(&prefB[i]);
	}	
}

End::End (uint32_t dep, uint32_t res, ThreadedProcedure * myTP, uint32_t st):
Codelet(dep,res,myTP,st)
{

}

void End::fire(void)
{
	Master *tp = static_cast<Master*>(myTP_);

	//tp->toSignal->decDep ();
	tp->toSignal->decDep ();
	//delete myTP_;
}


int main( int argc, char ** argv )
{
	char* inputEnd;
	if( argc != 3 )
        	return 0;
        
	row_size = strtol(argv[1], &inputEnd, 10);
	blockSize = strtol(argv[2], &inputEnd, 10); 
		
	int num_blocks = (row_size)/(blockSize);
	size = row_size;	
		
	num_blocks_p = (row_size)/(blockSize);

	if(size%blockSize)
	{
		num_blocks++;
		num_blocks_p++;
	}
	
	total_blocks = num_blocks*num_blocks;

	size=row_size*row_size;
	
	matrix_a = new double[num_blocks*num_blocks*blockSize*blockSize];
	matrix_b = new double[num_blocks*num_blocks*blockSize*blockSize];//size];
	matrix_c = new double[num_blocks*num_blocks*blockSize*blockSize]();//size]();
	//matrix_a = new double[size];
	//matrix_b = new double[size];//size];
	//matrix_c = new double[size]();//size]();

	initMatrix( size, matrix_a);
	initMatrix( size, matrix_b);

	//finalSignal = new ResultCodelet;
	
	ThreadAffinity affin(31U, 1U, COMPACT, 0, 0);
	if (affin.generateMask())
	{
		//std::cout << "fib(" << fibnum << ")" << std::endl;

		//Runtime(&affin).run(launch<fib >(fibnum, &result, &Runtime::finalSignal));
		Runtime(&affin).run(launch<Master>());
	}

	return 0;
}
