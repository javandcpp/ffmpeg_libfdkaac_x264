// Copyright (C) 2002-2008 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef __IRR_COMPILE_CONFIG_H_INCLUDED__
#define __IRR_COMPILE_CONFIG_H_INCLUDED__

//! Irrlicht SDK Version
#define IRRLICHT_SDK_VERSION "1.4.1"

//! The defines for different operating system are:
//! _IRR_XBOX_PLATFORM_ for XBox
//! _IRR_WINDOWS_ for all irrlicht supported Windows versions
//! _IRR_WINDOWS_API_ for Windows or XBox
//! _IRR_LINUX_PLATFORM_ for Linux (it is defined here if no other os is defined)
//! _IRR_SOLARIS_PLATFORM_ for Solaris
//! _IRR_OSX_PLATFORM_ for Apple systems running OSX
//! _IRR_POSIX_API_ for Posix compatible systems
//! _IRR_USE_SDL_DEVICE_ for platform independent SDL framework
//! _IRR_USE_WINDOWS_DEVICE_ for Windows API based device
//! _IRR_USE_LINUX_DEVICE_ for X11 based device
//! _IRR_USE_OSX_DEVICE_ for Cocoa native windowing on OSX
//! Note: PLATFORM defines the OS specific layer, API can groups several platforms
//! DEVICE is the windowing system used, several PLATFORMs support more than one DEVICE
//! Moreover, the DEVICE defined here is not directly related to the Irrlicht devices created in the app (but may depend on each other).

//#define _IRR_USE_SDL_DEVICE_ 1

//! WIN32 for Windows32
//! WIN64 for Windows64
// The windows platform and API support SDL and WINDOW device
#define _IRR_WINDOWS_
#define _IRR_WINDOWS_API_
#define _IRR_USE_WINDOWS_DEVICE_

#include <stdio.h> // TODO: Although included elsewhere this is required at least for mingw

#define _IRR_COMPILE_WITH_DIRECT3D_9_
#define _IRR_COMPILE_WITH_OPENGL_

#define _IRR_OPENGL_USE_EXTPOINTER_
#define _IRR_COMPILE_WITH_GUI_
#define _IRR_COMPILE_WITH_ZLIB_
#define _IRR_USE_NON_SYSTEM_ZLIB_
#define _IRR_COMPILE_WITH_LIBJPEG_
#define _IRR_USE_NON_SYSTEM_JPEG_LIB_
#define _IRR_COMPILE_WITH_LIBPNG_
#define _IRR_USE_NON_SYSTEM_LIB_PNG_
#define _IRR_D3D_NO_SHADER_DEBUGGING

#ifdef IRRLICHT_EXPORTS //! DLL
  #define IRRLICHT_API __declspec(dllexport)
#else
  #define IRRLICHT_API __declspec(dllimport)
#endif
// Declare the calling convention.
#if defined(_STDCALL_SUPPORTED)
#define IRRCALLCONV __stdcall
#else
#define IRRCALLCONV __cdecl
#endif // STDCALL_SUPPORTED

#define BURNINGVIDEO_RENDERER_BEAUTIFUL

#define _IRR_COMPILE_WITH_SKINNED_MESH_SUPPORT_

#define _IRR_COMPILE_WITH_B3D_LOADER_
#define _IRR_COMPILE_WITH_MS3D_LOADER_
#define _IRR_COMPILE_WITH_X_LOADER_
#define _IRR_COMPILE_WITH_IRR_MESH_LOADER_
#define _IRR_COMPILE_WITH_IRR_WRITER_
#define _IRR_COMPILE_WITH_MD2_LOADER_
#define _IRR_COMPILE_WITH_MD3_LOADER_
#define _IRR_COMPILE_WITH_3DS_LOADER_
#define _IRR_COMPILE_WITH_COLLADA_LOADER_
#define _IRR_COMPILE_WITH_COLLADA_WRITER_
#define _IRR_COMPILE_WITH_CSM_LOADER_
#define _IRR_COMPILE_WITH_BSP_LOADER_
#define _IRR_COMPILE_WITH_DMF_LOADER_
#define _IRR_COMPILE_WITH_LMTS_LOADER_
#define _IRR_COMPILE_WITH_MY3D_LOADER_
#define _IRR_COMPILE_WITH_OBJ_LOADER_
#define _IRR_COMPILE_WITH_OCT_LOADER_
#define _IRR_COMPILE_WITH_OGRE_LOADER_
#define _IRR_COMPILE_WITH_STL_LOADER_
#define _IRR_COMPILE_WITH_STL_WRITER_
#define _IRR_COMPILE_WITH_BMP_LOADER_
#define _IRR_COMPILE_WITH_BMP_WRITER_
#define _IRR_COMPILE_WITH_JPG_LOADER_
#define _IRR_COMPILE_WITH_JPG_WRITER_
#define _IRR_COMPILE_WITH_PCX_LOADER_
#define _IRR_COMPILE_WITH_PCX_WRITER_
#define _IRR_COMPILE_WITH_PNG_LOADER_
#define _IRR_COMPILE_WITH_PNG_WRITER_
#define _IRR_COMPILE_WITH_PPM_LOADER_
#define _IRR_COMPILE_WITH_PPM_WRITER_
#define _IRR_COMPILE_WITH_PSD_LOADER_
#define _IRR_COMPILE_WITH_PSD_WRITER_
#define _IRR_COMPILE_WITH_TGA_LOADER_
#define _IRR_COMPILE_WITH_TGA_WRITER_


#endif // __IRR_COMPILE_CONFIG_H_INCLUDED__
