#ifndef _TSU_H_INCLUDED
#define _TSU_H_INCLUDED

#define _XS(s) #s

#define _TSU_BASE 0x2DAF0000
#define _TSUOP(x) (_TSU_BASE+(x))
#define _TSUOPI(x,i) (_TSU_BASE+((i)<<8)+(x))
#define _ASMOP(x) ((x)-_TSU_BASE + 5000)

#define _TSU_TINIT          _TSUOP(0x01)
#define _TSU_TSCHEDULE      _TSUOP(0x02) // uncond schedule
#define _TSU_TREAD          _TSUOP(0x03) // byte offset
#define _TSU_TWRITE         _TSUOP(0x04) // byte offset
#define _TSU_TALLOC         _TSUOP(0x05)
#define _TSU_TFREE          _TSUOP(0x06)
#define _TSU_TPOLL          _TSUOP(0x07)
#define _TSU_TRESET         _TSUOP(0x08)
#define _TSU_TSTAMP         _TSUOP(0x09)
#define _TSU_TDESTROY       _TSUOP(0x0a)

#define _TSU_TREADI(i)      _TSUOPI(0x0b,i) // byte imm. offset
#define _TSU_TWRITEI(i)     _TSUOPI(0x0c,i) // byte imm. offset
#define _TSU_TSCHEDULEI(i)  _TSUOPI(0x0d,i) // uncond schedule

#define _TSU_TREADQI(i)     _TSUOPI(0x0e,i) // 64b imm. offset
#define _TSU_TWRITEQI(i)    _TSUOPI(0x0f,i) // 64b imm. offset

#define _TSU_TSCHEDULEP     _TSUOP(0x10)     // predicated schedule
#define _TSU_TSCHEDULEPI(i) _TSUOPI(0x11,i)  // predicated schedule

#define _TSU_TLOAD          _TSUOP(0x12) 	 // load frame
#define _TSU_TSTORE         _TSUOP(0x13) 	 // store frame
#define _TSU_TSTOREQI(i)    _TSUOPI(0x14,i)  // store frame (immediate)

#define _TSU_TSCHEDULEF     _TSUOP(0x15)	// tschedule (ret. local frame)
#define _TSU_TSCHEDULEFI(i) _TSUOPI(0x16,i) // tschedule immediate (ret. local frame)

#define _TSU_TCACHE         _TSUOP(0x17)	// caches and returns target frame

#define _TSU_TDECREASE      _TSUOP(0x18)
#define _TSU_TDECREASEN     _TSUOP(0x19)
#define _TSU_TDECREASENI(i)  _TSUOPI(0x1a,i)

#define _TSU_TWRITEP        _TSUOP(0x1b) // byte offset
#define _TSU_TWRITEPI(i)    _TSUOPI(0x1c,i) // byte imm. offset
#define _TSU_TWRITEQPI(i)   _TSUOPI(0x1d,i) // 64b imm. offset
#define _TSU_TSCHEDULEZ     _TSUOP(0x1e) // byte offset

#define _TSU_TWRITE32P        _TSUOP(0x1f) // byte offset
#define _TSU_TWRITE32PI(i)    _TSUOPI(0x20,i) // byte imm. offset

#define _TSU_TSTOREP         _TSUOP(0x21)       // store frame
#define _TSU_TSTOREPI(i)     _TSUOPI(0x22,i)  // store frame (immediate)

#define _COMBINE(_sc,_sz) ((unsigned long long)(_sc)<<32|_sz)

#define _LO32(_x) ((unsigned long long)(_x)&0xffffffffULL)
#define _HI32(_x) ((unsigned long long)(_x)>>32)
#define _TLOC(_tp,_l) ((_tp)+((unsigned long long)(_l)<<3))
#define _TLOC_TID(_x) _HI32(_x)
#define _TLOC_OFF(_x) _LO32(_x)

#define _ASMVOL  __asm__ volatile 
#define _ASMNV   __asm__ 
#define _INL  __always_inline 

#define _REG_ARG1  "R"     /* argument 1 */
#define _REGQ_ARG1 "Q"     /* argument 1 (only ABCD)*/
#define _REG_ARG1d "0"     /* argument 1 = dest*/
#define _REG_ARG2  "R"     /* argument 2 */
#define _REGQ_ARG2 "Q"     /* argument 2 (only ABCD)*/
#define _REG_RES   "=R"    /* result */

#define _REG_CLOB  /*example: #define _REG_CLOB :"%rax" // don't forget the colon! */

#define _ASM_SEQ1(_of)  _XS(prefetchnta _of(%q0,%q0,1))
#define _ASM_SEQ1a(_of) _XS(prefetchnta _of(%q0,%%rax,1))
#define _ASM_SEQ2(_of)  _XS(prefetchnta _of(%q0,%q1,1))
#define _ASM_SEQ3(_of)  _XS(prefetchnta _of(%q0,%q2,1))

#define TSU_TSTAMP(_ts,_buf) \
    _ASMVOL (_ASM_SEQ1(_TSU_TSTAMP):_REG_RES(_ts):_REG_ARG1d(_buf) _REG_CLOB)
#define TSU_RESET(_s,_n) \
    _ASMVOL (_ASM_SEQ2(_TSU_TRESET)::_REG_ARG1(_s),_REG_ARG2(_n) _REG_CLOB)
#define TSU_TINIT(_nop,_sp) \
    _ASMVOL (_ASM_SEQ2(_TSU_TINIT)::_REG_ARG1(_nop),_REG_ARG2(_sp) _REG_CLOB)
#define TSU_POLL(_ip) \
    _ASMVOL (_ASM_SEQ1(_TSU_TPOLL):_REG_RES(_ip): _REG_CLOB)
#define TSU_DESTROY(_ip) \
    _ASMVOL (_ASM_SEQ1(_TSU_TDESTROY):_REG_RES(_ip): _REG_CLOB)

#define TSU_TSCHEDULE(_ip,_sc,_tid) \
    _ASMVOL (_ASM_SEQ3(_TSU_TSCHEDULE):_REG_RES(_tid):_REG_ARG1d(_ip),_REG_ARG2(_sc) _REG_CLOB)

#define TSU_TSCHEDULEF(_ip,_sc,_tfp) \
    _ASMVOL (_ASM_SEQ3(_TSU_TSCHEDULEF):_REG_RES(_tfp):_REG_ARG1d(_ip),_REG_ARG2(_sc) _REG_CLOB)

#define TSU_TSCHEDULEP(_ip,_sc_cnd,_tid) \
    _ASMVOL (_ASM_SEQ3(_TSU_TSCHEDULEP):_REG_RES(_tid):_REG_ARG1d(_ip),_REG_ARG2(_sc_cnd) _REG_CLOB)

#define TSU_TSCHEDULEI(_ip,_im,_tid) \
    _ASMVOL (_ASM_SEQ1(_TSU_TSCHEDULEI(_im)):_REG_RES(_tid):_REG_ARG1d(_ip) _REG_CLOB)

#define TSU_TSCHEDULEFI(_ip,_im,_tfp) \
    _ASMVOL (_ASM_SEQ1(_TSU_TSCHEDULEFI(_im)):_REG_RES(_tfp):_REG_ARG1d(_ip) _REG_CLOB)

#define TSU_TSCHEDULEPI(_ip,_im,_cnd,_tid) \
    _ASMVOL (_ASM_SEQ3(_TSU_TSCHEDULEPI(_im)):_REG_RES(_tid):_REG_ARG1d(_ip),_REG_ARG2(_cnd) _REG_CLOB)

#define TSU_TSCHEDULEZ(_ip,_sc_sz,_tid) \
    _ASMVOL (_ASM_SEQ3(_TSU_TSCHEDULEZ):_REG_RES(_tid):_REG_ARG1d(_ip),_REG_ARG2(_sc_sz) _REG_CLOB)

#define TSU_WRITE(_tloc,_val) \
    _ASMNV  (_ASM_SEQ2(_TSU_TWRITE)::_REG_ARG1(_tloc),_REG_ARG2(_val) _REG_CLOB)

#define TSU_WRITEI(_tid,_im,_val) \
    _ASMNV (_ASM_SEQ2(_TSU_TWRITEI(_im))::_REG_ARG1(_tid),_REG_ARG2(_val) _REG_CLOB)

#define TSU_WRITEP(_tid,_off,_val) \
    _ASMNV  (_ASM_SEQ2(_TSU_TWRITEP)::_REG_ARG1(_tid),_REGQ_ARG2(_val),"c"(_off) _REG_CLOB)

#define TSU_WRITEPI(_tid,_im,_val) \
  _ASMNV  (_ASM_SEQ2(_TSU_TWRITEPI(_im))::_REG_ARG1(_tid),_REGQ_ARG2(_val) _REG_CLOB)

#define TSU_WRITE32P(_tid,_off,_val) \
    _ASMNV  (_ASM_SEQ2(_TSU_TWRITE32P)::_REG_ARG1(_tid),_REGQ_ARG2(_val),"c"(_off) _REG_CLOB)

#define TSU_WRITE32PI(_tid,_im,_val) \
  _ASMNV  (_ASM_SEQ2(_TSU_TWRITE32PI(_im))::_REG_ARG1(_tid),_REGQ_ARG2(_val) _REG_CLOB)

#define TSU_TDEC(_ip) \
    _ASMVOL (_ASM_SEQ1(_TSU_TDECREASE)::_REG_ARG1(_ip) _REG_CLOB)

#define TSU_TDECN(_ip, _n)							\
  _ASMVOL (_ASM_SEQ2(_TSU_TDECREASEN)::_REG_ARG1(_ip),_REG_ARG2(_n) _REG_CLOB)

#define TSU_READ(_off,_res) \
    _ASMNV  (_ASM_SEQ1(_TSU_TREAD):_REG_RES(_res):_REG_ARG1d(_off) _REG_CLOB)

#define TSU_READI(_im,_res) \
    _ASMNV (_ASM_SEQ1(_TSU_TREADI(_im)):_REG_RES(_res): _REG_CLOB)

#define TSU_WRITEQI(_tid,_im,_val) \
    _ASMNV (_ASM_SEQ2(_TSU_TWRITEQI(_im))::_REG_ARG1(_tid),_REG_ARG2(_val) _REG_CLOB)

#define TSU_READQI(_im,_res) \
    _ASMNV (_ASM_SEQ1(_TSU_TREADQI(_im)):_REG_RES(_res): _REG_CLOB)

#define TSU_TLOAD(_res) \
    _ASMNV  (_ASM_SEQ1(_TSU_TLOAD):_REG_RES(_res): _REG_CLOB)

#define TSU_TSTORE(_tloc,_ptr,_len) \
	_ASMNV  (_ASM_SEQ2(_TSU_TSTORE)::_REG_ARG1(_tloc),_REGQ_ARG2(_ptr),"c"(_len) _REG_CLOB)

#define TSU_TSTOREP(_tloc,_ptr,_len) \
	_ASMNV  (_ASM_SEQ2(_TSU_TSTOREP)::_REG_ARG1(_tloc),_REGQ_ARG2(_ptr),"c"(_len) _REG_CLOB)

#define TSU_TSTOREQI(_tloc,_ptr,_len) \
	_ASMNV  (_ASM_SEQ2(_TSU_TSTOREQI(_len))::_REG_ARG1(_tloc),_REGQ_ARG2(_ptr) _REG_CLOB)

#define TSU_TCACHE(_tfp,_tloc) \
    _ASMNV  (_ASM_SEQ1(_TSU_TCACHE):_REG_RES(_tfp):_REG_ARG1d(_tloc) _REG_CLOB)

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <pthread.h>

#ifdef __cplusplus
extern "C" {
#endif
typedef void (*df_thread_t)(void);
void     df_exit(void);
void     df_printstats(const uint64_t *, const uint64_t *, uint64_t);


// _INL static uint64_t df_self() { uint64_t s; _ASMVOL ("mov %%fs:0x10,%0":"=r"(s)::); return s; }
// _INL static uint64_t df_self() { return (uint64_t)pthread_self(); }

_INL static uint64_t df_tscheduleN(df_thread_t ip, uint64_t sc)
{
    uint64_t tid; 
    TSU_TSCHEDULE((uint64_t)ip,sc,tid); 
    return tid; 
}
#define df_tschedule(ip,sc) df_tschedule##sc(ip)
#define df_tschedule_imm(sc) _INL static uint64_t df_tschedule##sc(df_thread_t ip) \
    { uint64_t tid; TSU_TSCHEDULEI((uint64_t)ip,sc,tid); return tid; }
df_tschedule_imm(0)
df_tschedule_imm(1)
df_tschedule_imm(2)
df_tschedule_imm(3)
df_tschedule_imm(4)
df_tschedule_imm(5)
df_tschedule_imm(6)
df_tschedule_imm(7)
df_tschedule_imm(8)
df_tschedule_imm(9)
df_tschedule_imm(10)
df_tschedule_imm(11)
df_tschedule_imm(12)
df_tschedule_imm(13)
df_tschedule_imm(14)
df_tschedule_imm(15)
df_tschedule_imm(16)
df_tschedule_imm(17)
df_tschedule_imm(18)
df_tschedule_imm(19)
df_tschedule_imm(20)
df_tschedule_imm(21)
df_tschedule_imm(22)
df_tschedule_imm(23)
df_tschedule_imm(24)
df_tschedule_imm(25)
df_tschedule_imm(26)
df_tschedule_imm(27)
df_tschedule_imm(28)
df_tschedule_imm(29)
df_tschedule_imm(30)

_INL static uint64_t df_tschedulez(df_thread_t ip, uint32_t sc, uint32_t sz)
{
    uint64_t tid;
    TSU_TSCHEDULEZ((uint64_t)ip,_COMBINE(sc,sz),tid);
    return tid;
}

_INL static uint64_t df_tschedule_condN(df_thread_t ip, uint64_t sc, uint8_t f)
{
    uint64_t tid; 
    TSU_TSCHEDULEP((uint64_t)ip,(sc<<1)+f,tid);
    return tid; 
}
#define df_tschedule_cond(ip,sc,f) df_tschedule_cond##sc(ip,f)
#define df_tschedule_cond_imm(sc) _INL static uint64_t df_tschedule_cond##sc(df_thread_t ip,uint8_t f) \
    { uint64_t tid; TSU_TSCHEDULEPI((uint64_t)ip,sc,f,tid); return tid; }
df_tschedule_cond_imm(0)
df_tschedule_cond_imm(1)
df_tschedule_cond_imm(2)
df_tschedule_cond_imm(3)
df_tschedule_cond_imm(4)
df_tschedule_cond_imm(5)
df_tschedule_cond_imm(6)
df_tschedule_cond_imm(7)
df_tschedule_cond_imm(8)
df_tschedule_cond_imm(9)
df_tschedule_cond_imm(10)
df_tschedule_cond_imm(11)
df_tschedule_cond_imm(12)
df_tschedule_cond_imm(13)
df_tschedule_cond_imm(14)
df_tschedule_cond_imm(15)
df_tschedule_cond_imm(16)
df_tschedule_cond_imm(17)
df_tschedule_cond_imm(18)
df_tschedule_cond_imm(19)
df_tschedule_cond_imm(20)
df_tschedule_cond_imm(21)
df_tschedule_cond_imm(22)
df_tschedule_cond_imm(23)
df_tschedule_cond_imm(24)
df_tschedule_cond_imm(25)
df_tschedule_cond_imm(26)
df_tschedule_cond_imm(27)
df_tschedule_cond_imm(28)
df_tschedule_cond_imm(29)
df_tschedule_cond_imm(30)

_INL static uint64_t* df_tschedulefN(df_thread_t ip, uint64_t sc)
{
    uint64_t* tfp; 
    TSU_TSCHEDULEF((uint64_t)ip,sc,tfp); 
    return tfp; 
}
#define df_tschedulef(ip,sc) df_tschedulef##sc(ip)
#define df_tschedulef_imm(sc) _INL static uint64_t* df_tschedulef##sc(df_thread_t ip) \
    { uint64_t* tfp; TSU_TSCHEDULEFI((uint64_t)ip,sc,tfp); return tfp; }
df_tschedulef_imm(0)
df_tschedulef_imm(1)
df_tschedulef_imm(2)
df_tschedulef_imm(3)
df_tschedulef_imm(4)
df_tschedulef_imm(5)
df_tschedulef_imm(6)
df_tschedulef_imm(7)
df_tschedulef_imm(8)
df_tschedulef_imm(9)
df_tschedulef_imm(10)
df_tschedulef_imm(11)
df_tschedulef_imm(12)
df_tschedulef_imm(13)
df_tschedulef_imm(14)
df_tschedulef_imm(15)
df_tschedulef_imm(16)
df_tschedulef_imm(17)
df_tschedulef_imm(18)
df_tschedulef_imm(19)
df_tschedulef_imm(20)
df_tschedulef_imm(21)
df_tschedulef_imm(22)
df_tschedulef_imm(23)
df_tschedulef_imm(24)
df_tschedulef_imm(25)
df_tschedulef_imm(26)
df_tschedulef_imm(27)
df_tschedulef_imm(28)
df_tschedulef_imm(29)
df_tschedulef_imm(30)


_INL static uint64_t df_readN(uint64_t off) { uint64_t val; TSU_READ(off<<3,val); return val; }
#define df_read_imm(off) _INL static uint64_t df_read##off() \
    { uint64_t val; TSU_READQI(off,val); return val; }
df_read_imm(0)
df_read_imm(1)
df_read_imm(2)
df_read_imm(3)
df_read_imm(4)
df_read_imm(5)
df_read_imm(6)
df_read_imm(7)
df_read_imm(8)
df_read_imm(9)
df_read_imm(10)
df_read_imm(11)
df_read_imm(12)
df_read_imm(13)
df_read_imm(14)
df_read_imm(15)
df_read_imm(16)
df_read_imm(17)
df_read_imm(18)
df_read_imm(19)
df_read_imm(20)
df_read_imm(21)
df_read_imm(22)
df_read_imm(23)
df_read_imm(24)
df_read_imm(25)
df_read_imm(26)
df_read_imm(27)
df_read_imm(28)
df_read_imm(29)
df_read_imm(30)
#define df_read(off) df_read##off()

_INL static void df_writeN(uint64_t tloc, uint64_t val) { TSU_WRITE(tloc,val); }
#define df_write(_tid,_off,_val) TSU_WRITEQI(_tid,_off,_val)

/* df plain write.  */
#define df_twritep(_tid,_off,_val) TSU_WRITEP (_tid,_off,_val)

#define df_twrite32p(_tid,_off,_val) TSU_WRITE32P (_tid,_off,_val)
#define df_tdecrease(_tid) TSU_TDEC (_tid)
#define df_tdecrease_n(_tid,_n) TSU_TDECN (_tid,_n)

_INL static void df_destroy()
{
	df_thread_t dft;
    TSU_DESTROY(dft); // shortcut: avoids returning to df_thread
    (*dft)();
    // _ASMVOL ("jmp *%0\n"::"R"(dft));
}

_INL static void df_reset(const char* s)
{
    if (s)
        TSU_RESET(s,strlen(s));
    else
        TSU_RESET(0,0);
}

_INL static uint64_t df_tstamp(uint64_t *buf)
{
    uint64_t ts;
    TSU_TSTAMP(ts,buf);
    return ts;
}

_INL static void* df_tload() 
{
    uint64_t* p; 
	TSU_TLOAD(p); 
	return p; 
}

_INL static void df_tstore(uint64_t tid,uint32_t off,uint64_t* ptr, uint64_t len) 
{
    TSU_TSTORE(_TLOC(tid,off>>3),ptr,len); 
}

_INL static void df_tstorep(uint64_t tid,uint32_t off,uint64_t* ptr, uint64_t len) 
{
    TSU_TSTOREP(_TLOC(tid,off>>3),ptr,len); 
}

_INL static uint64_t* df_tcache(uint64_t tloc) 
{
    uint64_t* tfp;
	TSU_TCACHE(tfp,tloc);
	return tfp;
}

#define df_tstorei(_tid,_off,_ptr,_len) TSU_TSTOREQI(_TLOC(_tid,_off>>3),_ptr,((_len)>>3))

#define TSU_FRAME_PADDING 4

/* PRELOADING FRAMES */
#ifdef TSU_PRELOAD_FRAME
/* 
** We cache the frame into the buffer provided by the worker thread 
** so that it can be accessed with standard loads/stores
*/

# define df_ldframe(_sz) const uint64_t* __frame=df_tload()
# define df_frame(_off) (__frame[_off])
#else
# define df_ldframe(_sz) /**/
# define df_frame(_off) (df_read(_off))
#endif

#ifdef __cplusplus
}
#endif
#endif
