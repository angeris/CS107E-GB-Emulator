#ifndef PRINTF_DEFS
#define PRINTF_DEFS

#include <stdarg.h>
#include "uart.h"

#define F_DEC 'd'
#define F_HEX 'x'
#define F_BIN 'b'
#define F_CHR 'c'
#define F_STR 's'

#define BUF_SIZE 256

#define MIN(X,Y) (((X)<(Y))?(X):(Y))
#define MAX(X,Y) (((X)>(Y))?(X):(Y))
#define ABS(X) (((X)<0)?-(X):(X))

char* inttobin(char *buf, int n, unsigned val, int width); 
char* inttodec(char *buf, int n, int val, int width); 
char* inttohex(char *buf, int n, unsigned val, int width); 

int digsize(int i, int base);

void strapp(char *s, char *q, int start, int len); 
int _strlen(const char *s);

int is_valid_formatting_str(const char *s, int idx, int len);
int replace_per(char *buf, int start, int len, unsigned val, int width, char type);

int snprintf(char *s, int n, char *format, ...);
int vsnprintf(char *s, int n, const char *format, va_list v_list);
int printf(char *format, ...);

#endif
