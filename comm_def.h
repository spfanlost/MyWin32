/**
 * @file    common_def.h
 * @author  meng_yu
 * @brief   common use define header file
 * @version 0.0.1
 * @date    2020-04-19
 *
 * @copyright Copyright (c) 2020 imyumeng@qq.com All rigthts reserved.
 */

#ifndef _COMMON_DEF_h_
#define _COMMON_DEF_h_

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>

/* Exported types ------------------------------------------------------------*/
typedef signed char char_t;
typedef signed short int short_t;
typedef signed int int_t;
typedef signed long int long_t;
typedef unsigned char byte_t;
typedef unsigned short int word_t;
typedef unsigned int dword_t;
typedef unsigned long int qword_t;

enum ASCII
{
    NUL = 0,
    SOH,
    STX,
    ETX,
    EOT,
    ENQ,
    ACK,
    BEL,
    BS,
    HT,
    LF,
    VT,
    FF,
    CR,
    SO,
    SI,
    DLE,
    DC1,
    DC2,
    DC3,
    DC4,
    NAK,
    SYN,
    ETB,
    CAN,
    EM,
    SUB,
    ESC,
    FS,
    GS,
    RS,
    US,
    SPACE, //32
    DEL = 127
};

/* Exported macro ------------------------------------------------------------*/
/* definition macro */
#ifndef DISABLE
#define DISABLE 0
#endif
#ifndef ENABLE
#define ENABLE 1
#endif
#ifndef SUCCEED
#define SUCCEED 1
#endif
#ifndef FAILED
#define FAILED 0
#endif
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif
#ifndef NULL
#ifdef __cplusplus // EC++
#define NULL 0
#else
#define NULL ((void *)0)
#endif
#endif

//#define TCD(tf, en) { tf, #tf, en, FAILED }

//get array element size
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
//get field in struct's offset
#define TYPE_OFST(type, field) ((dword_t) & ((type *)0)->field)
//get field in struct's size
#define TYPE_SIZE(type, field) sizeof(((type *)0)->field)

#define CONTAINER_OFST(ptr, type, member) \
    (type *)((char *)(ptr) - (char *)&((type *)0)->member)

#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

#define MEM32_GET(adr) (*((volatile dword_t *)(adr)))
#define MEM16_GET(adr) (*((volatile word_t *)(adr)))
#define MEM8_GET(adr) (*((volatile byte_t *)(adr)))
#define REG_GET(adr) (*((volatile dword_t *)(adr)))

#define MEM32_SET(adr, val) (*((volatile dword_t *)(adr)) = (dword_t)(val))
#define MEM16_SET(adr, val) (*((volatile word_t *)(adr)) = (word_t)(val))
#define MEM8_SET(adr, val) (*((volatile byte_t *)(adr)) = (byte_t)(val))
#define REG_SET(adr, val) (*((volatile dword_t *)(adr)) = (dword_t)(val))

#define VALUE_PTR(var) ((byte_t *)(void *)&(var))

#define RAND_INIT() (srand(time(NULL)))
#define BYTE_RAND() ((byte_t)rand())
#define WORD_RAND() ((word_t)rand())
#define DWORD_RAND() ((dword_t)rand() << 16 | rand())
#define RAND_RANGE(s, e) (rand() % ((e) - (s) + 1) + (s))

#define UPCASE(c) (((c) >= 'a' && (c) <= 'z') ? ((c)-0x20) : (c))
#define DECCHK(c) ((c) >= '0' && (c) <= '9')
#define HEXCHK(c) (((c) >= '0' && (c) <= '9') || ((c) >= 'A' && (c) <= 'F') || ((c) >= 'a' && (c) <= 'f'))

//--------------------------------------------------------------------------------
#define LOG_DBUG 0x10000000
#define LOG_INFO 0x20000000
#define LOG_WARN 0x40000000
#define LOG_ERROR 0x80000000
#define ALL_LOG 0xFFFFFFFF
#define LOG_FILTER (LOG_INFO | LOG_WARN | LOG_ERROR) //|LOG_DBUG)

// #define LOG_FILTER (ALL_LOG)
/*
ANSI stand define:
_LINE_
_FILE_
_DATE_ //M/D/Y,source code -> target date
_TIME_ //H:M:S,source code -> target time
_STDC_
*/
#define LOG_PRINT_IF printf
#define LOG(filter, fmt, ...)                                                                                              \
    do                                                                                                                     \
    {                                                                                                                      \
        if (filter & LOG_FILTER)                                                                                           \
        {                                                                                                                  \
            if ((filter & LOG_ERROR) == LOG_ERROR)                                                                         \
            {                                                                                                              \
                LOG_PRINT_IF("\033[31mERROR!!! %s:%d %s()\033[0m: " fmt, __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__); \
            }                                                                                                              \
            else if ((filter & LOG_WARN) == LOG_WARN)                                                                      \
            {                                                                                                              \
                LOG_PRINT_IF("\033[33mWARNING! %s:%d %s()\033[0m: " fmt, __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__); \
            }                                                                                                              \
            else                                                                                                           \
                LOG_PRINT_IF(fmt, ##__VA_ARGS__);                                                                          \
        }                                                                                                                  \
    } while (0)
//--------------------------------------------------------------------------------
// DEBUG ctrl
#define DEBUG
#define LOG_ERR(fmt, ...) LOG_PRINT_IF("[E]:%s:%d: " fmt "\n", \
                                           __FILE__, __LINE__, ##__VA_ARGS__)
#ifdef DEBUG
#define LOG_DBG(fmt, ...) LOG_PRINT_IF(fmt "\n", ##__VA_ARGS__)
#else
#define LOG_DBG(fmt, ...)
#endif

//! NOT ALL TERMAINL SUPPORT
#define DISP_LOG_COLOR

#ifdef DISP_LOG_COLOR
#define BLACK_LOG "30"
#define RED_LOG "31"
#define GREEN_LOG "32"
#define YELLOW_LOG "33"
#define BLUE_LOG "34"
#define PURPLE_LOG "35"
#define SKBLU_LOG "36"
#define WHITE_LOG "37"
#define COLOR_STR(color, str) ("\033[" color "m" str "\033[0m")
#define LOG_COLOR(color, format, ...) \
    LOG_PRINT_IF("\033[" color "m" format "\033[0m", ##__VA_ARGS__)
#else
#define LOG_COLOR(color, format, ...)
#endif

#define TEST_PASS "\n"                                \
                  "PPPPPP   AAAA   SSSSSS  SSSSSS \n" \
                  "PP  PP  AA  AA  SS      SS     \n" \
                  "PPPPPP  AAAAAA  SSSSSS  SSSSSS \n" \
                  "PP      AA  AA      SS      SS \n" \
                  "PP      AA  AA  SSSSSS  SSSSSS \n\n"
#define TEST_FAIL "\n"                                \
                  "FFFFFF   AAAA   IIIIII  LL     \n" \
                  "FF      AA  AA    II    LL     \n" \
                  "FFFFFF  AAAAAA    II    LL     \n" \
                  "FF      AA  AA    II    LL     \n" \
                  "FF      AA  AA  IIIIII  LLLLLL \n\n"

#endif /* _COMMON_DEF_h_ */
