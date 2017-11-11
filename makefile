GoCompiler: SHELL:=/bin/bash 

GoCompiler:
	mkdir -p db

	cat Definitions.h | grep -E "KEYWORD_|DATA_TYPE_|CONSTANT_" | sed -r "s/.*_//" | tr '[:upper:]' '[:lower:]' > db/keywords.db

	gcc -Wall *.c */*.c -o GoCompiler -lfl

clean:
	rm -Rf db
	rm -f GoCompiler