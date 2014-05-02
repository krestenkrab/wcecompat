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


#include <io.h>
#include <stdio.h>
#include <errno.h>
#include <conio.h>
#include "ts_string.h"

int _waccess( const wchar_t *path, int mode )
{
    SHFILEINFO fi;
    if( !SHGetFileInfo( path, 0, &fi, sizeof(fi), SHGFI_ATTRIBUTES ) )
    {
        errno = ENOENT;
        return -1;
    }
    // existence ?
    if( mode == 0 )
      {
	errno = ENOENT;
        return 0;
      }
    // write permission ?
    if( mode & 2 )
    {
        if( fi.dwAttributes & SFGAO_READONLY )
        {
            errno = EACCES;
            return -1;
        }
    }
    return 0;
}

int access(const char* path, int mode)
{
    wchar_t wpath[_MAX_PATH];
    
    if( !MultiByteToWideChar( CP_ACP, 0, path, -1, wpath, _MAX_PATH ) )
    {
        errno = ENOENT;
        return -1;
    }
    return _waccess( wpath, mode );
}


FILE* freopen(const char* filename, const char* mode, FILE* file)
{
  wchar_t* wfilename = ts_strdup_ascii_to_unicode(filename);
  wchar_t* wmode = (mode == NULL) ? NULL : ts_strdup_ascii_to_unicode(mode);
  FILE* result = _wfreopen(wfilename, wmode, file);
  free(wfilename);
  free(wmode);
  return result;
}

int rename(const char *oldfile, const char *newfile)
{
    int res;    
    size_t lenold;
    size_t lennew;
    wchar_t *wsold;
    wchar_t *wsnew;
    
    /* Covert filename buffer to Unicode. */

    /* Old filename */
    lenold = MultiByteToWideChar (CP_ACP, 0, oldfile, -1, NULL, 0) ;
    wsold = (wchar_t*)malloc(sizeof(wchar_t) * lenold);
    MultiByteToWideChar( CP_ACP, 0, oldfile, -1, wsold, lenold);
    
    /* New filename */
    lennew = MultiByteToWideChar (CP_ACP, 0, newfile, -1, NULL, 0) ;
    wsnew = (wchar_t*)malloc(sizeof(wchar_t) * lennew);
    MultiByteToWideChar(CP_ACP, 0, newfile, -1, wsnew, lennew);

    /* Delete file using Win32 CE API call */
    res = MoveFile(wsold, wsnew);
    
    /* Free wide-char string */
    free(wsold);
    free(wsnew);
    
    if (res)
        return 0; /* success */
    else
        return -1;
}

int _kbhit()
{
  return 0;
}
