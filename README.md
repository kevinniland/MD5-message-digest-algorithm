# Calculation of the MD5 hash digest of an input
 _4th year Theory of Algorithms project. An application written in C that takes in a user input/file and calculates the MD5 hash digest of the user input/file._

 ### Project Statement
 _You must write a program in the C programming language that calculates the MD5 hash digest of an input. The algorithm is specified in the Request For Comments 1321 document supplied by the Internet Engineering Task Force. The only pre-requisite is that your program performs the algorithm — you are free to decide what input the algorithm should be performed on. I suggest you allow the user to input some free text or a filename via the command line._
 
  ### Developer
* Kevin Niland - G00342279

## MD5 message-digest algorithm overview
From the MD5 wikipedia [1], _"MD5 processes a variable-length message into a fixed-length output of 128 bits. The input message is broken up into chunks of 512-bit blocks (sixteen 32-bit words); the message is padded so that its length is divisible by 512. The padding works as follows: first a single bit, 1, is appended to the end of the message. This is followed by as many zeros as are required to bring the length of the message up to 64 bits fewer than a multiple of 512. The remaining bits are filled up with 64 bits representing the length of the original message, modulo 2^64."_

### How to run
* Download the project or clone the repository using `git clone https://github.com/kevinniland97/Calculation-of-the-MD5-hash-digest-of-an-input`.
* Navigate to the project directory and set your working directory to the `md5` directory: `../Calculation-of-the-MD5-hash-digest-of-an-input/md5 $`.
* A compiled version of the program is already provided. If you want to compile the program yourself, use the command `gcc -o md5 .\md5.c`.
* Once compiled, simply enter the name of the compiled program on the command line to run it: `.\md5`.
* Once ran, the user will be presented with a menu. They have the option of specifying a file to hash (starter files are located in the `files` directory), specifying a string to hash, or to exit the program.
   1. To hash a file, choose option 1 and then enter the path to the file (to use one of the files provided, enter `files/name-of-file.txt` when prompted (for example, to hash the file containing the letters of the alphabet, enter `files/alphabet.txt` when prompted).
   2. To hash a string, choose option 2 and ...
   3. To exit the program, choose option 3.
* Once a file or string is chosen to hash, the output of the hash will be printed to screen. As the starter files contain test suites defined on page 21 of the [MD5 Message-Digest Algorithm memo](https://tools.ietf.org/html/rfc1321), the output can be quickly verified. Alternatively, if the user wishes to specify their own string/message, the output can be verified using the [Online MD5 Hash Generator & SHA1 Hash Generator](http://onlinemd5.com/).

## Research, Project Overview, and Developer Diary
* **Week 1:** When we first received the project spec, I proceeded to research the MD5 message-digest algorithm through various different sources, such as the Request For Comments 1321 document [2] and from watching videos based on it [4], [5]. My initial commits to this repository comprised of code taken from the intital videos put up by our lecturer, Ian McLoughlin, and some simple functionality written in C that would be implemented at a later date, such as reading from a file and taking in a user's input. I also started to set up my own Virtual Machine instance on Google Cloud and refamiliarized myself with VI through it.
* **Week 2:** In this week a video on the SHA Standard was made available to us, which gave us an overview of the SHA specification. In this video, our lecturer went through the Secure Hash Standard document [8] published by the National Institute of Standards and Technology. Further reading included researching and discerning the differences between the SHA and MD5 hash algorithms, from various resources [9], [10], [11]. Some differences include MD5 is faster than SHA, SHA is more secure than MD5, MD5 can have a bit length of 128 while SHA can have a bit length of 160. They also disccused how MD5 is considered broken in the sense that you can generate a collision and thus should not be used for any security applications. SHA is not known to be broken and is believed to be secure. I also researched how each generated a hash of a message.
* **Week 3:** In this week a video on C bit operations was made available to us and coded up the file from this. I proceeded to research bitwise operators in C to get a better understanding of how each worked [12]. I then re-read the suggested documents on MD5 and SHA [1], [8] due to the fact that my understanding of each would be improved.
* **Week 4:** In this week a video on the functions and constants in SHA was made available to us.
* **Week 5:** In this week a video on padding a message in C was made available to us.
* **Week 6:** In this week a video on unions in C was made available to us.
* **Week 7:** 
* **Week 8:** 
* **Week 9 - 11:** Week 9 - 11 was mainly comprised of finsihing off the project and writing up documentation.

Week 4-5: In the fourth week, I looked at the video on functions and constants in SHA and followed along with the coding recommended in the video. I also read over the paragraph in the SHA documentation in section 4.1.2 on functions to help understand it. [3]. In the fifth week, I looked at the padding video on the SHA padding and how it works, as well as reading into the padding in SHA. I also did some research as to how the padding works in MD5 by reading the sections on padding in the RFC document about MD5 in section 3.1 and 3.2. [1].

Week 5-7: In week 5, I watched the video on unions that was put up on Moodle and coded up the unions file. After watching this, I then went and looked up the comparisons of a union vs a struct as I was already familiar with these. [8]. The sixth week involved watching the merging of padding with the hashing, I then carried out this in my own code. In the seventh week, I watched both the hash function video and finalising the padding videos and again followed along in my own repository.

Week 8: In week 8, I followed the video on testing and refactoring the code and carried this out in my project. I then looked back at the code I had previously written and looked at the comparisons to see the scale of changes that the refactoring of the code had done to the project.

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
<!-- * []() -->
* Several videos done by [Ian McLoughlin](https://github.com/ianmcloughlin). [Repository for videos](https://github.com/ianmcloughlin/sha256).
