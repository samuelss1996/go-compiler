GoCompiler: SHELL:=/bin/bash 

GoCompiler:
	mkdir -p db

	cat Definitions.h | grep -E "KEYWORD_|DATA_TYPE_|CONSTANT_" | sed -r "s/.*_//" | tr '[:upper:]' '[:lower:]' > db/keywords.db
	cat Definitions.h | grep -E "PUNCTUATION_" | sed -r "s/.*\/\/ //"

	gcc *.c */*.c -o GoCompiler