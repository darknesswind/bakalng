#pragma once
class Token
{
public:
	enum Type
	{
		tkUnknown,
		tkIdentifier,
		tkString,
	};
public:
	Token();
	~Token();

private:
	Type m_type{ tkUnknown };

	size_t m_line{ 0 };
	size_t m_char{ 0 };
};

