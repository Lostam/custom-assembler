#ifndef LOGGER_H
#define LOGGER_H

/**
 * Handles all the logging for the project, will log in several log levels
*/

// TURNED OFF BY DEFUALT, UNCOMMENT IN .C FILE TO ALLOW DEBUGGING
void debug(const char* format, ...);
void info(const char* format, ...);
void warn(const char* format, ...);
void error(const char* format, ...);

#endif  /* LOGGER_H */
