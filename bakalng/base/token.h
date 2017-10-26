#pragma once

class Token
{
public:
	enum Type
	{
		tkEof,
		// keyword
		tkDef,
		tkExtern,
		// value
		tkIdentifier,
		tkNumber,
	};
public:
	Token();
	~Token();

private:
	Type m_type;
	union
	{
		void* m_hName;
		double m_dblVal;
	};
};

