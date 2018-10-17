#include <stdint.h>

extern "C"
{
/**
 * @brief Sets up the round keys for encryption.
 * @param key The 16 byte key to use for encryption.
 * @param round_keys Pointer to the round keys to be used.
 * @code
 * 
 *   uint8_t round_keys_encrypt[11*16];  // Create the memory for the round keys.
 *   memcpy(rk, key, 16);  // Copy encryption key for the first round.
 *   AES_128_keyschedule(key, round_keys_encrypt + 16);  // setup the remaining rounds.
 *
 */
void AES_128_keyschedule(const uint8_t* key, uint8_t* round_keys);

/**
 * @brief Sets up the round keys for decryption.
 * @param key The 16 byte key to use for decryption.
 * @param round_keys Pointer to the round keys to be used.
 * @code
 * 
 *   uint8_t round_keys_decrypt[11*16];  // Create the memory for the round keys
 *   memcpy(rk + 160, key, 16);  // Copy encryption key for the first round.
 *   AES_128_keyschedule_dec(key, round_keys_decrypt);  // setup the remaining rounds.
 *
 */
void AES_128_keyschedule_dec(const uint8_t* key, uint8_t* round_keys);

/**
 * @brief Perform encryption of one block (16 bytes) using the round keys.
 * @param round_keys The round keys to use for encryption.
 * @param plaintext The plaintext to encrypt.
 * @param ciphertext The ciphertext from the encryption.
 * @code
 *     AES_128_encrypt(round_keys_encrypt, plaintext, ciphertext);
 */
void AES_128_encrypt(const uint8_t* round_keys, const uint8_t* plaintext, uint8_t* ciphertext);

/**
 * @brief Perform decryption of one block (16 bytes) using the round keys.
 * @param round_keys The round keys to use for decryption.
 * @param ciphertext The ciphertext to decrypt.
 * @param plaintext The plaintext from the decryption.
 * @code
 *     AES_128_decrypt(round_keys_decrypt, ciphertext, plaintext);
 */
void AES_128_decrypt(const uint8_t* round_keys, const uint8_t* ciphertext, uint8_t* plaintext);
}
