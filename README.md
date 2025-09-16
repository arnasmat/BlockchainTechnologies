# Hash! - BlockchainTechnologies
A repository for a university blockchain technologies course. 
This project implements two hashing algorithms: MatrixHash (a custom algorithm co-created by us)
and VibeHash(an algorithm created using Claude AI).

[Click here for the setup guide](#setup-guides)

[//]: # (TODO: fix readme hash description to reflect better after improvements)
## MatrixHash
The MatrixHash algorithm is a three-part algorithm (after improvements the 3 parts are not as clear in the code):
1. Firstly the algorithm:
- Converts the input string into a binary representation 
- Stores the representation in a temporary array (henceforth referred to as *k*)
- Tracks how many 1s were in the binary representation and initializes the hashArray
(which later becomes the resulting hash) with how many ones there were thus far to introduce randomness.
2. Then it does something similar to matrix multiplying *k* (size: 1x|k|) with a matrix of size |k|x64 
resulting in a matrix of size 1x64 (here: hashArray of size 64). It does so by creating a double for loop of sizes 64(i) and |k|(j)
and adding i+j multiplied by the value of k[j]. The result is stored in hashArray[i].
3. Finally, it does some final scrambling of hashArray by iterating through it as a binary representation and adding 1 to the original array (not binary)
if the value is 1 in the binary representation and adding the value of the previous character if the value is 0 in the binary representation.
Then the result is converted to hexadecimal and returned.
   (note: pt3 is also a hashing algorithm on its own (HumanHash), but it's more useful here than being standalone)

### Strenghts of the algorithm:
- Deterministic
- Output is always the same size (64 hex chars)
- Avalanche effect (average hex-wise similarity between hashes of inputs with a 1 char difference char is ~7%)
- Low collision rate

### Weaknesses of the algorithm:
- Efficiency (could be faster) -> fixed by using bitwise operations instead of bitset<8> and casting to string
- Collisions sometimes happen with similar inputs (1 char difference)  -> fixed with some minor tweaks

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

[//]: # (TODO: GRAPH FOR KONSTITUCIJA)

## VibeHash
VibeHash is a hashing algorithm created by Claude AI.

- We will not go in depth explaining the algorithm, but you can see the [chats here](https://claude.ai/share/1ce99725-f78d-4212-9ac7-555b50029023) (the issues were later fixed by us)

### Strenghts of the algorithm:
- Deterministic
- Output is always the same size (64 hex chars)
- Avalanche effect (average hex-wise similarity between hashes of inputs with a 1 char difference char
- Low collision rate (no found in the tests done)
- Fast

### Weaknesses of the algorithm:
- i have no clue how it works
- 

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

[//]: # (TODO: setup guide)
# Setup guides
Note: if you plan on testing the application (-t), please put [konstitucija.txt](https://bit.ly/33nYy2v) in the data/input/test directory

Linux setup guide:
1. Install git and [clone this repository](https://docs.github.com/en/repositories/creating-and-managing-repositories/cloning-a-repository)
2. Install g++ and CMake using your package manager, e.g. for arch linux `sudo pacman -S gcc cmake`
3. Open your terminal emulator in the repository folder
4. Build the project `cmake -B build -S .; cmake --build build` (if you get errors, you may need to specify -G "Unix Makefiles")
5. Navigate to /bin/ directory
6. Run ./BlockchainTechnologies. You may need to use the -h flag to see the usage instructions.

SETUP GUIDE for Windows users
1. Install git and [clone this repository](https://docs.github.com/en/repositories/creating-and-managing-repositories/cloning-a-repository)
2. Install the [MSYS2 environment](https://www.msys2.org/)
3. After setting up open the MSYS2 mingw64 bash terminal (not the plain msys2)
4. navigate to the project folder where this repository is cloned
5. Install the g++ compiler and cmake for the environment: `pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-cmake`
6. Build the project `cmake -B build -S .; cmake --build build`
7. Navigate to /bin/ directory
8. Run ./BlockchainTechnologies. You may need to use the -h flag to see the usage instructions.
