#include "generate_sequence.h"
#include <stdlib.h>

float generateRandomFloat() {
    return (float)rand() / (float)(RAND_MAX);
}

struct Sequence generate_sequence(unsigned int num_elements) {
    struct Sequence sequence;
    sequence.depths = malloc(num_elements * sizeof(float));
    sequence.index_in = malloc(num_elements * sizeof(unsigned int));
    sequence.index_out = malloc(num_elements * sizeof(unsigned int));
    for (int i = 0; i < num_elements; i++) {
        sequence.depths[i] = generateRandomFloat();
        sequence.index_in[i] = i;
        sequence.index_out[i] = i;
    }
    return sequence;
}
