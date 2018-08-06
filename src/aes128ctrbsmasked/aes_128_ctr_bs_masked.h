#include <stdint.h>

// The masked implementation depends on the hardware random number generator
// in the STM32F407 chip.

typedef struct AES_128_ctr_bs_masked_param {
    uint32_t ctr;
    uint8_t nonce[12];
    uint8_t rk[2*11*16];
} AES_128_ctr_bs_masked_param;

extern "C"
{
void AES_128_keyschedule_bs_masked(const uint8_t *, uint8_t *);
void AES_128_encrypt_ctr_bs_masked(AES_128_ctr_bs_masked_param const *, const uint8_t *, uint8_t *, uint32_t);
#define AES_128_decrypt_ctr_bs_masked AES_128_encrypt_ctr_bs_masked
}
