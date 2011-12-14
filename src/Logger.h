#ifndef _LOGGER_H
#define _LOGGER_H

#include<syslog.h>
#include<stdarg.h>

void Logger__log(int level, char* msg, ...);
void Logger__debug(char* msg, ...);
void Logger__notice(char* msg, ...);
void Logger__warn(char* msg, ...);
void Logger__error(char* msg, ...);

#endif
