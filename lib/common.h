 #ifndef _COMMON_H_
 #define _COMMON_H_

#define __DEBUG

#ifdef __DEBUG
    #define DEBUG(format, args...) \
        printf("FAHW-Lib: " format, ## args)
#else
    #define DEBUG(format, args...)
#endif

#include <string.h>
#include <unistd.h> 
#include <stdlib.h> 
#include <sys/types.h> 
#include <sys/stat.h> 
#include <sys/ioctl.h> 
#include <fcntl.h>
#include <linux/fs.h> 
#include <errno.h> 
#include <stdarg.h>
#include <stdio.h>
#include <pthread.h>

extern void clearLastError();
extern void setLastError(const char *fmt, ...);

#define EXPORT
extern int writeValueToFile(char* fileName, char* buff);
extern int writeIntValueToFile(char* fileName, int value);
extern int readValueFromFile(char* fileName, char* buff, int len);
extern int readIntValueFromFile(char* fileName);

#define FILE_PATH_LENGTH           (128)

#endif
