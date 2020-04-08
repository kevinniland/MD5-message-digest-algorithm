# Calculation of the MD5 hash digest of an input
 _4th year Theory of Algorithms project. An application written in C that takes in a user input/file and calculates the MD5 hash digest of the user input/file._

 ### Repository and README.md Overview
 This repository and accompanying README.md contain the following:
 * [MD5 directory](https://github.com/kevinniland97/MD5-message-digest-algorithm/tree/master/md5) - Contains the main code for the project (md5.c), and pre-compiled executable (does not work on Google Cloud VM, must make a new one on the instance itself), and files that can be read in and hashed.
 * [Practice directory](https://github.com/kevinniland97/MD5-message-digest-algorithm/tree/master/practice) - Contains code for bit operations, file manipulation, and SHA256 code from the various videos our lecturer put up and went through.
 * README.md - Contains all documentation for the project.
 * [Screencast](https://www.youtube.com/watch?v=ghIuf0bnVhw&feature=youtu.be) - Overview of code and Google Cloud VM demo
 
 ### Project Statement
 _You must write a program in the C programming language that calculates the MD5 hash digest of an input. The algorithm is specified in the Request For Comments 1321 document supplied by the Internet Engineering Task Force. The only pre-requisite is that your program performs the algorithm — you are free to decide what input the algorithm should be performed on. I suggest you allow the user to input some free text or a filename via the command line._
 
 ### Developer
* Kevin Niland - G00342279

## MD5 message-digest algorithm overview
From the MD5 wikipedia [1], _"MD5 processes a variable-length message into a fixed-length output of 128 bits. The input message is broken up into chunks of 512-bit blocks (sixteen 32-bit words); the message is padded so that its length is divisible by 512. The padding works as follows: first a single bit, 1, is appended to the end of the message. This is followed by as many zeros as are required to bring the length of the message up to 64 bits fewer than a multiple of 512. The remaining bits are filled up with 64 bits representing the length of the original message, modulo 2^64."_

## MD5 Steps
There are five steps involved in computing the message digest of the message, which this section will discuss.

### Step 1 - Append Padding Bits (Section 3.1 of RFC 1321 document) [2]
The message is "padded" (extended) so that its length (in bits) is congruent to 448, modulo 512. Padding is always performed, even if the length of the message is already congruent to 448, modulo 512. Padding is performed as follows: a single "1" bit is appended to the message, and then "0" bits are appended so that the length in bits of the padded message becomes congruent to 448, modulo 512. In all, at least one bit and at most 512 bits are appended.

### Step 2 - Append Length (Section 3.2 of RFC 1321 document) [2]
A 64-bit representation of b (the length of the message before the padding bits were added) is appended to the result of the previous 
step. In the unlikely event that b is greater than 2^64, then only the low-order 64 bits of b are used. At this point the resulting 
message (after padding with bits and with b) has a length that is an exact multiple of 512 bits. Equivalently, this message has a length 
that is an exact multiple of 16 (32-bit) words. Let M[0 ... N-1] denote the words of the resulting message, where N is a multiple of 16.

### Step 3 - Initialize MD Buffer (Section 3.3 of RFC 1321 document) [2]
A four-word buffer (A, B, C, D) is used to compute the message digest. Here each of A, B, C, D is a 32-bit register. These registers are
initialized to the following values in hexadecimal, low-order bytes first):

|  Word   |  Hex Values  | 
|:-------:|:------------:|
| word A: |  01 23 45 67 | 
| word B: |  89 ab cd ef | 
| word C: |  fe dc ba 98 | 
| word D: |  76 54 32 10 |

### Step 4 - Process Message in 16-Word Blocks (Section 3.4 of RFC 1321 document) [2]
Define four auxiliary functions that each take as input three 32-bit words and produce as output one 32-bit word:

![functions](https://github.com/kevinniland97/MD5-message-digest-algorithm/blob/master/images/functions.PNG)

### Step 5 - Output (Section 3.5 of RFC 1321 document) [2]
The message digest produced as output is A, B, C, D. That is, we begin with the low-order byte of A, and end with the high-order byte of
D.

![output](https://github.com/kevinniland97/MD5-message-digest-algorithm/blob/master/images/output.PNG)

### How to run
* Download the project or clone the repository using `git clone https://github.com/kevinniland97/Calculation-of-the-MD5-hash-digest-of-an-input`.
* Navigate to the project directory and set your working directory to the `md5` directory: `../Calculation-of-the-MD5-hash-digest-of-an-input/md5 $`.
* A compiled version of the program is already provided. If you want to compile the program yourself, use the command `gcc -o md5 .\md5.c`.
* Once compiled, simply enter the name of the compiled program on the command line to run it: `.\md5`.
* Once ran, the user will be presented with a menu. They have the option of specifying a file to hash (starter files are located in the `files` directory), specifying a string to hash, or to exit the program.
   1. To hash a file, choose option 1 and then enter the path to the file (to use one of the files provided, enter `files/name-of-file.txt` when prompted (for example, to hash the file containing the letters of the alphabet, enter `files/alphabet.txt` when prompted).
   2. To hash a string, choose option 2 and enter a string (currently only supports entering one word, not a sentence). This string will 
   then be written to file, at which point it will be automatically hashed and the result will be printed out.
   3. To exit the program, choose option 3.
* Once a file or string is chosen to hash, the output of the hash will be printed to screen. As the starter files contain test suites defined on page 21 of the [MD5 Message-Digest Algorithm memo](https://tools.ietf.org/html/rfc1321), the output can be quickly verified. Alternatively, if the user wishes to specify their own string/message, the output can be verified using the [Online MD5 Hash Generator & SHA1 Hash Generator](http://onlinemd5.com/).

## Research, Project Overview, and Developer Diary
* **Week 1:** When we first received the project spec, I proceeded to research the MD5 message-digest algorithm through various different sources, such as the Request For Comments 1321 document [2] and from watching videos based on it [4], [5]. My initial commits to this repository comprised of code taken from the intital videos put up by our lecturer, Ian McLoughlin, and some simple functionality written in C that would be implemented at a later date, such as reading from a file and taking in a user's input. I also started to set up my own Virtual Machine instance on Google Cloud and refamiliarized myself with VI through it.
* **Week 2:** In this week a video on the SHA Standard was made available to us, which gave us an overview of the SHA specification. In this video, our lecturer went through the Secure Hash Standard document [8] published by the National Institute of Standards and Technology. Further reading included researching and discerning the differences between the SHA and MD5 hash algorithms, from various resources [9], [10], [11]. Some differences include MD5 is faster than SHA, SHA is more secure than MD5, MD5 can have a bit length of 128 while SHA can have a bit length of 160. They also disccused how MD5 is considered broken in the sense that you can generate a collision and thus should not be used for any security applications. SHA is not known to be broken and is believed to be secure. I also researched how each generated a hash of a message.
* **Week 3:** In this week a video on C bit operations was made available to us and I coded up the file from this. I proceeded to research bitwise operators in C to get a better understanding of how each worked [12]. I then re-read the suggested documents on MD5 and SHA [1], [8] due to the fact that my understanding of each would be improved.
* **Week 4:** In this week a video on the functions and constants in SHA was made available to us and I coded up the file from this.
* **Week 5:** In this week a video on padding a message in C was made available to us and I coded up the file from this. For further research and reading, I then looked up the differences between the padding in MD5 and SHA, both from the relative documents on each and from other sources. One major difference would be the fact that when you add the message length (after padding), MD5 uses little-endian but SHA256 uses big-endian. This week is where the main project was properly started. I began with the padding as I coded up that aspect of it in a similar fashion to that of padding shown to us in the padding video.
* **Week 6:** In this week videos on unions in C and merging the padding with hashing was made available to us and I coded up the files from these. I then proceeded to implement a union into my main MD5 file, similar to that shown in the unions in C video.
* **Week 7:** In this week videos on the hash function and finalising the padding with hashing was made available to us and I coded up the files from these.
* **Week 8:** In this week a video on testing and refactoring the entire code done so far was made available to us. From viewing this, it was evident that coding up the SHA specification in this way was beneficial to us as it allowed us to get a firm grasp on each part before moving onto the next part.
* **Week 9 - 11:** Week 9 - 11 was mainly comprised of finsihing off the project and writing up documentation.

## References
* [1] [MD5](https://en.wikipedia.org/wiki/MD5)
* [2] [Request For Comments 1321 document](https://tools.ietf.org/html/rfc1321).
* [3] [Sourav Punoriyar's MD5 implementation in C](https://github.com/Souravpunoriyar/md5-in-c).
* [4] [Network Security - MD5 Algorithm (Sundeep Saradhi Kanthety)](https://www.youtube.com/watch?v=53O9J2J5i14).
* [5] [Message Digest Algorithm: MD5 (DrVikasThada)](https://www.youtube.com/watch?v=-uRpRMpvdm0).
* [6] [Bitwise Operators in C/C++](https://www.geeksforgeeks.org/bitwise-operators-in-c-cpp/).
* [7] [Bitwise operations in C](https://en.wikipedia.org/wiki/Bitwise_operations_in_C).
* [8] [Secure Hash Standard PDF](https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.180-4.pdf)
* [9] [Algorithm Complexity & Security: MD5 or SHA1](https://stackoverflow.com/questions/2948156/algorithm-complexity-security-md5-or-sha1)
* [10] [Difference between MD5 and SHA1](https://www.geeksforgeeks.org/difference-between-md5-and-sha1/)
* [11] [Is SHA1 better than MD5 only because it generates a hash of 160 bits?](https://security.stackexchange.com/questions/19705/is-sha1-better-than-md5-only-because-it-generates-a-hash-of-160-bits)
* [12] [Bitwise Operators in C](https://www.tutorialspoint.com/cprogramming/c_bitwise_operators.htm)
* [13] [What is the purpose of padding an md5 message if it is already the right length?](https://stackoverflow.com/questions/3701550/what-is-the-purpose-of-padding-an-md5-message-if-it-is-already-the-right-length)
* [14] [Is md5's padding the same that sh256?](https://stackoverflow.com/questions/54606597/is-md5s-padding-the-same-that-sh256)
* Several videos done by [Ian McLoughlin](https://github.com/ianmcloughlin). [Repository for videos](https://github.com/ianmcloughlin/sha256).
