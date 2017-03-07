#ifndef _OMP2CD_TYPEDEFS_H
#define _OMP2CD_TYPEDEFS_H

#include <string>
#include <utility>
#include <tuple>
#include <map>
#include <unordered_map>

#include "clang/AST/Decl.h"

namespace omp2cd_space
{
class DFGNode;

typedef enum {OMP_SHARED = 0, OMP_PRIVATE, OMP_FIRSTPRIVATE,
	OMP_LASTPRIVATE, OMP_SHARED_PRIVATE, OMP_THREADPRIVATE} OMP_VARSCOPE; 

typedef std::tuple<   std::string, /*0 = type*/
							OMP_VARSCOPE,/*1 = scope*/ 
							int,         /*2 = pos*/
							std::string, /*3 = init_value*/
							std::string,  /*4 = array specifier*/
							bool          /* 5 = declared locally*/  
				> OMP_VARINFO;
/* key: variable's name*/                  
typedef std::map<std::string, omp2cd_space::OMP_VARINFO> TPInputs_t;

typedef std::map< std::string/*call*/, std::string/*temp var name to replace the call*/> TPCallees_t;

typedef std::map<std::string/*call*/, std::string/*extra args to add to the call*/> TPExtraArgs_t;

// typedef int CHUNK_SIZE;
typedef clang::Expr* CHUNK_SIZE;
typedef std::tuple<clang::Expr*, int> chunkTuple_t;
typedef enum {STATIC_SCHED = 0, DYNAMIC_SCHED, GUIDED_SCHED,
	AUTO_SCHED, RUNTIME_SCHED} OMP_SCHEDULE;

/* Typedefs for the function table we used to keep track of all the functions in all
* the decl units*/

/* The "extra args" are used for functions that need additional params when called from a codelet.
* For instance int y(int x); uses omp_get_thread_num() from within, so when called from a codelet,
* it needs the codelet's id as another arg, so the function becomes int y(int x, int codeletID).
* FIXME: Don't like it very much. */
typedef std::tuple<  std::pair < std::string, std::string >  /*0 = extra args(interface): pair < type, name > */,
				   std::string  /*1 = extra args(on call)*/
				>  ExtraArgsTuple_t;

typedef std::map  <  std::string  /*0 = cause for the addition of extra args*/,
				   ExtraArgsTuple_t /*1 = extra args*/
				>  ExtraArgsMap_t;

typedef std::tuple <  std::string /*0 = functionType*/, 
					bool  /*1 = isOmp2dartsFunction*/, 
					int /*2 = functionNodeID*/,
					ExtraArgsMap_t  /*3 = extra args*/,
					bool  /*4 = isSingleThreaded*/,
					int /*5 = Num of Parallel Regions*/
				  > FunctionTableTuple_t;

typedef std::pair <  std::string /*old name*/, FunctionTableTuple_t > FunctionTablePair_t;
						  
typedef std::unordered_map <  std::string /*old name*/, FunctionTableTuple_t > FunctionTable_t;

/* Typedefs for the var table. Especially useful for global and threadprivate variables*/
typedef std::tuple <
					std::string,      /* 0 = type*/
					std::string,      /* 1 = init*/
					std::string,      /* 2 = storage: extern or static*/
					bool,             /* 3 = is it global?*/
					bool,             /* 4 = is is threadprivate?*/
					int,              /* 5 = ParentNode id*/
					std::string,      /* 6 = array specifier*/
					std::string,      /* 7 = original name*/
					std::string,      /* 8 = array size*/
					bool			  /* 9 = needs to be freed()*/
				  > VarTableTuple_t; 
typedef std::pair< std::string, VarTableTuple_t > VarTablePair_t;
/* key: variable's name*/
typedef std::unordered_map< std::string, VarTableTuple_t > VarTable_t;

/* Typedefs for the var table. Especially useful for global and threadprivate variables*/
typedef std::tuple <  
					std::vector<std::string>,      	/* 0 = type*/
					std::vector<std::string>,      	/* 1 = init*/
					std::vector<std::string>,      	/* 2 = storage: extern or static*/
					std::vector<bool>,             	/* 3 = is it global?*/
					std::vector<bool>,             	/* 4 = is is threadprivate?*/
					std::vector<int>,              	/* 5 = ParentNode id*/
					std::vector<std::string>,      	/* 6 = array specifier*/
					std::vector<std::string>,      	/* 7 = original name*/
					std::vector<std::string>,		/* 8 = array size*/
					std::vector<bool>      			/* 9 = needs to be freed()*/
				  > VarTableTuple2_t; 
typedef std::pair< std::string, VarTableTuple2_t > VarTablePair2_t;
/* key: variable's name*/
typedef std::unordered_map< std::string, VarTableTuple2_t > VarTable2_t;
}
#endif
