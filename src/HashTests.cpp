//
// Created by arnas on 9/12/25.
//

#include "HashTests.h"

// TODO: test if these tests work lmao

namespace HashTests {
    // TODO: Patikrink išvedimo dydį – nepriklausomai nuo įvedimo, rezultatas visada tokio pat ilgio.
    // Patikrink deterministiškumą – tas pats failas duoda tą patį hash’ą.
    // TODO: Išmatuok efektyvumą:
    // Kolizijų paieška
    // TODO: Lavinos efektas
    // TODO: Negrįžtamumo demonstracija (hiding, puzzle-friendliness) - HASH(input + salt)

    // Not sure how else to test this besides run it like 10 times lol
    void determinismTest(const HashGenInterface* hashGen, const std::string& input) {
        for (int i=0; i<10; i++) {
            std::string hash1 = hashGen->generateHash(input);
            std::string hash2 = hashGen->generateHash(input);
            if (hash1 != hash2) {
                throw "Determinism test failed: Hashes do not match.";
            }
        }
    }

    void collisionSearch(const HashGenInterface* hashGen) {
        constexpr int poruSimboliuKiekis[5] = {10, 100, 500, 1000};
        constexpr std::string validSymbols = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789@#$%^&*()_+-=[]{}|;:',.<>/?`~\"\\";
        std::mt19937 rng(std::random_device{}());
        for (int i: poruSimboliuKiekis) {
            for (int j=0; i<100000; i++){
                // rng is kinda vibecoded here. is very likely ass
                std::string input1(i, ' '), input2(i, ' ');
                std::uniform_int_distribution<> dist(0, validSymbols.size() - 1);
                std::generate_n(input1.begin(), i, [&]() { return validSymbols[dist(rng)]; });
                std::generate_n(input2.begin(), i, [&]() { return validSymbols[dist(rng)]; });

                std::string hash1 = hashGen->generateHash(input1);
                std::string hash2 = hashGen->generateHash(input2);
                if (hash1 == hash2) {
                    throw "Collision test failed: Different inputs produced the same hash.";
                }
            }
        }
    }
}
