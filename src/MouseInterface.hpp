#include "./6821.hpp"
#include "./Common.hpp"

#define WRITE_HANDLER(func)		void func( void* objFrom, void* objTo, int nAddr, std::uint8_t byData )
#define CALLBACK_HANDLER(func)	void func( void* objFrom, void* objTo, LPARAM lParam )

extern class CMouseInterface sg_Mouse;

class CMouseInterface
{
public:
	CMouseInterface();
	virtual ~CMouseInterface();

	void Initialize(std::uint8_t * pCxRomPeripheral, UINT uSlot);
	void Uninitialize(){ m_bActive = false; }
	void SetSlotRom();
	static std::uint8_t IORead(std::uint16_t PC, std::uint16_t uAddr, std::uint8_t bWrite, std::uint8_t uValue, unsigned long nCyclesLeft);
	static std::uint8_t IOWrite(std::uint16_t PC, std::uint16_t uAddr, std::uint8_t bWrite, std::uint8_t uValue, unsigned long nCyclesLeft);

	void SetPosition(int xvalue, int xrange, int yvalue, int yrange);
	void SetButton(eBUTTON Button, eBUTTONSTATE State);
	bool Active() { return m_bActive; }
	void SetVBlank(bool bVBL);

protected:
	void On6821_A(std::uint8_t byData);
	void On6821_B(std::uint8_t byData);
	void OnCommand();
	void OnWrite();
	void OnMouseEvent();
	void Reset();

	friend WRITE_HANDLER( M6821_Listener_A );
	friend WRITE_HANDLER( M6821_Listener_B );
	//friend CALLBACK_HANDLER( MouseHandler );

	void SetPosition(int xvalue, int yvalue);
	void ClampX(int iMinX, int iMaxX);
	void ClampY(int iMinY, int iMaxY);


	C6821	m_6821;

	int		m_nDataLen;
	std::uint8_t	m_byMode;

	std::uint8_t	m_by6821B;
	std::uint8_t	m_by6821A;
	std::uint8_t	m_byBuff[8];			// m_byBuff[0] is mode byte
	int		m_nBuffPos;

	std::uint8_t	m_byState;
	int		m_nX;
	int		m_nY;
	bool	m_bBtn0;
	bool	m_bBtn1;

	bool	m_bVBL;

	//

	UINT	m_iX;
	UINT	m_iRangeX;
	UINT	m_iMinX;
	UINT	m_iMaxX;
	UINT	m_iY;
	UINT	m_iRangeY;
	UINT	m_iMinY;
	UINT	m_iMaxY;

	bool	m_bButtons[2];

	//

	bool	m_bActive;
	std::uint8_t *	m_pSlotRom;
	UINT	m_uSlot;
};
