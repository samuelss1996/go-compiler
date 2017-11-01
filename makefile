GoCompiler: SHELL:=/bin/bash 

GoCompiler:
	mkdir -p db

	cat Definitions.h | grep -E "KEYWORD_|DATA_TYPE_|CONSTANT_" | sed -r "s/.*_//" | tr '[:upper:]' '[:lower:]' > db/keywords.db
	cat Definitions.h | grep -E "PUNCTUATION_" | sed -r "s/[^0-9]*([0-9]*).*\/\/\s*(.*)\s*/\2 \1/" > db/operators.db

	gcc *.c */*.c -o GoCompiler