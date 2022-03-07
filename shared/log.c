#include "log.h"

void log_error(char *message) { printf("[ERROR!] %s\n", message); }

void log_info(char *tag, char *message) { printf("[%s] %s\n", tag, message); }
