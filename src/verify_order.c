#include <assert.h>

void verify_order(int num_elements, unsigned int *index_out, float *depths) {
    for (int i = 0; i < num_elements - 1; i++) {
        assert(depths[index_out[i]] <= depths[index_out[i + 1]]);
    }
}