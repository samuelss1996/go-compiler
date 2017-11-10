%{
#include "../Definitions.h"
%}

SingleLineCommentStart	\/\/
MultiLineCommentStart 	\/\*
MultiLineCommentEnd		\*\/

DecimalLiteral			[1-9]([0-9])*
OctalLiteral			0([0-7])*
HexadecimalLiteral		0(x|X)[0-9a-fA-F]([0-9a-fA-F])*

Decimals				[0-9]+
Exponent				(e|E)(\+|-)?{Decimals}
FloatLit				({Decimals}\.{Decimals}?{Exponent}?)|({Decimals}{Exponent})|(\.{Decimals}{Exponent}?)
%%

{MultiLineCommentStart}(.|\n)*{MultiLineCommentEnd}		{printf("TOKEN_COMMENT");}
{SingleLineCommentStart}.*								{printf("TOKEN_COMMENT");}

{DecimalLiteral}|{OctalLiteral}|{HexadecimalLiteral}	{printf("I");}
{FloatLit}												{printf("f");}
({Decimals}|{FloatLit})i								{printf("i");}

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