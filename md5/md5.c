// Kevin Niland
// MD5 message-digest algorithm implementation

#define uchar unsigned char
#define uint unsigned int

#define ROTLEFT(a, b) ((a << b) | (a >> (32 - b)))