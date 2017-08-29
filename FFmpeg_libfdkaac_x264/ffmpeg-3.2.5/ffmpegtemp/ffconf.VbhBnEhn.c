#include <windows.h>
#include <security.h>
#include <stdint.h>
long check_InitializeSecurityContext(void) { return (long) InitializeSecurityContext; }
int main(void) { int ret = 0;
 ret |= ((intptr_t)check_InitializeSecurityContext) & 0xFFFF;
return ret; }
