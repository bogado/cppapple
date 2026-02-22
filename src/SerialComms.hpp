#pragma once

#include <pthread.h>

extern class CSuperSerialCard sg_SSC;

enum {COMMEVT_WAIT=0, COMMEVT_ACK, COMMEVT_TERM, COMMEVT_MAX};
enum eFWMODE {FWMODE_CIC=0, FWMODE_SIC_P8, FWMODE_PPC, FWMODE_SIC_P8A};	// NB. CIC = SSC

//////// windows specific values
#define NOPARITY            0
#define ODDPARITY           1
#define EVENPARITY          2
#define MARKPARITY          3
#define SPACEPARITY         4

#define ONESTOPBIT          0
#define ONE5STOPBITS        1
#define TWOSTOPBITS         2
///////////////////////////////
// Undocumented CMSPAR flag for MARKPARITY and SPACEPARITY???
#ifndef CMSPAR
#define CMSPAR   010000000000
#endif
/////////////////////////////////////

typedef struct
{
	//DIPSW1
	UINT	uBaudRate;
	eFWMODE	eFirmwareMode;

	//DIPSW2
	UINT	uStopBits;
	UINT	uByteSize;
	UINT	uParity;
	bool	bLinefeed;
	bool	bInterrupts;
} SSC_DIPSW;

class CSuperSerialCard
{
public:
	CSuperSerialCard();
	virtual ~CSuperSerialCard() {}

	void	CommInitialize(std::uint8_t * pCxRomPeripheral, UINT uSlot);
	void    CommReset();
	void    CommDestroy();
	void    CommSetSerialPort(/*HWND,*/std::uint32_t);
	void    CommUpdate(std::uint32_t);
	std::uint32_t   CommGetSnapshot(SS_IO_Comms* pSS);
	std::uint32_t   CommSetSnapshot(SS_IO_Comms* pSS);

	std::uint32_t	GetSerialPort() { return m_dwSerialPort; }
	void	SetSerialPort(std::uint32_t dwSerialPort) { m_dwSerialPort = dwSerialPort; }

	static std::uint8_t SSC_IORead(std::uint16_t PC, std::uint16_t uAddr, std::uint8_t bWrite, std::uint8_t uValue, unsigned long nCyclesLeft);
	static std::uint8_t SSC_IOWrite(std::uint16_t PC, std::uint16_t uAddr, std::uint8_t bWrite, std::uint8_t uValue, unsigned long nCyclesLeft);

private:
	std::uint8_t CommCommand(std::uint16_t pc, std::uint16_t addr, std::uint8_t bWrite, std::uint8_t d, unsigned long nCyclesLeft);
	std::uint8_t CommControl(std::uint16_t pc, std::uint16_t addr, std::uint8_t bWrite, std::uint8_t d, unsigned long nCyclesLeft);
	std::uint8_t CommDipSw(std::uint16_t pc, std::uint16_t addr, std::uint8_t bWrite, std::uint8_t d, unsigned long nCyclesLeft);
	std::uint8_t CommReceive(std::uint16_t pc, std::uint16_t addr, std::uint8_t bWrite, std::uint8_t d, unsigned long nCyclesLeft);
	std::uint8_t CommStatus(std::uint16_t pc, std::uint16_t addr, std::uint8_t bWrite, std::uint8_t d, unsigned long nCyclesLeft);
	std::uint8_t CommTransmit(std::uint16_t pc, std::uint16_t addr, std::uint8_t bWrite, std::uint8_t d, unsigned long nCyclesLeft);

	void	GetDIPSW();
	void	SetDIPSWDefaults();
	std::uint8_t	GenerateControl();
	UINT	BaudRateToIndex(UINT uBaudRate);
	void	UpdateCommState();
	BOOL	CheckComm();
	void	CloseComm();
	void	CheckCommEvent(std::uint32_t dwEvtMask);
	static std::uint32_t CommThread(void * lpParameter);
	bool	CommThInit();
	void	CommThUninit();

	//

private:
	std::uint32_t	m_dwSerialPort;

	static SSC_DIPSW	m_DIPSWDefault;
	SSC_DIPSW			m_DIPSWCurrent;

	// Derived from DIPSW1
	UINT	m_uBaudRate;

	// Derived from DIPSW2
	UINT	m_uStopBits;
	UINT	m_uByteSize;
	UINT	m_uParity;

	// SSC Registers
	std::uint8_t   m_uControlByte;
	std::uint8_t   m_uCommandByte;

	//

	int    m_hCommHandle;	// file for communication with COM
	std::uint32_t  m_dwCommInactivity;


// how does CRITICAL_SECTION work in Linux? -- see in Wikipedia: http://en.wikipedia.org/wiki/Critical_section
// --> to main file
//	CRITICAL_SECTION	m_CriticalSection;	// To guard /g_vRecvBytes/
	std::uint8_t			m_RecvBuffer[uRecvBufferSize];	// NB: More work required if >1 is used
	volatile std::uint32_t		m_vRecvBytes;

	//

	bool m_bTxIrqEnabled;
	bool m_bRxIrqEnabled;

	bool m_bWrittenTx;

	//

	volatile bool m_vbCommIRQ;
	HANDLE m_hCommThread;

	HANDLE m_hCommEvent[COMMEVT_MAX];
	OVERLAPPED m_o;

	std::uint8_t* m_pExpansionRom;
};
