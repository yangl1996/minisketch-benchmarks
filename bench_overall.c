/* Based on example.c of Minisketch, authored by Pieter Wuille, Greg Maxwell,
 * Gleb Naumenko. Adaptation by Lei Yang. */

#include <stdio.h>
#include <assert.h>
#include "time.h"
#include "./minisketch.h"

int main(int argc, char *argv[]) {
	if (argc < 5) {
		printf("usage: ./benchmark <common> <diff> <n_tests> <elem_bit>\n");
		return 0;
	}
	int COMMON = atoi(argv[1]);
	int DIFF = atoi(argv[2]);
	int NTESTS = atoi(argv[3]);
	int LEN = atoi(argv[4]); // this is bits not bytes!
	if (!minisketch_bits_supported(LEN)) {
		printf("%d-bit elements are not supported\n", LEN);
		return 1;
	}
	uint64_t *differences = malloc(DIFF * sizeof(uint64_t));

	clock_t start = clock();
	for (int testN = 0; testN < NTESTS; testN++) {
		minisketch *sketch_a = minisketch_create(LEN, 0, DIFF);
		minisketch *sketch_b = minisketch_create(LEN, 0, DIFF);

		for (int i = 1; i <= COMMON; ++i) {
			minisketch_add_uint64(sketch_a, i);
			minisketch_add_uint64(sketch_b, i);
		}
		for (int i = COMMON+1; i <= COMMON+DIFF; ++i) {
			minisketch_add_uint64(sketch_a, i);
		}

		minisketch_merge(sketch_b, sketch_a);

		ssize_t num_differences = minisketch_decode(sketch_b, DIFF, differences);
		if (num_differences != DIFF) {
			printf("FAIL: got %zd diff\n", num_differences);
		}
		minisketch_destroy(sketch_a);
		minisketch_destroy(sketch_b);
	}
	clock_t end = clock();
	float seconds = (float)(end - start) / CLOCKS_PER_SEC / (float)NTESTS;

	printf("%.2f ms, %.2f diff/s\n", seconds * 1000, (float)DIFF/seconds);
}
