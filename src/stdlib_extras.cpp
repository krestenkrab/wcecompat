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


#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <windows.h>
#include "ts_string.h"

int _fmode;		/* default file translation mode */


void abort(void)
{
	exit(errno);
}


void* bsearch(const void* key, const void* base, size_t nmemb, size_t size,
		int (*compar)(const void*, const void*))
{
	while (nmemb-- > 0)
	{
		if (compar(key, base) == 0)
			return (void*)base;

		base = (char*)base + size;
	}

	return NULL;
}

int system(const char *command)
{
	SHELLEXECUTEINFO sei;
	unsigned long exit_code = 0;
	BOOL ret;
	wchar_t *wpath;
	wchar_t *wfile;
	wchar_t *wdir;
	wchar_t *wparams;
	wchar_t stopch;

	if(command == 0) {
		return -1;
	}

	// trim front
	while(*command == ' ' || *command == '\t') {
		command++;
	}

	// if starting char is a quote then look for end quote
	if(*command == '\"') {
		command++;
		stopch = L'\"';
	} else {
		stopch = L' ';
	}

	// create wide version of command
	wpath = ts_strdup_ascii_to_unicode(command);

	// find the end of the path and start of parameters
	wparams = wcschr(wpath, stopch);
	if(wparams != NULL) {
		*wparams = L'\0';
		wparams++;
	}

	// find the filename
	wfile = wcsrchr(wpath, L'\\');
	if(wfile != NULL) {
		wfile++;
	} else {
		wfile = wpath;
	}

	// copy the directory
	wdir = (wchar_t*) malloc((wfile-wpath + 1) * sizeof(wchar_t));
	wcsncpy(wdir, wpath, wfile-wpath);
	wdir[wfile-wpath] = L'\0';

	// set up call
	memset(&sei, 0, sizeof(sei));
	sei.cbSize = sizeof(sei);
	sei.lpFile = wpath;
	sei.lpParameters = wparams;
	sei.lpDirectory = wdir;
	sei.fMask = SEE_MASK_NOCLOSEPROCESS;
	sei.nShow = SW_SHOWNORMAL;

	// call now
	ret = ShellExecuteEx(&sei);

	if(ret) {
		// wait for process to exit
		WaitForSingleObject(sei.hProcess, INFINITE);
		GetExitCodeProcess(sei.hProcess, &exit_code);
		CloseHandle(sei.hProcess);
	} else {
		exit_code = -1;
	}

	free(wdir);
	free(wpath);

	return exit_code;
}
