# Hash! - BlockchainTechnologies
A repository for a university blockchain technologies course. 
This project implements two hashing algorithms: MatrixHash (a custom algorithm co-created by us)
and VibeHash(an algorithm created using Claude AI).


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
- Collisions sometimes happen with similar inputs (1 char difference)  -> fixed same way and minor improvements

### Benchmarks (HashTests with 100k):
(our comments in the results in square brackets[])

```
Average char similarity: 6.21052%
Min char similarity: 0%
Max char similarity: 43.75%
------------------------
Average bit similarity: 66.2491%
Min bit similarity: 58.2031%
Max bit similarity: 81.6406%
------------------------
Collisions rate for size 10: 0
Collisions rate for size 100: 0
Collisions rate for size 500: 0
Collisions rate for size 1000: 0
[reading konstitucija txt]
Lines: 1 Average time: 6.218e-06
Lines: 2 Average time: 6.39833e-06
Lines: 4 Average time: 7.741e-06
Lines: 8 Average time: 9.247e-06
Lines: 16 Average time: 1.89477e-05
Lines: 32 Average time: 4.12993e-05
Lines: 64 Average time: 7.00223e-05
Lines: 128 Average time: 0.000130538
Lines: 256 Average time: 0.000303046
Lines: 512 Average time: 0.000651051
Full file average time 0.000965815
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
- Fast (significantly faster than MatrixHash)

### Weaknesses of the algorithm:
- i have no clue how it works
- 

### Benchmarks (HashTests with 100k):
```
Average char similarity: 6.1728%
Min char similarity: 0%
Max char similarity: 21.875%
------------------------
Average bit similarity: 66.2901%
Min bit similarity: 56.4453%
Max bit similarity: 76.1719%
------------------------
Collisions rate for size 10: 0
Collisions rate for size 100: 0
Collisions rate for size 500: 0
Collisions rate for size 1000: 0
[konstitucija.txt]
Lines: 1 Average time: 4.399e-06
Lines: 2 Average time: 5.47267e-06
Lines: 4 Average time: 8.71467e-06
Lines: 8 Average time: 1.5012e-05
Lines: 16 Average time: 4.03053e-05
Lines: 32 Average time: 7.40847e-05
Lines: 64 Average time: 0.000149867
Lines: 128 Average time: 0.000354775
Lines: 256 Average time: 0.000733858
Lines: 512 Average time: 0.00151693
Full file average time 0.00219872
```

[//]: # (TODO: setup guide)
SETUP GUIDE (will be nicer later):
(gal vietoj mingw msys2 siulyt whatever)
1. Install g++/mingw (link)
2. Install cmake
3. Clone this repository
4. Open terminal in the repository folder
5. `cmake -B build -S .; cmake --build build`
6. Go to the bin folder and run!


SETUP GUIDE for Windows users
1. clone this repository
2. Install msys2 compiler from https://www.msys2.org/
3. After setting up open the msys2 mingw64 bash terminal (not the plain msys2)
4. navigate to the project folder where repository is cloned
5. run this command: pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-cmake
6. run this command: pacman -S mingw-w64-x86_64-gcc
7. run this command `cmake -B build -S .; cmake --build build`
8. navigate to /bin directory
9. run ./blockchaintechnologies
