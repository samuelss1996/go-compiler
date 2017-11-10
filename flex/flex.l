%{
#include "../Definitions.h"
%}

IdLetter				[a-zA-Z_]

SingleLineCommentStart	\/\/
MultiLineCommentStart 	\/\*
MultiLineCommentEnd		\*\/

HexDigit				[0-9a-fA-F]

DecimalLiteral			[1-9]([0-9])*
OctalLiteral			0([0-7])*
HexadecimalLiteral		0(x|X){HexDigit}+

Decimals				[0-9]+
Exponent				(e|E)(\+|-)?{Decimals}
FloatLit				({Decimals}\.{Decimals}?{Exponent}?)|({Decimals}{Exponent})|(\.{Decimals}{Exponent}?)

CommonEscapableChar		a|b|f|n|r|t|v|\\
RuneEscapedChar			\\({CommonEscapableChar}|')
StringEscapedChar		\\({CommonEscapableChar}|\")

BigUValue				\\U{HexDigit}{8}
LittleUValue			\\u{HexDigit}{4}
HexByteValue			\\x{HexDigit}{2}
OctalByteValue			\\[0-7]{3}
ByteValue 				{OctalByteValue}|{HexByteValue}

CommonUnicodeValue		[^\\\n\"']|{LittleUValue}|{BigUValue}
RuneUnicodeValue		{CommonUnicodeValue}|{RuneEscapedChar}|\"
StringUnicodeValue		{CommonUnicodeValue}|{StringEscapedChar}|'

RawStringLiteral		`[^`]*`
InterStringLiteral		\"({StringUnicodeValue}|{ByteValue})*\"

%%

{IdLetter}({IdLetter}|[0-9])*							{printf("ID");}

{MultiLineCommentStart}(.|\n)*{MultiLineCommentEnd}		{printf("TOKEN_COMMENT");}
{SingleLineCommentStart}.*								{printf("TOKEN_COMMENT");}

{DecimalLiteral}|{OctalLiteral}|{HexadecimalLiteral}	{printf("I");}
{FloatLit}												{printf("f");}
({Decimals}|{FloatLit})i								{printf("i");}

'({RuneUnicodeValue}|{ByteValue})'						{printf("TOKEN_RUNE_LITERAL");}
{RawStringLiteral}|{InterStringLiteral}					{printf("TOKEN_STRING_LITERAL");}

%%

// TODO distinguish actual mutline comments
int main(int argc, char *argv[]) {
    if(argc == 2) {
		yyin = fopen(argv[1], "r");
	    yylex();
	    fclose(yyin);
    } else {
    	printf("Usage: %s fileToBeProcessed\n", argv[0]);
    }
}