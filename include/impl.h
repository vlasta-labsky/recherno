#ifndef IMPL_H
#define IMPL_H

#include <cstdint>

class VirtualMachine;

uint32_t impl_msvcrt_fputc(VirtualMachine *machine, uint32_t c, uint32_t filePtr);
void impl_msvcrt_lock(VirtualMachine *machine, uint32_t filePtr);
void impl_msvcrt_unlock(VirtualMachine *machine, uint32_t filePtr);
uint32_t impl_msvcrt_calloc(VirtualMachine *machine, uint32_t num, uint32_t size);
uint32_t impl_msvcrt_malloc(VirtualMachine *machine, uint32_t size);
uint32_t impl_msvcrt_errno(VirtualMachine *machine, uint32_t num, uint32_t size);
void impl_msvcrt_initterm(VirtualMachine *machine, uint32_t a, uint32_t b);
uint32_t impl_kernel32_GetModuleHandleA(VirtualMachine *machine, uint32_t lpModuleNamePtr);
uint32_t impl_kernel32_SetUnhandledExceptionFilter(VirtualMachine *machine, uint32_t);
void impl_ciexp(VirtualMachine *machine);
void impl_cisqrt(VirtualMachine *machine);


#endif // IMPL_H
