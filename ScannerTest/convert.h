#pragma once
#include "CppUnitTest.h"
#include "scanner/lextoken.h"

#define BeginEnum(type)  switch (type) {
#define AddEnumVal(value) case value: return L#value;
#define EndEnum() default: { RETURN_WIDE_STRING(t); } }

namespace Microsoft {
	namespace VisualStudio {
		namespace CppUnitTestFramework
		{
			template<> inline std::wstring ToString<LexToken::Type>(const LexToken::Type& t)
			{
				BeginEnum(t);
				AddEnumVal(LexToken::tkUnknown);
				AddEnumVal(LexToken::tkIdentifier);
				AddEnumVal(LexToken::tkKeyword);
				AddEnumVal(LexToken::tkOperation);
				AddEnumVal(LexToken::tkString);
				AddEnumVal(LexToken::tkComment);

				AddEnumVal(LexToken::tkNumInt32);
				AddEnumVal(LexToken::tkNumUInt32);
				AddEnumVal(LexToken::tkNumInt64);
				AddEnumVal(LexToken::tkNumUInt64);
				AddEnumVal(LexToken::tkNumFloat );
				AddEnumVal(LexToken::tkNumDouble);
				EndEnum();
			}
		}
	}
}
