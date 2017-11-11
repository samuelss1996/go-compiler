%{
#include "../Definitions.h"
#include "../symbols/SymbolsTable.h"
#include "../errors/Errors.h"

#define YY_DECL int yylex(SymbolsTable* symbolsTable)

int column = 0;
int charCount(char* string, char character);
%}

IdLetter				[a-zA-Z_]
Blank					[ \t\r]
NewLine					{Blank}*\n

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

{IdLetter}({IdLetter}|[0-9])*							{column += yyleng; return findOrAdd(symbolsTable, yytext, TOKEN_IDENTIFIER);}
{NewLine}+ 												{column = 0; yylineno += charCount(yytext, '\n'); return '\n';}
{Blank}+												{column += yyleng;}

{SingleLineCommentStart}.*								{column += yyleng;}
"/*"([^*\n]|(\*+[^*/\n]))*\*+\/							{column += yyleng;}
{NewLine}*("/*"([^*]|(\*+[^*/]))*\*+\/){NewLine}*		{column = 0; yylineno += charCount(yytext, '\n'); return '\n';}

{DecimalLiteral}|{OctalLiteral}|{HexadecimalLiteral}	{column += yyleng; return TOKEN_INTEGER_LITERAL;}
{FloatLit}												{column += yyleng; return TOKEN_FLOATING_POINT_LITERAL;}
({Decimals}|{FloatLit})i								{column += yyleng; return TOKEN_IMAGINARY_LITERAL;}

'({RuneUnicodeValue}|{ByteValue})'						{column += yyleng; return TOKEN_RUNE_LITERAL;}
{RawStringLiteral}|{InterStringLiteral}					{column += yyleng; return TOKEN_STRING_LITERAL;}

"+" 													{column += yyleng; return PUNCTUATION_ADD;}
"&" 													{column += yyleng; return PUNCTUATION_BITWISE_AND;}
"+=" 													{column += yyleng; return PUNCTUATION_ADD_EQUALS;}
"&=" 													{column += yyleng; return PUNCTUATION_AND_EQUALS;}
"&&" 													{column += yyleng; return PUNCTUATION_AND;}
"==" 													{column += yyleng; return PUNCTUATION_IS_EQUAL;}
"!=" 													{column += yyleng; return PUNCTUATION_NOT_EQUALS;}
"(" 													{column += yyleng; return PUNCTUATION_LEFT_PARENTHESES;}
")" 													{column += yyleng; return PUNCTUATION_RIGHT_PARENTHESES;}
"-" 													{column += yyleng; return PUNCTUATION_MINUS;}
"|" 													{column += yyleng; return PUNCTUATION_BITWISE_OR;}
"-=" 													{column += yyleng; return PUNCTUATION_MINUS_EQUALS;}
"|=" 													{column += yyleng; return PUNCTUATION_OR_EQUALS;}
"||" 													{column += yyleng; return PUNCTUATION_OR;}
"<" 													{column += yyleng; return PUNCTUATION_LOWER;}
"<=" 													{column += yyleng; return PUNCTUATION_LOWER_EQUALS;}
"[" 													{column += yyleng; return PUNCTUATION_LEFT_BRACKET;}
"]" 													{column += yyleng; return PUNCTUATION_RIGHT_BRACKET;}
"*" 													{column += yyleng; return PUNCTUATION_START;}
"^" 													{column += yyleng; return PUNCTUATION_BITWISE_XOR;}
"*=" 													{column += yyleng; return PUNCTUATION_STAR_EQUALS;}
"^=" 													{column += yyleng; return PUNCTUATION_XOR_EQUALS;}
"<-" 													{column += yyleng; return PUNCTUATION_RECEIVE;}
">" 													{column += yyleng; return PUNCTUATION_GREATER;}
">=" 													{column += yyleng; return PUNCTUATION_GREATER_EQUALS;}
"{" 													{column += yyleng; return PUNCTUATION_LEFT_BRACE;}
"}" 													{column += yyleng; return PUNCTUATION_RIGHT_BRACE;}
"/" 													{column += yyleng; return PUNCTUATION_SLASH;}
"<<" 													{column += yyleng; return PUNCTUATION_LEFT_SHIFT;}
"/=" 													{column += yyleng; return PUNCTUATION_SLASH_EQUALS;}
"<<=" 													{column += yyleng; return PUNCTUATION_LEFT_SHIFT_EQUALS;}
"++" 													{column += yyleng; return PUNCTUATION_INCREMENT;}
"=" 													{column += yyleng; return PUNCTUATION_ASSIGN;}
":=" 													{column += yyleng; return PUNCTUATION_SPECIAL_ASSIGN;}
"," 													{column += yyleng; return PUNCTUATION_COMMA;}
";" 													{column += yyleng; return PUNCTUATION_SEMICOLON;}
"%" 													{column += yyleng; return PUNCTUATION_MODULUS;}
">>" 													{column += yyleng; return PUNCTUATION_RIGHT_SHIFT;}
"%=" 													{column += yyleng; return PUNCTUATION_MODULUS_EQUALS;}
">>=" 													{column += yyleng; return PUNCTUATION_RIGHT_SHIFT_EQUALS;}
"--" 													{column += yyleng; return PUNCTUATION_DECREMENT;}
"!" 													{column += yyleng; return PUNCTUATION_NOT;}
"..." 													{column += yyleng; return PUNCTUATION_ELLIPSIS;}
"." 													{column += yyleng; return PUNCTUATION_DOT;}
":" 													{column += yyleng; return PUNCTUATION_COLON;}
"&^" 													{column += yyleng; return PUNCTUATION_AND_XOR;}
"&^=" 													{column += yyleng; return PUNCTUATION_AND_XOR_EQUALS;}

<<EOF>>													{column += yyleng; return EOF;}

\"({StringUnicodeValue}|{ByteValue})* 					{column += yyleng; expectingEndOfString(yylineno, column); return yylex(symbolsTable);}
\"[^\n]*\"												{column += yyleng; invalidEscapedCharInsideString(yylineno, column - yyleng + 1, yytext); return yylex(symbolsTable);}
''														{column += yyleng; emptyRune(yylineno, column); return yylex(symbolsTable);}
'({RuneUnicodeValue}|{ByteValue})*						{column += yyleng; expectingEndOfRune(yylineno, column); return yylex(symbolsTable);}
'({RuneUnicodeValue}|{ByteValue}){2,}'					{column += yyleng; multiCharacterRune(yylineno, column); return yylex(symbolsTable);}
'[^\n]+'												{column += yyleng; invalidEscapedCharInsideRune(yylineno, column - yyleng + 1, yytext); return yylex(symbolsTable);}
.														{column += yyleng; invalidSymbol(yylineno, column, *yytext); return yylex(symbolsTable);}

%%

int charCount(char* string, char character) {
	int result = 0;
	char *pointer;

	for(pointer = string; *pointer != '\0'; pointer++) {
		if(*pointer == character) {
			result++;
		}
	}

	return result;
}

/*int main(int argc, char *argv[]) {
    if(argc == 2) {
		yyin = fopen(argv[1], "r");
	    yylex();
	    fclose(yyin);
    } else {
    	printf("Usage: %s fileToBeProcessed\n", argv[0]);
    }
}*/