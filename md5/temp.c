#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
//constants required for message hash
//as per https://tools.ietf.org/html/rfc1321
const uint32_t k[64] = {
0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee ,
0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501 ,
0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be ,
0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821 ,
0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa ,
0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8 ,
0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed ,
0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a ,
0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c ,
0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70 ,
0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05 ,
0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665 ,
0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039 ,
0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1 ,
0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1 ,
0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391 };
//required for hashing
//as per as per https://tools.ietf.org/html/rfc1321
const uint32_t r[64] = {7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
                      5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20,
                      4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
                      6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21};
//macros for performing the hash
//adapted from https://tools.ietf.org/html/rfc1321
#define LEFTROTATE(x, c) (((x) << (c)) | ((x) >> (32 - (c))))
#define F(x, y, z) (((x) & (y)) | ((~x) & (z)))
#define G(x, y, z) (((x) & (z)) | ((y) & (~z)))
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define I(x, y, z) ((y) ^ ((x) | (~z)))
//adapted from https://www.slideshare.net/sourav777/sourav-md5
#define FF(a,b,c,d,m,s,t) { a += F(b,c,d) + m + t;  a = b + LEFTROTATE(a,s); }
#define GG(a,b,c,d,m,s,t) { a += G(b,c,d) + m + t;  a = b + LEFTROTATE(a,s); }
#define HH(a,b,c,d,m,s,t) { a += H(b,c,d) + m + t;  a = b + LEFTROTATE(a,s); }
#define II(a,b,c,d,m,s,t) { a += I(b,c,d) + m + t;  a = b + LEFTROTATE(a,s); }
//bits for manually adding the padding
const uint8_t PADDING = 0x00;
const uint8_t ONEBIT = 0x80;
//block manipulating the bits
union messageAndOrPadding{
  uint64_t sizeOfFile[8];
  uint32_t hash[16];
  uint8_t readFile[64];
  uint8_t padding[64];
};
//actual message block which contains the hash aswell
struct messageBlock{
  //message block of 512 bits -> 16 32bit integers
  union messageAndOrPadding currentBlock;
  //placeholer for the initial and final hash value
  uint32_t initialHash[4];
};
void tenShpot(FILE *hashFile, struct messageBlock *currentMessage)
{
  //Four variables for storing previous hash and updating afterwards
  uint32_t a,b,c,d,temp;
  //counter for bits consumed
  uint64_t bitsInFile = 0;
  //control for continuing to hash
  int continueDigest = 1;
  //a control for adding a special block of padding if required
  int specialPadding = 0;
  size_t check;
  //main digest loop
  while(continueDigest == 1)
  {
    check = fread(&currentMessage->currentBlock.readFile,1,64,hashFile);
    //check the amount of bytes consumed from the file decide padding if required
    if(check==64)//64 bytes consumed -- all good. Keep looping
    {
      bitsInFile+=512;
    }
    else if(check<64 && check>56)//not enough space for 64 bit size of file at the end
    {
      bitsInFile+=(check*8);
      specialPadding = 1;//make sure that special padding is added on next loop
      //padd out to end of block correctly
      currentMessage->currentBlock.padding[check] = ONEBIT;
      for(int i = check+1; i<64; i++)
      {
        currentMessage->currentBlock.padding[i] = PADDING;
      }
    }
    else if(check<56 && check>0)//pad out with the 64 bit size of file at the end
    {
      bitsInFile += (check*8);
      currentMessage->currentBlock.padding[check]=ONEBIT;
      for(int i=check+1; i<56; i++)
      {
        currentMessage->currentBlock.padding[i] = PADDING;
      }
      currentMessage->currentBlock.sizeOfFile[7] = bitsInFile;
      continueDigest = 0;
    }
    //end of file/hashing
    else if(check == 0 && specialPadding == 0)//file was a multiple of 64 in bytes add 10000000 and pad regular
    {
      currentMessage->currentBlock.padding[0]=ONEBIT;
      for(int i=1; i<56; i++)
      {
        currentMessage->currentBlock.padding[i] = PADDING;
      }
      currentMessage->currentBlock.sizeOfFile[7] = bitsInFile;
      continueDigest = 0;//file consumed - end digest
    }//no special padding
    else if(check == 0 && specialPadding == 1)//file is finished but padding started in previous block pad all zeros followed by size of file
    {
      currentMessage->currentBlock.padding[0]=PADDING;
      for(int i=1; i<56; i++)
      {
        currentMessage->currentBlock.padding[i] = PADDING;
      }
      currentMessage->currentBlock.sizeOfFile[7] = bitsInFile;
      continueDigest = 0;
    }//add special padding

    //adapted from https://github.com/Souravpunoriyar/md5-in-C/blob/master/md5.c
    //prepare the message
    for(int i = 0, j=0; i<16; ++i, j+=4)
    {
      currentMessage->currentBlock.hash[i] = 
                               (currentMessage->currentBlock.readFile[j])+ 
                               (currentMessage->currentBlock.readFile[j+1] << 8)+ 
                               (currentMessage->currentBlock.readFile[j+2] << 16)+ 
                               (currentMessage->currentBlock.readFile[j+3] << 24);
    }
    //put the initial hash into a placeholder in memeory
    a = currentMessage->initialHash[0];
    b = currentMessage->initialHash[1];
    c = currentMessage->initialHash[2];
    d = currentMessage->initialHash[3];

   //this to be optimized into a loop
   //adapted from https://github.com/Souravpunoriyar/md5-in-c
   FF(a,b,c,d,currentMessage->currentBlock.hash[0],  7,0xd76aa478);
   FF(d,a,b,c,currentMessage->currentBlock.hash[1], 12,0xe8c7b756);
   FF(c,d,a,b,currentMessage->currentBlock.hash[2], 17,0x242070db);
   FF(b,c,d,a,currentMessage->currentBlock.hash[3], 22,0xc1bdceee);
   FF(a,b,c,d,currentMessage->currentBlock.hash[4],  7,0xf57c0faf);
   FF(d,a,b,c,currentMessage->currentBlock.hash[5], 12,0x4787c62a);
   FF(c,d,a,b,currentMessage->currentBlock.hash[6], 17,0xa8304613);
   FF(b,c,d,a,currentMessage->currentBlock.hash[7], 22,0xfd469501);
   FF(a,b,c,d,currentMessage->currentBlock.hash[8],  7,0x698098d8);
   FF(d,a,b,c,currentMessage->currentBlock.hash[9], 12,0x8b44f7af);
   FF(c,d,a,b,currentMessage->currentBlock.hash[10],17,0xffff5bb1);
   FF(b,c,d,a,currentMessage->currentBlock.hash[11],22,0x895cd7be);
   FF(a,b,c,d,currentMessage->currentBlock.hash[12], 7,0x6b901122);
   FF(d,a,b,c,currentMessage->currentBlock.hash[13],12,0xfd987193);
   FF(c,d,a,b,currentMessage->currentBlock.hash[14],17,0xa679438e);
   FF(b,c,d,a,currentMessage->currentBlock.hash[15],22,0x49b40821);

   GG(a,b,c,d,currentMessage->currentBlock.hash[1],  5,0xf61e2562);
   GG(d,a,b,c,currentMessage->currentBlock.hash[6],  9,0xc040b340);
   GG(c,d,a,b,currentMessage->currentBlock.hash[11],14,0x265e5a51);
   GG(b,c,d,a,currentMessage->currentBlock.hash[0], 20,0xe9b6c7aa);
   GG(a,b,c,d,currentMessage->currentBlock.hash[5],  5,0xd62f105d);
   GG(d,a,b,c,currentMessage->currentBlock.hash[10], 9,0x02441453);
   GG(c,d,a,b,currentMessage->currentBlock.hash[15],14,0xd8a1e681);
   GG(b,c,d,a,currentMessage->currentBlock.hash[4], 20,0xe7d3fbc8);
   GG(a,b,c,d,currentMessage->currentBlock.hash[9],  5,0x21e1cde6);
   GG(d,a,b,c,currentMessage->currentBlock.hash[14], 9,0xc33707d6);
   GG(c,d,a,b,currentMessage->currentBlock.hash[3], 14,0xf4d50d87);
   GG(b,c,d,a,currentMessage->currentBlock.hash[8], 20,0x455a14ed);
   GG(a,b,c,d,currentMessage->currentBlock.hash[13], 5,0xa9e3e905);
   GG(d,a,b,c,currentMessage->currentBlock.hash[2],  9,0xfcefa3f8);
   GG(c,d,a,b,currentMessage->currentBlock.hash[7], 14,0x676f02d9);
   GG(b,c,d,a,currentMessage->currentBlock.hash[12],20,0x8d2a4c8a);

   HH(a,b,c,d,currentMessage->currentBlock.hash[5],  4,0xfffa3942);
   HH(d,a,b,c,currentMessage->currentBlock.hash[8], 11,0x8771f681);
   HH(c,d,a,b,currentMessage->currentBlock.hash[11],16,0x6d9d6122);
   HH(b,c,d,a,currentMessage->currentBlock.hash[14],23,0xfde5380c);
   HH(a,b,c,d,currentMessage->currentBlock.hash[1],  4,0xa4beea44);
   HH(d,a,b,c,currentMessage->currentBlock.hash[4], 11,0x4bdecfa9);
   HH(c,d,a,b,currentMessage->currentBlock.hash[7], 16,0xf6bb4b60);
   HH(b,c,d,a,currentMessage->currentBlock.hash[10],23,0xbebfbc70);
   HH(a,b,c,d,currentMessage->currentBlock.hash[13], 4,0x289b7ec6);
   HH(d,a,b,c,currentMessage->currentBlock.hash[0], 11,0xeaa127fa);
   HH(c,d,a,b,currentMessage->currentBlock.hash[3], 16,0xd4ef3085);
   HH(b,c,d,a,currentMessage->currentBlock.hash[6], 23,0x04881d05);
   HH(a,b,c,d,currentMessage->currentBlock.hash[9],  4,0xd9d4d039);
   HH(d,a,b,c,currentMessage->currentBlock.hash[12],11,0xe6db99e5);
   HH(c,d,a,b,currentMessage->currentBlock.hash[15],16,0x1fa27cf8);
   HH(b,c,d,a,currentMessage->currentBlock.hash[2], 23,0xc4ac5665);

   II(a,b,c,d,currentMessage->currentBlock.hash[0],  6,0xf4292244);
   II(d,a,b,c,currentMessage->currentBlock.hash[7], 10,0x432aff97);
   II(c,d,a,b,currentMessage->currentBlock.hash[14],15,0xab9423a7);
   II(b,c,d,a,currentMessage->currentBlock.hash[5], 21,0xfc93a039);
   II(a,b,c,d,currentMessage->currentBlock.hash[12], 6,0x655b59c3);
   II(d,a,b,c,currentMessage->currentBlock.hash[3], 10,0x8f0ccc92);
   II(c,d,a,b,currentMessage->currentBlock.hash[10],15,0xffeff47d);
   II(b,c,d,a,currentMessage->currentBlock.hash[1], 21,0x85845dd1);
   II(a,b,c,d,currentMessage->currentBlock.hash[8],  6,0x6fa87e4f);
   II(d,a,b,c,currentMessage->currentBlock.hash[15],10,0xfe2ce6e0);
   II(c,d,a,b,currentMessage->currentBlock.hash[6], 15,0xa3014314);
   II(b,c,d,a,currentMessage->currentBlock.hash[13],21,0x4e0811a1);
   II(a,b,c,d,currentMessage->currentBlock.hash[4],  6,0xf7537e82);
   II(d,a,b,c,currentMessage->currentBlock.hash[11],10,0xbd3af235);
   II(c,d,a,b,currentMessage->currentBlock.hash[2], 15,0x2ad7d2bb);
   II(b,c,d,a,currentMessage->currentBlock.hash[9], 21,0xeb86d391);

  //accumulate hash value
  currentMessage->initialHash[0] += a;
  currentMessage->initialHash[1] += b;
  currentMessage->initialHash[2] += c;
  currentMessage->initialHash[3] += d;
}//end digest loop
}
void initialiseHash(struct messageBlock *currentMessage)
{
  currentMessage->initialHash[0] = 0x67452301;
  currentMessage->initialHash[1] = 0xefcdab89;
  currentMessage->initialHash[2] = 0x98badcfe;
  currentMessage->initialHash[3] = 0x10325476;
}
int main(int argc, char **argv)
{
  //check file selected
  if(argc!=2)
  {
    printf("Please select file. \n");
    return 1;
  }
  //error handling for file
  FILE *hashFile = fopen(argv[1],"rb");
  if(!hashFile)
  {
    printf("error opening file");
    return 1;
  }
  else
  {
    //create the initial block for reading in 512 bits
    //this also contains the initial hashvalue
    struct messageBlock hashedValue;
    //set the value of the initial hash
    initialiseHash(&hashedValue);
    //pass the file and the block 
    tenShpot(hashFile,&hashedValue);
    for(int i = 0; i< 4; i++)
    {
      //display little endian
      //adapted from https://stackoverflow.com/questions/4169424/little-endian-big-endian-problem
      printf("%02x%02x%02x%02x",(hashedValue.initialHash[i]>> 0 )&0x000000ff, 
                                (hashedValue.initialHash[i] >> 8)&0x000000ff, 
                                (hashedValue.initialHash[i] >> 16)&0x000000ff, 
                                (hashedValue.initialHash[i] >> 24)&0x000000ff);
    }
    printf("\n");
  }//end hashing file
  fclose(hashFile);
  return 0; 
}