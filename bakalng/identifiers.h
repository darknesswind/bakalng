#pragma once

struct Identifier
{
	HString hString;

	const static size_t MaxLength = 2048;

	// https://docs.microsoft.com/zh-cn/cpp/cpp/identifiers-cpp
	template <char16_t firstChar, char16_t lastChar>
	inline static bool isInRange(char16_t ch) { return (ch >= firstChar && ch <= lastChar); }
	inline static bool isFirstUnicodeChar(char16_t ch)
	{
		return (
			ch == 0x00A8 || ch == 0x00AA || ch == 0x00AD || ch == 0x00AF || ch == 0x2054 ||
			isInRange<0x00B2, 0x00B5>(ch) || isInRange<0x00B7, 0x00BA>(ch) ||
			isInRange<0x00BC, 0x00BE>(ch) || isInRange<0x00C0, 0x00D6>(ch) ||
			isInRange<0x00D8, 0x00F6>(ch) || isInRange<0x00F8, 0x00FF>(ch) ||
			isInRange<0x0100, 0x02FF>(ch) || isInRange<0x0370, 0x167F>(ch) ||
			isInRange<0x1681, 0x180D>(ch) || isInRange<0x180F, 0x1DBF>(ch) ||
			isInRange<0x1E00, 0x1FFF>(ch) || isInRange<0x200B, 0x200D>(ch) ||
			isInRange<0x202A, 0x202E>(ch) || isInRange<0x203F, 0x2040>(ch) ||
			isInRange<0x2060, 0x206F>(ch) || isInRange<0x2070, 0x20CF>(ch) ||
			isInRange<0x2100, 0x218F>(ch) || isInRange<0x2460, 0x24FF>(ch) ||
			isInRange<0x2776, 0x2793>(ch) || isInRange<0x2C00, 0x2DFF>(ch) ||
			isInRange<0x2E80, 0x2FFF>(ch) || isInRange<0x3004, 0x3007>(ch) ||
			isInRange<0x3021, 0x302F>(ch) || isInRange<0x3031, 0x303F>(ch) ||
			isInRange<0x3040, 0xD7FF>(ch) || isInRange<0xF900, 0xFD3D>(ch) ||
			isInRange<0xFD40, 0xFDCF>(ch) || isInRange<0xFDF0, 0xFE1F>(ch) ||
			isInRange<0xFE30, 0xFE44>(ch) || isInRange<0xFE47, 0xFFFD>(ch)
			// Utf32
			// 		isInRange<0x10000, 0x1FFFD>(ch) || isInRange<0x20000, 0x2FFFD>(ch) ||
			// 		isInRange<0x30000, 0x3FFFD>(ch) || isInRange<0x40000, 0x4FFFD>(ch) ||
			// 		isInRange<0x50000, 0x5FFFD>(ch) || isInRange<0x60000, 0x6FFFD>(ch) ||
			// 		isInRange<0x70000, 0x7FFFD>(ch) || isInRange<0x80000, 0x8FFFD>(ch) ||
			// 		isInRange<0x90000, 0x9FFFD>(ch) || isInRange<0xA0000, 0xAFFFD>(ch) ||
			// 		isInRange<0xB0000, 0xBFFFD>(ch) || isInRange<0xC0000, 0xCFFFD>(ch) ||
			// 		isInRange<0xD0000, 0xDFFFD>(ch) || isInRange<0xE0000, 0xEFFFD>(ch)
			);
	}
	inline static bool isFirstChar(char16_t ch)
	{
		return (
			ch == u'_' ||
			isInRange<u'a', u'z'>(ch) ||
			isInRange<u'A', u'Z'>(ch) ||
			isFirstUnicodeChar(ch)
			);
	}
	inline static bool isFollowUnicodeChar(char16_t ch)
	{
		return (
			isInRange<0x0300, 0x036F>(ch) ||
			isInRange<0x1DC0, 0x1DFF>(ch) ||
			isInRange<0x20D0, 0x20FF>(ch) ||
			isInRange<0xFE20, 0xFE2F>(ch)
			);
	}
	inline static bool isFollowChar(char16_t ch)
	{
		return (
			isFirstChar(ch) ||
			isInRange<u'0', u'9'>(ch) ||
			isFollowUnicodeChar(ch)
			);
	}
};
