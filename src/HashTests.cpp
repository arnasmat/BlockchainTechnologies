#include "../include/HashTests.h"



// TODO: test if these tests work lmao

namespace HashTests {
    // const int totalTests{100000};
    const int totalTests{100};
    // TODO: Patikrink išvedimo dydį – nepriklausomai nuo įvedimo, rezultatas visada tokio pat ilgio.
    // Patikrink deterministiškumą – tas pats failas duoda tą patį hash’ą.
    // TODO: Išmatuok efektyvumą:
    // Kolizijų paieška
    // TODO: Lavinos efektas
    // TODO: Negrįžtamumo demonstracija (hiding, puzzle-friendliness) - HASH(input + salt)

    // Not sure how else to test this besides run it like 10 times lol
    void determinismTest(const HashGenInterface *hashGen, const std::string &input) {
        for (int i = 0; i < 10; i++) {
            std::string hash1{hashGen->generateHash(input)};
            std::string hash2{hashGen->generateHash(input)};
            if (hash1 != hash2) {
                throw "Determinism failed :(";
            }
        }
    }

    void collisionSearch(const HashGenInterface *hashGen) {
        constexpr int pairStringLength[4] = {10, 100, 500, 1000};
        const std::string validSymbols{
            "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789@#$%^&*()_+-=[]{}|;:',.<>/?`~\"\\"
        };
        static std::mt19937 rng(std::random_device{}());

        std::vector<std::pair<std::string, std::string>> collisions{};
        for (int i: pairStringLength) {
            for (int j = 0; j < totalTests; j++) {
                std::string input1{generateRandomString(i, validSymbols, rng)};
                std::string input2{generateRandomString(i, validSymbols, rng)};

                if (input1==input2) {
                    j--;
                    std::cout<<"Identical inputs!";
                    continue;
                }

                std::string hash1{hashGen->generateHash(input1)};
                std::string hash2{hashGen->generateHash(input2)};
                if (hash1 == hash2) {
                    collisions.emplace_back(input1, input2);
                    std::cout<<"Collision found for length "<<i<<": "<<input1<<" and "<<input2<<"\n";
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

        Similarity bitSimilarity{};
        Similarity hexSimilarity{};

        static std::mt19937 rng(std::random_device{}());
        for (int i=0; i < totalTests; i++) {
            std::string input{generateRandomString(100, validSymbols, rng)};
            std::string input2{input};
            //randomly change one character
            std::uniform_int_distribution<size_t> dist(0, input.size() - 1);
            size_t indexToChange{dist(rng)};
            std::uniform_int_distribution<size_t> charDist(0, validSymbols.size() - 1);
            input2[indexToChange] = validSymbols[charDist(rng)];

            std::string hash1{hashGen->generateHash(input)};
            std::string hash2{hashGen->generateHash(input2)};

            double percentageSimilarityBit{calculateSimilarityPercentageBit(hash1, hash2)};
            bitSimilarity.total += percentageSimilarityBit;
            bitSimilarity.min = std::min(bitSimilarity.min, percentageSimilarityBit);
            bitSimilarity.max = std::max(bitSimilarity.max, percentageSimilarityBit);

            double percentageSimilarityHex{calculateSimilarityPercentageHex(hash1, hash2)};
            hexSimilarity.total += percentageSimilarityHex;
            hexSimilarity.min = std::min(hexSimilarity.min, percentageSimilarityHex);
            hexSimilarity.max = std::max(hexSimilarity.max, percentageSimilarityHex);
        }

        std::ostringstream ss;
        ss<<"Average bit similarity: "<<(bitSimilarity.total/totalTests)<<"%\n";
        ss<<"Min bit similarity: "<<bitSimilarity.min<<"%\n";
        ss<<"Max bit similarity: "<<bitSimilarity.max<<"%\n";
        ss<<"------------------------\n";
        ss<<"Average hex similarity: "<<(hexSimilarity.total/totalTests)<<"%\n";
        ss<<"Min hex similarity: "<<hexSimilarity.min<<"%\n";
        ss<<"Max hex similarity: "<<hexSimilarity.max<<"%\n";
        ss<<"------------------------\n";

        std::cout<<ss.str();
    }

    std::string generateRandomString(const size_t length, const std::string &validSymbols, std::mt19937 &rng) {
        std::uniform_int_distribution<size_t> dist(0, validSymbols.size() - 1);
        std::string result{"abs"};

        for (size_t i = 0; i < length; ++i) {
            result += validSymbols[dist(rng)];
        }

        return result;
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

    double calculateSimilarityPercentageHex(std::string hash1, std::string hash2) {
    // TODO:
        return 0;
    }
}
