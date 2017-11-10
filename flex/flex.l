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

"/*"([^*\n]|(\*+[^*/\n]))*\*+\/							{printf("TOKEN_COMMENT");}
"/*"([^*]|(\*+[^*/]))*\*+\/								{printf("TOKEN_COMMENT\n");}
{SingleLineCommentStart}.*								{printf("TOKEN_COMMENT");}

{DecimalLiteral}|{OctalLiteral}|{HexadecimalLiteral}	{printf("I");}
{FloatLit}												{printf("f");}
({Decimals}|{FloatLit})i								{printf("i");}

'({RuneUnicodeValue}|{ByteValue})'						{printf("TOKEN_RUNE_LITERAL");}
{RawStringLiteral}|{InterStringLiteral}					{printf("TOKEN_STRING_LITERAL");}

"+" 													{printf("PUNCTUATION_ADD");}
"&" 													{printf("PUNCTUATION_BITWISE_AND");}
"+=" 													{printf("PUNCTUATION_ADD_EQUALS");}
"&=" 													{printf("PUNCTUATION_AND_EQUALS");}
"&&" 													{printf("PUNCTUATION_AND");}
"==" 													{printf("PUNCTUATION_IS_EQUAL");}
"!=" 													{printf("PUNCTUATION_NOT_EQUALS");}
"(" 													{printf("PUNCTUATION_LEFT_PARENTHESES");}
")" 													{printf("PUNCTUATION_RIGHT_PARENTHESES");}
"-" 													{printf("PUNCTUATION_MINUS");}
"|" 													{printf("PUNCTUATION_BITWISE_OR");}
"-=" 													{printf("PUNCTUATION_MINUS_EQUALS");}
"|=" 													{printf("PUNCTUATION_OR_EQUALS");}
"||" 													{printf("PUNCTUATION_OR");}
"<" 													{printf("PUNCTUATION_LOWER");}
"<=" 													{printf("PUNCTUATION_LOWER_EQUALS");}
"[" 													{printf("PUNCTUATION_LEFT_BRACKET");}
"]" 													{printf("PUNCTUATION_RIGHT_BRACKET");}
"*" 													{printf("PUNCTUATION_START");}
"^" 													{printf("PUNCTUATION_BITWISE_XOR");}
"*=" 													{printf("PUNCTUATION_STAR_EQUALS");}
"^=" 													{printf("PUNCTUATION_XOR_EQUALS");}
"<-" 													{printf("PUNCTUATION_RECEIVE");}
">" 													{printf("PUNCTUATION_GREATER");}
">=" 													{printf("PUNCTUATION_GREATER_EQUALS");}
"{" 													{printf("PUNCTUATION_LEFT_BRACE");}
"}" 													{printf("PUNCTUATION_RIGHT_BRACE");}
"/" 													{printf("PUNCTUATION_SLASH");}
"<<" 													{printf("PUNCTUATION_LEFT_SHIFT");}
"/=" 													{printf("PUNCTUATION_SLASH_EQUALS");}
"<<=" 													{printf("PUNCTUATION_LEFT_SHIFT_EQUALS");}
"++" 													{printf("PUNCTUATION_INCREMENT");}
"=" 													{printf("PUNCTUATION_ASSIGN");}
":=" 													{printf("PUNCTUATION_SPECIAL_ASSIGN");}
"," 													{printf("PUNCTUATION_COMMA");}
";" 													{printf("PUNCTUATION_SEMICOLON");}
"%" 													{printf("PUNCTUATION_MODULUS");}
">>" 													{printf("PUNCTUATION_RIGHT_SHIFT");}
"%=" 													{printf("PUNCTUATION_MODULUS_EQUALS");}
">>=" 													{printf("PUNCTUATION_RIGHT_SHIFT_EQUALS");}
"--" 													{printf("PUNCTUATION_DECREMENT");}
"!" 													{printf("PUNCTUATION_NOT");}
"..." 													{printf("PUNCTUATION_ELLIPSIS");}
"." 													{printf("PUNCTUATION_DOT");}
":" 													{printf("PUNCTUATION_COLON");}
"&^" 													{printf("PUNCTUATION_AND_XOR");}
"&^=" 													{printf("PUNCTUATION_AND_XOR_EQUALS");}

%%

int main(int argc, char *argv[]) {
    if(argc == 2) {
		yyin = fopen(argv[1], "r");
	    yylex();
	    fclose(yyin);
    } else {
    	printf("Usage: %s fileToBeProcessed\n", argv[0]);
    }
}