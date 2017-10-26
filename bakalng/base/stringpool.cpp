#include "stdafx.h"
#include "stringpool.h"


StringPool::StringPool()
{
}


StringPool::~StringPool()
{
}

HString StringPool::insert(const char* pstr)
{
	auto pair = m_set.insert(pstr);
	return HString{ (*pair.first).c_str() };
}

HString StringPool::insert(const std::string& str)
{
	auto pair = m_set.insert(str);
	return HString{ (*pair.first).c_str() };
}
