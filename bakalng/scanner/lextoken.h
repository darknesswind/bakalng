#pragma once
class LexToken
{
public:
	enum Type
	{
		tkUnknown,
		tkIdentifier,
		tkKeyword,
		tkOperation,
		tkNumber,
		tkString,
	};
public:
	LexToken();
	~LexToken();

private:
	Type m_type{ tkUnknown };

	size_t m_line{ 0 };
	size_t m_char{ 0 };
};

