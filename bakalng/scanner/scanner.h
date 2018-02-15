#pragma once
#include "LTextStream.h"
#include "LString.h"
#include "LexToken.h"

class Scanner
{
	enum StateType
	{
		stNormal,
		stString,
		stMax,
	};
	struct State
	{
		StateType type;
		size_t beginLine;
		size_t beginChar;
	};
public:
	Scanner();
	~Scanner();

	void scan(const char* szFile);

private:
	void enterState(StateType st);
	void leaveState(StateType st);

	typedef void (Scanner::*FuncProcState)(char16_t);
	void procFileScope(char16_t ch);
	void procStringScope(char16_t ch);

private:
	void genStringToken(const State& st);
	

private:
	LTextStream m_stream;
	std::vector<State> m_stkState;
	std::vector<LexToken> m_tokens;

	LString m_buf;
	size_t m_line{ 0 };
	size_t m_char{ 0 };
};

