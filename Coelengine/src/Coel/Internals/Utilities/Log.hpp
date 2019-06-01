#pragma once

#ifdef CONFIG_DEBUG
#define __LOG_RESET__ "\033[0m"
#define __LOG_BOLD__ "\033[1m"
#define __LOG_UNDERLINE__ "\033[4m"
#define __LOG_INVERSE__ "\033[7m"

#define __LOG_TEXT_BLACK__ "\033[30m"
#define __LOG_TEXT_RED__ "\033[31m"
#define __LOG_TEXT_GREEN__ "\033[32m"
#define __LOG_TEXT_YELLOW__ "\033[33m"
#define __LOG_TEXT_BLUE__ "\033[34m"
#define __LOG_TEXT_MAGENTA__ "\033[35m"
#define __LOG_TEXT_CYAN__ "\033[36m"
#define __LOG_TEXT_WHITE__ "\033[37m"

#define __LOG_FORE_BLACK__ "\033[40m"
#define __LOG_FORE_RED__ "\033[41m"
#define __LOG_FORE_GREEN__ "\033[42m"
#define __LOG_FORE_YELLOW__ "\033[43m"
#define __LOG_FORE_BLUE__ "\033[44m"
#define __LOG_FORE_MAGENTA__ "\033[45m"
#define __LOG_FORE_CYAN__ "\033[46m"
#define __LOG_FORE_WHITE__ "\033[47m"

#define LOG_INFO(source, msg) printf(__LOG_BOLD__ "[" #source "]" __LOG_RESET__ " " msg)
#define LOG_SUCCESS(source, msg) printf(__LOG_BOLD__ "[" #source "]" __LOG_RESET__ __LOG_TEXT_GREEN__ " " msg __LOG_RESET__)
#define LOG_ERROR(source, msg) printf(__LOG_BOLD__ __LOG_FORE_RED__ "[ERROR::"#source"]" __LOG_RESET__ " " __LOG_TEXT_RED__ msg __LOG_RESET__)
#else
#define LOG_INFO(source, msg)
#define LOG_SUCCESS(source, msg)
#define LOG_ERROR(source, msg)
#endif
