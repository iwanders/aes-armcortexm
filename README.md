# Fast AES on ARM Cortex-M3 and M4

This collection consists of the following highly optimized FIPS 197 compliant AES software implementations. *Repackaged in an Arduino compatible library.*

* AES-128 key expansion.
* Single-block AES-128 encryption/decryption.
* AES-128 encryption/decryption in CTR mode.
* AES-192 key expansion.
* AES-192 encryption/decryption in CTR mode.
* AES-256 key expansion.
* AES-256 encryption/decryption in CTR mode.
* AES-128 key expansion to bitsliced state.
* Constant-time bitsliced AES-128 encryption/decryption in CTR mode (protected against timing attacks).
* Masked constant-time bitsliced AES-128 encryption/decryption in CTR mode (protected against timing attacks and first-order side-channel attacks).

They are part of the publication "All the AES You Need on Cortex-M3 and M4", published at [SAC 2016](https://www.engr.mun.ca/~sac2016/), by [Peter Schwabe](https://cryptojedi.org/) and [Ko Stoffelen](https://ko.stoffelen.nl/). The paper is available [here](https://ko.stoffelen.nl/papers/sac2016-aesarm.pdf).

# Arduino Library
All credit for this library goes to the [original authors](https://github.com/Ko-/aes-armcortexm), the following changes were made to allow it to be used as an Arduino library.

* Shuffle folder structure around, remove all bringup code.
* Changed the `.c` files into `.h` files, removed the main function at the end.
* Changed function names to avoid name collisions.
* Changed struct names to avoid name collisions.
* Changed assembly file names to end in `.S`, changed function names to match headers.
* Adapted the original `.c` files to examples that produce the NIST test vectors.

It should work just like any other Arduino library you are used to, examples are provided and should work out of the box.

## Hardware test
Tested this library on Teensy 3.2, using the `benchmark_teensy3` example (in cycles):
```
    Averaged over: 100000
    aes128_keyschedule_benchmark: 465
    aes128_encrypt_benchmark: 1360
```

Technically this library should also work on the popular STM32F103 'Bluepill' device since it is a Cortex M3. I tried this using [Arduino_STM32](https://github.com/rogerclarkmelbourne/Arduino_STM32/wiki/Installation) but found the compiler to complain on some assembly instructions:

```
.../aes-armcortexm/src/aes128/aes.S: Assembler messages:
.../aes-armcortexm/src/aes128/aes.S:948: Error: cannot honor width suffix -- `pop.w {r4}'
.../aes-armcortexm/src/aes128/aes.S:1099: Error: cannot honor width suffix -- `push.w {r4-r7}'
.../aes-armcortexm/src/aes128/aes.S:1122: Error: cannot honor width suffix -- `push.w {r4-r7}'
...
.../aes-armcortexm/src/aes128/aes.S:1914: Error: cannot honor width suffix -- `pop.w {r4-r7}'
.../aes-armcortexm/src/aes128/aes.S:2577: Error: cannot honor width suffix -- `pop.w {r2}'

```
I'm not sure why this is, could have to do with the fact that the version of `arm-none-eabi-gcc` is at `4.8.3-2014q1`. If anyone knows how to resolve this, feel free to put up a PR.

# Original Performance
All numbers mentioned below were obtained on the STM32L100C (Cortex-M3) and STM32F407 (Cortex-M4) development boards. Encryption/decryption speeds are cycle/block averages for processing 4096 bytes, averaging over 10000 runs with random inputs, nonces, and keys. Note that all implementations are optimized for speed and therefore fully unrolled. ROM sizes can be reduced drastically with only a small penalty in cycle counts.

| Algorithm | Speed M3 (cycles) | Speed M4 (cycles) | ROM (bytes) | RAM (bytes) |
|-----------|------------------:|------------------:|------------:|------------:|
| AES-128 key expansion | 243.9 | 254.9 | 742 (code) + 1024 (data) | 176 (in/out) + 32 (stack) |
| AES-128 single block encryption | 639.5 | 644.7 | 1970 (code) + 1024 (data) | 176+2m (in/out) + 44 (stack) |
| AES-128 encryption/decryption in CTR mode | 531.8 | 537.9 | 2128 (code) + 1024 (data) | 192+2m (in/out) + 72 (stack) |  
| AES-192 key expansion | 232.9 | 240.2 | 682 (code) + 1024 (data) | 208 (in/out) + 32 (stack) |
| AES-192 encryption/decryption in CTR mode | 651.0 | 656.0 | 2512 (code) + 1024 (data) | 224+2m (in/out) + 72 (stack) |
| AES-256 key expansion | 315.8 | 319.9 | 958 (code) + 1024 (data) | 240 (in/out) + 32 (stack) |
| AES-256 encryption/decryption in CTR mode | 767.9 | 774.6 | 2896 (code) + 1024 (data) | 256+2m (in/out) + 72 (stack) | 
| AES-128 key expansion to bitsliced state | 1027.8 | 1033.8 | 3434 (code) + 1036 (data) | 368 (in/out) + 188 (stack) |
| Constant-time bitsliced AES-128 encryption/decryption in CTR mode | 1618.6 | 1619.6 | 11806 (code) + 12 (data) | 368+2m (in/out) + 108 (stack) |
| Masked constant-time bitsliced AES-128 encryption/decryption in CTR mode | N/A | 2132.51 (generating randomness) + 5291.6 (rest) | 39224 (code) + 12 (data) | 368+2m (in/out) + 1312 (storing randomness) + 276 (stack rest) |  
