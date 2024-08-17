#include "radix_sort.h"
#include "verify_order.h"
#include <stdio.h>
#include "generate_sequence.h"

void print_results(unsigned int *index, float* depths, unsigned int size) {
    for (unsigned int i = 0; i < size; i++) {
        printf("%d:\t%f \t %d \n", index[i], depths[index[i]], float_to_uint(depths[index[i]]));
    }
    printf("\n");
}

int main() {
    int num_elements = 10;
    unsigned int index_in[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    unsigned int index_out[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    float depths[] = {170.23f, 170.1f, 45.23213f, 45.1f, 88890.231f, 32.231f, 88824.2343f, 2.2123f, 66.123f, 12.3432f};
    //float depths[] = {2.23f, 2.1f, 2.23213f, 88890.1f, 2.231f, 2.231f, 2.2343f, 2.2123f, 2.123f, 2.3432f};

    radix_sort(
        num_elements,
        index_in,
        index_out,
        depths
    );

    print_results(index_in, depths, num_elements);

    verify_order(
        num_elements,
        index_in,
        depths
    );


    int large_num_elements = 1000;
    struct Sequence sequence = generate_sequence(large_num_elements);
    radix_sort(
        large_num_elements,
        sequence.index_in,
        sequence.index_out,
        sequence.depths
    );
    print_results(sequence.index_in, sequence.depths, large_num_elements);
    verify_order(
        large_num_elements,
        sequence.index_in,
        sequence.depths
    );

    return 0;
}