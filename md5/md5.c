/**
 * Kevin Niland
 * MD5 message digest algorithm implementation
 */ 
#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Union block
union block {
  uint64_t sixfour[8];
  uint32_t threetwo[16];
  uint8_t eight[64];
  uint8_t eight_pad[64];
};

// MD5 context
typedef struct {
  uint32_t state[4];
} MD5_CTX;

// MD5 function prototypes
void md5_init(MD5_CTX *md5_ctx);
FILE *md5_hash(MD5_CTX *md5_ctx, union block *B, char *file);

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
 * 
 * & - bitwise AND  
 * | - bitwise OR 
 * ~ - bitwise NOT(X)
 * ^ - bitwise XOR
 * << - Shift left 
 * >> - Shift right  
 */ 

/**
 * F(x, y, z) = ((x AND y) OR (NOT(x) and z))
 * G(x, y, z) = ((x AND z) OR (y NOT(z))
 * H(x, y, z) = (x XOR y XOR z)
 * I(x, y, z) = (y XOR (x OR NOT(z))
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

// Constants for md5_hash function
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

#define FNSZ 128

// Bits for manual padding
const uint8_t ZEROBIT = 0x00;
const uint8_t ONEBIT = 0x80;

// MD5 initialization method - Starts an MD5 operation
void md5_init(MD5_CTX *md5_ctx) {
  // Load magic initialization constants to set state values
  md5_ctx -> state[0] = 0x67452301;
  md5_ctx -> state[1] = 0xefcdab89;
  md5_ctx -> state[2] = 0x98badcfe;
  md5_ctx -> state[3] = 0x10325476;
}

// Performs the MD5 message digest
FILE *md5_hash(MD5_CTX *md5_ctx, union block *B, char *file) {
  // File pointer
  FILE *fptr;

  // Stores previous hash values
  uint32_t a, b, c, d;

  // Keeps track of bits in file
  uint64_t counter = 0;
  size_t size;
  
  bool keepAlive = true;
  int i, pad = 0;
  
  // Assign initial values to temp variables in memory
  a = md5_ctx -> state[0];
  b = md5_ctx -> state[1];
  c = md5_ctx -> state[2];
  d = md5_ctx -> state[3];

  // Open file
  fptr = fopen(file, "r");

  printf("\nFile: %s\n", file);
  printf("String hash: ");

  // Checks if file exists
  if (fptr == NULL) {
    fprintf(stderr, "ERROR: File %s does not exist", file);
  }

  // Padding
  while (keepAlive) {
    size = fread(&B -> eight, 1, 64, fptr);

    /**
     * Padding operations
     * 
     * 1. If size is equal to 64, continue - no padding needed
     * 2. If size is greater than 56 and less than 64, not enough space for 64 bits at end of file
     * 3. If size is less than 56 and greater than 0, pad file at the end
     * 4. If size is equal to 0 and pad is equal to 0, end of file - file was a size of multiple 64
     * 5. If size is equal to 0 and pad is equal to 1, end of file - padding started in previous block, pad file with all zeros
     */ 
    if (size == 64) {
      counter += 512;
    } else if (size < 64 && size > 56) {
      // Update counter to current size multiplied by 8
      counter += (size * 8);
      pad = 1; 

      // Pad out to end of block
      B -> eight_pad[size] = ONEBIT;

      for (i = size + 1; i < 64; i++) {
        B -> eight_pad[i] = ZEROBIT;
      }
    } else if (size < 56 && size > 0) { 
      // Update counter to current size multiplied by 8 
      counter += (size * 8);

      // Pad out to end of block
      B -> eight_pad[size] = ONEBIT;

      for (i = size + 1; i < 56; i++) {
        B -> eight_pad[i] = ZEROBIT;
      }

      B -> sixfour[7] = counter;

      // Exit while loop
      keepAlive = false;
    } else if (size == 0 && pad == 0) {
      B -> eight_pad[0] = ONEBIT;

      for(i = 1; i < 56; i++) {
        B -> eight_pad[i] = ZEROBIT;
      }

      B -> sixfour[7] = counter;

      // Exit while loop
      keepAlive = false;
    } else if (size == 0 && pad == 1) {
      B -> eight_pad[0]=ZEROBIT;

      for(i = 1; i < 56; i++) {
        B -> eight_pad[i] = ZEROBIT;
      }

      B -> sixfour[7] = counter;

      // Exit while loop
      keepAlive = false;
    }
    
    /**
     * Adapted from: https://github.com/Souravpunoriyar/md5-in-c and https://tools.ietf.org/html/rfc1321
     */

    // Round 1 - FF function
    FF(a, b, c, d, B -> threetwo[0], S7, K[0]);
    FF(d, a, b, c, B -> threetwo[1], S12, K[1]);
    FF(c, d, a, b, B -> threetwo[2], S17, K[2]);
    FF(b, c, d, a, B -> threetwo[3], S22, K[3]);
    FF(a, b, c, d, B -> threetwo[4], S7, K[4]);
    FF(d, a, b, c, B -> threetwo[5], S12, K[5]);
    FF(c, d, a, b, B -> threetwo[6], S17, K[6]);
    FF(b, c, d, a, B -> threetwo[7], S22, K[7]);
    FF(a, b, c, d, B -> threetwo[8], S7, K[8]);
    FF(d, a, b, c, B -> threetwo[9], S12, K[9]);
    FF(c, d, a, b, B -> threetwo[10], S17, K[10]);
    FF(b, c, d, a, B -> threetwo[11], S22, K[11]);
    FF(a, b, c, d, B -> threetwo[12], S7, K[12]);
    FF(d, a, b, c, B -> threetwo[13], S12, K[13]);
    FF(c, d, a, b, B -> threetwo[14], S17, K[14]);
    FF(b, c, d, a, B -> threetwo[15], S22, K[15]);

    // Round 2 - GG function
    GG(a, b, c, d, B -> threetwo[1], S5, K[16]);
    GG(d, a, b, c, B -> threetwo[6], S9, K[17]);
    GG(c, d, a, b, B -> threetwo[11], S14, K[18]);
    GG(b, c, d, a, B -> threetwo[0], S20, K[19]);
    GG(a, b, c, d, B -> threetwo[5], S5, K[20]);
    GG(d, a, b, c, B -> threetwo[10], S9, K[21]);
    GG(c, d, a, b, B -> threetwo[15], S14, K[22]);
    GG(b, c, d, a, B -> threetwo[4], S20, K[23]);
    GG(a, b, c, d, B -> threetwo[9], S5, K[24]);
    GG(d, a, b, c, B -> threetwo[14], S9, K[25]);
    GG(c, d, a, b, B -> threetwo[3], S14, K[26]);
    GG(b, c, d, a, B -> threetwo[8], S20, K[27]);
    GG(a, b, c, d, B -> threetwo[13], S5, K[28]);
    GG(d, a, b, c, B -> threetwo[2], S9, K[29]);
    GG(c, d, a, b, B -> threetwo[7], S14, K[30]);
    GG(b, c, d, a, B -> threetwo[12], S20, K[31]);

    // Round 3 - HH function
    HH(a, b, c, d, B -> threetwo[5], S4, K[32]);
    HH(d, a, b, c, B -> threetwo[8], S11, K[33]);
    HH(c, d, a, b, B -> threetwo[11], S16, K[34]);
    HH(b, c, d, a, B -> threetwo[14], S23, K[35]);
    HH(a, b, c, d, B -> threetwo[1], S4, K[36]);
    HH(d, a, b, c, B -> threetwo[4], S11, K[37]);
    HH(c, d, a, b, B -> threetwo[7], S16, K[38]);
    HH(b, c, d, a, B -> threetwo[10], S23, K[39]);
    HH(a, b, c, d, B -> threetwo[13], S4, K[40]);
    HH(d, a, b, c, B -> threetwo[0], S11, K[41]);
    HH(c, d, a, b, B -> threetwo[3], S16, K[42]);
    HH(b, c, d, a, B -> threetwo[6], S23, K[43]);
    HH(a, b, c, d, B -> threetwo[9], S4, K[44]);
    HH(d, a, b, c, B -> threetwo[12], S11, K[45]);
    HH(c, d, a, b, B -> threetwo[15], S16, K[46]);
    HH(b, c, d, a, B -> threetwo[2], S23, K[47]);

    // Round 4 - II function
    II(a, b, c, d, B -> threetwo[0], S6, K[48]);
    II(d, a, b, c, B -> threetwo[7], S10, K[49]);
    II(c, d, a, b, B -> threetwo[14], S15, K[50]);
    II(b, c, d, a, B -> threetwo[5], S21, K[51]);
    II(a, b, c, d, B -> threetwo[12], S6, K[52]);
    II(d, a, b, c, B -> threetwo[3], S10, K[53]);
    II(c, d, a, b, B -> threetwo[10], S15, K[54]);
    II(b, c, d, a, B -> threetwo[1], S21, K[55]);
    II(a, b, c, d, B -> threetwo[8], S6, K[56]);
    II(d, a, b, c, B -> threetwo[15], S10, K[57]);
    II(c, d, a, b, B -> threetwo[6], S15, K[58]);
    II(b, c, d, a, B -> threetwo[13], S21, K[59]);
    II(a, b, c, d, B -> threetwo[4], S6, K[60]);
    II(d, a, b, c, B -> threetwo[11], S10, K[61]);
    II(c, d, a, b, B -> threetwo[2], S15, K[62]);
    II(b, c, d, a, B -> threetwo[9], S21, K[63]);
    
    // Update states after rounds
    md5_ctx -> state[0] += a;
    md5_ctx -> state[1] += b;
    md5_ctx -> state[2] += c;
    md5_ctx -> state[3] += d;

    // Final update value
    for(int i = 0, j = 0; i < 4; i++, j += 4) {
      B -> threetwo[j] = (B -> eight[i] & 0xFF);
      B -> threetwo[j + 1] = ((B -> eight[i]) >> 8 & 0xFF);
      B -> threetwo[j + 2] = ((B -> eight[i]) >> 16 & 0xFF);
      B -> threetwo[j + 3] = ((B -> eight[i]) >> 24 & 0xFF);
    }
  }
}

// Main function
int main(int argc, char **argv) {
  FILE *file = NULL; // File pointer for files in the 'files' directory
  FILE *userPtr; // File pointer for file created from user input
  MD5_CTX md5_ctx_val; // Context
  union block B; // Union block
  bool keepAlive = true; // Keep while loop running until user manually exits
  int menuOption, i;
  char initOption, file_name[FNSZ] = {0};
  char user_file[50] = "user_input.txt"; // File that stores user input
  char string[100]; // User input

  // Command line arguments
  if (argc == 2 && strcmp(argv[1], "--help") == 0) {
    printf("\nUsage: ./md5 (Displays menu system - program will keep running, user can keep hashing a file/input)");
    printf("\n   or  ./md5 [options]");
    printf("\n\nwhere options include:");
    printf("\n--test     Run tests to check if code is correct");
    printf("\n--version  Display the current version of the program");
    printf("\n--file     Allows you to specify a file to hash from the command line");
    printf("\n--string   Allows you to specify a string/sentence to hash from the command line");

    exit(1);
  }

  // Test cases
  if (argc == 2 && strcmp(argv[1], "--test") == 0) {
    printf("Test command");

    exit(1);
  }

  // Version
  if (argc == 2 && strcmp(argv[1], "--version") == 0) {
    printf("md5.c - 1.0\n");

    #ifdef __GNUC__
      printf ("gcc.exe - %d.%d\n", __GNUC__, __GNUC_MINOR__);
    #endif

    exit(1);
  }

  printf("\nMD5 Message Digest Implementation");
  printf("\n=================================\n");

  // Menu - User can perform MD5 message digest on a given string or file
  printf("Enter 1 to pass in a file (files/name_of_file.extension), \n"); 
  printf("Enter 2 to pass in a word/sentence or\n");
  printf("Enter 3 to exit: ");  
	scanf("%d", &menuOption);

  while (keepAlive = true) {
    switch (menuOption) {
      case 1:
        /**
         * Adapted from: https://stackoverflow.com/a/34738555/8721358
         * 
         * Prompt user for file name
         */ 
        printf("Enter a file name: ");
        
        if (scanf ("%127s%*c", file_name) != 1) {
          fprintf (stderr, "ERROR: file_name entry failed.\n");

          return 1;
        }
        
        // Hash file
        md5_init(&md5_ctx_val);
        file = md5_hash(&md5_ctx_val, &B, file_name);

        // Notifies user of error if unable to open file
        if (!file) {
          fprintf (stderr, "ERROR: Failed to open file '%s'\n", file_name);

          return 1;
        }

        // Display results of MD5 digest
        for(i = 0; i < 4; i++) {
          printf("%02x", (md5_ctx_val.state[i] >> 0 ) & 0x000000ff);
          printf("%02x", (md5_ctx_val.state[i] >> 8) & 0x000000ff);
          printf("%02x", (md5_ctx_val.state[i] >> 16) & 0x000000ff); 
          printf("%02x", (md5_ctx_val.state[i] >> 24) & 0x000000ff);
        }

        break;
      case 2:
        /**
         * Not perfect - string is first written to file and is then hashed
         * Could have been done better - md5_hash function was written to work with files specifically
         */

        // Open file for writing
        userPtr = fopen(user_file, "w");
        
        if(userPtr == NULL) {
          printf("ERROR: File doesn't exist");   
          exit(1);             
        }

        // Prompt user for input
        printf("Enter a word/sentence: ");
        scanf("%s", &string);
        // fgets(string, sizeof(string), stdin);
        // getchar();

        // Write input to file
        fprintf(userPtr, "%s", string);
        fclose(userPtr);
        
        // Hash file
        md5_init(&md5_ctx_val);
        file = md5_hash(&md5_ctx_val, &B, user_file);

        if (!file) {
          fprintf (stderr, "ERROR: Failed to open file '%s'\n", user_file);

          return 1;
        }

        // Display results of MD5 digest
        for(i = 0; i < 4; i++) {
          printf("%02x", (md5_ctx_val.state[i] >> 0 ) & 0x000000ff);
          printf("%02x", (md5_ctx_val.state[i] >> 8) & 0x000000ff);
          printf("%02x", (md5_ctx_val.state[i] >> 16) & 0x000000ff); 
          printf("%02x", (md5_ctx_val.state[i] >> 24) & 0x000000ff);
        }

        // string_to_file();
        break;
        case 3:
          // Terminate the program
          printf("\nTerminating program...\n");
          fclose(file);

          exit(1);
          break;
        default:
          break;
    }
    
    // Prompt user for another option
    printf("\n\nEnter 1 to pass in a file (files/name_of_file.extension),\n"); 
    printf("Enter 2 to pass in a word/sentence or\n");
    printf("Enter 3 to exit: ");  
	  scanf("%d", &menuOption);
  }
  
  return 0; 
}