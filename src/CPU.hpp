#pragma once

#include <cinttypes>

typedef struct _regsrec {
  std::uint8_t a;   // accumulator
  std::uint8_t x;   // index X
  std::uint8_t y;   // index Y
  std::uint8_t ps;  // processor status
  WORD pc;  // program counter
  WORD sp;  // stack pointer
  std::uint8_t bJammed; // CPU has crashed (NMOS 6502 only)
} regsrec, *regsptr;

extern regsrec    regs;
extern unsigned __int64 g_nCumulativeCycles;

void    CpuDestroy ();
void    CpuCalcCycles(ULONG nExecutedCycles);
DWORD   CpuExecute (DWORD);
ULONG   CpuGetCyclesThisFrame(ULONG nExecutedCycles);
void    CpuInitialize ();
void    CpuSetupBenchmark ();
void	CpuIrqReset();
void	CpuIrqAssert(eIRQSRC Device);
void	CpuIrqDeassert(eIRQSRC Device);
void	CpuNmiReset();
void	CpuNmiAssert(eIRQSRC Device);
void	CpuNmiDeassert(eIRQSRC Device);
void    CpuReset ();
DWORD   CpuGetSnapshot(SS_CPU6502* pSS);
DWORD   CpuSetSnapshot(SS_CPU6502* pSS);
