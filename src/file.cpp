//-------------------------------------------------------------------------
// <copyright file="file.c" company="Adeneo">
//  Copyright (c) Microsoft Corporation.  All rights reserved.
//
//    The use and distribution terms for this software are covered by the
//    Limited Permissive License (Ms-LPL) 
//    which can be found in the file LPL.txt at the root of this distribution.
//    By using this software in any fashion, you are agreeing to be bound by
//    the terms of this license.
//
//    The software is licensed "as-is."
//
//    You must not remove this notice, or any other, from this software.
// </copyright> 
//-------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//! \addtogroup	SSHCOmpat
//! @{
//!
//! All rights reserved ADENEO SAS 2005
//!
//! \file		file.c
//!
//! \brief		file related functions
//!
//! 
//-----------------------------------------------------------------------------

// System include
#include "ts_string.h"
#include <windows.h>
#include "io.h"
#include "errno.h"
#include "strings.h"
#include "time.h"
#include <sys/stat.h>
#include <fcntl.h>
#include "internal.h"

int _setmode(int fd,  int mode)
{
  return O_BINARY;
}

int _wopen(const WCHAR* wFileName, int flags, int mode);

int _open(const char* filename, int flags, int mode)
{
  WCHAR wFileName[MAX_PATH];
  ascii2unicode(filename, wFileName);
  return _wopen(wFileName, flags, mode);
}

int _wopen(const WCHAR* wFileName, int flags, int mode)
{
	DWORD dwDesiredAccess = 0;
	DWORD dwCreateDispo = 0;
	HANDLE hFile;


	if ((flags & _O_RDONLY) && (flags & _O_WRONLY))
	{
		SET_ERRNO(EINVAL);
		return -1;
	}

	if (flags & _O_RDONLY)
	{
		dwDesiredAccess |= GENERIC_READ;
	}
	if (flags & _O_WRONLY)
	{
		dwDesiredAccess |= GENERIC_WRITE;
	}
	if (flags & _O_RDWR)
	{
		dwDesiredAccess |= GENERIC_WRITE | GENERIC_READ;
	}
	
	
	if (flags & _O_CREAT)
	{
		if (flags & O_EXCL)
		{
			dwCreateDispo |= CREATE_NEW;
		}
		else
		{
			if (flags & O_TRUNC)
			{
				dwCreateDispo |= CREATE_ALWAYS;
			}
			else
			{
				dwCreateDispo |= OPEN_ALWAYS;
			}
		}
	}
	else
	{
		dwCreateDispo |= OPEN_EXISTING;
		if (flags & O_TRUNC)
		{
			dwCreateDispo |= TRUNCATE_EXISTING;
		}
	}
	


	if (dwCreateDispo == 0)
	{
		dwCreateDispo = OPEN_EXISTING;
	}
	if (dwDesiredAccess == 0)
	{
		dwDesiredAccess = GENERIC_READ;
	}

	hFile = CreateFile(wFileName,dwDesiredAccess,FILE_SHARE_WRITE | FILE_SHARE_READ,NULL,dwCreateDispo,FILE_ATTRIBUTE_NORMAL,NULL);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		SET_ERRNO(-1);
		return -1;
	}


	if (flags & _O_APPEND)
	{
		SetFilePointer(hFile,0,NULL,FILE_END);
	}


	if ((int) hFile <0)
	{
		// !!!!!! on Unix it is used sometimes considered as an error (error is -1)
		DEBUGCHK(0);
	}

	return (int) hFile;

}

FILE* fdopen(int fildes, const char *mode)
{
  WCHAR* wMode = ts_strdup_ascii_to_unicode(mode);
  FILE* file = _wfdopen((HANDLE)fildes,wMode);		
  free(wMode);	
  return file;
}


int _close(int fd)
{
	if (CloseHandle((HANDLE) fd))
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

int _read(int fd, void *buffer, unsigned int count)
{
	DWORD dwRead = 0;
	if (ReadFile((HANDLE)fd,buffer,count,&dwRead,NULL))
	{
		return dwRead;
	}
	else
	{
		return -1;
	}	
}

int _write(int fd, const void *buffer, unsigned int count)
{
	DWORD dwWritten = 0;
	if (WriteFile((HANDLE)fd,buffer,count,&dwWritten,NULL))
	{
		return dwWritten;
	}
	else
	{
		return -1;
	}	
}

long _lseek(int fd, long offset, int whence)
{	
	DWORD dwMethod,dwDistance;
	switch (whence)
	{
		case SEEK_SET:
			dwMethod = FILE_BEGIN;
		break;
		case SEEK_CUR:
			dwMethod = FILE_CURRENT;
		break;
		case SEEK_END:
			dwMethod = FILE_END;
		break;
		default:
			return -1;
	}
	
	dwDistance = SetFilePointer((HANDLE)fd,offset,NULL,dwMethod);
	if (dwDistance == (DWORD)-1)
	{
		return -1;
	}
	else
	{
		return (long) dwDistance;
	}
}


int unlink(const char* pathname)
{
	BOOL bSuccess;
	WCHAR* wPathName = ts_strdup_ascii_to_unicode(pathname);
	bSuccess = DeleteFile(wPathName);
	free(wPathName);	
	if (bSuccess)
	{
		return 0;	
	}
	else
	{			
		return -1;
	}	
}

void __cdecl rewind(FILE *stream)
{
	fseek(stream, 0L, SEEK_SET);
}


int stat(const char* filename, struct stat* st)
{
	WCHAR	*wFileName;
	WIN32_FILE_ATTRIBUTE_DATA	s;
	
	if (filename == NULL || st == NULL)
	{
		SET_ERRNO( EINVAL );
		return -1;
	}

	wFileName = ts_strdup_ascii_to_unicode(filename);	
	if (!GetFileAttributesEx(wFileName, GetFileExInfoStandard, (LPVOID)&s))
	{
		free(wFileName);
		SET_ERRNO( ENOENT );
		return -1;
	}
	
	st->st_mode = 0;
	if (s.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
	{
		st->st_mode |= _S_IFDIR;
		st->st_mode |= S_IEXEC | S_IXGRP | S_IXOTH;
	}
	else
	{
		st->st_mode |= S_IFREG;
	}
	if (!(s.dwFileAttributes & (FILE_ATTRIBUTE_READONLY | FILE_ATTRIBUTE_INROM)))
	{
		st->st_mode |= S_IWRITE | S_IWGRP | S_IWOTH;
	}
	if (!(s.dwFileAttributes & (FILE_ATTRIBUTE_ROMMODULE)))
	{
		st->st_mode |= S_IREAD | S_IROTH | S_IRGRP;	// TODO: assuming readable, but this may not be the case
	}
	else
	{
		st->st_mode &= ~(S_IREAD | S_IROTH | S_IRGRP | S_IWRITE | S_IWGRP | S_IWOTH);	// ROM module is executable but nor readable nor writeable
		st->st_mode |= S_IEXEC | S_IXGRP | S_IXOTH;	// ROM module is executable
	}

	
	st->st_size = s.nFileSizeLow;
	st->st_atime = w32_filetime_to_time_t(&s.ftLastAccessTime);
	st->st_mtime = w32_filetime_to_time_t(&s.ftLastWriteTime);
	st->st_ctime = w32_filetime_to_time_t(&s.ftCreationTime);
	st->st_dev = 0;
	st->st_ino = 0;
	st->st_uid = 0;
	st->st_gid = 0;
	st->st_rdev = 0;	
	st->st_nlink = 1;
	
	free(wFileName);
	return 0;
}


int lstat(const char* filename, struct stat* st)
{
	return stat(filename, st);
}

int fstat(int desc, struct stat* st)
{
	HANDLE h;
	BY_HANDLE_FILE_INFORMATION info;

	h = (HANDLE) desc;
	if (h == INVALID_HANDLE_VALUE || st == NULL)
	{
		SET_ERRNO( EINVAL );
		return -1;
	}

	
	if (!GetFileInformationByHandle(h, &info))
	{
		SET_ERRNO( ENOENT );
		return -1;
	}

	st->st_mode = 0;

	if (info.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
	{
		st->st_mode |= _S_IFDIR;
		st->st_mode |= S_IEXEC | S_IXGRP | S_IXOTH;
	}
	else
	{
		st->st_mode |= S_IFREG;		
	}
	
	if (!(info.dwFileAttributes & (FILE_ATTRIBUTE_READONLY | FILE_ATTRIBUTE_INROM)))
	{
		st->st_mode |= S_IWRITE | S_IWGRP | S_IWOTH;
	}

	if (!(info.dwFileAttributes & (FILE_ATTRIBUTE_ROMMODULE)))
	{
		st->st_mode |= S_IREAD | S_IROTH | S_IRGRP;
	}
	else
	{
		st->st_mode &= ~(S_IREAD | S_IROTH | S_IRGRP | S_IWRITE | S_IWGRP | S_IWOTH); // ROM module is executable but nor readable nor writeable
		st->st_mode |= S_IEXEC | S_IXGRP | S_IXOTH;	// ROM module is executable
	}

	st->st_size = info.nFileSizeLow;
	st->st_atime = w32_filetime_to_time_t(&info.ftLastAccessTime);
	st->st_mtime = w32_filetime_to_time_t(&info.ftLastWriteTime);
	st->st_ctime = w32_filetime_to_time_t(&info.ftCreationTime);
	st->st_dev = 0;
	st->st_ino = 0;
	st->st_uid = 0;
	st->st_gid = 0;
	st->st_rdev = 0;	
	st->st_nlink = 1;

	return 0;
}


int truncate(const char *path, long length)
{
	int iRet = -1;
	HANDLE h;
	WCHAR* wFileName = ts_strdup_ascii_to_unicode(path);
	
	h = CreateFile(wFileName,GENERIC_WRITE,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if (h != INVALID_HANDLE_VALUE)
	{
		iRet = ftruncate((int)h,length);
		CloseHandle(h);
	}

	free(wFileName);

	return iRet;
}

int ftruncate(int fd, long length)
{
	int iRet = -1;

	if (SetFilePointer((HANDLE)fd,length,NULL,FILE_BEGIN))
	{
		if (SetEndOfFile((HANDLE)fd))
		{
			iRet = 0;
		}
	}
	return iRet;
}

int _mkdir(const char *pathname, int mode)
{

	BOOL bRet;
	WCHAR* wzDirectoryName = ts_strdup_ascii_to_unicode(pathname);
	bRet = CreateDirectory(wzDirectoryName,NULL);
	free(wzDirectoryName);

	if (bRet)
	{
		return 0;
	}
	switch(GetLastError())
	{
		case ERROR_PATH_NOT_FOUND:
			SET_ERRNO(ENOENT);
		break;
		default:
			SET_ERRNO(0);
			break;
	}
	return -1;
}


// End of Doxygen group SSHCompat
//! @}
