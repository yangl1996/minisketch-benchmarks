/* Based on example.c of Minisketch, authored by Pieter Wuille, Greg Maxwell,
 * Gleb Naumenko. Adaptation by Lei Yang. */

#include <stdio.h>
#include <assert.h>
#include "time.h"
#include "./minisketch.h"

int main(void) {
  int DIFF = 200;
  int NTESTS = 1000;
  int LEN = 64; // this is bits not bytes!

  minisketch *sketch_a = minisketch_create(LEN, 0, DIFF);

  for (int i = 1; i <= DIFF; ++i) {
    minisketch_add_uint64(sketch_a, i);
    if (i % 1000 == 0) {
      printf("%d symbols added\n", i);
    }
  }

  size_t sersize = minisketch_serialized_size(sketch_a);
  unsigned char *buffer_a = malloc(sersize);
  minisketch_serialize(sketch_a, buffer_a);
  minisketch_destroy(sketch_a);

  minisketch *sketch_b = minisketch_create(LEN, 0, DIFF); // Bob's own sketch

  sketch_a = minisketch_create(LEN, 0, DIFF);     // Alice's sketch
  minisketch_deserialize(sketch_a, buffer_a); // Load Alice's sketch
  free(buffer_a);

  printf("merging sketches\n");
  // Merge the elements from sketch_a into sketch_b. The result is a sketch_b
  // which contains all elements that occurred in Alice's or Bob's sets, but not
  // in both.
  minisketch_merge(sketch_b, sketch_a);

  uint64_t *differences = malloc(DIFF * sizeof(uint64_t));
  printf("reconciling\n");
  clock_t start = clock();
  for (int i = 0; i < NTESTS; i++) {
	  ssize_t num_differences = minisketch_decode(sketch_b, DIFF, differences);
	  if (num_differences != DIFF) {
		  printf("FAIL: got %zd diff\n", num_differences);
	  }
  }
  clock_t end = clock();
  float seconds = (float)(end - start) / CLOCKS_PER_SEC / (float)NTESTS;
  printf("reconciliation complete\n");
  printf("%.2f ms, %.2f diff/s\n", seconds * 1000, (float)DIFF/seconds);
  minisketch_destroy(sketch_a);
  minisketch_destroy(sketch_b);
}
