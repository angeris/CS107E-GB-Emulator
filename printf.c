#include "printf.h"

char* inttobin(char *buf, int n, unsigned val, int width) {
    buf[width] = '\0';
    for(int i=width-1; i>=0; i--) {
        buf[i] = (val >> (width-1-i)) & 1 ? '1' : '0';
    }
    return buf;
}

char* inttodec(char *buf, int n, int val, int width)  {
    int neg = val < 0;
    val = ABS(val);
    buf[width] = '\0';
    for(int i=width-1; i>=0; i--) {
        buf[i] = '0' + val%10;
        val /= 10;
    }
    if(neg) buf[0] = '-';
    return buf;
}

char* inttohex(char *buf, int n, unsigned val, int width) {
    buf[width] = '\0';
    int t=0;
    for(int i=width-1; i>=0; i--) {
        t = val%16;
        if(t>=10) {
            buf[i] = 'a' + (t-10);
        } else {
            buf[i] = '0' + t; 
        }
        val /= 16;
    }
    return buf;
}

int digsize(int i, int base) {
    int t = 1;
    if(i<0) t++;
    i = ABS(i);
    switch(base) {
        case 10:
            while(i/=10) 
                t++;
            break;
        case 2:
            while(i/=2)
                t++;
            break;
        case 16:
            while(i/=16)
                t++;
            break;
        default:
            return -1;
    }
    return t;
}

/* 
 * 's' is the original buffer to which we are appending to
 * 'q' is what we wish to append
 * 'start' is the beginning index to start appending to
 * 'len' is the length fo the total buffer
 */

void strapp(char *s, char *q, int start, int len) {
    int t = 0;
    while(t < len-start && q[t]!='\0') {
        s[t+start] = q[t];
        t++;
    }
    if(t == len-start) s[t+start-1] = '\0';
    else s[t+start] = '\0';
}

int _strlen(const char *s) {
    const char *p;
    p=s;
    while(*p) p++;
    return p-s;
}

int is_valid_formatting_str(const char *s, int idx, int len) {
    if(s[idx] != '%') return 0;
    if(++idx >= len) return 0;

    char t = s[idx];

    if(s[idx] == '0') {
        if(++idx >= len) return 0;
        t = s[idx];
        if(t <= '0' || t >= '9') return 0;
        if(++idx >= len) return 0;

        t = s[idx];

        if(t == F_DEC || t == F_HEX || t == F_BIN) return 1;
        else return 0;
    } 

    if(t == F_DEC || t == F_HEX || t == F_BIN || t == F_CHR || t == F_STR)
        return 1;

    return 0;
}

/*
 * Given some start index, this will write the given value in the given format
 * starting at position 'start.' It will also only write to, at most, the given
 * size of the buffer in question. If there is an invaid assignment, it will
 * return -1, or, if that's not the case, it will return 1+ the last-assigned 
 * index in the buffer. Writing any nonpositive value to width will try to
 * fit the entire number into the buffer (or as much as can be done without
 * overflowing). 
 */

int replace_per(char *buf, int start, int len, unsigned val, int width, char type) {
    switch(type) {
        char *p;

    case F_CHR:
        buf[start] = (char)val;
        return start+1;
    case F_STR:
        strapp(buf, (char*)val, start, len);
        return MIN(len-1, start+_strlen((char*)val));
    case F_BIN:
        if(width==0) width = digsize((int)val, 2);
        p = inttobin(buf+start, len-start, (int)val, width);
        return _strlen(p)+start;
    case F_DEC:
        if(width==0) width = digsize((int)val, 10);
        p = inttodec(buf+start, len-start, (int)val, width);
        return _strlen(p)+start;
    case F_HEX:
        if(width==0) width = digsize((int)val, 16);
        p = inttohex(buf+start, len-start, (int)val, width);
        return _strlen(p)+start;
    default:
        return -1;
    }
}

int snprintf(char *s, int n, char *format, ...) {
    va_list v;
    va_start(v, format);

    return vsnprintf(s, n, format, v);
}

int vsnprintf(char *s, int n, const char *format, va_list v_list) {
    int p = 0;
    int f = 0;

    while(p<n) {
        s[p] = format[f];
        if(s[p] == '\0') 
            return 0;
        if(is_valid_formatting_str(format, f, n)) {
            if(format[f+1]=='0') {
                p = replace_per(s, p, n, va_arg(v_list, unsigned), format[f+2]-'0', format[f+3]); 
                f+=4;
                if(p<0) return -1;
                continue;
            } else {
                p = replace_per(s, p, n, va_arg(v_list, unsigned), 0, format[f+1]);
                f+=2;
                if(p<0) return -1;
                continue;
            }
        }
        p++;
        f++;
    }
    s[n-1] = '\0';
    return 0;
}

int printf(char *format, ...) {
    char buf[256];
    char *p = buf;
    va_list v_list;
    va_start(v_list, format);

    int ret = vsnprintf(buf, 256, format, v_list); 

    while(*p != '\0') uart_putc(*(p++));

    uart_flush();

    return ret;
}

