# Blockchain and Hash! - BlockchainTechnologies

A repository for a university blockchain technologies course.
This project implements:

- A blockchain network simulation with parallelized mining and UTXO functionality.
- two hashing algorithms: MatrixHash (a custom algorithm created by us)
  and VibeHash(an algorithm created using Claude AI).

[Click here for the setup guide](#setup-guides)

# Blockchain

## Overview

The blockchain implementation simulates a blockchain network with the following features:

- Blockchain blocks that are created with a proof-of-work mechanism with self-adjusting difficulty.
- Merkle tree for transaction verification within blocks
- UTXO (Unspent Transaction Output) model for transactions
- User wallets with public and private keys (ids). Some of the users are also miners.
- Parallelized mining with OpenMP
- All of these use our custom hashing algorithm (currently set to MatrixHash, but can easily be changed in
  SystemAlgorithm.h)

## Showcase

This part explains the current setup in the main.cpp code. All of the functionality can easily be tweaked by the user.

Initial mining - at first the blockchain is empty, so miners have to create transactions by mining 50 blocks (including
genesis block).

<img width="1228" alt="image" src="https://github.com/user-attachments/assets/92252066-eee1-4018-9a99-7e555450dfb8" />


Afterwards 100000 transactions are created - note that only valid ones are added to the blockchain, while invalid ones
are not.

Then the blockchain is continued indefinitely until user interrupts the program (Ctrl + C) adding the generated
transactions to blocks.

<img width="1637" alt="image" src="https://github.com/user-attachments/assets/e244c0e3-3caf-41a8-be3a-88cbcdac4d91" />

## Unique solutions

- Parallelized mining was done using OpenMP. We felt it was the most appropriate and safest way to implement
  multithreading for our project.
- Certain "managers" use a singleton pattern to ensure only one instance exists. E.g. UTXOSystem, TransactionQueue,
  HeadBlock are done this way
- Difficulty is automatically adjusted every 20 blocks based on the time taken to mine the previous 20 blocks, therefore
  it auto adjusts to the speed of the system (simulated miners on the blockchain simulation)

## Instructions

[Click here for the setup guide](#setup-guides)

To work with the application, edit the main.cpp file as you desire. You can alter the numbers, change flow order or
functionality. The current main.cpp file is an example of what can be done, but users are encouraged to experiment and
alter it to their desires.

Afterwards, build and run the application. Note that two files are generated in the bin/ directory: hashCli and
blockchain. HashCLI is used to test out the hashing algorithms, while blockchain is used to run the blockchain
simulation.

There is also an unfinished CLI application for the blockchain simulation, however reading from files is not finished.
If you'd like to try it out, see `feature/blockchain-cli` branch.

## AI Assistance

AI was primarily used for assistance and help understanding certain functionality such as UTXOs in bitcoin, not for code
generation.
NOTE: In the CLI branch AI was used for blockchain reading implementation. As you can notice, reading isn't done. AI has only caused harm and made the codebase terrible, unusable, ugly and not functional lol

---

# Hashing algorithms

## MatrixHash

MatrixHash is a hashing algorithm created by us. The main idea was using matrix multiplication to create a hash,
however after a lot of changes, the final algorithm is quite different from the original idea to be more efficient.
Nonetheless, the name related to matrix multiplication has remained. (see old versions of readmes to see the original
idea)

Detailed description of algorithm:
We initialize an integer array of size 64 and a oneSum with a starting value of 0
We iterate through each input's char symbol, converting it to an bit representation (e.g. if we encounter 'a' symbol we
would convert it to 01100001)
We iterate through each bit of this bit representation (0 or 1). We also calculate the index, which is calculated by (
char's index * 8 + bit's index) mod 64; this way index will stay between 0 and 63, no matter the input's length. This
index will be used to add different values to the corresponding hash array element. We will also keep track of the
amount of 1s in the bit representation of input using the oneSum variable.
If the bit that we were iterating on was 1, we add 1 to the hash[index]. Else, If that bit was 0 and the current hash
index is more than 0, we perform hash[index] += hash[index - 1]. Finally, if index is 0, we perform this operation:
hash[0] += hash[input.size % 64] + oneSum. No matter the scenario, then we will add to the corresponding hash element
bit value multiplied by the sum of current char's index, current bit's index and oneSum mod 64.
After finishing iterating through input, we start to iterate through each element of the hash array itself. Then we
while iterating through its bits we apply same processes as defined previously, except the last step. Finally we convert
each element inside the hash array into hexadecimal by applying modulo 16 to it and choosing corresponding hexadecimal
symbol.

### Strenghts of the algorithm:

- Deterministic
- Output is always the same size (64 hex chars)
- Avalanche effect (average hex-wise similarity between hashes of inputs with a 1 char difference char is ~7%)
- Low collision rate
- Faster than VibeHash

### Weaknesses of the algorithm:

- Slower than SHA256 and MD5

### Benchmarks (HashTests with 100k):

[SEE FULL BENCHMARKS OF ALL 3 HASHING ALGORITHMS + SHA256 AND MD5](https://pastebin.com/YHC5JTiN)

```
Running all tests with MatrixHash:
Output size test successful
Running avalanche effect test
Average char similarity: 6.1767%
Min char similarity: 0%
Max char similarity: 29.6875%
------------------------
Average bit similarity: 49.4459%
Min bit similarity: 35.1562%
Max bit similarity: 66.0156%
------------------------
Searching for collisions by generating pairs of random strings
Collisions rate for size 10: 0
Collisions rate for size 100: 0
Collisions rate for size 500: 0
Collisions rate for size 1000: 0
Searching for collisions by generating many random strings and putting them in a set
Running efficiency test with file "../data/input/test/konstitucija.txt"
Opening file "../data/input/test/konstitucija.txt"
Lines: 1 Average time: 1.3592e-05
Lines: 2 Average time: 1.07767e-05
Lines: 4 Average time: 1.16377e-05
Lines: 8 Average time: 1.54693e-05
Lines: 16 Average time: 3.06493e-05
Lines: 32 Average time: 4.13803e-05
Lines: 64 Average time: 7.93643e-05
Lines: 128 Average time: 0.000220387
Lines: 256 Average time: 0.000495177
Lines: 512 Average time: 0.00110799
Full file average time 0.00173936
Running salting test with input Man I love hash functions
Input: Man I love hash functions Salt: M}Hnv6&~K;~2w&S
Hash without salt: 5bbdfd0a4126bcb3eb98e31ad83d03da67ba2f76d80e648274c598acef47a03b
Hash with salt: 1a849731a89263dc6dbc147863034f56daca809ee8fbf663740039ea38ce054b
```

konsitucija.txt test times graph
<img width="1680" height="920" alt="image" src="https://github.com/user-attachments/assets/532271ab-7ef1-4605-89d3-311ed883ecc3" />
It can be observed, that when input gets exponentially larger, hash takes exponentially more time. Thus it proves that
our hash's time complexity is O(n)=n (linear)

## VibeHash

VibeHash is a hashing algorithm created with Claude AI.

- We will not go in depth explaining the algorithm, but you can see
  the [chats here](https://claude.ai/share/1ce99725-f78d-4212-9ac7-555b50029023) (the issues we faced were later fixed
  by us)

### Strenghts of the algorithm:

- Deterministic
- Output is always the same size (64 hex chars)
- Avalanche effect (average hex-wise similarity between hashes of inputs with a 1 char difference char
- Low collision rate (no found in the tests done)

### Weaknesses of the algorithm:

- Slower than MatrixHash, SHA256 and MD5
- i have no clue how it works

### Benchmarks (HashTests with 100k):

[SEE FULL BENCHMARKS OF ALL 3 HASHING ALGORITHMS + SHA256 AND MD5](https://pastebin.com/YHC5JTiN)

```
Running all tests with VibeHash:
Output size test successful
Running avalanche effect test
Average char similarity: 6.17091%
Min char similarity: 0%
Max char similarity: 23.4375%
------------------------
Average bit similarity: 49.4469%
Min bit similarity: 37.1094%
Max bit similarity: 66.0156%
------------------------
Searching for collisions by generating pairs of random strings
Collisions rate for size 10: 0
Collisions rate for size 100: 0
Collisions rate for size 500: 0
Collisions rate for size 1000: 0
Searching for collisions by generating many random strings and putting them in a set
Running efficiency test with file "../data/input/test/konstitucija.txt"
Opening file "../data/input/test/konstitucija.txt"
Lines: 1 Average time: 6.55267e-06
Lines: 2 Average time: 9.137e-06
Lines: 4 Average time: 1.46293e-05
Lines: 8 Average time: 2.52257e-05
Lines: 16 Average time: 6.96073e-05
Lines: 32 Average time: 0.000117317
Lines: 64 Average time: 0.000235619
Lines: 128 Average time: 0.000557033
Lines: 256 Average time: 0.00115018
Lines: 512 Average time: 0.00237113
Full file average time 0.00370043
Running salting test with input Man I love hash functions
Input: Man I love hash functions Salt: Zlz<ce']}_0aB
Hash without salt: d7abcf6e7194821519d039f98571090b99e7016807adad99e4cdb1a895e621f9
Hash with salt: fff34ac3a26f86c8b64a75f7a4cf841a72ebc26e2910e51cdeda51ed7a5bf419
```

konsitucija.txt test times graph
<img width="1225" height="697" alt="image" src="https://github.com/user-attachments/assets/e87a9000-5056-4005-b6c8-53be500b0f38" />
We can observe that while vibe hash's performance was a bit slower than our hash's, it still exhibits linear time
complexion.

[//]: # (TODO: SHA256 and MD5)

## SHA256 and MD5

SHA256 and MD5 are widely used hashing functions. Famously, SHA256 is used as a proof of work in bitcoin's blockhain.

### Comparison of SHA256 and MD5 performance with respect to our algorithms

[SEE FULL BENCHMARKS OF ALL 3 HASHING ALGORITHMS + SHA256 AND MD5](https://pastebin.com/YHC5JTiN)
<img width="1392" height="987" alt="image" src="https://github.com/user-attachments/assets/0fc3f96e-f59c-4186-8dce-10e7f5ce5301" />

# Setup guides

Note: if you plan on testing the application (-t), please put [konstitucija.txt](https://bit.ly/33nYy2v) in the
data/input/test directory

Linux setup guide:

1. Install git
   and [clone this repository](https://docs.github.com/en/repositories/creating-and-managing-repositories/cloning-a-repository)
2. Install g++, CMake and OpenSSL (may have to google the name, on some platforms it's called libssl-dev)[1] using your
   package manager, e.g. for arch linux `sudo pacman -S gcc cmake openssl openmp`
3. Open your terminal emulator in the repository folder
4. Build the project `cmake -B build -S .; cmake --build build` (if you get errors, you may need to specify -G "Unix
   Makefiles")
5. Navigate to /bin/ directory
6. Run ./BlockchainTechnologies. You may need to use the -h flag to see the usage instructions.

SETUP GUIDE for Windows users

1. Install git
   and [clone this repository](https://docs.github.com/en/repositories/creating-and-managing-repositories/cloning-a-repository)
2. Install the [MSYS2 environment](https://www.msys2.org/)
3. After setting up open the MSYS2 mingw64 bash terminal (not the plain msys2)
4. navigate to the project folder where this repository is cloned
5. Install the g++ compiler, cmake and OpenSSL[1] for the environment:
   `pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-cmake mingw-w64-x86_64-openssl mingw-w64-x86_64-openmp`
6. Build the project `cmake -B build -S .; cmake --build build`
7. Navigate to /bin/ directory
8. Run ./BlockchainTechnologies. You may need to use the -h flag to see the usage instructions.

[1]: OpenSSL is only used for testing our algorithms with other popular hashing algorithms(sha256 and md5).
If you don't want to install it - comment out everything in OtherAlgorithms.h, all lines related to sha256 and md5 in
CliArgHandler and find_package(OpenSSL ...) as well as target_link_libraries (... OpenSSL ...) in CMakeLists.txt.
Then the application should build without having to install OpenSSL.
