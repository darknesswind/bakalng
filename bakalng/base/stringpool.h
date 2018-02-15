#pragma once
#include <unordered_set>
#include "LString.h"
#include "lrefobject.h"

struct HString { const void* handle = nullptr; };
class StringPool
{
public:
	StringPool();
	~StringPool();

	HString insert(const LString& str);
	const char16_t* getString(HString handle);

public:

private:
	const static size_t c_blockSize = 0x1000;
	typedef std::array<char, c_blockSize> MemBlock;
	struct Block
	{
		MemBlock memory;
		size_t freeSize{ c_blockSize };
	};
	typedef LRefObject<Block> BlockItem;
	struct HashItem
	{
		size_t size;
		const LString::value_type* pRawDat;

		struct hash
		{
			size_t operator()(const HashItem& key) const
			{
				return std::_Hash_array_representation(key.pRawDat, key.size);
			}
			bool operator()(const HashItem& lhs, const HashItem& rhs) const
			{
				return (lhs.size == rhs.size) && !memcmp(lhs.pRawDat, rhs.pRawDat, lhs.size);
			}
		};
	};

	std::list<Block> m_blockList;
	std::list<Block> m_fullList;
	std::unordered_set<HashItem, HashItem::hash, HashItem::hash> m_set;
};
