#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>
// #include <byteswap.h>

#define WORD uint32_t

const uint32_t T[64] = {
    0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
    0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
    0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
    0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
    0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
    0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
    0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
    0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
    0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
    0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
    0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
    0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
    0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
    0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
    0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
    0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
};

enum flag {READ, PAD0, FINISH};

union block{
  uint64_t sixtyfour[8];
  uint32_t thirtytwo[16];
  uint8_t eight[64];
};

#define F(x, y, z) ((x & y) | (~x & z)) 
#define G(x, y, z) ((x & z) | (y & ~z)) 
#define H(x, y, z) (x ^ y ^ z) 
#define I(x, y, z) (y ^ (x | ~z)) 

#define ROTL(x, n) ((x << n) | (x >> (32 - n)))

#define FF(a, b, c, d, m, s, t) { a += F(b, c, d) + m + t; a = b + ROTL(a, s); }
#define GG(a, b, c, d, m, s, t) { a += G(b, c, d) + m + t; a = b + ROTL(a, s); }
#define HH(a, b, c, d, m, s, t) { a += H(b, c, d) + m + t; a = b + ROTL(a, s); }
#define II(a, b, c, d, m, s, t) { a += I(b, c, d) + m + t; a = b + ROTL(a, s); }

void nexthash(WORD *M, WORD *H) {
    int i;
    WORD W[64];
    WORD a, b, c, d;

    for (i = 0; i < 16; i++){ 
        W[i] = M[i];
    }

	a = H[0];
    b = H[1];
    c = H[2];
    d = H[3];

    // Round 1
    FF(a, b, c, d, W[0], 7, 0xd76aa478);
    FF(d, a, b, c, W[1], 12, 0xe8c7b756);
    FF(c, d, a, b, W[2], 17, 0x242070db);
    FF(b, c, d, a, W[3], 22, 0xc1bdceee);
    FF(a, b, c, d, W[4], 7, 0xf57c0faf);
    FF(d, a, b, c, W[5], 12, 0x4787c62a);
    FF(c, d, a, b, W[6], 17, 0xa8304613);
    FF(b, c, d, a, W[7], 22, 0xfd469501);
    FF(a, b, c, d, W[8], 7, 0x698098d8);
    FF(d, a, b, c, W[9], 12, 0x8b44f7af);
    FF(c, d, a, b, W[10], 17, 0xffff5bb1);
    FF(b, c, d, a, W[11], 22, 0x895cd7be);
    FF(a, b, c, d, W[12], 7, 0x6b901122);
    FF(d, a, b, c, W[13], 12, 0xfd987193);
    FF(c, d, a, b, W[14], 17, 0xa679438e);
    FF(b, c, d, a, W[15], 22, 0x49b40821);

    // Round 2
    GG(a, b, c, d, W[1], 5, 0xf61e2562);
    GG(d, a, b, c, W[6], 9, 0xc040b340);
    GG(c, d, a, b, W[11], 14, 0x265e5a51);
    GG(b, c, d, a, W[0], 20, 0xe9b6c7aa);
    GG(a, b, c, d, W[5], 5, 0xd62f105d);
    GG(d, a, b, c, W[10], 9, 0x02441453);
    GG(c, d, a, b, W[15], 14, 0xd8a1e681);
    GG(b, c, d, a, W[4], 20, 0xe7d3fbc8);
    GG(a, b, c, d, W[9], 5, 0x21e1cde6);
    GG(d, a, b, c, W[14], 9, 0xc33707d6);
    GG(c, d, a, b, W[3], 14, 0xf4d50d87);
    GG(b, c, d, a, W[8], 20, 0x455a14ed);
    GG(a, b, c, d, W[13], 5, 0xa9e3e905);
    GG(d, a, b, c, W[2], 9, 0xfcefa3f8);
    GG(c, d, a, b, W[7], 14, 0x676f02d9);
    GG(b, c, d, a, W[12], 20, 0x8d2a4c8a);

    // Round 3
    HH(a, b, c, d, W[5], 4, 0xfffa3942);
    HH(d, a, b, c, W[8], 11, 0x8771f681);
    HH(c, d, a, b, W[11], 16, 0x6d9d6122);
    HH(b, c, d, a, W[14], 23, 0xfde5380c);
    HH(a, b, c, d, W[1], 4, 0xa4beea44);
    HH(d, a, b, c, W[4], 11, 0x4bdecfa9);
    HH(c, d, a, b, W[7], 16, 0xf6bb4b60);
    HH(b, c, d, a, W[10], 23, 0xbebfbc70);
    HH(a, b, c, d, W[13], 4, 0x289b7ec6);
    HH(d, a, b, c, W[0], 11, 0xeaa127fa);
    HH(c, d, a, b, W[3], 16, 0xd4ef3085);
    HH(b, c, d, a, W[6], 23, 0x04881d05);
    HH(a, b, c, d, W[9], 4, 0xd9d4d039);
    HH(d, a, b, c, W[12], 11, 0xe6db99e5);
    HH(c, d, a, b, W[15], 16, 0x1fa27cf8);
    HH(b, c, d, a, W[2], 23, 0xc4ac5665);

    // Round 4
    II(a, b, c, d, W[0],  6, 0xf4292244);
    II(d, a, b, c, W[7], 10, 0x432aff97);
    II(c, d, a, b, W[14],15, 0xab9423a7);
    II(b, c, d, a, W[5], 21, 0xfc93a039);
    II(a, b, c, d, W[12], 6, 0x655b59c3);
    II(d, a, b, c, W[3], 10, 0x8f0ccc92);
    II(c, d, a, b, W[10],15, 0xffeff47d);
    II(b, c, d, a, W[1], 21, 0x85845dd1);
    II(a, b, c, d, W[8],  6, 0x6fa87e4f);
    II(d, a, b, c, W[15],10, 0xfe2ce6e0);
    II(c, d, a, b, W[6], 15, 0xa3014314);
    II(b, c, d, a, W[13],21, 0x4e0811a1);
    II(a, b, c, d, W[4],  6, 0xf7537e82);
    II(d, a, b, c, W[11],10, 0xbd3af235);
    II(c, d, a, b, W[2], 15, 0x2ad7d2bb);
    II(b, c, d, a, W[9], 21, 0xeb86d391);

    H[0] += a;
    H[1] += b;
    H[2] += c;
    H[3] += d;
}

int nextblock(union block *M, FILE *infile, uint64_t *nobits, enum flag *status ){
    int i;
    size_t nobytesread;
    
    switch(*status){
        case FINISH:
            return 0;
        case PAD0:
            for (int i = 0; i < 56; i++){
                M->eight[i] = 0x00;
            }
            
            // M->sixtyfour[7] = bswap_64(*nobits);
            *status = FINISH;
            break;
        default:
            nobytesread = fread(M->eight, 1, 64, infile);
            *nobits += (8ULL * ((uint64_t) nobytesread));
            
            if (nobytesread < 56) {
                M->eight[nobytesread] = 0x80;

                for (i = nobytesread + 1; i < 56; i++){
                    M->eight[i] = 0x00;
                }

                // M->sixtyfour[7] = bswap_64(*nobits);
                *status = FINISH;
            } 
            else if (nobytesread < 64) {
                M->eight[nobytesread] = 0x80;

                for (int i = nobytesread + 1; i < 64; i++){
                    M->eight[i] = 0x00;
                }

                *status = PAD0;
            }
    }

    for (i = 0, j = 0; i < 16; i++, j += 4) {
        M->thirtytwo[i] = bswap_32(M->thirtytwo[i]);
    }

    return 1;
}

int main(int argc, char *argv[]) {
    if (argc != 2){
        printf("Error: expected single filename as argument.");
        return 1;
    }
    
    FILE *infile = fopen(argv[1], "rb");

    if(!infile){
        printf("Error: couldn't open file %s. \n", argv[1]);
        return 1;
    }   

    union block M;
    uint64_t nobits = 0;
    enum flag status = READ;

    WORD H[] = { 0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476 };

    while (nextblock(&M, infile, &nobits, &status)){
      nexthash(M.thirtytwo, H);
    }

    for(int i = 0; i< 4; i++)
    {
      printf("%02x%02x%02x%02x", (H[i] >> 0 )&0x000000ff, (H[i] >> 8)&0x000000ff, 
                                 (H[i] >> 16)&0x000000ff, (H[i] >> 24)&0x000000ff);
    }
    
    printf("\n");

    for (int i = 0; i < 4; i++){
        printf("%08" PRIx32 "", H[i]);
    }
    printf("\n");

    fclose(infile);

    return 0;
}