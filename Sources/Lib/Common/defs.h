/*-- File description -------------------------------------------------------*/
/**
 *   @file:    defs.h
 *             File extends standard and hardware specific definitions with
 *             macro extensions, which are often used by NIK Team.
 *  
 *             This File should be included to project only in
 *             "Project specific includes" block, to avoid conflicts.
 *
 *   @author:  valeriy.williams.
 *   @company: Lab.
 */

#ifndef __DEFS_H
#define __DEFS_H

#ifdef __cplusplus
extern "C" {
#endif

/*-- Standart C/C++ Libraries -----------------------------------------------*/
#include <stdint.h>

/*-- Other libraries --------------------------------------------------------*/
/*-- Hardware specific libraries --------------------------------------------*/
/*-- Project specific includes ----------------------------------------------*/
/*-- Exported macro ---------------------------------------------------------*/
#if (!defined(__CC_ARM)) &&   \
    (!defined(__ICCARM__)) && \
	(!defined(__GNUC__)) &&   \
	(!defined(__TASKING__)) && \
    (!defined(__ICCSTM8__))

 #define __CC_ARM    1
// #define __ICCARM__  1
// #define __GNUC__    1
// #define __TASKING__ 1
// #define __ICCSTM8__ 1

#endif

#if (!defined(__CC_ARM)) &&   \
    (!defined(__ICCARM__)) && \
	(!defined(__GNUC__)) &&   \
	(!defined(__TASKING__)) && \
    (!defined(__ICCSTM8__))

#warning "Compiler is unknown! Uncomment line above."
#endif

#if defined ( __CC_ARM )
	#ifndef __ASM
		#define __ASM        __asm    // !< asm keyword for ARM Compiler
	#endif

	#ifndef __WEAK
		#define __WEAK       __weak // !< weak keyword for ARM Compiler
	#endif
	
	#ifndef __INLINE
		#define __INLINE     __inline // !< inline keyword for ARM Compiler
	#endif

	#ifndef __STATIC_INLINE
		#define __STATIC_INLINE  static __inline
	#endif

#elif defined ( __ICCARM__ )
	#ifndef __ASM
		#define __ASM        __asm    // !< asm keyword for IAR Compiler
	#endif

	#ifndef __inline
		#define __inline     inline
	#endif

	#ifndef __INLINE
		#define __INLINE     __inline // !< inline keyword for IAR Compiler.
		                              // Only available in High optimization mode!
	#endif

	#ifndef __STATIC_INLINE
		#define __STATIC_INLINE  static inline
	#endif

#elif defined ( __GNUC__ )
	#ifndef __ASM
		#define __ASM        __asm   // !< asm keyword for GNU Compiler
	#endif

	#ifndef __WEAK
		#define __WEAK       __attribute__((weak)) // !< weak keyword for ARM Compiler
	#endif

	#ifndef __INLINE
		#define __INLINE     inline  // !< inline keyword for GNU Compiler
	#endif

	#ifndef __STATIC_INLINE
		#define __STATIC_INLINE  static inline
	#endif

#elif defined ( __TASKING__ )
	#ifndef __ASM
		#define __ASM        __asm   // !< asm keyword for TASKING Compiler
	#endif

	#ifndef __INLINE
		#define __INLINE     inline  // !< inline keyword for TASKING Compiler
	#endif

	#ifndef __STATIC_INLINE
		#define __STATIC_INLINE  static inline
	#endif
#endif

// Bit Mask Definition
#define BIT00       0x00000001
#define BIT01       0x00000002
#define BIT02       0x00000004
#define BIT03       0x00000008
#define BIT04       0x00000010
#define BIT05       0x00000020
#define BIT06       0x00000040
#define BIT07       0x00000080

#define BIT08       0x00000100
#define BIT09       0x00000200
#define BIT10       0x00000400
#define BIT11       0x00000800
#define BIT12       0x00001000
#define BIT13       0x00002000
#define BIT14       0x00004000
#define BIT15       0x00008000

#define BIT16       0x00010000
#define BIT17       0x00020000
#define BIT18       0x00040000
#define BIT19       0x00080000
#define BIT20       0x00100000
#define BIT21       0x00200000
#define BIT22       0x00400000
#define BIT23       0x00800000

#define BIT24       0x01000000
#define BIT25       0x02000000
#define BIT26       0x04000000
#define BIT27       0x08000000
#define BIT28       0x10000000
#define BIT29       0x20000000
#define BIT30       0x40000000
#define BIT31       0x80000000

// Bit Operations Macro
#define BIT_SET(_reg_, _mask_)      (_reg_ |= _mask_)
#define BIT_CLR(_reg_, _mask_)      (_reg_ &= ~(_mask_))
#define BIT_TOGGLE(_reg_, _mask_)   (_reg_ ^= _mask_)
#define BIT_GET(_reg_, _mask_)      (_reg_ & _mask_)
#define BIT_READ(value, bit)        (((value) >> (bit)) & 0x01)

#define ROOT_SQUARE_SUMM(_a_, _b_)  (sqrt((_a_)*(_a_) + (_b_)*(_b_)))

#define HIGH_U16(_a)                (((_a) >> 8) & 0x00FF)
#define LOW_U16(_a)                 ((_a) & 0x00FF)
#define SWAP_U16(_w)                ((((_w) << 8) & 0xFF00) + ((_w >> 8) & 0x00FF))
#define MERGE_U16(_h, _l)           ((((_h) << 8) & 0xFF00) | (_l))

#define HIGHEST_U32(_a)             (((_a) >> 24) & 0x000000FF)
#define HIGH_U32(_a)                (((_a) >> 16) & 0x000000FF)
#define LOW_U32(_a)                 (((_a) >> 8) & 0x000000FF)
#define LOWEST_U32(_a)              ((_a) & 0x000000FF)
#define SWAP_U32(_w)                ((_w << 24) | ((_w & 0xff00) << 8) | ((_w & 0xff0000) >> 8) | (_w >> 24))
#define MERGE_U32(_a, _b, _c, _d)   ((((_a) << 24) & 0xFF000000) | (((_b) << 16) & 0xFF0000) | (((_c) << 8) & 0xFF00) | (_d))

#define SWAP_U64(_w)                ((_w << 56) | ((_w & 0xff00) << 40) | ((_w & 0xff0000) << 24) | ((_w & 0xff000000) << 8) | ((_w & 0xff00000000) >> 8) | ((_w & 0xff0000000000) >> 24) | ((_w & 0xff000000000000) >> 40) | ((_w & 0xff00000000000000) >> 56))

#define HIGH_U8(_a)                 (((_a) >> 4) & 0x0F)
#define LOW_U8(_a)                  ((_a) & 0x0F)
#define MERGE_U8(_h, _l)            ((LOW_U8(_h) << 4) | LOW_U8(_l))

#define DIGIT_TO_ASCII(_d_)         _d_ + ((_d_ <= 9) ? 0x30 : 0x37)

#define ARRAY_SIZE(_a)              sizeof(_a)/sizeof(_a[0])

#define ERROR_ACTION(CODE,POS)	    do{}while(1)

/*-- Typedefs ---------------------------------------------------------------*/

/*-- Exported variables -----------------------------------------------------*/
/*-- Exported functions -----------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif // _NIK_DEFS_H
/*-- EOF --------------------------------------------------------------------*/
