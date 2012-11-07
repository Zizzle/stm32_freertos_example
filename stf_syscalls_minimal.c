/**
 * <b>File:</b> stf_syscalls_minimal.c
 *
 * <b>Project:</b> FreeRTOS.org STM32 demo using Eclipse
 *
 * <b>Description:</b> This is the complete set of system definitions (primarily subroutines) required.
 * It implements the minimal functionality required to allow libc to link, and fail gracefully where OS services
 * are not available.
 *
 * For more information see the newlib documentation at http://sourceware.org/newlib/
 *
 * <b>Cereated:</b> 09/04/2009
 *
 * <dl>
 * <dt><b>Autor</b>:</dt>
 * <dd>Stefano Oliveri</dd>
 * <dt><b>E-mail:</b></dt>
 * <dd>software@stf12.net</dd>
 * </dl>
 */

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <time.h>
#include <sys/stat.h>
#include "FreeRTOS.h"

// Function declaration.
void _exit(int i);
int _open(const char *name, int flags, int mode);
int _read(int file, char *ptr, int len);
int _write(int file, char *buffer, unsigned int count);
int _lseek(int file, int ptr, int dir);
int _fstat(int file, struct stat *st);
int _link(char *old, char *new);
int _unlink(char *name);
int _stat(char *file, struct stat *st);
int _close(int file);
int _execve(char *name, char **argv, char **env);
int _fork();
int _getpid();
int _isatty(int file);
int _kill(int pid, int sig);
caddr_t _sbrk(int incr);
int times(struct tm *buf);
int _wait(int *status);

#undef errno
extern int errno;
char *__env[1] = {0};
char **__environ = __env;
extern unsigned int _heap;
extern unsigned int _eheap;
 caddr_t heap = NULL;


// Function definition.

void _exit(int i)
{
	printf("Program exit with code %d", i);
	while (1);
}

int _write(int file, char *buffer, unsigned int count)
{
	register int i;
	for (i=0; i<count; ++i) {
            //putChar(*buffer++);
            comm_put(*buffer++);
            //xSerialPutChar(NULL, *buffer++, 0);
	}

	return count;
}

int _close(int file)
{
	return -1;
}

int _fstat(int file, struct stat *st)
{
	st->st_mode = S_IFCHR;
	return 0;
}

int _isatty(int file)
{
	return 1;
}

int _lseek(int file, int ptr, int dir)
{
	return 0;
}

int _read(int file, char *ptr, int len)
{
	return 0;
}

caddr_t _sbrk(int incr)
{
    comm_puts("SBRK should be used!\r\n");
    return NULL;
}

int _open(const char *name, int flags, int mode)
{
	return -1;
}

int _link(char *old, char *new)
{
	errno = EMLINK;
	return -1;
}

int _unlink(char *name)
{
	errno = ENOENT;
	return -1;
}

int _stat(char *file, struct stat *st)
{
	st->st_mode = S_IFCHR;
	return 0;
}

int _execve(char *name, char **argv, char **env)
{
	errno = ENOMEM;
	return -1;
}

int _fork()
{
	errno = EAGAIN;
	return -1;
}

int _getpid()
{
	return 1;
}

int _kill(int pid, int sig)
{
	errno = EINVAL;
	return (-1);
}

int times(struct tm *buf)
{
	return -1;
}

int _wait(int *status)
{
	errno = ECHILD;
	return -1;
}

/* definition of block structure, copied from heap2 allocator */
typedef struct A_BLOCK_LINK
{
	struct A_BLOCK_LINK *pxNextFreeBlock;	/*<< The next free block in the list. */
	size_t xBlockSize;						/*<< The size of the free block. */
} xBlockLink;

static const unsigned short  heapSTRUCT_SIZE	= ( sizeof( xBlockLink ) + portBYTE_ALIGNMENT - ( sizeof( xBlockLink ) % portBYTE_ALIGNMENT ) );

_PTR _realloc_r(struct _reent *re, _PTR oldAddr, size_t newSize) {
	xBlockLink *block;
	size_t toCopy;
	void *newAddr;

	newAddr = pvPortMalloc(newSize);

	if (newAddr == NULL)
		return NULL;

	/* We need the block struct pointer to get the current size */
	block = oldAddr;
	block -= heapSTRUCT_SIZE;

	/* determine the size to be copied */
	toCopy = (newSize<block->xBlockSize)?(newSize):(block->xBlockSize);

	/* copy old block into new one */
	memcpy((void *)newAddr, (void *)oldAddr, (size_t)toCopy);

	vPortFree(oldAddr);
	
	return newAddr;
}

_PTR _calloc_r(struct _reent *re, size_t num, size_t size) {
	return pvPortMalloc(num*size);
}

_PTR _malloc_r(struct _reent *re, size_t size) {
	return pvPortMalloc(size);
}

_VOID _free_r(struct _reent *re, _PTR ptr) {
	vPortFree(ptr);
}

void *malloc(size_t size)
{
    return pvPortMalloc(size);
}
