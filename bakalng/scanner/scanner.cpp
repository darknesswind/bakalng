#include "stdafx.h"
#include "scanner.h"


Scanner::Scanner()
{
}


Scanner::~Scanner()
{
}

void Scanner::scan(const char* szFile)
{
	m_stream.openRead(szFile);

	const FuncProcState proc[stMax] =
	{
		&Scanner::procFileScope,
		&Scanner::procStringScope
	};

	enterState(stNormal);
	
	while (!m_stream.eof())
	{
		char16_t ch = m_stream.readChar();
		(this->*proc[m_stkState.back().type])(ch);
		++m_char;
	}
}

void Scanner::enterState(StateType st)
{
	m_stkState.push_back(State{ st, m_line, m_char });
}

void Scanner::leaveState(StateType st)
{
	const State& state = m_stkState.back();
	assert(state.type == st);
	switch (st)
	{
	case Scanner::stString:

		break;
	default:
		break;
	}
	m_stkState.pop_back();
}

void Scanner::procFileScope(char16_t ch)
{
	switch (ch)
	{
	case u'"':
		enterState(stString);
		return;
	default:
		break;
	}
}

void Scanner::procStringScope(char16_t ch)
{
	if (ch == u'"')
		leaveState(stString);
	else
		m_buf.push_back(ch);
}

void Scanner::genStringToken(const State& st)
{

}
