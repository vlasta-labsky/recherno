#include "impl.h"
#include "virtualmachine.h"

#include <stdio.h>
#include <math.h>

#include <QDebug>


// int fputc(int char, FILE *stream)
uint32_t impl_msvcrt_fputc(VirtualMachine *machine, uint32_t c, uint32_t filePtr)
{
    fputc(c, stdout);
    fflush(stdout);

    return c;
}

// void _lock(FILE *stream)
void impl_msvcrt_lock(VirtualMachine *machine, uint32_t filePtr)
{
    qDebug() << "msvcrt.dll:_lock stub";
}

// void _unlock(FILE *stream)
void impl_msvcrt_unlock(VirtualMachine *machine, uint32_t filePtr)
{
    qDebug() << "msvcrt.dll:_unlock stub";
}

// void* calloc( size_t num, size_t size );
uint32_t impl_msvcrt_calloc(VirtualMachine *machine, uint32_t num, uint32_t size)
{
    qDebug() << "msvcrt.dll:calloc stub";

    return 0;
}

// void* malloc( size_t size );
uint32_t impl_msvcrt_malloc(VirtualMachine *machine, uint32_t size)
{
    qDebug() << "msvcrt.dll:malloc" << size << "stub";

    return 0xdeadbeef;
    //return 0;
}

// int* errno();
uint32_t impl_msvcrt_errno(VirtualMachine *machine, uint32_t num, uint32_t size)
{
    qDebug() << "msvcrt.dll:errno stub";

    return 0;
}

// void _initterm(void*, void*);
void impl_msvcrt_initterm(VirtualMachine *machine, uint32_t a, uint32_t b)
{
    qDebug() << "msvcrt.dll:_initterm stub";
}

// HMODULE GetModuleHandleA(LPCSTR lpModuleName);
uint32_t impl_kernel32_GetModuleHandleA(VirtualMachine *machine, uint32_t lpModuleNamePtr)
{
    char lpModuleName[255];
    machine->read(lpModuleNamePtr, lpModuleName, sizeof(lpModuleName));

    qDebug() << "impl_kernel32_GetModuleHandleA" << lpModuleName << "stub";

    return 0;
}

// LPTOP_LEVEL_EXCEPTION_FILTER SetUnhandledExceptionFilter(LPTOP_LEVEL_EXCEPTION_FILTER);
uint32_t impl_kernel32_SetUnhandledExceptionFilter(VirtualMachine *machine, uint32_t)
{
    qDebug() << "kernel32.dll:SetUnhandledExceptionFilter stub";

    return 0;
}

// void _CIexp(void) in: x87 stack out: x87 stack
void impl_ciexp(VirtualMachine *machine)
{
    uint8_t st0[10]; // 80bit raw float
    machine->reg(UC_X86_REG_ST0, &st0);

    float val = *reinterpret_cast<long double *>(st0);

    float res = exp(val);

    *reinterpret_cast<long double *>(st0) = res;
    machine->setReg(UC_X86_REG_ST0, &st0);

    //qDebug() << "impl_ciexp" << val << res;
}

// void _CIsqrt(void) in: x87 stack out: x87 stack
void impl_cisqrt(VirtualMachine *machine)
{
    uint8_t st0[10]; // 80bit raw float
    machine->reg(UC_X86_REG_ST0, &st0);

    float val = *reinterpret_cast<long double *>(st0);

    float res = sqrt(val);

    *reinterpret_cast<long double *>(st0) = res;
    machine->setReg(UC_X86_REG_ST0, &st0);

    //qDebug() << "impl_cisqrt" << val << res;
}
