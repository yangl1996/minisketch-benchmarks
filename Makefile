all: overall encoding decoding

overall: bench_overall.c
	clang -O2 -o overall bench_overall.c -L. -lminisketch -lstdc++

encoding: bench_encoding.c
	clang -O2 -o encoding bench_encoding.c -L. -lminisketch -lstdc++

decoding: bench_decoding.c
	clang -O2 -o decoding bench_decoding.c -L. -lminisketch -lstdc++

clean: .PHONY
	rm -f overall encoding decoding
