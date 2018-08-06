#include <aes-armcortexm.h>

//
// This example will likely only work on STM32F407 chips.
// The assembly code requires the hardware random source.
// 

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
    unsigned long start = micros();

    // Page 55 from https://nvlpubs.nist.gov/nistpubs/Legacy/SP/nistspecialpublication800-38a.pdf

    const uint32_t ctr = 0XF3F2F1F0;
    const uint8_t nonce[12] = {0xF4, 0xF5, 0xF6, 0xF7, 0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF};
    const uint8_t key[16] = {0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6, 0xAB, 0xF7, 0x15, 0x88, 0x09, 0xCF, 0x4F, 0x3C
};
    const uint32_t LEN = 1*16;
    const uint32_t LEN_ROUNDED = ((LEN+31)/32)*32;
    uint8_t in[LEN_ROUNDED] = {0};
    uint8_t out[LEN_ROUNDED];

    AES_128_ctr_bs_masked_param p;
    p.ctr = ctr;
    memcpy(p.nonce, nonce, 12);
    // memcpy(p.rk, key, 16);

    start = micros();
    AES_128_keyschedule_bs_masked(key, p.rk);
    Serial.print("AES_128_keyschedule_bs_masked: ");
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
    AES_128_encrypt_ctr_bs_masked(&p, in, out, LEN);
    Serial.print("AES_128_encrypt_ctr_bs_masked: ");
    Serial.print(micros() - start);
    Serial.println(" us.");
  
    // Print ciphertext
    Serial.print("out:    ");
    AES_HexPrint(out, LEN);
    
    // Verify
    const uint8_t expected[] = {0xEC, 0x8C, 0xDF, 0x73, 0x98, 0x60, 0x7C, 0xB0, 0xF2, 0xD2, 0x16, 0x75, 0xEA, 0x9E, 0xA1, 0xE4};
    if (memcmp(out, expected, sizeof(expected)) != 0)
    {
      Serial.println("Test vector failed.");
      // while(1){};
    }

    /**/
    // Perform decryption
    p.ctr = ctr;

    start = micros();
    AES_128_decrypt_ctr_bs_masked(&p, out, in, LEN);
    Serial.print("AES_128_decrypt_ctr_bs_masked: ");
    Serial.print(micros() - start);
    Serial.println(" us.");

    // Print plaintext
    Serial.print("in was: ");
    AES_HexPrint(in, LEN);
}
