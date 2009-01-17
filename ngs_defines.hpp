#ifndef FILE_NGS_DEFINES
#define FILE_NGS_DEFINES

/**************************************************************************/
/* File:   ngs_defines.hpp                                                */
/* Author: Joachim Schoeberl                                              */
/* Date:   21. Feb. 03                                                    */
/**************************************************************************/


// performs range-checking
// #define DEBUG


// maximal system dimension (3D elasticity = 3, piezo = 4)
// 8 for multiharmonic

#ifndef MAX_SYS_DIM
#define MAX_SYS_DIM 2
#endif

#ifndef MAX_CACHEBLOCKS
#define MAX_CACHEBLOCKS 0
#endif



// 
// Use Robert's high order curved elements
// #define NETGEN_ELTRANS  ---> now in Makefile


// use Cholesky-Factors for block smoother
// undef = band - Cholesky, usually faster and less mem
// define only for special needs (mixed smoother)
// #define SYMCHOLESKY


#endif
