benchmark: bench.c
	clang -O2 -o benchmark bench.c -L. -lminisketch -lstdc++
