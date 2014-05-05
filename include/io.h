/*  wcecompat: Windows CE C Runtime Library "compatibility" library.
 *
 *  Copyright (C) 2001-2002 Essemer Pty Ltd.  All rights reserved.
 *  http://www.essemer.com.au/
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */


#ifndef __wcecompat__IO_H__
#define __wcecompat__IO_H__

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif


int __cdecl _access(const char *pathname, int mode);
int __cdecl _open(const char* filename, int flags, int mode);
int __cdecl _wopen(const wchar_t* filename, int flags, int mode);
  // int __cdecl _wopen(const unsigned short* filename, int flags, int mode);
int __cdecl _close(int fd);
long __cdecl _lseek(int fd, long offset, int whence);
int __cdecl _read(int fd, void *buffer, unsigned int count);
int __cdecl _write(int fd, const void *buffer, unsigned int count);
int __cdecl _unlink(const char *pathname);
int __cdecl _ftruncate(int fd, long length);
int __cdecl _setmode(int fd, int mode);

#define access _access
#define open _open
#define wopen _wopen
#define wopen _wopen
#define close _close
#define lseek _lseek
#define read _read
#define write _write
#define unlink _unlink
#define ftruncate _ftruncate
#define setmode _setmode


#ifndef O_NONBLOCK      /* Non Blocking Open */
# define O_NONBLOCK      00004
#endif

#ifndef S_ISDIR
# define S_ISDIR(mode)  (((mode) & (_S_IFMT)) == (_S_IFDIR))
#endif /* S_ISDIR */

#ifndef S_ISREG
# define S_ISREG(mode)  (((mode) & (_S_IFMT)) == (_S_IFREG))
#endif /* S_ISREG */

#ifndef S_ISLNK
# define S_ISLNK(mode)  (((mode) & _S_IFMT) == S_IFLNK)
#endif /* S_ISLNK */

#ifndef S_IXUSR
# define S_IXUSR                        0000100 /* execute/search permission, */
# define S_IXGRP                        0000010 /* execute/search permission, */
# define S_IXOTH                        0000001 /* execute/search permission, */
# define _S_IWUSR                       0000200 /* write permission, */
# define S_IWUSR                        _S_IWUSR        /* write permission, owner */
# define S_IWGRP                        0000020 /* write permission, group */
# define S_IWOTH                        0000002 /* write permission, other */
# define S_IRUSR                        0000400 /* read permission, owner */
# define S_IRGRP                        0000040 /* read permission, group */
# define S_IROTH                        0000004 /* read permission, other */
# define S_IRWXU                        0000700 /* read, write, execute */
# define S_IRWXG                        0000070 /* read, write, execute */
# define S_IRWXO                        0000007 /* read, write, execute */
#endif /* S_IXUSR */


#ifdef __cplusplus
}
#endif


#endif // __wcecompat__IO_H__
