﻿// bakalng.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "scanner/scanner.h"

int main(int argc, char** argv)
{
// 	for (int i = 0; i < argc; ++i)
// 	{
// 		char* arg = argv[i];
// 		if (arg[0] == '-')
// 	}
	if (argc < 1)
		return -1;
	char* input = argv[1];

	Scanner scanner;
	scanner.scan(input);

    return 0;
}

