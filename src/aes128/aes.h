#include <stdint.h>

extern "C"
{
    void AES_128_keyschedule(const uint8_t *, uint8_t *);
    void AES_128_keyschedule_dec(const uint8_t *, uint8_t *);
    void AES_128_encrypt(const uint8_t *, const uint8_t *, uint8_t *);
    void AES_128_decrypt(const uint8_t *, const uint8_t *, uint8_t *);
}
