%{
#include "../Definitions.h"
%}

SingleLineCommentStart	\/\/
MultiLineCommentStart 	\/\*
MultiLineCommentEnd		\*\/

DecimalLiteral			[1-9]([:digit:])*
OctalLiteral			0([0-7])*
HexadecimalLiteral		0(x|X)[0-9a-fA-F]([0-9a-fA-F])*
%%

{MultiLineCommentStart}(.|\n)*{MultiLineCommentEnd}		{printf("TOKEN_COMMENT");}
{SingleLineCommentStart}.*								{printf("TOKEN_COMMENT");}

({DecimalLiteral}|{OctalLiteral}|{HexadecimalLiteral})	{printf("TOKEN_INTEGER");}

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