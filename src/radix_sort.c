#include <assert.h>
#include <stdlib.h>
#include <string.h>

#define WORKGROUP_SIZE 256
#define RADIX_SORT_BINS 256

int get_index_in(unsigned int index, unsigned int iteration, unsigned int *index_in, unsigned int *index_out) {
    if (iteration % 2 == 0) {
        return index_in[index];
    } else {
        return index_out[index];
    }
}

int sum_array(unsigned int *array, unsigned int size) {
    unsigned int sum = 0;
    for (unsigned int i = 0; i < size; i++) {
        sum += array[i];
    }
    return sum;
}

struct BinFlags {
    unsigned int flags[8];
};

int bitCount(unsigned int n) {
    int count = 0;
    while (n) {
        count += n & 1;
        n >>= 1;
    }
    return count;
}

int min(int a, int b) {
    return a < b ? a : b;
}

unsigned int float_to_uint(float f) {
    unsigned int result;
    memcpy(&result, &f, sizeof(result));
    return result;
}

void radix_sort(int num_elements, unsigned int *index_in, unsigned int *index_out, float *depths) {
    assert(sizeof(index_in) == sizeof(index_out));
    assert(sizeof(index_in) == sizeof(depths));

    for (int iteration = 0; iteration < 4; iteration++) {
        int shift = 8 * iteration;
        
        // compute the histogram
        unsigned int histogram[RADIX_SORT_BINS] = {0};
        for(int lID = 0; lID < WORKGROUP_SIZE; lID++) {
            for (int ID = lID; ID < num_elements; ID += WORKGROUP_SIZE) {
                unsigned int index = get_index_in(ID, iteration, index_in, index_out);
                unsigned int depth = float_to_uint(depths[index]);
                // determine the bin
                unsigned int bin = (depth >> shift) & (RADIX_SORT_BINS - 1);
                // increment the histogram
                histogram[bin] += 1;
                assert(bin < RADIX_SORT_BINS);
            }
        }
        assert(sum_array(histogram, RADIX_SORT_BINS) == num_elements);


        // compute the prefix sum
        int prefix_sums[WORKGROUP_SIZE] = {0};
        unsigned int value_holder[WORKGROUP_SIZE] = {0};
        unsigned int shared_data[WORKGROUP_SIZE] = {0};

        for(int lID = 0; lID < WORKGROUP_SIZE; lID++) {
            shared_data[lID] = histogram[lID];
        }

        for (int stride = 1; stride < WORKGROUP_SIZE; stride *= 2) {
            for(int lID = 0; lID < WORKGROUP_SIZE; lID++) {
                value_holder[lID] = 0;
                if (lID >= stride) {
                    value_holder[lID] = shared_data[lID - stride];
                }
            }

            for(int lID = 0; lID < WORKGROUP_SIZE; lID++) {
                prefix_sums[lID] += value_holder[lID];
                shared_data[lID] += value_holder[lID];
            }
        }

        assert(prefix_sums[0] == 0);

        // Scatter phase
        unsigned int *output = (iteration % 2 == 0) ? index_out : index_in;
        struct BinFlags bin_flags[WORKGROUP_SIZE];
        unsigned int global_offsets[WORKGROUP_SIZE] = {0};

        for (int blockID = 0; blockID < num_elements; blockID += WORKGROUP_SIZE) {
            for(int lID = 0; lID < WORKGROUP_SIZE; lID++) {
                for (int i = 0; i < 8; i++) {
                    bin_flags[lID].flags[i] = 0;
                }
            }

            for (int lID = 0; lID < WORKGROUP_SIZE; lID++) {
                int flags_bin = lID / 32;
                int flags_bit = 1 << (lID % 32);
                int ID = blockID + lID;

                if (ID < num_elements) {
                    unsigned int index = get_index_in(ID, iteration, index_in, index_out);
                    unsigned int depth = float_to_uint(depths[index]);
                    unsigned int bin = (depth >> shift) & (RADIX_SORT_BINS - 1);
                    global_offsets[lID] = prefix_sums[bin];
                    bin_flags[bin].flags[flags_bin] += flags_bit;
                }
            }

            int flag_count = 0;
            for (int lID = 0; lID < WORKGROUP_SIZE; lID++) {
                for (int i = 0; i < 8; i++) {
                    flag_count += bitCount(bin_flags[lID].flags[i]);
                }
            }
            assert((flag_count == min(WORKGROUP_SIZE, num_elements)) | (flag_count == num_elements % WORKGROUP_SIZE));

            for (int lID = 0; lID < WORKGROUP_SIZE; lID++) {
                int flags_bin = lID / 32;
                int flags_bit = 1 << (lID % 32);
                int ID = blockID + lID;

                if (ID < num_elements) {
                    unsigned int index = get_index_in(ID, iteration, index_in, index_out);
                    unsigned int depth = float_to_uint(depths[index]);
                    unsigned int bin = (depth >> shift) & (RADIX_SORT_BINS - 1);
                    
                    int prefix = 0;
                    int count = 0;
                    for (int i = 0; i < WORKGROUP_SIZE / 32; i++) {
                        const int bits = bin_flags[bin].flags[i];
                        const int full_count = bitCount(bits);
                        const int partial_count = bitCount(bits & (flags_bit - 1));
                        prefix += (i < flags_bin) ? full_count : 0;
                        prefix += (i == flags_bin) ? partial_count : 0;
                        count += full_count;
                    }

                    output[global_offsets[lID] + prefix] = index;
                    if (prefix == count - 1) {
                        prefix_sums[bin] += count;
                    }
                    if (bin < RADIX_SORT_BINS - 1) {
                        assert(prefix_sums[bin] <= prefix_sums[bin + 1]);
                    }
                }
            }
        }
        assert(prefix_sums[RADIX_SORT_BINS - 1] == num_elements);
    }
}