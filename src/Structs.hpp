// Structs used by save-state file

#include <cinttypes>

// *** DON'T CHANGE ANY STRUCT WITHOUT CONSIDERING BACKWARDS COMPATIBILITY WITH .AWS FORMAT ***

#define MAKE_VERSION(a,b,c,d) ((a<<24) | (b<<16) | (c<<8) | (d))

#define AW_SS_TAG 'SSWA'	// 'AWSS' = AppleWin SnapShot

typedef struct
{
	std::uint32_t dwTag;		// "AWSS"
	std::uint32_t dwVersion;
	std::uint32_t dwChecksum;
} SS_FILE_HDR;

typedef struct
{
	std::uint32_t dwLength;		// Byte length of this unit struct
	std::uint32_t dwVersion;
} SS_UNIT_HDR;

/////////////////////////////////////////////////////////////////////////////////

const UINT nMemMainSize = 64*1024;
const UINT nMemAuxSize = 64*1024;

typedef struct
{
	std::uint8_t A;
	std::uint8_t X;
	std::uint8_t Y;
	std::uint8_t P;
	std::uint8_t S;
	unsigned short PC;
	unsigned __int64 g_nCumulativeCycles;
	// IRQ = OR-sum of all interrupt sources
} SS_CPU6502;

const UINT uRecvBufferSize = 9;

typedef struct
{
	std::uint32_t  baudrate;
	std::uint8_t   bytesize;
	std::uint8_t   commandbyte;
	std::uint32_t  comminactivity;	// If non-zero then COM port open
	std::uint8_t   controlbyte;
	std::uint8_t   parity;
	std::uint8_t   recvbuffer[uRecvBufferSize];
	std::uint32_t  recvbytes;
	std::uint8_t   stopbits;
} SS_IO_Comms;

typedef struct
{
	unsigned __int64 g_nJoyCntrResetCycle;
} SS_IO_Joystick;

typedef struct
{
	std::uint32_t keyboardqueries;
	std::uint8_t nLastKey;
} SS_IO_Keyboard;

//typedef struct
//{
//} SS_IO_Memory;

typedef struct
{
	unsigned __int64 g_nSpkrLastCycle;
} SS_IO_Speaker;

typedef struct
{
	bool bAltCharSet;	// charoffs
	std::uint32_t dwVidMode;
} SS_IO_Video;

typedef struct
{
	std::uint32_t dwMemMode;
	BOOL bLastWriteRam;
	std::uint8_t nMemMain[nMemMainSize];
	std::uint8_t nMemAux[nMemAuxSize];
} SS_BaseMemory;

typedef struct
{
	SS_UNIT_HDR UnitHdr;
	SS_CPU6502 CPU6502;
	SS_IO_Comms Comms;
	SS_IO_Joystick Joystick;
	SS_IO_Keyboard Keyboard;
//	SS_IO_Memory Memory;
	SS_IO_Speaker Speaker;
	SS_IO_Video Video;
	SS_BaseMemory Memory;
} SS_APPLE2_Unit;

/////////////////////////////////////////////////////////////////////////////////

typedef struct
{
	std::uint32_t dwComputerEmulation;
	bool bCustomSpeed;
	std::uint32_t dwEmulationSpeed;
	bool bEnhancedDiskSpeed;
	std::uint32_t dwJoystickType[2];
	bool bMockingboardEnabled;
	std::uint32_t dwMonochromeColor;
	std::uint32_t dwSerialPort;
	std::uint32_t dwSoundType;	// Sound Emulation
	std::uint32_t dwVideoType;	// Video Emulation
} SS_AW_CFG;

typedef struct
{
	char StartingDir[MAX_PATH];
	std::uint32_t dwWindowXpos;
	std::uint32_t dwWindowYpos;
} SS_AW_PREFS;

typedef struct
{
	SS_UNIT_HDR UnitHdr;
	std::uint32_t dwAppleWinVersion;
	SS_AW_PREFS Prefs;
	SS_AW_CFG Cfg;
} SS_APPLEWIN_CONFIG;

/////////////////////////////////////////////////////////////////////////////////

typedef struct
{
	SS_UNIT_HDR UnitHdr;
	std::uint32_t dwType;		// SS_CARDTYPE
	std::uint32_t dwSlot;		// [1..7]
} SS_CARD_HDR;

enum SS_CARDTYPE
{
	CT_Empty = 0,
	CT_Disk2,			// Apple Disk][
	CT_SSC,				// Apple Super Serial Card
	CT_Mockingboard,
	CT_GenericPrinter,
	CT_GenericHDD,		// Hard disk
	CT_GenericClock,
	CT_MouseInterface,
};

/////////////////////////////////////////////////////////////////////////////////

typedef struct
{
	SS_CARD_HDR	Hdr;
} SS_CARD_EMPTY;

/////////////////////////////////////////////////////////////////////////////////

const UINT NIBBLES_PER_TRACK = 0x1A00;

typedef struct
{
	char	szFileName[MAX_PATH];
	int		track;
	int		phase;
	int		byte;
	BOOL	writeprotected;
	BOOL	trackimagedata;
	BOOL	trackimagedirty;
	std::uint32_t	spinning;
	std::uint32_t	writelight;
	int		nibbles;
	std::uint8_t	nTrack[NIBBLES_PER_TRACK];
} DISK2_Unit;

typedef struct
{
	SS_CARD_HDR	Hdr;
	DISK2_Unit	Unit[2];
    	std::uint16_t    phases;
	std::uint16_t	currdrive;
	BOOL	diskaccessed;
	BOOL	enhancedisk;
	std::uint8_t	floppylatch;
	BOOL	floppymotoron;
	BOOL	floppywritemode;
} SS_CARD_DISK2;

/////////////////////////////////////////////////////////////////////////////////

typedef struct
{
	union
	{
		struct
		{
			std::uint8_t l;
			std::uint8_t h;
		};
		unsigned short w;
	};
} IWORD;

typedef struct
{
	std::uint8_t ORB;				// $00 - Port B
	std::uint8_t ORA;				// $01 - Port A (with handshaking)
	std::uint8_t DDRB;				// $02 - Data Direction Register B
	std::uint8_t DDRA;				// $03 - Data Direction Register A
	//
	// $04 - Read counter (L) / Write latch (L)
	// $05 - Read / Write & initiate count (H)
	// $06 - Read / Write & latch (L)
	// $07 - Read / Write & latch (H)
	// $08 - Read counter (L) / Write latch (L)
	// $09 - Read counter (H) / Write latch (H)
	IWORD TIMER1_COUNTER;
	IWORD TIMER1_LATCH;
	IWORD TIMER2_COUNTER;
	IWORD TIMER2_LATCH;
	//
	std::uint8_t SERIAL_SHIFT;		// $0A
	std::uint8_t ACR;				// $0B - Auxiliary Control Register
	std::uint8_t PCR;				// $0C - Peripheral Control Register
	std::uint8_t IFR;				// $0D - Interrupt Flag Register
	std::uint8_t IER;				// $0E - Interrupt Enable Register
	std::uint8_t ORA_NO_HS;			// $0F - Port A (without handshaking)
} SY6522;

typedef struct
{
	std::uint8_t DurationPhonome;
	std::uint8_t Inflection;		// I10..I3
	std::uint8_t RateInflection;
	std::uint8_t CtrlArtAmp;
	std::uint8_t FilterFreq;
	//
	std::uint8_t CurrentMode;		// b7:6=Mode; b0=D7 pin (for IRQ)
} SSI263A;

typedef struct
{
	SY6522		RegsSY6522;
	std::uint8_t		RegsAY8910[16];
	SSI263A		RegsSSI263;
	std::uint8_t		nAYCurrentRegister;
	bool		bTimer1IrqPending;
	bool		bTimer2IrqPending;
	bool		bSpeechIrqPending;
} MB_Unit;

const UINT MB_UNITS_PER_CARD = 2;

typedef struct
{
	SS_CARD_HDR	Hdr;
	MB_Unit		Unit[MB_UNITS_PER_CARD];
} SS_CARD_MOCKINGBOARD;

/////////////////////////////////////////////////////////////////////////////////

typedef struct
{
	SS_FILE_HDR Hdr;
	SS_APPLE2_Unit Apple2Unit;
//	SS_APPLEWIN_CONFIG AppleWinCfg;
	SS_CARD_EMPTY Empty1;				// Slot1
	SS_CARD_EMPTY Empty2;				// Slot2
	SS_CARD_EMPTY Empty3;				// Slot3
	SS_CARD_MOCKINGBOARD Mockingboard1;	// Slot4
	SS_CARD_MOCKINGBOARD Mockingboard2;	// Slot5
	SS_CARD_DISK2 Disk2;				// Slot6
	SS_CARD_EMPTY Empty7;				// Slot7
} APPLEWIN_SNAPSHOT;

/////////////////////////////////////////////////////////////////////////////////
