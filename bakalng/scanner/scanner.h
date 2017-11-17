#pragma once

class LTextStream;
class Scanner
{
public:
	Scanner();
	~Scanner();

	void scan(const char* szFile);

private:
	void procFileScope(LTextStream& stream);
};

