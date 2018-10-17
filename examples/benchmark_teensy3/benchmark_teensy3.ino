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

void makeRandomBlock(uint8_t* data)
{
    static bool have_setup = false;
    // Just use the cipher to create 'random' bytes.
    static uint8_t rk[11*16];
    static uint8_t prev[16] = {0};
    if (!have_setup)
    {
        uint8_t key[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
        memcpy(rk, key, 16);
        AES_128_keyschedule(key, rk+16);
        have_setup = true;
    }
    // perform the step.
    AES_128_encrypt(rk, prev, data);

    // copy the new data into the previous result.
    memcpy(prev, data, 16); 
}

void setup()
{
    Serial.begin(9600);
    // https://forum.pjrc.com/threads/28407-Teensyduino-access-to-counting-cpu-cycles
    ARM_DEMCR |= ARM_DEMCR_TRCENA;
    ARM_DWT_CTRL |= ARM_DWT_CTRL_CYCCNTENA;
}

uint32_t aes128_keyschedule_benchmark(uint32_t count)
{
    uint64_t cumulative = 0;
    uint8_t rk[11*16];
    uint8_t key[16];
    uint64_t start, finish;
    for (uint32_t i = 0; i < count ; i++)
    {
        // make a random key.
        makeRandomBlock(key);
        memcpy(rk, key, 16);

        // benchmark the schedule.
        start = ARM_DWT_CYCCNT;
        AES_128_keyschedule(key, rk+16);
        finish = ARM_DWT_CYCCNT;
        cumulative += finish - start;
    }
    return cumulative /  uint64_t{count};
}

uint32_t aes128_encrypt_benchmark(uint32_t count)
{
    uint64_t cumulative = 0;
    uint8_t rk[11*16];
    uint64_t start, finish;
    uint8_t key[16];
    uint8_t in[16];
    uint8_t out[16];
    for (uint32_t i = 0; i < count ; i++)
    {
        // make random input.
        makeRandomBlock(in);
        // make a random key.
        makeRandomBlock(key);

        // Prepare the round keys.
        memcpy(rk, key, 16);
        AES_128_keyschedule(key, rk+16);

        // benchmark the encrypt step.
        start = ARM_DWT_CYCCNT;
        AES_128_encrypt(rk, in, out);
        finish = ARM_DWT_CYCCNT;
        cumulative += finish - start;
    }
    return cumulative /  uint64_t{count};
}

void loop()
{
    const uint32_t count = 100000;
    Serial.print("Averaged over: "); Serial.println(count);
    Serial.print("aes128_keyschedule_benchmark: "); Serial.println(aes128_keyschedule_benchmark(count));
    Serial.print("aes128_encrypt_benchmark: "); Serial.println(aes128_encrypt_benchmark(count));
}
