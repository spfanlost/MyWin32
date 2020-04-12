#ifndef SHARE_H_INCLUDED
#define SHARE_H_INCLUDED
#include <stdio.h>

#include <vector>               // Add support for std::vector
#include <map>                  // Add support for std::map
#include <string>               // Add support for std::string
#include <sstream>              // Add support for stringstream
#include <assert.h>             // Add support for the assert macro
#include <stdio.h>              // Add support for C style printf, sprintf, etc.
#include <stdlib.h>             // Add support for C style character conversions atoi etc.
#include <tchar.h>              // Add support for C style TCHARs.

#define DEBUG
#define LEVEL "YM"

#define LOG_ERR(fmt, ...)    \
    printf("%s:%s:%d: " fmt "\n", \
    LEVEL, __FILE__, __LINE__, ## __VA_ARGS__)

#ifdef DEBUG
#define LOG_DBG(fmt, ...)    \
    printf("%s: " fmt "\n", \
    LEVEL, ## __VA_ARGS__)
#else
#define LOG_DBG(fmt, ...)
#endif

#define INI_NAME ".\\test.ini"
#define INI_SECTION1 "Section1"
#define INI_SECTION2 "Section2"
#define INI_KEY1_NAME "Key1"
#define INI_KEY2_NAME "Key2"
#define INI_DEF_STR "INI file access fail"
#define INI_DEF_VAL (66)

#endif // SHARE_H_INCLUDED
