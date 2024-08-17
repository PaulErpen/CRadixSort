#include "radix_sort.h"
#include "verify_order.h"
#include <stdio.h>
#include "generate_sequence.h"

int main() {
    int num_elements = 10;
    unsigned int index_in[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    unsigned int index_out[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    float depths[] = {170.23f, 170.1f, 45.23f, 45.1f, 88890.231f, 888802.231f, 88824.2343f, 2.2123f, 66.123f, 888255.3432f};

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


    /*int large_num_elements = 10000;
    struct Sequence sequence = generate_sequence(large_num_elements);
    radix_sort(
        large_num_elements,
        sequence.index_in,
        sequence.index_out,
        sequence.depths
    );
    verify_order(
        large_num_elements,
        sequence.index_in,
        sequence.depths
    );*/

    return 0;
}