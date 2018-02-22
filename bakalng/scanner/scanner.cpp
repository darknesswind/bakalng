#include "stdafx.h"
#include "scanner.h"
#include "base/stringpool.h"

bool isSplitChar(char16_t ch)
{
	return (
		ch == u' ' ||
		ch == u'\t' ||
		ch == u'\r' ||
		ch == u'\n'
		);
}

bool isIdBeginChar(char16_t ch)
{
	return (
		ch == u'_' ||
		(ch >= u'a' && ch <= u'z') ||
		(ch >= u'A' && ch <= u'Z') ||
		ch > 0x7F
		);
}
bool isIdFollowChar(char16_t ch)
{
	return (
		isIdBeginChar(ch) ||
		(ch >= u'0' && ch <= u'9')
		);
}

Scanner::Scanner()
{
}

Scanner::~Scanner()
{
}

void Scanner::scan(const char* szFile)
{
	m_stream.openRead(szFile);
	proc();
}

void Scanner::scan(const void* pRawData, size_t size)
{
	m_stream.openRead(pRawData, size);
	proc();
}

void Scanner::proc()
{
	m_tokens.clear();
	while (!m_stream.eof())
	{
		m_startLoc = currLocation();
		char16_t ch = m_stream.readChar();

		if (procSpecialChar(ch))
			continue;

		if (ch >= u'0' && ch <= u'9')
		{
			procNumber();
			continue;
		}

		if (isIdBeginChar(ch))
			procIdentifier();
	}
}

bool Scanner::procSpecialChar(char16_t ch)
{
	switch (ch)
	{
	case u'/':
		if (m_stream.peekChar() == u'/')
		{
			procLineComment();
			return true;
		}
		else if (m_stream.peekChar() == u'*')
		{
			procScopeComment();
			return true;
		}
		break;
	case u'"':
		procString();
		return true;
	case u'.':
		if (m_stream.peekChar() >= u'0' && m_stream.peekChar() <= u'9')
		{
			procNumber();
			return true;
		}
		break;
	default:
		break;
	}
	return matchOperation(ch);
}

void Scanner::procLineComment()
{
	m_buf.resize(0);
	m_buf.push_back(m_stream.currChar());
	do 
	{
		char16_t ch = m_stream.readChar();
		if (ch == u'\n')
		{
			if (m_buf.back() == u'\r')
				m_buf.pop_back();
			break;
		}
		m_buf.push_back(ch);
	} while (!m_stream.eof());

	genCommentToken(m_buf);
	m_buf.resize(0);
}

void Scanner::procScopeComment()
{
	m_buf.resize(0);
	m_buf.push_back(m_stream.currChar());
	m_buf.push_back(m_stream.readChar());
	while (!m_stream.eof())
	{
		char16_t ch = m_stream.readChar();
		m_buf.push_back(ch);
		if (ch == u'*' && m_stream.peekChar() == u'/')
		{
			m_buf.push_back(m_stream.readChar());
			break;
		}
	}

	genCommentToken(m_buf);
	m_buf.resize(0);
}

void Scanner::procString()
{
	m_buf.resize(0);
	while (!m_stream.eof())
	{
		char16_t ch = m_stream.readChar();
		if (ch == u'\\')
		{
			switch (m_stream.peekChar())
			{
			case u'"':
			case u'\\':
				ch = m_stream.readChar();
				break;
			case u't': ch = u'\t'; m_stream.readChar(); break;
			case u'n': ch = u'\n'; m_stream.readChar(); break;
			default:
				break;
			}
		}
		else if (ch == u'"')
		{
			break;
		}
		m_buf.push_back(ch);
	}

	genStringToken(m_buf);
	m_buf.resize(0);
}

void Scanner::procNumber()
{
	m_buf.resize(0);

	char16_t ch = m_stream.currChar();
	if (ch == u'0')
	{
		switch (m_stream.peekChar())
		{
		case u'x': return procHex();
		case u'o': return procOct();
		case u'b': return procBin();
		default: break;
		}
	}
	else if (ch == u'.')
	{
		return procFloatPoint();
	}
	m_buf.push_back(ch);
	while (!m_stream.eof())
	{
		ch = m_stream.peekChar();
		if (ch == u'.')
		{
			m_stream.readChar();
			return procFloatPoint();
		}
		else if (ch == u'e' || ch == u'E')
			return procSciFloat();
		else if (ch >= u'0' && ch <= u'9')
			m_buf.push_back(m_stream.readChar());
		else
			break;
	}
	// 最大进位值，uint64_t.max必然不能被10整除
	const uint64_t maxLvUp = std::numeric_limits<uint64_t>::max() / 10;
	uint64_t val = 0;
	const wchar_t* pCur = m_buf.c_str();
	const wchar_t* pEnd = pCur + m_buf.size();
	while (pCur < pEnd && *pCur == u'0')
		++pCur;

	while (pCur < pEnd)
	{
		if (val > maxLvUp)
			return genError(Errors::E_Scan_IntegerTooLarge);

		val = val * 10 + (*pCur - u'0');
		++pCur;
	}
	return genIntegerToken(val);
}

void Scanner::procFloatPoint()
{
	char16_t ch = m_stream.currChar();
	m_buf.push_back(ch);
	assert(ch == u'.');
	while (!m_stream.eof())
	{
		char16_t ch = m_stream.peekChar();
		if (ch >= u'0' && ch <= u'9')
			m_buf.push_back(m_stream.readChar());
		else if (ch == 'e' || ch == 'E')
			return procSciFloat();
		else
			break;
	}
	assert(m_buf.size() != 1);
	genDoubleToken();
}

void Scanner::procSciFloat()
{
	char16_t ch = m_stream.readChar();
	m_buf.push_back(ch);
	assert(ch == u'e' || ch == 'E');
	ch = m_stream.peekChar();
	if (ch == u'+' || ch == u'-')
		m_buf.push_back(m_stream.readChar());

	while (!m_stream.eof())
	{
		char16_t ch = m_stream.peekChar();
		if (ch >= u'0' && ch <= u'9')
			m_buf.push_back(m_stream.readChar());
		else
			break;
	}
	genDoubleToken();
}

void Scanner::procHex()
{
	m_buf.push_back(m_stream.currChar());
	m_buf.push_back(m_stream.readChar());
	assert(m_buf.back() == u'x');
	while (!m_stream.eof())
	{
		char16_t ch = m_stream.peekChar();
		if ((ch >= u'0' && ch <= u'9') ||
			(ch >= u'a' && ch <= u'f') ||
			(ch >= u'A' && ch <= u'F'))
		{
			m_buf.push_back(m_stream.readChar());
		}
		else
		{
			break;
		}
	}
	if (m_buf.size() == 2)
		return genError(Errors::E_Scan_InvalidHex);

	const size_t bitsPerChar = 4;
	const size_t bitsPerByte = 8;
	size_t szBits = (m_buf.size() - 2) * bitsPerChar;
	if (szBits > bitsPerByte * sizeof(uint64_t))
		return genError(Errors::E_Scan_IntegerTooLarge);

	uint64_t val = 0;
	for (size_t i = 2; i < m_buf.size(); ++i)
	{
		char16_t ch = m_buf[i];
		size_t charVal = 0;
		if (ch >= u'0' && ch <= u'9')
			charVal = ch - u'0';
		else if (ch >= u'a' && ch <= u'f')
			charVal = ch - u'a';
		else if (ch >= u'A' && ch <= u'F')
			charVal = ch - u'A';
		val = (val << bitsPerChar) | charVal;
	}
	genIntegerToken(val);
}

void Scanner::procOct()
{
	m_buf.push_back(m_stream.currChar());
	m_buf.push_back(m_stream.readChar());
	assert(m_buf.back() == u'o');
	while (!m_stream.eof())
	{
		char16_t ch = m_stream.peekChar();
		if (ch >= u'0' && ch <= u'7')
			m_buf.push_back(m_stream.readChar());
		else
			break;
	}
	if (m_buf.size() == 2)
		return genError(Errors::E_Scan_InvalidOct);

	const size_t charCount = m_buf.size() - 2;
	const size_t bitsPerChar = 3;
	const size_t bitsPerByte = 8;
	size_t szBits = charCount * bitsPerChar;
	if (szBits > bitsPerByte * sizeof(uint64_t))
		return genError(Errors::E_Scan_IntegerTooLarge);

	uint64_t val = 0;
	for (size_t i = 2; i < m_buf.size(); ++i)
		val = (val << bitsPerChar) | (m_buf[i] - u'0');

	genIntegerToken(val);
}

void Scanner::procBin()
{
	m_buf.push_back(m_stream.currChar());
	m_buf.push_back(m_stream.readChar());
	assert(m_buf.back() == u'b');
	while (!m_stream.eof())
	{
		char16_t ch = m_stream.peekChar();
		if (ch == u'0' || ch == u'1')
			m_buf.push_back(m_stream.readChar());
		else
			break;
	}
	if (m_buf.size() == 2)
		return genError(Errors::E_Scan_InvalidBin);

	const size_t charCount = m_buf.size() - 2;
	const size_t bitsPerChar = 1;
	const size_t bitsPerByte = 8;
	size_t szBits = charCount * bitsPerChar;
	if (szBits > bitsPerByte * sizeof(uint64_t))
		return genError(Errors::E_Scan_IntegerTooLarge);

	uint64_t val = 0;
	for (size_t i = 2; i < m_buf.size(); ++i)
		val = (val << bitsPerChar) | (m_buf[i] - u'0');

	genIntegerToken(val);
}

void Scanner::procIdentifier()
{
	m_buf.resize(0);
	m_buf.push_back(m_stream.currChar());
	while (!m_stream.eof())
	{
		wchar_t ch = m_stream.peekChar();
		if (isIdFollowChar(ch))
			m_buf.push_back(m_stream.readChar());
		else
			break;
	}

	genIdentifierToken();
}

LexToken::Location Scanner::currLocation()
{
	return LexToken::Location{ m_hSource, m_stream.row(), m_stream.col(), m_stream.posChar() };
}

void Scanner::updateLastTokenLoc()
{
	LexToken& token = m_tokens.back();
	token.setLocation(m_startLoc);
}

void Scanner::clearBuffer()
{
	m_buf.clear();
}

bool Scanner::matchOperation(char16_t ch)
{
	if (ch > 0x7F)
		return false;

	struct OpItem { char opChar[4]; Operation::Type opType; };
	static std::unordered_map<char, std::vector<OpItem>> opDic;
	if (opDic.empty())
	{
		OpItem ops[] =
		{
		{ "+",	Operation::Add },
		{ "-",	Operation::Sub },
		{ "*",	Operation::Mul },
		{ "/",	Operation::Div },
		{ "%",	Operation::Mod },
		{ "&",	Operation::And },
		{ "|",	Operation::Or },
		{ "^",	Operation::Xor },
		{ "~",	Operation::Compl},
		{ "<<",	Operation::ShiftLeft },
		{ ">>",	Operation::ShiftRight },
		{ "+=",	Operation::AddAssign },
		{ "-=",	Operation::SubAssign },
		{ "*=",	Operation::MulAssign },
		{ "/=",	Operation::DivAssign },
		{ "%=",	Operation::ModAssign },
		{ "&=",	Operation::AndAssign },
		{ "|=",	Operation::OrAssign },
		{ "^=",	Operation::XorAssign },
		{ "&&",	Operation::LogicAnd },
		{ "||",	Operation::LogicOr },
		{ "!",	Operation::Not },
		{ "==",	Operation::Equal },
		{ "!=",	Operation::NotEqual },
		{ ">",	Operation::GreaterThen },
		{ ">=",	Operation::GreaterEqual },
		{ "<",	Operation::LessThen },
		{ "<=",	Operation::LessEqual },
		{ "=",	Operation::Assign },
		{ ".",	Operation::SelMemberO },
		{ "->",	Operation::SelMemberP },
		{ "::",	Operation::Scope },
		{ ",",	Operation::Comma },
		{ ";",	Operation::EndExpr },
		};
		for (size_t i = 0; i < sizeof(ops) / sizeof(OpItem); ++i)
			opDic[ops[i].opChar[0]].push_back(ops[i]);
	}

	auto iter = opDic.find(static_cast<char>(ch));
	if (iter != opDic.end())
	{
		char16_t next = m_stream.peekChar();
		Operation::Type defType = Operation::None;
		std::vector<OpItem>& vecOp = iter->second;
		for (auto itOp = vecOp.begin(); itOp != vecOp.end(); ++itOp)
		{
			if (next == itOp->opChar[1])
			{
				genOperToken(itOp->opType);
				return true;
			}
			else if ('\0' == itOp->opChar[1])
				defType = itOp->opType;
		}
		if (defType != Operation::None)
		{
			genOperToken(defType);
			return true;
		}
	}
	return false;
}

void Scanner::genOperToken(Operation op)
{
	if (op == Operation::Assign && !m_tokens.empty())
	{
		LexToken& token = m_tokens.back();
		if (token.isOperation() &&
			token.location().pos + token.operation().size() == m_startLoc.pos)
		{
			if (token.operation() == Operation::ShiftLeft)
			{
				token = LexToken(Operation::SHLAssign);
				return;
			}
			else if (token.operation() == Operation::ShiftRight)
			{
				token = LexToken(Operation::SHRAssign);
				return;
			}
		}
	}

	m_tokens.emplace_back(LexToken(op));
	updateLastTokenLoc();
}

void Scanner::genCommentToken(const LString& comment)
{
#ifdef _DEBUG
	HString hStr = StringPool::inst().insert(comment);
	m_tokens.emplace_back(LexToken(hStr, LexToken::tkComment));
	updateLastTokenLoc();
#endif
}

void Scanner::genStringToken(const LString& context)
{
	HString hStr = StringPool::inst().insert(context);
	m_tokens.emplace_back(LexToken(hStr));
	updateLastTokenLoc();
}

void Scanner::genError(Errors err)
{
	m_tokens.emplace_back(LexToken(err));
	updateLastTokenLoc();
}

void Scanner::genIntegerToken(uint64_t val)
{
	if (val & 0xFFFFFFFF00000000)
	{
		if (val & 0x8000000000000000)
			m_tokens.emplace_back(LexToken(val));
		else
			m_tokens.emplace_back(LexToken(static_cast<int64_t>(val)));
	}
	else
	{
		if (val & 0x80000000)
			m_tokens.emplace_back(LexToken(static_cast<uint32_t>(val)));
		else
			m_tokens.emplace_back(LexToken(static_cast<int32_t>(val)));
	}
	updateLastTokenLoc();
}

void Scanner::genDoubleToken()
{
	double dbl = _wtof(m_buf.c_str());
	if (dbl == 0.0)
	{
		if (errno == ERANGE)
			return genError(Errors::E_Scan_FloatTooLarge);
		else if (errno == EINVAL)
			return genError(Errors::E_Scan_InvalidFloat);
	}
	m_tokens.emplace_back(LexToken(dbl));
	updateLastTokenLoc();
}

void Scanner::genIdentifierToken()
{
	// TODO: check keyword
	HString hStr = StringPool::inst().insert(m_buf);
	m_tokens.emplace_back(LexToken(hStr, LexToken::tkIdentifier));
	updateLastTokenLoc();
}
