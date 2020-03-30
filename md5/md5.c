// Kevin Niland
// MD5 message digest algorithm implementation
#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

// Pre-defined hash values used for transform rounds 1, 2, 3, and 4
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
  0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391};

/**
 * https://tools.ietf.org/html/rfc1321
 * 
 * Push bits off to the left c places, however they are pushed in on the right again (loop around)
 */ 
#define ROTLEFT(x, c) ((x << c) | (x >> (32 - c)))

/**
 * Bitwise operators in C
 * & (bitwise AND)   
 * | (bitwise OR - v)
 * ^ (bitwise XOR)   
 * << (left shift) 
 * >> (right shift)  
 * ~ (bitwise NOT(X))
 */ 

/**
 * F(x, y, z) = XY v not(X) Z
 * G(x, y, z) = XZ v Y not(Z)
 * H(x, y, z) = X xor Y xor Z
 * I(x, y, z) = Y xor (X v not(Z))
 */
#define F(x, y, z) ((x & y) | (~x & z))
#define G(x, y, z) ((x & z) | (y & ~z))
#define H(x, y, z) (x ^ y ^ z)
#define I(x, y, z) (y ^ (x | ~z))

/**
 * Bit shifting functions used in transform rounds 1, 2, 3, and 4
 */ 
#define FF(a, b, c, d, m, s, t) { a += F(b, c, d) + m + t;  a = b + ROTLEFT(a, s); }
#define GG(a, b, c, d, m, s, t) { a += G(b, c, d) + m + t;  a = b + ROTLEFT(a, s); }
#define HH(a, b, c, d, m, s, t) { a += H(b, c, d) + m + t;  a = b + ROTLEFT(a, s); }
#define II(a, b, c, d, m, s, t) { a += I(b, c, d) + m + t;  a = b + ROTLEFT(a, s); }

// Constants for md5_hash
#define S7 7
#define S12 12
#define S17 17
#define S22 22
#define S5 5
#define S9 9
#define S14 14
#define S20 20
#define S4 4
#define S11 11
#define S16 16
#define S23 23
#define S6 6
#define S10 10
#define S15 15
#define S21 21

const uint8_t ZEROBIT = 0x00;
const uint8_t BIT = 0x80;

// Union block
union block {
  uint64_t sixfour[8];
  uint32_t threetwo[16];
  uint8_t eight[64];
  uint8_t padding[64];
};

// MD5 context
typedef struct {
  union block union_block;
  uint64_t sixfour[8];
  uint32_t threetwo[16];
  uint8_t eight[64];
  uint8_t padding[64];
  uint32_t state[4];
} MD5_CTX;

// MD5 initialization method - Begins an MD5 operation
void md5_init(MD5_CTX *md5_ctx) {
  // Load magic initialization constants to set state values
  md5_ctx -> state[0] = 0x67452301;
  md5_ctx -> state[1] = 0xefcdab89;
  md5_ctx -> state[2] = 0x98badcfe;
  md5_ctx -> state[3] = 0x10325476;
}

// Performs the MD5 message digest
void md5_hash(FILE *file, MD5_CTX *md5_ctx) {
  uint32_t a, b, c, d;
  uint64_t file_bits = 0;
  bool keepAlive = true;
  int specialPadding = 0;
  size_t check;
  
  a = md5_ctx -> state[0];
  b = md5_ctx -> state[1];
  c = md5_ctx -> state[2];
  d = md5_ctx -> state[3];

  while (keepAlive) {
    check = fread(&md5_ctx -> union_block.eight, 1, 64, file);

    if (check == 64) {
      file_bits += 512;
    } else if (check < 64 && check > 56) {
      file_bits += (check * 8);
      specialPadding = 1;

      md5_ctx -> union_block.padding[check] = BIT;

      for (int i = check + 1; i < 64; i++) {
        md5_ctx -> union_block.padding[i] = ZEROBIT;
      }
    } else if( check < 56 && check > 0) {
      file_bits += (check * 8);

      md5_ctx -> union_block.padding[check] = BIT;

      for (int i = check + 1; i < 56; i++) {
        md5_ctx -> union_block.padding[i] = ZEROBIT;
      }

      md5_ctx -> union_block.sixfour[7] = file_bits;

      keepAlive = false;
    } else if (check == 0 && specialPadding == 0) {
      md5_ctx -> union_block.padding[0] = BIT;

      for(int i = 1; i < 56; i++) {
        md5_ctx -> union_block.padding[i] = ZEROBIT;
      }

      md5_ctx -> union_block.sixfour[7] = file_bits;

      keepAlive = false;
    } else if (check == 0 && specialPadding == 1) {
      md5_ctx -> union_block.padding[0]=ZEROBIT;

      for(int i = 1; i < 56; i++) {
        md5_ctx -> union_block.padding[i] = ZEROBIT;
      }

      md5_ctx -> union_block.sixfour[7] = file_bits;

      keepAlive = false;
    }
    
    // Round 1 - FF function
    FF(a, b, c, d, md5_ctx -> union_block.threetwo[0], S7, K[0]);
    FF(d, a, b, c, md5_ctx -> union_block.threetwo[1], S12, K[1]);
    FF(c, d, a, b, md5_ctx -> union_block.threetwo[2], S17, K[2]);
    FF(b, c, d, a, md5_ctx -> union_block.threetwo[3], S22, K[3]);
    FF(a, b, c, d, md5_ctx -> union_block.threetwo[4], S7, K[4]);
    FF(d, a, b, c, md5_ctx -> union_block.threetwo[5], S12, K[5]);
    FF(c, d, a, b, md5_ctx -> union_block.threetwo[6], S17, K[6]);
    FF(b, c, d, a, md5_ctx -> union_block.threetwo[7], S22, K[7]);
    FF(a, b, c, d, md5_ctx -> union_block.threetwo[8], S7, K[8]);
    FF(d, a, b, c, md5_ctx -> union_block.threetwo[9], S12, K[9]);
    FF(c, d, a, b, md5_ctx -> union_block.threetwo[10], S17, K[10]);
    FF(b, c, d, a, md5_ctx -> union_block.threetwo[11], S22, K[11]);
    FF(a, b, c, d, md5_ctx -> union_block.threetwo[12], S7, K[12]);
    FF(d, a, b, c, md5_ctx -> union_block.threetwo[13], S12, K[13]);
    FF(c, d, a, b, md5_ctx -> union_block.threetwo[14], S17, K[14]);
    FF(b, c, d, a, md5_ctx -> union_block.threetwo[15], S22, K[15]);

    // Round 2 - GG function
    GG(a, b, c, d, md5_ctx -> union_block.threetwo[1], S5, K[16]);
    GG(d, a, b, c, md5_ctx -> union_block.threetwo[6], S9, K[17]);
    GG(c, d, a, b, md5_ctx -> union_block.threetwo[11], S14, K[18]);
    GG(b, c, d, a, md5_ctx -> union_block.threetwo[0], S20, K[19]);
    GG(a, b, c, d, md5_ctx -> union_block.threetwo[5], S5, K[20]);
    GG(d, a, b, c, md5_ctx -> union_block.threetwo[10], S9, K[21]);
    GG(c, d, a, b, md5_ctx -> union_block.threetwo[15], S14, K[22]);
    GG(b, c, d, a, md5_ctx -> union_block.threetwo[4], S20, K[23]);
    GG(a, b, c, d, md5_ctx -> union_block.threetwo[9], S5, K[24]);
    GG(d, a, b, c, md5_ctx -> union_block.threetwo[14], S9, K[25]);
    GG(c, d, a, b, md5_ctx -> union_block.threetwo[3], S14, K[26]);
    GG(b, c, d, a, md5_ctx -> union_block.threetwo[8], S20, K[27]);
    GG(a, b, c, d, md5_ctx -> union_block.threetwo[13], S5, K[28]);
    GG(d, a, b, c, md5_ctx -> union_block.threetwo[2], S9, K[29]);
    GG(c, d, a, b, md5_ctx -> union_block.threetwo[7], S14, K[30]);
    GG(b, c, d, a, md5_ctx -> union_block.threetwo[12], S20, K[31]);

    // Round 3 - HH function
    HH(a, b, c, d, md5_ctx -> union_block.threetwo[5], S4, K[32]);
    HH(d, a, b, c, md5_ctx -> union_block.threetwo[8], S11, K[33]);
    HH(c, d, a, b, md5_ctx -> union_block.threetwo[11], S16, K[34]);
    HH(b, c, d, a, md5_ctx -> union_block.threetwo[14], S23, K[35]);
    HH(a, b, c, d, md5_ctx -> union_block.threetwo[1], S4, K[36]);
    HH(d, a, b, c, md5_ctx -> union_block.threetwo[4], S11, K[37]);
    HH(c, d, a, b, md5_ctx -> union_block.threetwo[7], S16, K[38]);
    HH(b, c, d, a, md5_ctx -> union_block.threetwo[10], S23, K[39]);
    HH(a, b, c, d, md5_ctx -> union_block.threetwo[13], S4, K[40]);
    HH(d, a, b, c, md5_ctx -> union_block.threetwo[0], S11, K[41]);
    HH(c, d, a, b, md5_ctx -> union_block.threetwo[3], S16, K[42]);
    HH(b, c, d, a, md5_ctx -> union_block.threetwo[6], S23, K[43]);
    HH(a, b, c, d, md5_ctx -> union_block.threetwo[9], S4, K[44]);
    HH(d, a, b, c, md5_ctx -> union_block.threetwo[12], S11, K[45]);
    HH(c, d, a, b, md5_ctx -> union_block.threetwo[15], S16, K[46]);
    HH(b, c, d, a, md5_ctx -> union_block.threetwo[2], S23, K[47]);

    // Round 4 - II function
    II(a, b, c, d, md5_ctx -> union_block.threetwo[0], S6, K[48]);
    II(d, a, b, c, md5_ctx -> union_block.threetwo[7], S10, K[49]);
    II(c, d, a, b, md5_ctx -> union_block.threetwo[14], S15, K[50]);
    II(b, c, d, a, md5_ctx -> union_block.threetwo[5], S21, K[51]);
    II(a, b, c, d, md5_ctx -> union_block.threetwo[12], S6, K[52]);
    II(d, a, b, c, md5_ctx -> union_block.threetwo[3], S10, K[53]);
    II(c, d, a, b, md5_ctx -> union_block.threetwo[10], S15, K[54]);
    II(b, c, d, a, md5_ctx -> union_block.threetwo[1], S21, K[55]);
    II(a, b, c, d, md5_ctx -> union_block.threetwo[8], S6, K[56]);
    II(d, a, b, c, md5_ctx -> union_block.threetwo[15], S10, K[57]);
    II(c, d, a, b, md5_ctx -> union_block.threetwo[6], S15, K[58]);
    II(b, c, d, a, md5_ctx -> union_block.threetwo[13], S21, K[59]);
    II(a, b, c, d, md5_ctx -> union_block.threetwo[4], S6, K[60]);
    II(d, a, b, c, md5_ctx -> union_block.threetwo[11], S10, K[61]);
    II(c, d, a, b, md5_ctx -> union_block.threetwo[2], S15, K[62]);
    II(b, c, d, a, md5_ctx -> union_block.threetwo[9], S21, K[63]);
    
    // Update states after rounds
    md5_ctx -> state[0] += a;
    md5_ctx -> state[1] += b;
    md5_ctx -> state[2] += c;
    md5_ctx -> state[3] += d;

    // Update final digest value
    for(int i = 0, j = 0; i < 4; i++, j += 4) {
      md5_ctx -> union_block.threetwo[j] = (md5_ctx -> union_block.eight[i] & 0xFF);
      md5_ctx -> union_block.threetwo[j + 1] = ((md5_ctx -> union_block.eight[i]) >> 8 & 0xFF);
      md5_ctx -> union_block.threetwo[j + 2] = ((md5_ctx -> union_block.eight[i]) >> 16 & 0xFF);
      md5_ctx -> union_block.threetwo[j + 3] = ((md5_ctx -> union_block.eight[i]) >> 24 & 0xFF);
    }
  }
}

// Main
int main(int argc, char **argv) {
  MD5_CTX md5_ctx_val;
  FILE *file = fopen(argv[1], "rb");
  int option, i;
  char optionVal[50];
  
  // if (argc != 2) {
  //   printf("Please select file. \n");
  //   return 1;
  // }

  // if (!file) {
  //   printf("ERROR: Unable to open file");

  //   return 1;
  // } else {
  //   md5_init(&md5_ctx_val);
  //   md5_hash(file, &md5_ctx_val);

  //   for(i = 0; i < 4; i++) {
  //     printf("%02x%02x%02x%02x", (md5_ctx_val.state[i] >> 0 )&0x000000ff, 
  //                               (md5_ctx_val.state[i] >> 8)&0x000000ff, 
  //                               (md5_ctx_val.state[i] >> 16)&0x000000ff, 
  //                               (md5_ctx_val.state[i] >> 24)&0x000000ff);
  //   }
  // }

  // Menu - User can perform MD5 message digest on a given string or file
  printf("Enter 1 to pass in a string to,\n"); 
  printf("Enter 2 to pass in a file or\n");
  printf("Enter 3 to exit: \n");  
	scanf("%d", &option);

  fclose(file);
  
  return 0; 
}