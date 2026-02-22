#pragma once

enum MemoryInitPattern_e
{
	  MIP_ZERO
	, MIP_FF_FF_00_00

	, NUM_MIP
};
extern MemoryInitPattern_e g_eMemoryInitPattern;

extern iofunction IORead[256];
extern iofunction IOWrite[256];
extern std::uint8_t *     memwrite[0x100];
extern std::uint8_t *     mem;
extern std::uint8_t *     memdirty;

#ifdef RAMWORKS
extern UINT       g_uMaxExPages;	// user requested ram pages (from cmd line)
#endif

void	RegisterIoHandler(UINT uSlot, iofunction IOReadC0, iofunction IOWriteC0, iofunction IOReadCx, iofunction IOWriteCx, LPVOID lpSlotParameter, std::uint8_t* pExpansionRom);

void    MemDestroy ();
bool    MemGet80Store();
bool	MemCheckSLOTCXROM();
std::uint8_t *  MemGetAuxPtr (std::uint16_t);
std::uint8_t *  MemGetMainPtr (std::uint16_t);
std::uint8_t *  MemGetCxRomPeripheral();
void	MemPreInitialize ();
int    MemInitialize ();
std::uint8_t    MemReadFloatingBus(const unsigned long uExecutedCycles);
std::uint8_t    MemReadFloatingBus(const std::uint8_t highbit, const unsigned long uExecutedCycles);
void    MemReset ();
void    MemResetPaging ();
std::uint8_t    MemReturnRandomData (std::uint8_t highbit);
void    MemSetFastPaging (BOOL);
void    MemTrimImages ();
LPVOID	MemGetSlotParameters (UINT uSlot);
std::uint32_t   MemGetSnapshot(SS_BaseMemory* pSS);
std::uint32_t   MemSetSnapshot(SS_BaseMemory* pSS);

std::uint8_t IO_Null(std::uint16_t programcounter, std::uint16_t address, std::uint8_t write, std::uint8_t value, unsigned long nCycles);

std::uint8_t MemCheckPaging (std::uint16_t pc, std::uint16_t addr, std::uint8_t bWrite, std::uint8_t d, unsigned long nCyclesLeft);
std::uint8_t MemSetPaging(std::uint16_t pc, std::uint16_t addr, std::uint8_t bWrite, std::uint8_t d, unsigned long nCyclesLeft);
