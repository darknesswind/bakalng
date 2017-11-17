#include "stdafx.h"
#include "scanner.h"
#include "LTextStream.h"

Scanner::Scanner()
{
}


Scanner::~Scanner()
{
}

void Scanner::scan(const char* szFile)
{
	LTextStream stream;
	stream.openRead(szFile);
	procFileScope(stream);
}

void Scanner::procFileScope(LTextStream& stream)
{
	enum State
	{

	};

}
