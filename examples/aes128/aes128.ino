#include <aes-armcortexm.h>

void AES_HexPrint(const uint8_t* data, uint8_t length)
{
    char buffer[2 * length + 1];
    memset(buffer, 0, 2 * length + 1);
    for (uint16_t i = 0; i < length; ++i)
    {
        sprintf(buffer + 2 * i, "%02x", data[i]);
    }
    Serial.println(buffer);
}

void setup()
{
    Serial.begin(9600);
    while (!Serial){};
}

void loop()
{
    // Test vectors from:
    // https://csrc.nist.gov/Projects/Cryptographic-Algorithm-Validation-Program/Block-Ciphers
    unsigned long start = micros();

    // Page 20 from https://csrc.nist.gov/CSRC/media/Projects/Cryptographic-Algorithm-Validation-Program/documents/aes/AESAVS.pdf
    const uint8_t key[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

    uint8_t plaintext[16] = {0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

    uint8_t ciphertext[16];

    Serial.print("key:    ");
    AES_HexPrint(key, 16);

    Serial.print("in:     ");
    AES_HexPrint(plaintext, 16);

    uint8_t round_keys_encrypt[11 * 16];  // Create the memory for the round keys.

    memcpy(round_keys_encrypt, key, 16);  // Copy encryption key for the first round.

    start = micros();
    AES_128_keyschedule(key, round_keys_encrypt + 16);  // setup the remaining rounds.
    Serial.print("AES_128_keyschedule: ");
    Serial.print(micros() - start);
    Serial.println(" us.");
    
    /** /
    // Print all round keys
    for(uint16_t i = 0; i < 11*4; ++i)
    {
        Serial.print("rk["); Serial.print(i), Serial.print("]: ");
        AES_HexPrint(&(rk[i * 4]), 4);
    }
    / **/

    start = micros();
    AES_128_encrypt(round_keys_encrypt, plaintext, ciphertext);  // Encrypt one block
    Serial.print("AES_128_encrypt: ");
    Serial.print(micros() - start);
    Serial.println(" us.");
    
    /**/
    // Print ciphertext
    Serial.print("ciphertext:    ");
    AES_HexPrint(ciphertext, 16);
    /**/

    // Verify
    const uint8_t expected[] = {0x3A, 0xD7, 0x8E, 0x72, 0x6C, 0x1E, 0xC0, 0x2B, 0x7E, 0xBF, 0xE9, 0x2B, 0x23, 0xD9, 0xEC, 0x34};
    if (memcmp(ciphertext, expected, sizeof(expected)) != 0)
    {
        Serial.println("Test vector failed.");
        while(1){};
    }


    uint8_t decodedtext[16];

    uint8_t round_keys_decrypt[11 * 16];  // Create the memory for the round keys
    memcpy(round_keys_decrypt + 160, key, 16);  // Copy encryption key for the first round.

    start = micros();
    AES_128_keyschedule_dec(key, round_keys_decrypt);  // setup the remaining rounds.
    Serial.print("AES_128_keyschedule_dec: ");
    Serial.print(micros() - start);
    Serial.println(" us.");

    /** /
    // Print all decryption round keys
    for(uint16_t i = 0; i < 11*4; ++i)
    {
        Serial.print("rk["); Serial.print(i), Serial.print("]: ");
        AES_HexPrint(&(rk[i * 4]), 4);
    }
    / **/

    start = micros();
    AES_128_decrypt(round_keys_decrypt, ciphertext, decodedtext);  // decrypt one block.
    Serial.print("AES_128_decrypt: ");
    Serial.print(micros() - start);
    Serial.println(" us.");

    // Print plaintext
    Serial.print("decodedtext is: ");
    AES_HexPrint(decodedtext, 16);
}
