#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

const uint32_t K[64] = {
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

#define ROTLEFT(x, c) ((x << c) | (x >> (32 - c)))
#define F(x, y, z) ((x & y) | (~x & z))
#define G(x, y, z) ((x & z) | (y & ~z))
#define H(x, y, z) (x ^ y ^ z)
#define I(x, y, z) (y ^ (x | ~z))

#define FF(a, b, c, d, m, s, t) { a += F(b, c, d) + m + t;  a = b + ROTLEFT(a, s); }
#define GG(a, b, c, d, m, s, t) { a += G(b, c, d) + m + t;  a = b + ROTLEFT(a, s); }
#define HH(a, b, c, d, m, s, t) { a += H(b, c, d) + m + t;  a = b + ROTLEFT(a, s); }
#define II(a, b, c, d, m, s, t) { a += I(b, c, d) + m + t;  a = b + ROTLEFT(a, s); }

const uint8_t ZEROBIT = 0x00;
const uint8_t BIT = 0x80;

#define VAR uint32_t

enum status {
  NOSPACE,
  READ,
  ISFINISHED
};

union union_padding{
  uint64_t sixfour[8];
  uint32_t threetwo[16];
  uint8_t eight[64];
  uint8_t padding[64];
};

struct msg_block{
  union union_padding curr_block;
  uint32_t init_hash[4];
};

void md5_hash(FILE *file, struct msg_block *curr_msg) {
  uint32_t a, b, c, d, temp;
  uint64_t bitsInFile = 0;
  bool keepAlive = true;
  int specialPadding = 0;
  size_t check;

  while(keepAlive) {
    check = fread(&curr_msg -> curr_block.eight, 1, 64, file);

    if (check == 64) {
      bitsInFile += 512;
    } else if (check < 64 && check > 56) {
      bitsInFile += (check * 8);
      specialPadding = 1;

      curr_msg -> curr_block.padding[check] = BIT;

      for (int i = check + 1; i < 64; i++) {
        curr_msg -> curr_block.padding[i] = ZEROBIT;
      }
    } else if( check < 56 && check > 0) {
      bitsInFile += (check * 8);

      curr_msg -> curr_block.padding[check] = BIT;

      for (int i = check + 1; i < 56; i++) {
        curr_msg -> curr_block.padding[i] = ZEROBIT;
      }

      curr_msg -> curr_block.sixfour[7] = bitsInFile;

      keepAlive = false;
    } else if (check == 0 && specialPadding == 0) {
      curr_msg -> curr_block.padding[0] = BIT;

      for(int i = 1; i < 56; i++) {
        curr_msg -> curr_block.padding[i] = ZEROBIT;
      }

      curr_msg -> curr_block.sixfour[7] = bitsInFile;

      keepAlive = false;
    } else if (check == 0 && specialPadding == 1) {
      curr_msg -> curr_block.padding[0]=ZEROBIT;

      for(int i = 1; i < 56; i++) {
        curr_msg -> curr_block.padding[i] = ZEROBIT;
      }

      curr_msg -> curr_block.sixfour[7] = bitsInFile;

      keepAlive = false;
    }

    for(int i = 0, j = 0; i < 16; ++i, j += 4) {
        curr_msg -> curr_block.threetwo[i] = 
                               (curr_msg -> curr_block.eight[j]) + 
                               (curr_msg -> curr_block.eight[j + 1] << 8) + 
                               (curr_msg -> curr_block.eight[j + 2] << 16) + 
                               (curr_msg -> curr_block.eight[j + 3] << 24);
    }

    a = curr_msg -> init_hash[0];
    b = curr_msg -> init_hash[1];
    c = curr_msg -> init_hash[2];
    d = curr_msg -> init_hash[3];
    
    // Round 1
    FF(a, b, c, d, curr_msg -> curr_block.threetwo[0], 7, 0xd76aa478);
    FF(d, a, b, c, curr_msg -> curr_block.threetwo[1], 12, 0xe8c7b756);
    FF(c, d, a, b, curr_msg -> curr_block.threetwo[2], 17, 0x242070db);
    FF(b, c, d, a, curr_msg -> curr_block.threetwo[3], 22, 0xc1bdceee);
    FF(a, b, c, d, curr_msg -> curr_block.threetwo[4], 7, 0xf57c0faf);
    FF(d, a, b, c, curr_msg -> curr_block.threetwo[5], 12, 0x4787c62a);
    FF(c, d, a, b, curr_msg -> curr_block.threetwo[6], 17, 0xa8304613);
    FF(b, c, d, a, curr_msg -> curr_block.threetwo[7], 22, 0xfd469501);
    FF(a, b, c, d, curr_msg -> curr_block.threetwo[8], 7, 0x698098d8);
    FF(d, a, b, c, curr_msg -> curr_block.threetwo[9], 12, 0x8b44f7af);
    FF(c, d, a, b, curr_msg -> curr_block.threetwo[10], 17, 0xffff5bb1);
    FF(b, c, d, a, curr_msg -> curr_block.threetwo[11], 22, 0x895cd7be);
    FF(a, b, c, d, curr_msg -> curr_block.threetwo[12], 7, 0x6b901122);
    FF(d, a, b, c, curr_msg -> curr_block.threetwo[13], 12, 0xfd987193);
    FF(c, d, a, b, curr_msg -> curr_block.threetwo[14], 17, 0xa679438e);
    FF(b, c, d, a, curr_msg -> curr_block.threetwo[15], 22, 0x49b40821);

    // Round 2
    GG(a, b, c, d, curr_msg -> curr_block.threetwo[1], 5, 0xf61e2562);
    GG(d, a, b, c, curr_msg -> curr_block.threetwo[6], 9, 0xc040b340);
    GG(c, d, a, b, curr_msg -> curr_block.threetwo[11], 14, 0x265e5a51);
    GG(b, c, d, a, curr_msg -> curr_block.threetwo[0], 20, 0xe9b6c7aa);
    GG(a, b, c, d, curr_msg -> curr_block.threetwo[5], 5, 0xd62f105d);
    GG(d, a, b, c, curr_msg -> curr_block.threetwo[10], 9, 0x02441453);
    GG(c, d, a, b, curr_msg -> curr_block.threetwo[15], 14, 0xd8a1e681);
    GG(b, c, d, a, curr_msg -> curr_block.threetwo[4], 20, 0xe7d3fbc8);
    GG(a, b, c, d, curr_msg -> curr_block.threetwo[9], 5, 0x21e1cde6);
    GG(d, a, b, c, curr_msg -> curr_block.threetwo[14], 9, 0xc33707d6);
    GG(c, d, a, b, curr_msg -> curr_block.threetwo[3], 14, 0xf4d50d87);
    GG(b, c, d, a, curr_msg -> curr_block.threetwo[8], 20, 0x455a14ed);
    GG(a, b, c, d, curr_msg -> curr_block.threetwo[13], 5, 0xa9e3e905);
    GG(d, a, b, c, curr_msg -> curr_block.threetwo[2], 9, 0xfcefa3f8);
    GG(c, d, a, b, curr_msg -> curr_block.threetwo[7], 14, 0x676f02d9);
    GG(b, c, d, a, curr_msg -> curr_block.threetwo[12], 20, 0x8d2a4c8a);

    // Round 3
    HH(a, b, c, d, curr_msg -> curr_block.threetwo[5], 4, 0xfffa3942);
    HH(d, a, b, c, curr_msg -> curr_block.threetwo[8], 11, 0x8771f681);
    HH(c, d, a, b, curr_msg -> curr_block.threetwo[11], 16, 0x6d9d6122);
    HH(b, c, d, a, curr_msg -> curr_block.threetwo[14], 23, 0xfde5380c);
    HH(a, b, c, d, curr_msg -> curr_block.threetwo[1], 4, 0xa4beea44);
    HH(d, a, b, c, curr_msg -> curr_block.threetwo[4], 11, 0x4bdecfa9);
    HH(c, d, a, b, curr_msg -> curr_block.threetwo[7], 16, 0xf6bb4b60);
    HH(b, c, d, a, curr_msg -> curr_block.threetwo[10], 23, 0xbebfbc70);
    HH(a, b, c, d, curr_msg -> curr_block.threetwo[13], 4, 0x289b7ec6);
    HH(d, a, b, c, curr_msg -> curr_block.threetwo[0], 11, 0xeaa127fa);
    HH(c, d, a, b, curr_msg -> curr_block.threetwo[3], 16, 0xd4ef3085);
    HH(b, c, d, a, curr_msg -> curr_block.threetwo[6], 23, 0x04881d05);
    HH(a, b, c, d, curr_msg -> curr_block.threetwo[9], 4, 0xd9d4d039);
    HH(d, a, b, c, curr_msg -> curr_block.threetwo[12], 11, 0xe6db99e5);
    HH(c, d, a, b, curr_msg -> curr_block.threetwo[15], 16, 0x1fa27cf8);
    HH(b, c, d, a, curr_msg -> curr_block.threetwo[2], 23, 0xc4ac5665);

    // Round 4
    II(a, b, c, d, curr_msg -> curr_block.threetwo[0],  6, 0xf4292244);
    II(d, a, b, c, curr_msg -> curr_block.threetwo[7], 10, 0x432aff97);
    II(c, d, a, b, curr_msg -> curr_block.threetwo[14],15, 0xab9423a7);
    II(b, c, d, a, curr_msg -> curr_block.threetwo[5], 21, 0xfc93a039);
    II(a, b, c, d, curr_msg -> curr_block.threetwo[12], 6, 0x655b59c3);
    II(d, a, b, c, curr_msg -> curr_block.threetwo[3], 10, 0x8f0ccc92);
    II(c, d, a, b, curr_msg -> curr_block.threetwo[10],15, 0xffeff47d);
    II(b, c, d, a, curr_msg -> curr_block.threetwo[1], 21, 0x85845dd1);
    II(a, b, c, d, curr_msg -> curr_block.threetwo[8],  6, 0x6fa87e4f);
    II(d, a, b, c, curr_msg -> curr_block.threetwo[15],10, 0xfe2ce6e0);
    II(c, d, a, b, curr_msg -> curr_block.threetwo[6], 15, 0xa3014314);
    II(b, c, d, a, curr_msg -> curr_block.threetwo[13],21, 0x4e0811a1);
    II(a, b, c, d, curr_msg -> curr_block.threetwo[4],  6, 0xf7537e82);
    II(d, a, b, c, curr_msg -> curr_block.threetwo[11],10, 0xbd3af235);
    II(c, d, a, b, curr_msg -> curr_block.threetwo[2], 15, 0x2ad7d2bb);
    II(b, c, d, a, curr_msg -> curr_block.threetwo[9], 21, 0xeb86d391);
    
    curr_msg -> init_hash[0] += a;
    curr_msg -> init_hash[1] += b;
    curr_msg -> init_hash[2] += c;
    curr_msg -> init_hash[3] += d;
    }
}

void md5_init(struct msg_block *curr_msg) {
  curr_msg -> init_hash[0] = 0x67452301;
  curr_msg -> init_hash[1] = 0xefcdab89;
  curr_msg -> init_hash[2] = 0x98badcfe;
  curr_msg -> init_hash[3] = 0x10325476;
}

// Main
int main(int argc, char **argv) {
    FILE *file = fopen(argv[1],"rb");
    
    if(argc!=2) {
        printf("Please select file. \n");
        return 1;
    }

  if(!file) {
    printf("ERROR: Unable to open file");

    return 1;
  } else {
    struct msg_block hashedValue;

    md5_init(&hashedValue);
    md5_hash(file,&hashedValue);

    for(int i = 0; i < 4; i++) {
      printf("%02x%02x%02x%02x", (hashedValue.init_hash[i] >> 0 )&0x000000ff, 
                                (hashedValue.init_hash[i] >> 8)&0x000000ff, 
                                (hashedValue.init_hash[i] >> 16)&0x000000ff, 
                                (hashedValue.init_hash[i] >> 24)&0x000000ff);
    }
  }

  fclose(file);
  
  return 0; 
}