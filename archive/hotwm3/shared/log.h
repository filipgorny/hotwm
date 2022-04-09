#pragma once

#ifdef __cplusplus
extern "C" {
#endif

void log_error(char* message);
void log_info(char* tag, char* message);
void log_debug();
void log_debugv(char* v);
void log_debugi();

#ifdef __cplusplus
}
#endif

