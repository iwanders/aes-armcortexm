#include <aes-armcortexm.h>

void setup()
{
    Serial.begin(9600);
}

void loop()
{
    unsigned long start = micros();
    const uint8_t key[16] = {4,5,6,7,4,5,6,8,4,5,6,9,4,5,6,10};
    uint8_t in[16] = {0,0,0,0,1,2,3,1,2,4,1,2,5,1,2,6};
    uint8_t out[16];
    
    uint8_t rk[11*16];
    char buffer[36];

    memcpy(rk, key, 16);

    start = micros();
    AES_128_keyschedule(key, rk+16);
    Serial.print("AES_128_keyschedule: ");
    Serial.print(micros() - start);
    Serial.println(" us.");
    
  /** /
    // Print all round keys
    unsigned int i,j;
    for(i=0;i<11*4;++i) {
        sprintf(buffer, "rk[%2d]: ", i);
        for(j=0;j<4;++j)
            sprintf(buffer+2*j+8, "%02x", rk[i*4+j]);
        Serial.print(buffer); Serial.println();
    }
  / **/

    start = micros();
    AES_128_encrypt(rk, in, out);
    Serial.print("AES_128_encrypt: ");
    Serial.print(micros() - start);
    Serial.println(" us.");
    
  /** /
    // Print ciphertext
    sprintf(buffer, "out: ");
    Serial.print(buffer); Serial.println();
    for(i=0;i<16;++i)
        sprintf(buffer+2*i, "%02x", out[i]);
    Serial.print(buffer); Serial.println();
  / **/

    memcpy(rk+160, key, 16);

    start = micros();
    AES_128_keyschedule_dec(key, rk);
    Serial.print("AES_128_keyschedule_dec: ");
    Serial.print(micros() - start);
    Serial.println(" us.");

  /** /
    // Print all decryption round keys
    for(i=0;i<11*4;++i) {
        sprintf(buffer, "rk[%2d]: ", i);
        for(j=0;j<4;++j)
            sprintf(buffer+2*j+8, "%02x", rk[i*4+j]);
        Serial.print(buffer); Serial.println();
    }
  / **/

    start = micros();
    AES_128_decrypt(rk, out, in);
    Serial.print("AES_128_decrypt: ");
    Serial.print(micros() - start);
    Serial.println(" us.");

  /** /
    // Print plaintext
    sprintf(buffer, "in: ");
    Serial.print(buffer); Serial.println();
    for(i=0;i<16;++i)
        sprintf(buffer+2*i, "%02x", in[i]);
    Serial.print(buffer); Serial.println();
 / **/

}
