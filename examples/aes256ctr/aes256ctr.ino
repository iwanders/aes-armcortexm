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
    unsigned long start = micros();

    // Page 57 from https://nvlpubs.nist.gov/nistpubs/Legacy/SP/nistspecialpublication800-38a.pdf

    const uint32_t ctr = 0XF3F2F1F0;
    const uint8_t nonce[12] = {0xF4, 0xF5, 0xF6, 0xF7, 0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF};
    const uint8_t key[32] = {0x60, 0x3D, 0xEB, 0x10, 0x15, 0xCA, 0x71, 0xBE, 0x2B, 0x73, 0xAE, 0xF0, 0x85, 0x7D, 0x77, 0x81, 0x1F, 0x35, 0x2C, 0x07, 0x3B, 0x61, 0x08, 0xD7, 0x2D, 0x98, 0x10, 0xA3, 0x09, 0x14, 0xDF, 0xF4};
    const uint32_t LEN = 1*16;
    const uint32_t LEN_ROUNDED = ((LEN+31)/32)*32;
    uint8_t in[LEN_ROUNDED] = {0};
    uint8_t out[LEN_ROUNDED];

    AES_256_ctr_param p;
    p.ctr = ctr;
    memcpy(p.nonce, nonce, 12);
    memcpy(p.rk, key, 32);

    start = micros();
    AES_256_keyschedule(key, p.rk+32);
    Serial.print("AES_256_keyschedule: ");
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
    AES_256_encrypt_ctr(&p, in, out, LEN);
    Serial.print("AES_256_encrypt_ctr: ");
    Serial.print(micros() - start);
    Serial.println(" us.");
  
    // Print ciphertext
    Serial.print("out:    ");
    AES_HexPrint(out, LEN);
    
    // Verify
    const uint8_t expected[] = {0x0B, 0xDF, 0x7D, 0xF1, 0x59, 0x17, 0x16, 0x33, 0x5E, 0x9A, 0x8B, 0x15, 0xC8, 0x60, 0xC5, 0x02};
    if (memcmp(out, expected, sizeof(expected)) != 0)
    {
      Serial.println("Test vector failed.");
      while(1){};
    }

    /**/
    // Perform decryption
    p.ctr = ctr;

    start = micros();
    AES_256_decrypt_ctr(&p, out, in, LEN);
    Serial.print("AES_256_decrypt_ctr: ");
    Serial.print(micros() - start);
    Serial.println(" us.");

    // Print plaintext
    Serial.print("in was: ");
    AES_HexPrint(in, LEN);
}
