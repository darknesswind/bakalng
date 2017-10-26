#pragma once
#include <unordered_set>
#include <string>

struct HString { const void* handle = nullptr; };
class StringPool
{
public:
	StringPool();
	~StringPool();

	HString insert(const char* pstr);
	HString insert(const std::string& str);

private:
	std::unordered_set<std::string> m_set;
};

