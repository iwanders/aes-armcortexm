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

    // Page 56 from https://nvlpubs.nist.gov/nistpubs/Legacy/SP/nistspecialpublication800-38a.pdf

    const uint32_t ctr = 0XF3F2F1F0;
    const uint8_t nonce[12] = {0xF4, 0xF5, 0xF6, 0xF7, 0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF};
    const uint8_t key[24] = {0x8E, 0x73, 0xB0, 0xF7, 0xDA, 0x0E, 0x64, 0x52, 0xC8, 0x10, 0xF3, 0x2B, 0x80, 0x90, 0x79, 0xE5, 0x62, 0xF8, 0xEA, 0xD2, 0x52, 0x2C, 0x6B, 0x7B};
    const uint32_t LEN = 1*16;
    const uint32_t LEN_ROUNDED = ((LEN+31)/32)*32;
    uint8_t in[LEN_ROUNDED] = {0};
    uint8_t out[LEN_ROUNDED];

    AES_192_ctr_param p;
    p.ctr = ctr;
    memcpy(p.nonce, nonce, 12);
    memcpy(p.rk, key, 24);

    start = micros();
    AES_192_keyschedule(key, p.rk+24);
    Serial.print("AES_192_keyschedule: ");
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
    AES_192_encrypt_ctr(&p, in, out, LEN);
    Serial.print("AES_192_encrypt_ctr: ");
    Serial.print(micros() - start);
    Serial.println(" us.");
  
    // Print ciphertext
    Serial.print("out:    ");
    AES_HexPrint(out, LEN);
    
    // Verify
    const uint8_t expected[] = {0x71, 0x7D, 0x2D, 0xC6, 0x39, 0x12, 0x83, 0x34, 0xA6, 0x16, 0x7A, 0x48, 0x8D, 0xED, 0x79, 0x21};
    if (memcmp(out, expected, sizeof(expected)) != 0)
    {
      Serial.println("Test vector failed.");
      while(1){};
    }

    /**/
    // Perform decryption
    p.ctr = ctr;

    start = micros();
    AES_192_decrypt_ctr(&p, out, in, LEN);
    Serial.print("AES_192_decrypt_ctr: ");
    Serial.print(micros() - start);
    Serial.println(" us.");

    // Print plaintext
    Serial.print("in was: ");
    AES_HexPrint(in, LEN);
}
