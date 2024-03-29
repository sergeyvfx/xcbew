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

#ifndef __XCBEW_H__
#define __XCBEW_H__

#include <stdint.h>
#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Defines. */
#define XCBEW_VERSION_MAJOR 1
#define XCBEW_VERSION_MINOR 0

%definitions_all%

/* Types */

%types_definitions%

/* Function types. */

%functions_typedefs%

/* Wrapper function declarations. */

%functions_wrapper_declarations%

/* Function declarations. */

%functions_declarations%

/* XCBEW related API */

enum {
  XCBEW_SUCCESS = 0,
  XCBEW_ERROR_OPEN_FAILED = -1,
  XCBEW_ERROR_ATEXIT_FAILED = -2,
};

int xcbewInit(void);
const char* xcbewErrorString(int error);

#ifdef __cplusplus
}
#endif

#endif  /* __XCBEW_H__ */
