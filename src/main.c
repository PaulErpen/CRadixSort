#include "radix_sort.h"
#include "verify_order.h"
#include <stdio.h>
#include "generate_sequence.h"

int main() {
    int num_elements = 10;
    unsigned int index_in[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    unsigned int index_out[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    unsigned int depths[] = {170.1, 170.2, 45.32, 45.23, 90.23, 802.45, 24.433, 2.4534, 66.454, 255.45};

    radix_sort(
        num_elements,
        index_in,
        index_out,
        depths
    );

    verify_order(
        num_elements,
        index_out,
        depths
    );


    int large_num_elements = 255;
    struct Sequence sequence = generate_sequence(large_num_elements);
    radix_sort(
        large_num_elements,
        sequence.index_in,
        sequence.index_out,
        sequence.depths
    );
    verify_order(
        large_num_elements,
        sequence.index_out,
        sequence.depths
    );

    return 0;
}