// Motorola MC6821 PIA
#include <cinttypes>

using mem_write_handler = void (*)(void* objFrom, void* objTo, int nAddr, std::uint8_t byData);

typedef struct _STWriteHandler
{
	void* objTo;
	mem_write_handler func;
} STWriteHandler;

//

#define	PIA_DDRA	0
#define	PIA_CTLA	1
#define	PIA_DDRB	2
#define	PIA_CTLB	3

class C6821
{
public:
	C6821();
	virtual ~C6821();

	std::uint8_t GetPB();
	std::uint8_t GetPA();
	void SetPB(std::uint8_t byData);
	void SetPA(std::uint8_t byData);
	void SetCA1( std::uint8_t byData );
	void SetCA2( std::uint8_t byData );
	void SetCB1( std::uint8_t byData );
	void SetCB2( std::uint8_t byData );
	void Reset();
	std::uint8_t Read( std::uint8_t byRS );
	void Write( std::uint8_t byRS, std::uint8_t byData );

	void UpdateInterrupts();

	void SetListenerA( void *objTo, mem_write_handler func );
	void SetListenerB( void *objTo, mem_write_handler func );
	void SetListenerCA2( void *objTo, mem_write_handler func );
	void SetListenerCB2( void *objTo, mem_write_handler func );

protected:
	std::uint8_t	m_byIA;
	std::uint8_t	m_byCA1;
	std::uint8_t	m_byICA2;
	std::uint8_t	m_byOA;
	std::uint8_t	m_byOCA2;
	std::uint8_t	m_byDDRA;
	std::uint8_t	m_byCTLA;
	std::uint8_t	m_byIRQAState;

	std::uint8_t	m_byIB;
	std::uint8_t	m_byCB1;
	std::uint8_t	m_byICB2;
	std::uint8_t	m_byOB;
	std::uint8_t	m_byOCB2;
	std::uint8_t	m_byDDRB;
	std::uint8_t	m_byCTLB;
	std::uint8_t	m_byIRQBState;

	STWriteHandler m_stOutA;
	STWriteHandler m_stOutB;
	STWriteHandler m_stOutCA2;
	STWriteHandler m_stOutCB2;
	STWriteHandler m_stOutIRQA;
	STWriteHandler m_stOutIRQB;
};
