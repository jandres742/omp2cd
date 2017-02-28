/* C/Fortran interface is different on different machines. 
 * You may need to tweak this.
 */

#ifndef WTIME_H
#define WTIME_H

void wtime(double *t);

// #if defined(IBM)
// #define wtime wtime
// #elif defined(CRAY)
// #define wtime WTIME
// #else
// #define wtime wtime_
// #endif

#endif