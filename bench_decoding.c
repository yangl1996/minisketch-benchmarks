/* Based on example.c of Minisketch, authored by Pieter Wuille, Greg Maxwell,
 * Gleb Naumenko. Adaptation by Lei Yang. */

#include <stdio.h>
#include <assert.h>
#include "time.h"
#include "./minisketch.h"

__attribute__((optnone)) int
main(int argc, char *argv[]) {
	if (argc < 4) {
		printf("usage: ./benchmark <diff> <n_tests> <elem_bit>\n");
		return 0;
	}
	int DIFF = atoi(argv[1]);
	int NTESTS = atoi(argv[2]);
	int LEN = atoi(argv[3]); // this is bits not bytes!
	if (!minisketch_bits_supported(LEN)) {
		printf("%d-bit elements are not supported\n", LEN);
		return 1;
	}
	uint64_t *differences = malloc(DIFF * sizeof(uint64_t));

	minisketch **sketches = malloc(NTESTS * sizeof(minisketch*));
	uint64_t nextItem = 1;
	for (int testN = 0; testN < NTESTS; testN++) {
		sketches[testN] = minisketch_create(LEN, 0, DIFF);
		for (int i = 1; i <= DIFF; ++i) {
			minisketch_add_uint64(sketches[testN], nextItem++);
		}
	}
	clock_t start = clock();
	for (int testN = 0; testN < NTESTS; testN++) {
		ssize_t num_differences = minisketch_decode(sketches[testN], DIFF, differences);
		if (num_differences != DIFF) {
			printf("FAIL: got %zd diff\n", num_differences);
		}
	}
	clock_t end = clock();
	double seconds = (double)(end - start) / (double)CLOCKS_PER_SEC / (double)NTESTS;
	printf("%.2f\n", (double)DIFF/seconds);
}
