#include <stdint.h>

extern "C" 
{
typedef struct param {
    uint32_t ctr;
    uint8_t nonce[12];
    uint8_t rk[13*16];
} AES_192_ctr_param;

void AES_192_keyschedule(const uint8_t *, uint8_t *);
void AES_192_encrypt_ctr(AES_192_ctr_param const *, const uint8_t *, uint8_t *, uint32_t);
#define AES_192_decrypt_ctr AES_192_encrypt_ctr
}
