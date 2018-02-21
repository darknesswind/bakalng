#pragma once

enum class Errors
{
	E_No_Error = 0,
	E_Scan_InvalidHex,
	E_Scan_InvalidOct,
	E_Scan_InvalidBin,
	E_Scan_InvalidDec,
	E_Scan_IntegerTooLarge,
	E_Scan_FloatTooLarge,
	E_Scan_InvalidFloat,
};