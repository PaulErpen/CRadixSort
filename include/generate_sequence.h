struct Sequence {
    float *depths;
    unsigned int *index_in;
    unsigned int *index_out;
};

struct Sequence generate_sequence(unsigned int num_elements);