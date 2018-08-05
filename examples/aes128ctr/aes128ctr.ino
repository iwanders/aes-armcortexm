#include <aes-armcortexm.h>

void setup()
{
    Serial.begin(9600);
}

void loop()
{
    unsigned long start = micros();

    const uint32_t LEN = 256*16;
    const uint32_t LEN_ROUNDED = ((LEN+15)/16)*16;

    const uint8_t nonce[12] = {1,2,3,1,2,4,1,2,5,1,2,6};
    const uint8_t key[16] = {4,5,6,7,4,5,6,8,4,5,6,9,4,5,6,10};
    uint8_t in[LEN];
    uint8_t out[LEN_ROUNDED];
    
    unsigned int i;
    for(i=0;i<LEN;++i)
        in[i] = i%256;
    
    char buffer[36];
    AES_128_param p;
    p.ctr = 0;
    memcpy(p.nonce, nonce, 12);
    memcpy(p.rk, key, 16);

    start = micros();
    AES_128_keyschedule(key, p.rk+16);
    Serial.print("AES_128_keyschedule: ");
    Serial.print(micros() - start);
    Serial.println(" us.");

    /** /
    // Print all round keys
    unsigned int j;
    for(i=0;i<11*4;++i) {
        sprintf(buffer, "rk[%2d]: ", i);
        for(j=0;j<4;++j)
            sprintf(buffer+2*j+8, "%02x", p.rk[i*4+j]);
        Serial.print(buffer); Serial.println();
    }
    / **/

    start = micros();
    AES_128_encrypt_ctr(&p, in, out, LEN);
    Serial.print("AES_128_encrypt_ctr: ");
    Serial.print(micros() - start);
    Serial.println(" us.");
  
    // Print ciphertext
    sprintf(buffer, "out: ");
    Serial.print(buffer); Serial.println();
    for(i=0;i<LEN;++i) {
        sprintf(buffer+((2*i)%32), "%02x", out[i]);
        if(i%16 == 15){
            Serial.print(buffer); Serial.println();
        }
    }
    if(LEN%16 > 0) {
        Serial.print(buffer); Serial.println();
    }
    
    /**/
    // Perform decryption
    p.ctr = 0;

    start = micros();
    AES_128_decrypt_ctr(&p, out, in, LEN);
    Serial.print("AES_128_decrypt_ctr: ");
    Serial.print(micros() - start);
    Serial.println(" us.");

    // Print plaintext
    sprintf(buffer, "in: ");
    Serial.print(buffer); Serial.println();
    for(i=0;i<LEN;++i) {
        sprintf(buffer+((2*i)%32), "%02x", in[i]);
        if(i%16 == 15){
            Serial.print(buffer); Serial.println();
        }
    }
    if(LEN%16 > 0){
        Serial.print(buffer); Serial.println();
    }

    delay(1000);
}
