/* Based on example.c of Minisketch, authored by Pieter Wuille, Greg Maxwell,
 * Gleb Naumenko. Adaptation by Lei Yang. */

#include <stdio.h>
#include <assert.h>
#include "time.h"
#include "./minisketch.h"

int main(int argc, char *argv[]) {
	if (argc < 5) {
		printf("usage: ./benchmark <set> <diff> <n_tests> <elem_bit>\n");
		return 0;
	}
	int SET = atoi(argv[1]);
	int DIFF = atoi(argv[2]);
	int NTESTS = atoi(argv[3]);
	int LEN = atoi(argv[4]); // this is bits not bytes!
	if (!minisketch_bits_supported(LEN)) {
		printf("%d-bit elements are not supported\n", LEN);
		return 1;
	}

	clock_t start = clock();
	for (int testN = 0; testN < NTESTS; testN++) {
		minisketch *sketch_a = minisketch_create(LEN, 0, DIFF);
		for (int i = 1; i <= SET; ++i) {
			minisketch_add_uint64(sketch_a, i);
		}
		minisketch_destroy(sketch_a);
	}
	clock_t end = clock();

	double seconds = (double)(end - start) / (double)CLOCKS_PER_SEC / (double)NTESTS;
        printf("%.2f\n", (double)DIFF/seconds);
}
