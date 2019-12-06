CC=gcc

memtrace_analysis: src/functions.o 
	$(CC) -g src/memtrace_analysis.c src/functions.o -o memtrace_analysis
output_processing: src/functions.o
	$(CC) -g src/output_processing.c src/functions.o -lm -o output_processing
src/functions.o: src/filtering_functions.c src/structs.h
	$(CC) -g -c src/filtering_functions.c -o src/functions.o
all: memtrace_analysis output_processing

clean: 
	rm memtrace_analysis output_processing src/functions.o
