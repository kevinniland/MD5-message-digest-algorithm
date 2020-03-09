// Kevin Niland
// MD5 message-digest algorithm implementation

#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdbool.h>

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

const uint32_t R[64] = {7, 12, 17, 22,
                        7, 12, 17, 22,
                        7, 12, 17, 22,
                        7, 12, 17, 22,
                        5, 9, 14, 20, 
                        5, 9, 14, 20, 
                        5, 9, 14, 20,
                        5, 9, 14, 20, 
                        4, 11, 16, 23, 
                        4, 11, 16, 23, 
                        4, 11, 16, 23, 
                        4, 1, 16, 23, 
                        6, 10, 15, 21, 
                        6, 10, 15, 21,
                        6, 10, 15, 21, 
                        6, 10, 15, 21};

// https://tools.ietf.org/html/rfc1321
#define ROTLEFT(x, c) (((x) << (c)) | ((x) >> (32 - (c))))
#define F(x, y, z) (((x) & (y)) | ((~x) & (z)))
#define G(x, y, z) (((x) & (z)) | ((y) & (~z)))
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define I(x, y, z) ((y) ^ ((x) | (~z)))

#define FF(a, b, c, d, m, s, t) { a += F(b, c, d) + m + t;  a = b + ROTLEFT(a, s); }
#define GG(a, b, c, d, m, s, t) { a += G(b, c, d) + m + t;  a = b + ROTLEFT(a, s); }
#define HH(a, b, c, d, m, s, t) { a += H(b, c, d) + m + t;  a = b + ROTLEFT(a, s); }
#define II(a, b, c, d, m, s, t) { a += I(b, c, d) + m + t;  a = b + ROTLEFT(a, s); }

const uint8_t PADDING = 0x00;
const uint8_t BIT = 0x80;

union bits {
  uint64_t file_size[8];
  uint32_t hash[16];
  uint8_t read_file[64];
  uint8_t padding[64];
};

struct msg_block {
  union bits curr_block;
  uint32_t init_hash[4];
};

void md5_init(struct msg_block *msg) {
  msg -> init_hash[0] = 0x67452301;
  msg -> init_hash[1] = 0xefcdab89;
  msg -> init_hash[2] = 0x98badcfe;
  msg -> init_hash[3] = 0x10325476;
}

void md5_hashing_one(uint64_t file_bits, size_t check_bytes, struct msg_block *msg, int padding_block) {
  file_bits += (check_bytes * 8);
  padding_block = 1;

  msg -> curr_block.padding[check_bytes] = BIT;

  for (int i = check_bytes + 1; i < 64; i++) {
    msg -> curr_block.padding[i] = PADDING;
    }
}

void md5_transform(struct msg_block *msg) {
  int a, b, c, d, m[16], i, j;

  for(int i = 0, j=0; i<16; ++i, j+=4) {
    msg -> curr_block.hash[i] = 
    (msg -> curr_block.read_file[j]) + 
    (msg -> curr_block.read_file[j +1 ] << 8) + 
    (msg -> curr_block.read_file[j + 2] << 16)+ 
    (msg -> curr_block.read_file[j + 3] << 24);
    }

    a = msg -> init_hash[0];
    b = msg -> init_hash[1];
    c = msg -> init_hash[2];
    d = msg -> init_hash[3];

   // https://github.com/Souravpunoriyar/md5-in-c
   // Reverse bytes upon input
   FF(a, b, c, d, msg -> curr_block.hash[0],  7, 0xd76aa478);
   FF(d, a, b, c, msg -> curr_block.hash[1], 12, 0xe8c7b756);
   FF(c, d, a, b, msg -> curr_block.hash[2], 17, 0x242070db);
   FF(b, c, d, a, msg -> curr_block.hash[3], 22, 0xc1bdceee);
   FF(a, b, c, d, msg -> curr_block.hash[4],  7, 0xf57c0faf);
   FF(d, a, b, c, msg -> curr_block.hash[5], 12, 0x4787c62a);
   FF(c, d, a, b, msg -> curr_block.hash[6], 17, 0xa8304613);
   FF(b, c, d, a, msg -> curr_block.hash[7], 22, 0xfd469501);
   FF(a, b, c, d, msg -> curr_block.hash[8],  7, 0x698098d8);
   FF(d, a, b, c, msg -> curr_block.hash[9], 12, 0x8b44f7af);
   FF(c, d, a, b, msg -> curr_block.hash[10], 17, 0xffff5bb1);
   FF(b, c, d, a, msg -> curr_block.hash[11], 22, 0x895cd7be);
   FF(a, b, c, d, msg -> curr_block.hash[12], 7, 0x6b901122);
   FF(d, a, b, c, msg -> curr_block.hash[13], 12, 0xfd987193);
   FF(c, d, a, b, msg -> curr_block.hash[14], 17, 0xa679438e);
   FF(b, c, d, a, msg -> curr_block.hash[15], 22, 0x49b40821);

   GG(a, b, c, d, msg -> curr_block.hash[1],  5, 0xf61e2562);
   GG(d, a, b, c, msg -> curr_block.hash[6],  9, 0xc040b340);
   GG(c, d, a, b, msg -> curr_block.hash[11], 14, 0x265e5a51);
   GG(b, c, d, a, msg -> curr_block.hash[0], 20, 0xe9b6c7aa);
   GG(a, b, c, d, msg -> curr_block.hash[5],  5, 0xd62f105d);
   GG(d, a, b, c, msg -> curr_block.hash[10], 9, 0x02441453);
   GG(c, d, a, b, msg -> curr_block.hash[15], 14, 0xd8a1e681);
   GG(b, c, d, a, msg -> curr_block.hash[4], 20, 0xe7d3fbc8);
   GG(a, b, c, d, msg -> curr_block.hash[9],  5, 0x21e1cde6);
   GG(d, a, b, c, msg -> curr_block.hash[14], 9, 0xc33707d6);
   GG(c, d, a, b, msg -> curr_block.hash[3], 14, 0xf4d50d87);
   GG(b, c, d, a, msg -> curr_block.hash[8], 20, 0x455a14ed);
   GG(a, b, c, d, msg -> curr_block.hash[13], 5, 0xa9e3e905);
   GG(d, a, b, c, msg -> curr_block.hash[2],  9, 0xfcefa3f8);
   GG(c, d, a, b, msg -> curr_block.hash[7], 14, 0x676f02d9);
   GG(b, c, d, a, msg -> curr_block.hash[12], 20, 0x8d2a4c8a);

   HH(a, b, c, d, msg -> curr_block.hash[5],  4, 0xfffa3942);
   HH(d, a, b, c, msg -> curr_block.hash[8], 11, 0x8771f681);
   HH(c, d, a, b, msg -> curr_block.hash[11], 16, 0x6d9d6122);
   HH(b, c, d, a, msg -> curr_block.hash[14], 23, 0xfde5380c);
   HH(a, b, c, d, msg -> curr_block.hash[1],  4, 0xa4beea44);
   HH(d, a, b, c, msg -> curr_block.hash[4], 11, 0x4bdecfa9);
   HH(c, d, a, b, msg -> curr_block.hash[7], 16, 0xf6bb4b60);
   HH(b, c, d, a, msg -> curr_block.hash[10], 23, 0xbebfbc70);
   HH(a, b, c, d, msg -> curr_block.hash[13], 4, 0x289b7ec6);
   HH(d, a, b, c, msg -> curr_block.hash[0], 11, 0xeaa127fa);
   HH(c, d, a, b, msg -> curr_block.hash[3], 16, 0xd4ef3085);
   HH(b, c, d, a, msg -> curr_block.hash[6], 23, 0x04881d05);
   HH(a, b, c, d, msg -> curr_block.hash[9],  4, 0xd9d4d039);
   HH(d, a, b, c, msg -> curr_block.hash[12], 11, 0xe6db99e5);
   HH(c, d, a, b, msg -> curr_block.hash[15], 16, 0x1fa27cf8);
   HH(b, c, d, a, msg -> curr_block.hash[2], 23, 0xc4ac5665);

   II(a, b, c, d, msg -> curr_block.hash[0],  6, 0xf4292244);
   II(d, a, b, c, msg -> curr_block.hash[7], 10, 0x432aff97);
   II(c, d, a, b, msg -> curr_block.hash[14],15, 0xab9423a7);
   II(b, c, d, a, msg -> curr_block.hash[5], 21, 0xfc93a039);
   II(a, b, c, d, msg -> curr_block.hash[12], 6, 0x655b59c3);
   II(d, a, b, c, msg -> curr_block.hash[3], 10, 0x8f0ccc92);
   II(c, d, a, b, msg -> curr_block.hash[10],15, 0xffeff47d);
   II(b, c, d, a, msg -> curr_block.hash[1], 21, 0x85845dd1);
   II(a, b, c, d, msg -> curr_block.hash[8],  6, 0x6fa87e4f);
   II(d, a, b, c, msg -> curr_block.hash[15],10, 0xfe2ce6e0);
   II(c, d, a, b, msg -> curr_block.hash[6], 15, 0xa3014314);
   II(b, c, d, a, msg -> curr_block.hash[13],21, 0x4e0811a1);
   II(a, b, c, d, msg -> curr_block.hash[4],  6, 0xf7537e82);
   II(d, a, b, c, msg -> curr_block.hash[11],10, 0xbd3af235);
   II(c, d, a, b, msg -> curr_block.hash[2], 15, 0x2ad7d2bb);
   II(b, c, d, a, msg -> curr_block.hash[9], 21, 0xeb86d391);

  msg -> init_hash[0] += a;
  msg -> init_hash[1] += b;
  msg -> init_hash[2] += c;
  msg -> init_hash[3] += d;
}

void md5_hash(FILE *file, struct msg_block *msg) {
  uint32_t a, b, c, d, temp;
  uint64_t file_bits = 0;
  size_t check_bytes;
  bool keepAlive = true;
  int padding_block = 0;

  while (keepAlive) {
    check_bytes = fread(&msg -> curr_block.read_file, 1, 64, file);

    if (check_bytes == 64) {
      file_bits += 512;
    } else if (check_bytes < 64 && check_bytes > 56) {
      // file_bits += (check_bytes * 8);
      // padding_block = 1;

      // msg -> curr_block.padding[check_bytes] = BIT;

      // for (i = check_bytes + 1; i < 64; i++) {
      //   msg -> curr_block.padding[i] = PADDING;
      // }
      md5_hashing_one(file_bits, check_bytes, msg, padding_block);
    }

    md5_transform(msg);
  }
}