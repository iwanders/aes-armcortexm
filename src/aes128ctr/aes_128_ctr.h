#include <stdint.h>

extern "C"
{
typedef struct AES_128_ctr_param {
    uint32_t ctr;
    uint8_t nonce[12];
    uint8_t rk[11*16];
} AES_128_ctr_param;

void AES_128_keyschedule(const uint8_t *, uint8_t *);
void AES_128_encrypt_ctr(AES_128_ctr_param const *, const uint8_t *, uint8_t *, uint32_t);
#define AES_128_decrypt_ctr AES_128_encrypt_ctr
}
