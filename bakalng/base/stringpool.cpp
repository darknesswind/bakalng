#include "stdafx.h"
#include "stringpool.h"


StringPool::StringPool()
{
}


StringPool::~StringPool()
{
}

HString StringPool::insert(const LString& str)
{
	if (str.empty() || str.size() > c_blockSize)
		return HString{ nullptr };

	auto iterSet = m_set.find(HashItem{ str.size(), str.c_str() });
	if (iterSet != m_set.end())
		return HString{ iterSet->pRawDat };

	size_t szData = (str.size() + 1) * sizeof(LString::value_type);
	auto iter = std::find_if(m_blockList.begin(), m_blockList.end(), [szData](const Block& item)
	{
		return (item.freeSize >= szData);
	});
	if (iter == m_blockList.end())
	{
		m_blockList.emplace_back(Block());
		iter = m_blockList.end();
		--iter;
	}

	if (iter != m_blockList.end())
	{
		Block& item = (*iter);
		LString::value_type* pFreeDat = reinterpret_cast<LString::value_type*>
			(item.memory.data() + item.memory.size() - item.freeSize);
		memcpy(pFreeDat, str.data(), szData);
		pFreeDat[str.size()] = 0;
		item.freeSize -= szData;
		auto result = m_set.insert(HashItem{ str.size(), pFreeDat });
		assert(result.second);
		if (item.freeSize == 0)
			m_fullList.splice(m_fullList.end(), m_blockList, iter);

		return HString{ pFreeDat };
	}
	return HString{ nullptr };
}

const char16_t* StringPool::getString(HString handle)
{
	if (handle.handle)
		return reinterpret_cast<const char16_t*>(handle.handle);
	else
		return u"";
}

StringPool& StringPool::inst()
{
	static StringPool instance;
	return instance;
}
