/* global declare */
%option 8bit
%option c++
%option prefix="baka"
%option noyywrap
%option perf-report

/* Unicode Support */
ASC     [\x00-\x7f]
ASCN    [\x00-\t\v-\x7f]
U       [\x80-\xbf]
U2      [\xc2-\xdf]
U3      [\xe0-\xef]
U4      [\xf0-\xf4]

UANY    {ASC}|{U2}{U}|{U3}{U}{U}|{U4}{U}{U}{U}
UANYN   {ASCN}|{U2}{U}|{U3}{U}{U}|{U4}{U}{U}{U} 
UONLY   {U2}{U}|{U3}{U}{U}|{U4}{U}{U}{U}

/* base element */
Digit			[0-9]
Char			[a-zA-Z]|{UONLY}
Dot				\.


Word			{Char}({Digit}{Char})*

_Float_1		{Digit}{Dot}{Digit}?
_Float_2		{Dot}{Digit}

%% /* pattern */
{Word}
{
	
}

%% /* func */

