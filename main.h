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

#define INI_NAME ".\\test.ini"
#define INI_SECTION1 "Section1"
#define INI_SECTION2 "Section2"
#define INI_KEY1_NAME "Key1"
#define INI_KEY2_NAME "Key2"
#define INI_DEF_STR "INI file access fail"
#define INI_DEF_VAL (66)

#endif // SHARE_H_INCLUDED
