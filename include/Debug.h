#ifndef _LIBELFXX_DEBUG_H_
#define _LIBELFXX_DEBUG_H_

#define ERROR(arg...) fprintf(stderr, arg)
#define DEBUG(arg...) fprintf(stderr, arg)
#define PRINT(arg...) fprintf(stdout, arg)
#define FATAL(arg...) fprintf(stderr, arg), exit(1)

#endif
