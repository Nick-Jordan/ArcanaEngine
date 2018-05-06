#pragma once

#ifdef _WIN32
#define ARCANA_WINDOWS
#ifdef _WIN64
#define ARCANA_WINDOWS_64
#else
#define ARCANA_WINDOWS_32
#endif
/*#elif __APPLE__
#include "TargetConditionals.h"
#if TARGET_IPHONE_SIMULATOR
// iOS Simulator
#elif TARGET_OS_IPHONE
// iOS device
#elif TARGET_OS_MAC
// Other kinds of Mac OS
#else
#   error "Unknown Apple platform"
#endif*/
#elif __linux__
#define ARCANA_LINUX
#elif __unix__ // all unices not caught above
#define ARCANA_LINUX
#elif defined(_POSIX_VERSION)
#define ARCANA_POSIX
#else
#   error "Unknown compiler"
#endif
