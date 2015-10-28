/******************************************************************************
	CCAV:  Media play library based on Qt(UI),c/c++11 and FFmpeg
	Copyright (C) 2015-2016 Juno <junowendy@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/


#ifndef CCAV_GLOBAL_H
#define CCAV_GLOBAL_H

#include <qglobal.h>
#include <master_dptr.h>


#define CCAV_MAJOR 1	//((CCAV_VERSION&0xff0000)>>16)
#define CCAV_MINOR 1	//((CCAV_VERSION&0xff00)>>8)
#define CCAV_PATCH 1	//(CCAV_VERSION&0xff)

#define CCAV_VERSION_CHK(major, minor, patch) \
    (((major&0xff)<<16) | ((minor&0xff)<<8) | (patch&0xff))

#define CCAV_VERSION VERSION_CHK(CCAV_MAJOR, CCAV_MINOR, CCAV_PATCH)

#define _TOSTR(x)   #x
#define TOSTR(x)  _TOSTR(x)

static const char* const CCAV_version_string = TOSTR(CCAV_MAJOR) "." TOSTR(CCAV_MINOR) "." TOSTR(CCAV_PATCH) "(" __DATE__ ", " __TIME__ ")";
#define CCAV_VERSION_STR		TOSTR(CCAV_MAJOR) "." TOSTR(CCAV_MINOR) "." TOSTR(CCAV_PATCH)
#define CCAV_VERSION_STR_LONG	CCAV_VERSION_STR

#define GCC_VERSION_AT_LEAST(major, minor, patch) 0

#if defined(Q_DLL_LIBRARY)
#  undef Q_EXPORT
#  define Q_EXPORT Q_DECL_EXPORT
#else
#  undef Q_EXPORT
#  define Q_EXPORT //Q_DECL_IMPORT 
#endif

#endif // CCAV_GLOBAL_H

