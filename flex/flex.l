%{
#include "../Definitions.h"
%}

IdLetter				[a-zA-Z_]
Blank					[ \t\r]

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

{IdLetter}({IdLetter}|[0-9])*							{return TOKEN_IDENTIFIER;}
{Blank}+												{}

{SingleLineCommentStart}.*								{}
"/*"([^*\n]|(\*+[^*/\n]))*\*+\/							{}
"/*"([^*]|(\*+[^*/]))*\*+\/								{return '\n';}

{DecimalLiteral}|{OctalLiteral}|{HexadecimalLiteral}	{return TOKEN_INTEGER_LITERAL;}
{FloatLit}												{return TOKEN_FLOATING_POINT_LITERAL;}
({Decimals}|{FloatLit})i								{return TOKEN_IMAGINARY_LITERAL;}

'({RuneUnicodeValue}|{ByteValue})'						{return TOKEN_RUNE_LITERAL;}
{RawStringLiteral}|{InterStringLiteral}					{return TOKEN_STRING_LITERAL;}

"+" 													{return PUNCTUATION_ADD;}
"&" 													{return PUNCTUATION_BITWISE_AND;}
"+=" 													{return PUNCTUATION_ADD_EQUALS;}
"&=" 													{return PUNCTUATION_AND_EQUALS;}
"&&" 													{return PUNCTUATION_AND;}
"==" 													{return PUNCTUATION_IS_EQUAL;}
"!=" 													{return PUNCTUATION_NOT_EQUALS;}
"(" 													{return PUNCTUATION_LEFT_PARENTHESES;}
")" 													{return PUNCTUATION_RIGHT_PARENTHESES;}
"-" 													{return PUNCTUATION_MINUS;}
"|" 													{return PUNCTUATION_BITWISE_OR;}
"-=" 													{return PUNCTUATION_MINUS_EQUALS;}
"|=" 													{return PUNCTUATION_OR_EQUALS;}
"||" 													{return PUNCTUATION_OR;}
"<" 													{return PUNCTUATION_LOWER;}
"<=" 													{return PUNCTUATION_LOWER_EQUALS;}
"[" 													{return PUNCTUATION_LEFT_BRACKET;}
"]" 													{return PUNCTUATION_RIGHT_BRACKET;}
"*" 													{return PUNCTUATION_START;}
"^" 													{return PUNCTUATION_BITWISE_XOR;}
"*=" 													{return PUNCTUATION_STAR_EQUALS;}
"^=" 													{return PUNCTUATION_XOR_EQUALS;}
"<-" 													{return PUNCTUATION_RECEIVE;}
">" 													{return PUNCTUATION_GREATER;}
">=" 													{return PUNCTUATION_GREATER_EQUALS;}
"{" 													{return PUNCTUATION_LEFT_BRACE;}
"}" 													{return PUNCTUATION_RIGHT_BRACE;}
"/" 													{return PUNCTUATION_SLASH;}
"<<" 													{return PUNCTUATION_LEFT_SHIFT;}
"/=" 													{return PUNCTUATION_SLASH_EQUALS;}
"<<=" 													{return PUNCTUATION_LEFT_SHIFT_EQUALS;}
"++" 													{return PUNCTUATION_INCREMENT;}
"=" 													{return PUNCTUATION_ASSIGN;}
":=" 													{return PUNCTUATION_SPECIAL_ASSIGN;}
"," 													{return PUNCTUATION_COMMA;}
";" 													{return PUNCTUATION_SEMICOLON;}
"%" 													{return PUNCTUATION_MODULUS;}
">>" 													{return PUNCTUATION_RIGHT_SHIFT;}
"%=" 													{return PUNCTUATION_MODULUS_EQUALS;}
">>=" 													{return PUNCTUATION_RIGHT_SHIFT_EQUALS;}
"--" 													{return PUNCTUATION_DECREMENT;}
"!" 													{return PUNCTUATION_NOT;}
"..." 													{return PUNCTUATION_ELLIPSIS;}
"." 													{return PUNCTUATION_DOT;}
":" 													{return PUNCTUATION_COLON;}
"&^" 													{return PUNCTUATION_AND_XOR;}
"&^=" 													{return PUNCTUATION_AND_XOR_EQUALS;}

<<EOF>>													{return EOF;}

%%

/*int main(int argc, char *argv[]) {
    if(argc == 2) {
		yyin = fopen(argv[1], "r");
	    yylex();
	    fclose(yyin);
    } else {
    	printf("Usage: %s fileToBeProcessed\n", argv[0]);
    }
}*/