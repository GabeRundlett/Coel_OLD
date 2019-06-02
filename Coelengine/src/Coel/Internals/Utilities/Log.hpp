#pragma once

#ifdef CONFIG_DEBUG
#define __LOG_RESET__ "\033[0m"
#define __LOG_BOLD__ "\033[1m"
#define __LOG_UNDERLINE__ "\033[4m"
#define __LOG_INVERSE__ "\033[7m"

#define __LOG_TEXT_FOREGROUND_BLACK__ "\033[30m"
#define __LOG_TEXT_FOREGROUND_RED__ "\033[31m"
#define __LOG_TEXT_FOREGROUND_GREEN__ "\033[32m"
#define __LOG_TEXT_FOREGROUND_YELLOW__ "\033[33m"
#define __LOG_TEXT_FOREGROUND_BLUE__ "\033[34m"
#define __LOG_TEXT_FOREGROUND_MAGENTA__ "\033[35m"
#define __LOG_TEXT_FOREGROUND_CYAN__ "\033[36m"
#define __LOG_TEXT_FOREGROUND_WHITE__ "\033[37m"

#define __LOG_TEXT_BACKGROUND_BLACK__ "\033[40m"
#define __LOG_TEXT_BACKGROUND_RED__ "\033[41m"
#define __LOG_TEXT_BACKGROUND_GREEN__ "\033[42m"
#define __LOG_TEXT_BACKGROUND_YELLOW__ "\033[43m"
#define __LOG_TEXT_BACKGROUND_BLUE__ "\033[44m"
#define __LOG_TEXT_BACKGROUND_MAGENTA__ "\033[45m"
#define __LOG_TEXT_BACKGROUND_CYAN__ "\033[46m"
#define __LOG_TEXT_BACKGROUND_WHITE__ "\033[47m"
#define __LOG_TEXT_BACKGROUND_GRAY__ "\033[90m"

#define LOG_INFO(source, msg) printf(__LOG_TEXT_BACKGROUND_GRAY__ #source ": " __LOG_RESET__ msg)
#define LOG_SUCCESS(source, msg) printf(__LOG_TEXT_BACKGROUND_GRAY__ #source ": " __LOG_RESET__ __LOG_TEXT_FOREGROUND_GREEN__ msg __LOG_RESET__)
#define LOG_ERROR(source, msg) printf(__LOG_BOLD__ __LOG_TEXT_BACKGROUND_RED__ "[ERROR::" #source "]" __LOG_RESET__ " " __LOG_TEXT_FOREGROUND_RED__ msg __LOG_RESET__)
#else
#define LOG_INFO(source, msg)
#define LOG_SUCCESS(source, msg)
#define LOG_ERROR(source, msg)
#endif
