#include <stdint.h>

extern "C"
{
typedef struct AES_256_ctr_param {
    uint32_t ctr;
    uint8_t nonce[12];
    uint8_t rk[15*16];
} AES_256_ctr_param;

void AES_256_keyschedule(const uint8_t *, uint8_t *);
void AES_256_encrypt_ctr(AES_256_ctr_param const *, const uint8_t *, uint8_t *, uint32_t);
#define AES_256_decrypt_ctr AES_256_encrypt_ctr
}