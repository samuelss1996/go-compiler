REQUISITOS DEL SISTEMA
	Es necesario compilar en un sistema Linux. Además, el sistema debe disponer de BASH.

COMPILAR
	Para compilar basta con ejecutar 'make'. Aparte del ejecutable, se generarán dos ficheros *.db dentro de un directorio nuevo, llamado 'db'. Son necesarios para que el compilador funcione.
	También es posible ejecutar 'make clean', para eliminar todos los archivos generados al compilar.

EJECUTAR
	Basta con ejecutar './GoCompiler concurrentSum.go', donde concurrentSum.go es el fichero que se desea compilar.