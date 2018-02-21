#pragma once
#include "LTextStream.h"
#include "LString.h"
#include "LexToken.h"

class Scanner
{
public:
	Scanner();
	~Scanner();

	void scan(const char* pFileName);
	void scan(const void* pRawData, size_t size);
	const LexTokens& tokens() { return m_tokens; }

private:
	void proc();
	bool procSpecialChar(char16_t ch);
	void procLineComment();
	void procScopeComment();
	void procString();
	void procNumber();
	void procFloatPoint();
	void procSciFloat();
	void procHex();
	void procOct();
	void procBin();
	void procIdentifier();

private:
	LexToken::Location currLocation();
	void updateLastTokenLoc();
	void clearBuffer();
	bool matchOperation(char16_t ch);

private:
	void genOperToken(Operation op);
	void genCommentToken(const LString& comment);
	void genStringToken(const LString& context);
	void genError(Errors err);
	void genIntegerToken(uint64_t val);
	void genDoubleToken();

private:
	LTextStream m_stream;
	LexTokens m_tokens;
	LexToken::Location m_startLoc{ 0 };
	LString m_buf;
	HString m_hSource;
};

