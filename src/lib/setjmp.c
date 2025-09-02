#include <lib/setjmp.h>

extern int setjmp(jmp_buf_t *buf);

extern void longjmp(jmp_buf_t *buf, int arg);