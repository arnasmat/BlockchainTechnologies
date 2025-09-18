# Hash! - BlockchainTechnologies
A repository for a university blockchain technologies course.
This project implements two hashing algorithms: MatrixHash (a custom algorithm created by us)
and VibeHash(an algorithm created using Claude AI).

[Click here for the setup guide](#setup-guides)

## MatrixHash
MatrixHash is a hashing algorithm created by us. The main idea was using matrix multiplication to create a hash,
however after a lot of changes, the final algorithm is quite different from the original idea to be more efficient.
Nonetheless, the name related to matrix multiplication has remained. (see old versions of readmes to see the original idea)

### Strenghts of the algorithm:
- Deterministic
- Output is always the same size (64 hex chars)
- Avalanche effect (average hex-wise similarity between hashes of inputs with a 1 char difference char is ~7%)
- Low collision rate
- Faster than VibeHash

### Weaknesses of the algorithm:
- Slower than SHA256 and MD5


### Benchmarks (HashTests with 100k):
(our comments in the results in square brackets[])

```
Running all tests with MatrixHash:
Output size test successful
Running avalanche effect test
Average char similarity: 6.20275%
Min char similarity: 0%
Max char similarity: 35.9375%
------------------------
Average bit similarity: 66.2878%
Min bit similarity: 57.0312%
Max bit similarity: 78.7109%
-----------------------
Searching for collisions by generating pairs of random strings
Collisions rate for size 10: 0
Collisions rate for size 100: 0
Collisions rate for size 500: 0
Collisions rate for size 1000: 0
Searching for collisions by generating many random strings and putting them in a set
Running efficiency test with file "../data/input/test/konstitucija.txt"
Opening file "../data/input/test/konstitucija.txt"
Lines: 1 Average time: 1.09463e-05
Lines: 2 Average time: 1.0955e-05
Lines: 4 Average time: 1.10037e-05
Lines: 8 Average time: 1.43527e-05
Lines: 16 Average time: 2.81997e-05
Lines: 32 Average time: 4.4375e-05
Lines: 64 Average time: 8.8412e-05
Lines: 128 Average time: 0.000228676
Lines: 256 Average time: 0.000488885
Lines: 512 Average time: 0.00109383
Full file average time 0.0016808
Running salting test with input Man I love hash functions
Input: Man I love hash functions Salt:  ihn o
Hash without salt: 5bbdfd0a4126bcb3eb98e31ad83d03da67ba2f76d80e648274c598acef47a03b
Hash with salt: 59c114d327749323e7a111f32ce080f73d0b43005a4158284537f56ff0166fdf
```

konsitucija.txt test times graph
<img width="1680" height="920" alt="image" src="https://github.com/user-attachments/assets/532271ab-7ef1-4605-89d3-311ed883ecc3" />
It can be observed, that when input gets exponentially larger, hash takes exponentially more time. Thus it proves that our hash's time complexity is O(n)=n (linear)

## VibeHash
VibeHash is a hashing algorithm created with Claude AI.

- We will not go in depth explaining the algorithm, but you can see the [chats here](https://claude.ai/share/1ce99725-f78d-4212-9ac7-555b50029023) (the issues we faced were later fixed by us)

### Strenghts of the algorithm:
- Deterministic
- Output is always the same size (64 hex chars)
- Avalanche effect (average hex-wise similarity between hashes of inputs with a 1 char difference char
- Low collision rate (no found in the tests done)

### Weaknesses of the algorithm:
- Slower than MatrixHash, SHA256 and MD5
- i have no clue how it works

### Benchmarks (HashTests with 100k):
```
Running all tests with VibeHash:
Output size test successful
Running avalanche effect test
Average char similarity: 6.17486%
Min char similarity: 0%
Max char similarity: 23.4375%
------------------------
Average bit similarity: 66.2462%
Min bit similarity: 57.4219%
Max bit similarity: 77.1484%
------------------------
Searching for collisions by generating pairs of random strings
Collisions rate for size 10: 0
Collisions rate for size 100: 0
Collisions rate for size 500: 0
Collisions rate for size 1000: 0
Searching for collisions by generating many random strings and putting them in a set
Running efficiency test with file "../data/input/test/konstitucija.txt"
Opening file "../data/input/test/konstitucija.txt"
Lines: 1 Average time: 6.11233e-06
Lines: 2 Average time: 8.57667e-06
Lines: 4 Average time: 1.37933e-05
Lines: 8 Average time: 2.55953e-05
Lines: 16 Average time: 6.39753e-05
Lines: 32 Average time: 0.000120345
Lines: 64 Average time: 0.000235874
Lines: 128 Average time: 0.000543159
Lines: 256 Average time: 0.00138955
Lines: 512 Average time: 0.00264727
Full file average time 0.00426793
Running salting test with input Man I love hash functions
Input: Man I love hash functions Salt: ntlsa
Hash without salt: d7abcf6e7194821519d039f98571090b99e7016807adad99e4cdb1a895e621f9
Hash with salt: ebfd0746718aadaf3ff780939dce414e36e16b2043bcbe640e9698dcf71826a2
```
konsitucija.txt test times graph
<img width="1225" height="697" alt="image" src="https://github.com/user-attachments/assets/e87a9000-5056-4005-b6c8-53be500b0f38" />
We can observe that while vibe hash's performance was a bit slower than our hash's, it still exhibits linear time complexion.

[//]: # (TODO: SHA256 and MD5)

## SHA256 and MD5
SHA256 and MD5 are widely used hashing functions. Famously, SHA256 is used as a proof of work in bitcoin's blockhain.

### Comparison of SHA256 and MD5 performance with respect to our algorithms
<img width="1392" height="987" alt="image" src="https://github.com/user-attachments/assets/0fc3f96e-f59c-4186-8dce-10e7f5ce5301" />


# Setup guides
Note: if you plan on testing the application (-t), please put [konstitucija.txt](https://bit.ly/33nYy2v) in the data/input/test directory

Linux setup guide:
1. Install git and [clone this repository](https://docs.github.com/en/repositories/creating-and-managing-repositories/cloning-a-repository)
2. Install g++, CMake and OpenSSL (may have to google the name, on some platforms it's called libssl-dev)[1] using your package manager, e.g. for arch linux `sudo pacman -S gcc cmake openssl`
3. Open your terminal emulator in the repository folder
4. Build the project `cmake -B build -S .; cmake --build build` (if you get errors, you may need to specify -G "Unix Makefiles")
5. Navigate to /bin/ directory
6. Run ./BlockchainTechnologies. You may need to use the -h flag to see the usage instructions.

SETUP GUIDE for Windows users
1. Install git and [clone this repository](https://docs.github.com/en/repositories/creating-and-managing-repositories/cloning-a-repository)
2. Install the [MSYS2 environment](https://www.msys2.org/)
3. After setting up open the MSYS2 mingw64 bash terminal (not the plain msys2)
4. navigate to the project folder where this repository is cloned
5. Install the g++ compiler, cmake and OpenSSL[1] for the environment: `pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-cmake mingw-w64-x86_64-openssl`
6. Build the project `cmake -B build -S .; cmake --build build`
7. Navigate to /bin/ directory
8. Run ./BlockchainTechnologies. You may need to use the -h flag to see the usage instructions.

[1]: OpenSSL is only used for testing our algorithms with other popular hashing algorithms(sha256 and md5).
If you don't want to install it - comment out everything in OtherAlgorithms.h, all lines related to sha256 and md5 in CliArgHandler and find_package(OpenSSL ...) as well as target_link_libraries (... OpenSSL ...) in CMakeLists.txt.
Then the application should build without having to install OpenSSL.
