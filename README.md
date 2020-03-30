# Calculation of the MD5 hash digest of an input
 _4th year Theory of Algorithms project. An application written in C that takes in a user input/file and calculates the MD5 hash digest of the user input/file._

 ### Project Statement
 _You must write a program in the C programming language that calculates the MD5 hash digest of an input. The algorithm is specified in the Request For Comments 1321 document supplied by the Internet Engineering Task Force. The only pre-requisite is that your program performs the algorithm — you are free to decide what input the algorithm should be performed on. I suggest you allow the user to input some free text or a filename via the command line._
 
  ### Developer
* Kevin Niland
* G00342279

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

## Project Overview and Developer Diary
* **Week 1:** 
* **Week 2:** 
* **Week 3:** 
* **Week 4:** 
* **Week 5:** 
* **Week 6:** 
* **Week 7:** 
* **Week 8:** 
* **Week 9:** 
* **Week 10:** 
* **Week 11:** 

## References
* [Request For Comments 1321 document](https://tools.ietf.org/html/rfc1321).
* [Sourav Punoriyar's MD5 implementation in C](https://github.com/Souravpunoriyar/md5-in-c).
* [Network Security - MD5 Algorithm (Sundeep Saradhi Kanthety)](https://www.youtube.com/watch?v=53O9J2J5i14).
* [Message Digest Algorithm: MD5 (DrVikasThada)](https://www.youtube.com/watch?v=-uRpRMpvdm0).
* [Bitwise Operators in C/C++](https://www.geeksforgeeks.org/bitwise-operators-in-c-cpp/).
* [Bitwise operations in C](https://en.wikipedia.org/wiki/Bitwise_operations_in_C).
<!-- * []() -->
* Several videos done by [Ian McLoughlin](https://github.com/ianmcloughlin). [Repository for videos](https://github.com/ianmcloughlin/sha256).
