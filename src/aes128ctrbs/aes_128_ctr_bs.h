#include <stdint.h>

typedef struct AES_128_ctr_bs_param {
    uint32_t ctr;
    uint8_t nonce[12];
    uint8_t rk[2*11*16];
} AES_128_ctr_bs_param;

extern "C"
{
void AES_128_keyschedule_bs(const uint8_t *, uint8_t *);
void AES_128_encrypt_ctr_bs(AES_128_ctr_bs_param const *, const uint8_t *, uint8_t *, uint32_t);
#define AES_128_decrypt_ctr_bs AES_128_encrypt_ctr_bs
}
