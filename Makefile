CC=gcc

memtrace_analysis: src/filter/functions.o 
	$(CC) -g src/analysisprocess/memtrace_analysis.c src/filter/functions.o -o src/analysisprocess/memtrace_analysis
output_processing: src/filter/functions.o
	$(CC) -g src/analysisprocess/output_processing.c src/filter/functions.o -lm -o src/analysisprocess/output_processing
src/filter/functions.o: src/filter/filtering_functions.c src/libs/functions.h src/libs/structs.h
	$(CC) -g -c src/filter/filtering_functions.c -o src/filter/functions.o
src/tcl/tcl.o : src/tcl/tcl.c src/filter/functions.o src/libs/functions.h src/libs/structs.h
	$(CC) -g -c src/tcl/tcl.c -o src/tcl/tcl.o
src/tab/tabcompletion.o : src/tab/tabcompletion.c src/tab/tabcompletion.h
	$(CC) -g -c src/tab/tabcompletion.c -o src/tab/tabcompletion.o
all: src/libs/structs.h src/libs/functions.h src/tcl/tcl.h memtrace_analysis output_processing src/filter/functions.o src/tcl/tcl.o src/tab/tabcompletion.o
	$(CC) -g src/mapvis.c src/filter/functions.o src/tcl/tcl.o src/tab/tabcompletion.o -ltcl -lreadline -lm -o mapvis

clean: 
	rm src/analysisprocess/memtrace_analysis src/analysisprocess/output_processing src/filter/functions.o src/tcl/tcl.o src/tab/tabcompletion.o mapvis
