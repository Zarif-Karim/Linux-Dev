#ifndef OSP_BITS_H
#define OSP_BITS_H
/* The above lines prevent multiple inclusion problems */

void print_bits(uint8_t value);

uint8_t reverse_bits(uint8_t value);

uint8_t check_bit(uint8_t value, uint8_t bit);

uint8_t toggle_bit(uint8_t *value, uint8_t bit);

uint8_t get_subfield(uint8_t value, uint8_t start, uint8_t stop);

#endif