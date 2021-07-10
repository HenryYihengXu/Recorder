#ifdef RECORDER_GOTCHA
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/time.h>
#include <stdarg.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/uio.h>
#include <sys/mman.h>
#include <search.h>
#include <assert.h>
#include <limits.h>
#include <pthread.h>

#ifndef HAVE_OFF64_T
typedef int64_t off64_t;
#endif

#define RECORDER_POSIX_DECL(func) __gotcha_wrap_##func

int RECORDER_POSIX_DECL(close)(int fd);
int RECORDER_POSIX_DECL(fclose)(FILE *fp);
int RECORDER_POSIX_DECL(fsync)(int fd);
int RECORDER_POSIX_DECL(fdatasync)(int fd);
void* RECORDER_POSIX_DECL(mmap64)(void *addr, size_t length, int prot, int flags, int fd, off64_t offset);
void* RECORDER_POSIX_DECL(mmap)(void *addr, size_t length, int prot, int flags, int fd, off_t offset);
int RECORDER_POSIX_DECL(msync)(void *addr, size_t length, int flags);
int RECORDER_POSIX_DECL(creat)(const char *path, mode_t mode);
int RECORDER_POSIX_DECL(creat64)(const char *path, mode_t mode);
int RECORDER_POSIX_DECL(open64)(const char *path, int flags, ...);
int RECORDER_POSIX_DECL(open)(const char *path, int flags, ...);
FILE* RECORDER_POSIX_DECL(fopen64)(const char *path, const char *mode);
FILE* RECORDER_POSIX_DECL(fopen)(const char *path, const char *mode);
int RECORDER_POSIX_DECL(__xstat)(int vers, const char *path, struct stat *buf);
int RECORDER_POSIX_DECL(__xstat64)(int vers, const char *path, struct stat64 *buf);
int RECORDER_POSIX_DECL(__lxstat)(int vers, const char *path, struct stat *buf);
int RECORDER_POSIX_DECL(__lxstat64)(int vers, const char *path, struct stat64 *buf);
int RECORDER_POSIX_DECL(__fxstat)(int vers, int fd, struct stat *buf);
int RECORDER_POSIX_DECL(__fxstat64)(int vers, int fd, struct stat64 *buf);
ssize_t RECORDER_POSIX_DECL(pread64)(int fd, void *buf, size_t count, off64_t offset);
ssize_t RECORDER_POSIX_DECL(pread)(int fd, void *buf, size_t count, off_t offset);
ssize_t RECORDER_POSIX_DECL(pwrite64)(int fd, const void *buf, size_t count, off64_t offset);
ssize_t RECORDER_POSIX_DECL(pwrite)(int fd, const void *buf, size_t count, off_t offset);
ssize_t RECORDER_POSIX_DECL(readv)(int fd, const struct iovec *iov, int iovcnt);
ssize_t RECORDER_POSIX_DECL(writev)(int fd, const struct iovec *iov, int iovcnt);
size_t RECORDER_POSIX_DECL(fread)(void *ptr, size_t size, size_t nmemb, FILE *stream);
size_t RECORDER_POSIX_DECL(fwrite)(const void *ptr, size_t size, size_t nmemb, FILE *stream);
// int RECORDER_POSIX_DECL(fprintf)(FILE *stream, const char *format, ...);
ssize_t RECORDER_POSIX_DECL(read)(int fd, void *buf, size_t count);
ssize_t RECORDER_POSIX_DECL(write)(int fd, const void *buf, size_t count);
int RECORDER_POSIX_DECL(fseek)(FILE *stream, long offset, int whence);
long RECORDER_POSIX_DECL(ftell)(FILE *stream);
off64_t RECORDER_POSIX_DECL(lseek64)(int fd, off64_t offset, int whence);
off_t RECORDER_POSIX_DECL(lseek)(int fd, off_t offset, int whence);
char* RECORDER_POSIX_DECL(getcwd)(char *buf, size_t size);
int RECORDER_POSIX_DECL(mkdir)(const char *pathname, mode_t mode);
int RECORDER_POSIX_DECL(rmdir)(const char *pathname);
int RECORDER_POSIX_DECL(chdir)(const char *path);
int RECORDER_POSIX_DECL(link)(const char *oldpath, const char *newpath);
int RECORDER_POSIX_DECL(unlink)(const char *pathname);
int RECORDER_POSIX_DECL(linkat)(int fd1, const char *path1, int fd2, const char *path2, int flag);
int RECORDER_POSIX_DECL(symlink)(const char *path1, const char *path2);
int RECORDER_POSIX_DECL(symlinkat)(const char *path1, int fd, const char *path2);
ssize_t RECORDER_POSIX_DECL(readlink)(const char *path, char *buf, size_t bufsize);
ssize_t RECORDER_POSIX_DECL(readlinkat)(int fd, const char *path, char *buf, size_t bufsize);
int RECORDER_POSIX_DECL(rename)(const char *oldpath, const char *newpath);
int RECORDER_POSIX_DECL(chmod)(const char *path, mode_t mode);
int RECORDER_POSIX_DECL(chown)(const char *path, uid_t owner, gid_t group);
int RECORDER_POSIX_DECL(lchown)(const char *path, uid_t owner, gid_t group);
int RECORDER_POSIX_DECL(utime)(const char *filename, const struct utimbuf *buf);
DIR* RECORDER_POSIX_DECL(opendir)(const char *name);
struct dirent* RECORDER_POSIX_DECL(readdir)(DIR *dir);
int RECORDER_POSIX_DECL(closedir)(DIR *dir);
// void RECORDER_POSIX_DECL(rewinddir)(DIR *dir);
int RECORDER_POSIX_DECL(__xmknod)(int ver, const char *path, mode_t mode, dev_t dev);
int RECORDER_POSIX_DECL(__xmknodat)(int ver, int fd, const char *path, mode_t mode, dev_t dev);

#ifndef DISABLE_FCNTL_TRACE
int RECORDER_POSIX_DECL(fcntl)(int fd, int cmd, ...);
#endif

int RECORDER_POSIX_DECL(dup)(int oldfd);
int RECORDER_POSIX_DECL(dup2)(int oldfd, int newfd);
int RECORDER_POSIX_DECL(pipe)(int pipefd[2]);
int RECORDER_POSIX_DECL(mkfifo)(const char *pathname, mode_t mode);
mode_t RECORDER_POSIX_DECL(umask)(mode_t mask);
FILE* RECORDER_POSIX_DECL(fdopen)(int fd, const char *mode);
int RECORDER_POSIX_DECL(fileno)(FILE *stream);
int RECORDER_POSIX_DECL(access)(const char *path, int amode);
int RECORDER_POSIX_DECL(faccessat)(int fd, const char *path, int amode, int flag);
FILE* RECORDER_POSIX_DECL(tmpfile)(void);
int RECORDER_POSIX_DECL(remove)(const char *path);
int RECORDER_POSIX_DECL(truncate)(const char *path, off_t length);
int RECORDER_POSIX_DECL(ftruncate)(int fd, off_t length);
int RECORDER_POSIX_DECL(fseeko)(FILE *stream, off_t offset, int whence);
off_t RECORDER_POSIX_DECL(ftello)(FILE *stream);

#endif