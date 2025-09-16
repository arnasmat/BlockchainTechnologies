#include "../include/HashTests.h"

// TODO: test if these tests work lmao
// TODO: Improve test naming lmao

namespace HashTests {
    // TODO: change to 100k
    const int totalTests{100000};
    // const int totalTests{100};
    // Patikrink išvedimo dydį – nepriklausomai nuo įvedimo, rezultatas visada tokio pat ilgio.
    // Patikrink deterministiškumą – tas pats failas duoda tą patį hash’ą.
    // TODO: Išmatuok efektyvumą: -> liko padaryt graph
    // Kolizijų paieška
    // Lavinos efektas
    // TODO: Negrįžtamumo demonstracija (hiding, puzzle-friendliness) - HASH(input + salt)

    // This test uses properties of sets (see determinismTest)
    // also tests determinism with the input of files
    void runAllTests(const HashGenInterface *hashGen) {
        outputSizeTest(hashGen);
        avalancheEffect(hashGen);
        collisionSearchPairs(hashGen);
        collisionSearchSets(hashGen);
        determinismTest(hashGen, "bazinga!");
        // TODO: pridet konstitucija.txt prie buildo
        efficiencyTest(hashGen, "../data/input/test/konstitucija.txt");
    }

    void outputSizeTest(const HashGenInterface *hashgen) {
        std::filesystem::path testDir{"../data/input/test/"};
        std::set<size_t> outputSize{};
        for (const auto &file: std::filesystem::recursive_directory_iterator(testDir)) {
            std::ifstream in(file.path());
            std::ostringstream ss;
            ss << in.rdbuf();
            std::string content = ss.str();

            determinismTest(hashgen, content);
            std::string hash{hashgen->generateHash(content)};
            outputSize.insert(hash.size());
            in.close();
        }
        if (outputSize.size() != 1) {
            std::cout << "Output size test failed\nFound output sizes:";
            for (auto i: outputSize) {
                std::cout << i << " ";
            }
        }
    }

    // The logic behind this test relies on the way sets work. If different hashes are inputted into the set,
    // the size of the set will increase. Therefore if it's not equal to 1 after 100 times of doing it -> it fails,
    // otherwise, our algorithm is deterministic.
    void determinismTest(const HashGenInterface *hashGen, const std::string &input) {
        std::set<std::string> hashSet{};
        for (int i = 0; i < 10; i++) {
            std::string hash1{hashGen->generateHash(input)};
            hashSet.insert(hash1);
        }
        if (hashSet.size() != 1) {
            std::cout << "Determinism test failed with input " << input << "\n";
        }
    }

    void efficiencyTest(const HashGenInterface *hashGen, const std::filesystem::path &inputFile) {
        std::ifstream in(inputFile);

        if (!in) {
            std::cerr << "Error: Could not open file " << inputFile <<"\n";
            return;
        }

        // std::ostringstream ss{};
        // ss << in.rdbuf();
        // in.close();
        std::string input{};
        std::string line{};

        int lineCount{0};
        //inefficient way of reading the file probably, but whatever. will fix later lmao

        std::cout<<"Opening file "<<inputFile<<"\n";

        while (std::getline(in, line)) {
            input += line + "\n";
            lineCount++;
            // https://stackoverflow.com/a/108360
            // quite cool way of checking if linecount is a power of 2
            if (lineCount > 0 && (lineCount & (lineCount - 1)) == 0) {
                std::cout << "Lines: " << lineCount << " Average time: " << effTestHelper(hashGen, input) << "\n";
                // std::cout<<input<<"\n\n\n";
            }
        }
        std::cout << "Full file average time " << effTestHelper(hashGen, input) << "\n";
        in.close();
    }

    double effTestHelper(const HashGenInterface *hashGen, const std::string &input) {
        std::chrono::duration<double> totalDuration{};
        int testCount{3};
        for (int i = 0; i < testCount; i++) {
            const auto start = std::chrono::high_resolution_clock::now();
            std::string hash{hashGen->generateHash(input)};
            const auto end = std::chrono::high_resolution_clock::now();
            const std::chrono::duration<double> duration = end - start;
            totalDuration += duration;
        }

        return totalDuration.count() / testCount;
    }

    void collisionSearchPairs(const HashGenInterface *hashGen) {
        constexpr int pairStringLength[4] = {10, 100, 500, 1000};
        const std::string validSymbols{
            "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789@#$%^&*()_+-=[]{}|;:',.<>/?`~\"\\"
        };
        static std::mt19937 rng(std::random_device{}());

        std::vector<std::pair<std::string, std::string> > collisions{};
        for (int i: pairStringLength) {
            for (int j = 0; j < totalTests; j++) {
                std::string input1{generateRandomString(i, validSymbols, rng)};
                std::string input2{generateRandomString(i, validSymbols, rng)};

                if (input1 == input2) {
                    j--;
                    continue;
                }

                std::string hash1{hashGen->generateHash(input1)};
                std::string hash2{hashGen->generateHash(input2)};
                if (hash1 == hash2) {
                    collisions.emplace_back(input1, input2);
                    std::cout << "Collision found for length " << i << ": " << input1 << " and " << input2 << "\n";
                }
            }
            std::cout << "Collisions rate for size " << i << ": " << static_cast<double>(collisions.size()) / (
                totalTests) << "\n";
            collisions.clear();
        }
    }

    // This test also relies on the way sets work, the logic is similar to determinismTest
    void collisionSearchSets(const HashGenInterface *hashGen) {
        constexpr int inputStringLength[4] = {10, 100, 500, 1000};
        const std::string validSymbols{
            "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789@#$%^&*()_+-=[]{}|;:',.<>/?`~\"\\"
        };
        static std::mt19937 rng(std::random_device{}());

        std::set<std::string> inputs{};
        std::set<std::string> hashes{};

        for (int i: inputStringLength) {
            for (int j = 0; j < totalTests; j++) {
                std::string input{generateRandomString(i, validSymbols, rng)};

                if (inputs.contains(input)) {
                    j--;
                    // If the input was already generated, skip this iteration
                    continue;
                }

                inputs.insert(input);

                std::string hash{hashGen->generateHash(input)};
                if (hashes.contains(hash)) {
                    std::cout << "Collision found for input " << input << "\n";
                } else {
                    hashes.insert(hash);
                }
            }
        }
    }

    void avalancheEffect(const HashGenInterface *hashGen) {
        const std::string validSymbols{
            "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789@#$%^&*()_+-=[]{}|;:',.<>/?`~\"\\"
        };

        struct Similarity {
            double total{0};
            double min{100};
            double max{0};
        };

        Similarity charSimilarity{};
        Similarity bitSimilarity{};

        static std::mt19937 rng(std::random_device{}());
        for (int i = 0; i < totalTests; i++) {
            std::string input{generateRandomString(100, validSymbols, rng)};
            std::string input2{input};

            //randomly change one character
            std::uniform_int_distribution<size_t> dist(0, input.size() - 1);
            size_t indexToChange{dist(rng)};
            std::uniform_int_distribution<size_t> charDist(0, validSymbols.size() - 1);
            input2[indexToChange] = validSymbols[charDist(rng)];

            if (input == input2) {
                // In case the characters are the same after the change
                continue;
            }

            std::string hash1{hashGen->generateHash(input)};
            std::string hash2{hashGen->generateHash(input2)};

            if (hash1 == hash2) {
                std::cout << "Collision with similar inputs!\n" << input << "\nand\n" << input2 << "\n";
                continue;
            }

            double percentageSimilarityChar{calculateSimilarityPercentage(hash1, hash2)};
            charSimilarity.total += percentageSimilarityChar;
            charSimilarity.min = std::min(charSimilarity.min, percentageSimilarityChar);
            charSimilarity.max = std::max(charSimilarity.max, percentageSimilarityChar);

            double percentageSimilarityBit{calculateSimilarityPercentageBit(hash1, hash2)};
            bitSimilarity.total += percentageSimilarityBit;
            bitSimilarity.min = std::min(bitSimilarity.min, percentageSimilarityBit);
            bitSimilarity.max = std::max(bitSimilarity.max, percentageSimilarityBit);
        }

        std::ostringstream ss;
        ss << "Please note that collissions are not counted in the max similarity \n"
            << "Average char similarity: " << (charSimilarity.total / totalTests) << "%\n"
            << "Min char similarity: " << charSimilarity.min << "%\n"
            << "Max char similarity: " << charSimilarity.max << "%\n"
            << "------------------------\n"
            << "Average bit similarity: " << (bitSimilarity.total / totalTests) << "%\n"
            << "Min bit similarity: " << bitSimilarity.min << "%\n"
            << "Max bit similarity: " << bitSimilarity.max << "%\n"
            << "------------------------\n";

        std::cout << ss.str();
    }

    // Helpers

    std::string generateRandomString(const size_t length, const std::string &validSymbols, std::mt19937 &rng) {
        std::uniform_int_distribution<size_t> dist(0, validSymbols.size() - 1);
        std::string result{""};

        for (size_t i = 0; i < length; ++i) {
            result += validSymbols[dist(rng)];
        }

        return result;
    }

    double calculateSimilarityPercentage(std::string hash1, std::string hash2) {
        int identicalChars{0};
        int totalChars{static_cast<int>(std::max(hash1.length(), hash2.length()))};
        // ensure hash 1 is always the longer one -> go over hash2
        // -> all chars 1 has that are longer are not in h2, so they can't be identical
        if (hash1.length() < hash2.length()) {
            swap(hash1, hash2);
        }
        for (int i = 0; i < hash2.length(); i++) {
            if (hash1[i] == hash2[i]) {
                identicalChars++;
            }
        }
        return static_cast<double>(identicalChars) / totalChars * 100;
    }

    double calculateSimilarityPercentageBit(std::string hash1, std::string hash2) {
        int identicalBits{0};
        int totalBits{static_cast<int>(std::max(hash1.length(), hash2.length()) * 8)};
        // ensure hash 1 is always the longer one -> go over hash2
        // -> all bits 1 has that are longer are not in h2, so they can't be identical
        if (hash1.length() < hash2.length()) {
            swap(hash1, hash2);
        }
        for (int i = 0; i < hash2.length(); i++) {
            // weird asf this has to be a ulonglong
            std::bitset<8> bits1{static_cast<unsigned long long>(hash1[i])};
            std::bitset<8> bits2{static_cast<unsigned long long>(hash2[i])};
            for (int j = 0; j < 8; j++) {
                if (bits1[j] == bits2[j]) {
                    identicalBits++;
                }
            }
        }
        return static_cast<double>(identicalBits) / totalBits * 100;
    }
}
