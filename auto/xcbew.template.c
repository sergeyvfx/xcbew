/*
 * Copyright 2018 Blender Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License
 */

#ifdef _MSC_VER
#  if _MSC_VER < 1900
#    define snprintf _snprintf
#  endif
#  define popen _popen
#  define pclose _pclose
#  define _CRT_SECURE_NO_WARNINGS
#endif

#include <xcbew.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#ifdef _WIN32
#  define WIN32_LEAN_AND_MEAN
#  define VC_EXTRALEAN
#  include <windows.h>

/* Utility macros. */

typedef HMODULE DynamicLibrary;

#  define dynamic_library_open(path)         LoadLibraryA(path)
#  define dynamic_library_close(lib)         FreeLibrary(lib)
#  define dynamic_library_find(lib, symbol)  GetProcAddress(lib, symbol)
#else
#  include <dlfcn.h>

typedef void* DynamicLibrary;

#  define dynamic_library_open(path)         dlopen(path, RTLD_NOW)
#  define dynamic_library_close(lib)         dlclose(lib)
#  define dynamic_library_find(lib, symbol)  dlsym(lib, symbol)
#endif

#define GLUE_IMPL(A, B) A ## B
#define GLUE(A, B) GLUE_IMPL(A, B)

#define IMPL_SUFFIX _impl

#define _LIBRARY_FIND_CHECKED(lib, name) \
        GLUE(name, IMPL_SUFFIX) = (t##name)dynamic_library_find(lib, #name); \
        assert(name);

#define _LIBRARY_FIND(lib, name) \
        GLUE(name, IMPL_SUFFIX) = (t##name)dynamic_library_find(lib, #name);

#define XCB_LIBRARY_FIND_CHECKED(name) \
        _LIBRARY_FIND_CHECKED(xcb_lib, name)
#define XCB_LIBRARY_FIND(name) _LIBRARY_FIND(xcb_lib, name)

#define XPROTO_LIBRARY_FIND_CHECKED(name) \
        _LIBRARY_FIND_CHECKED(xcb_lib, name)
#define XPROTO_LIBRARY_FIND(name) _LIBRARY_FIND(xcb_lib, name)

static DynamicLibrary xcb_lib;

/* Function definitions. */

%functions_definitions%

/* Function wrappers. */

%functions_wrappers%

/* Main wrangling logic. */

static DynamicLibrary dynamic_library_open_find(const char** paths) {
  int i = 0;
  while (paths[i] != NULL) {
      DynamicLibrary lib = dynamic_library_open(paths[i]);
      if (lib != NULL) {
        return lib;
      }
      ++i;
  }
  return NULL;
}

/* Implementation function. */
static void xcbewExit(void) {
  if (xcb_lib != NULL) {
    /*  Ignore errors. */
    dynamic_library_close(xcb_lib);
    xcb_lib = NULL;
  }
}

int xcbewInit(void) {
  /* Library paths. */
#ifdef _WIN32
  const char* xcb_paths[] = {NULL};
#elif defined(__APPLE__)
  const char* xcb_paths[] = {NULL};
#else
  const char* xcb_paths[] = {"libxcb.so",
                             "libxcb.so.1",
                             NULL};
#endif
  static int initialized = 0;
  static int result = 0;
  int error;

  if (initialized) {
    return result;
  }

  initialized = 1;

  error = atexit(xcbewExit);
  if (error) {
    result = XCBEW_ERROR_ATEXIT_FAILED;
    return result;
  }

  /* Load library. */
  xcb_lib = dynamic_library_open_find(xcb_paths);
  if (xcb_lib == NULL) {
    result = XCBEW_ERROR_OPEN_FAILED;
    return result;
  }

  /* Fetch all function pointers. */

%functions_dynload%

  result = XCBEW_SUCCESS;
  return result;
}

const char* xcbewErrorString(int error) {
  switch (error) {
    case XCBEW_SUCCESS: return "SUCCESS";
    case XCBEW_ERROR_OPEN_FAILED: return "OPEN_FAILED";
    case XCBEW_ERROR_ATEXIT_FAILED: return "ATEXIT_FAILED";
  }
  return "UNKNOWN";
}
