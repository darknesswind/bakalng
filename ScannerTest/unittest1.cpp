#include "stdafx.h"
#include "scanner/scanner.h"
#include "CppUnitTestLogger.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ScannerTest
{
	TEST_CLASS(UnitTestSimple)
	{
	public:
		TEST_METHOD(TestComment)
		{
			Scanner scanner;
#ifdef _DEBUG
			wchar_t sComment[] = L" /*asdf*/ //asdf ";
			scanner.scan(sComment, sizeof(sComment));
			const LexTokens& tokens = scanner.tokens();
			Assert::AreEqual(tokens.size(), 2u);
			for (auto tok : tokens)
				Assert::AreEqual(tok.type(), LexToken::tkComment);
			Assert::AreEqual(tokens[0].location().col, 1u);
			Assert::AreEqual(tokens[1].location().col, 10u);
#endif
		}
		TEST_METHOD(TestNumber)
		{
			Scanner scanner;
			const wchar_t sNumber[] = L"0x1234+0b10101-0o777*0009999999/10./1e2+1.0e+3-.9";
			scanner.scan(sNumber, sizeof(sNumber));
			const LexTokens& tokens = scanner.tokens();
			Assert::AreEqual(tokens.size(), 15u);
			Assert::AreEqual(tokens[0].int32(), 0x1234);
			Assert::AreEqual(tokens[2].int32(), 0b10101);
			Assert::AreEqual(tokens[4].int32(), 0777);
			Assert::AreEqual(tokens[6].int32(), 9999999);
			Assert::AreEqual(tokens[8].float64(), 10.);
			Assert::AreEqual(tokens[10].float64(), 1e2);
			Assert::AreEqual(tokens[12].float64(), 1.0e+3);
			Assert::AreEqual(tokens[14].float64(), .9);

		}
	};
}