#pragma once

#include <cinttypes>

typedef struct _regsrec {
  std::uint8_t a;   // accumulator
  std::uint8_t x;   // index X
  std::uint8_t y;   // index Y
  std::uint8_t ps;  // processor status
  std::uint16_t pc;  // program counter
  std::uint16_t sp;  // stack pointer
  std::uint8_t bJammed; // CPU has crashed (NMOS 6502 only)
} regsrec, *regsptr;

extern regsrec    regs;
extern std::uint64_t g_nCumulativeCycles;

void    CpuDestroy ();
void    CpuCalcCycles(unsigned long nExecutedCycles);
std::uint32_t   CpuExecute (std::uint32_t);
unsigned long   CpuGetCyclesThisFrame(unsigned long nExecutedCycles);
void    CpuInitialize ();
void    CpuSetupBenchmark ();
void	CpuIrqReset();
void	CpuIrqAssert(eIRQSRC Device);
void	CpuIrqDeassert(eIRQSRC Device);
void	CpuNmiReset();
void	CpuNmiAssert(eIRQSRC Device);
void	CpuNmiDeassert(eIRQSRC Device);
void    CpuReset ();
std::uint32_t   CpuGetSnapshot(SS_CPU6502* pSS);
std::uint32_t   CpuSetSnapshot(SS_CPU6502* pSS);
