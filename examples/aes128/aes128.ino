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
}

void loop()
{
    // Test vectors from:
    // https://csrc.nist.gov/Projects/Cryptographic-Algorithm-Validation-Program/Block-Ciphers
    unsigned long start = micros();

    // Page 20 from https://csrc.nist.gov/CSRC/media/Projects/Cryptographic-Algorithm-Validation-Program/documents/aes/AESAVS.pdf
    const uint8_t key[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    uint8_t in[16] = {0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    uint8_t out[16];

    Serial.print("key:    ");
    AES_HexPrint(key, 16);

    Serial.print("in:     ");
    AES_HexPrint(in, 16);

    
    uint8_t rk[11*16];

    memcpy(rk, key, 16);

    start = micros();
    AES_128_keyschedule(key, rk+16);
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
    AES_128_encrypt(rk, in, out);
    Serial.print("AES_128_encrypt: ");
    Serial.print(micros() - start);
    Serial.println(" us.");
    
    /**/
    // Print ciphertext
    Serial.print("out:    ");
    AES_HexPrint(out, 16);
    /**/

    // Verify
    const uint8_t expected[] = {0x3A, 0xD7, 0x8E, 0x72, 0x6C, 0x1E, 0xC0, 0x2B, 0x7E, 0xBF, 0xE9, 0x2B, 0x23, 0xD9, 0xEC, 0x34};
    if (memcmp(out, expected, sizeof(expected)) != 0)
    {
      Serial.println("Test vector failed.");
      while(1){};
    }

    memcpy(rk+160, key, 16);

    start = micros();
    AES_128_keyschedule_dec(key, rk);
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
    AES_128_decrypt(rk, out, in);
    Serial.print("AES_128_decrypt: ");
    Serial.print(micros() - start);
    Serial.println(" us.");

    // Print plaintext
    Serial.print("in was: ");
    AES_HexPrint(in, 16);
}
