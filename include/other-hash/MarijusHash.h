#ifndef MARIUSHASH_H
#define MARIUSHASH_H
#include <random>

#include "HashGenInterface.h"

using namespace std;
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <filesystem>
// SOMETHING IS BROKEN IN MY ADAPTATION. NOT WORKING!

class MarijusHash : public HashGenInterface {

    void nuskaitytimp3(vector<int> &nuskaityti_binary_duomenys) const {
        ifstream in("../include/other-hash/irasas.mp3", ios::binary);
        //uzdedu binary input output flag, kad nuskaitomo failo nereiketu uzkoduoti i utf8. radau info https://cplusplus.com/doc/tutorial/files/
        if (!in) {
            cout << "Failas nerastas" << endl;
            return;
        } else {
            //cout<<"nuskaitoma is failo.";
            for (char byte; in.get(byte);) {
                if (byte != '\0') nuskaityti_binary_duomenys.push_back((unsigned char) byte);
                //nors duomenys nuskaitomi binary, kai char irasome i int vektoriu, jis konvertuojamas i desimtaine sistema
                //if (nuskaityti_binary_duomenys.size() % 50000 == 0) cout<<"."; //skaitant ilgesni faila rodys kad vyksta procesas
            }
            //cout<<endl<<"Nuskaityta sekmingai."<<endl;
        }
        in.close();
    }

    void KonvertCharIx10(string input, vector<int> &konvertuota_ivestis) const {
        char simbolis;
        for (int i = 0; i <= input.length(); i++) {
            simbolis = input[i]; //null patampa 0, o 0 yra 48
            //cout<< simbolis << " ";
            //cout<< (int)simbolis << " ";
            konvertuota_ivestis.push_back((int) simbolis);
        }
    }

    int Sumax10(vector<int> &konvertuota_ivestis) const {
        //apskaiciuosime visu ivestu simboliu desimtaine suma, kuria naudosime kreipiantis i mp3 suskirstyta veiktoriu
        int suma = 0;
        for (int i = 0; i < konvertuota_ivestis.size(); i++) {
            suma += konvertuota_ivestis[i];
            if (suma < 0) suma *= -1; //kad nepasidarytu neigiama suma (lengviau bus sitaip)
        }
        return suma;
    }

    void failo_pasirinkimas_ir_nuskaitymas(int &pasirinkimas, vector<string> &txtsarasas, string &input) {
        cout << "Pasirinkite faila, is kurio norite nuskaityti duomenis: " << endl;
        txtfailai(".", txtsarasas);
        int i;
        for (i = 0; i < txtsarasas.size(); i++) {
            cout << i + 1 << " - " << txtsarasas[i] << endl;
        }
        cout << "Pasirinkite ivesdami skaiciu: ";
        int temp;
        cin >> temp;

        ifstream fr(txtsarasas[temp - 1]);
        if (fr) {
            string line;
            while (getline(fr, line)) {
                input += line;
                input += '\n';
            }
        }
        //cout<<input.size()<<endl;
        fr.close();
    }

    void clearData(string &input, string &hash, string &salt, int &suma, vector<int> &konvertuota_ivestis,
                   vector<int> &konvertuota_druskyte) {
        input.clear();
        hash.clear();
        salt.clear();
        suma = 0;
        konvertuota_ivestis.clear();
        konvertuota_druskyte.clear();
    }

    static string read_first_n_lines(const string &filepath, int n) {
        //gpt parase patogesne funkcija nei as. Bet as ja modikavau nemazai
        string lines;
        if (n <= 0) return lines;
        ifstream in(filepath);
        if (!in) {
            std::cerr << "Nepavyko atidaryti failo: " << filepath << endl;
            return lines;
        }
        string line;
        int count = 0;
        while (count < n && std::getline(in, line)) {
            lines += line;
            ++count;
        }
        in.close();
        return lines;
    }

    void hashFunkcija(string &input, string &hash, string &salt) const {
        vector<int> nuskaityti_binary_duomenys;
        vector<int> konvertuota_ivestis;
        vector<int> konvertuota_druskyte;
        int suma;

        salt = druskyte(input);
        input += salt;
        KonvertCharIx10(input, konvertuota_ivestis);
        KonvertCharIx10(salt, konvertuota_druskyte);
        suma = Sumax10(konvertuota_ivestis);
        nuskaitytimp3(nuskaityti_binary_duomenys);

        int skyriu_sk = nuskaityti_binary_duomenys.size() / 32;
        int pradinis_skyrius = suma;
        while (pradinis_skyrius > skyriu_sk) pradinis_skyrius -= skyriu_sk;

        //pati hash funkcija
        for (int i = 0; i < 32; i++) {
            hash += desimtaine_i_16(
                konvertuota_ivestis[i] ^ nuskaityti_binary_duomenys[pradinis_skyrius * 32 + i] ^ konvertuota_druskyte[
                    i]);
        }
        int i = 0;
        while (hash.length() < 64) {
            hash += desimtaine_i_16(
                konvertuota_ivestis[i] ^ nuskaityti_binary_duomenys[pradinis_skyrius * (32 + i)] ^ konvertuota_druskyte[
                    i]);
        }
        if (hash.length() > 64) hash = hash.substr(0, 64);
    }


// Simple timer helpers. Use startTimer() to get a timestamp token and pass it to
// stopTimerMs() to get elapsed milliseconds as double.
using timer_point = std::chrono::steady_clock::time_point;

static inline timer_point startTimer() {
    return std::chrono::steady_clock::now();
}

// Return elapsed seconds as a double
static inline double stopTimerSec(const timer_point &start) {
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> diff = end - start; // seconds
    return diff.count();
}


string desimtaine_i_16(int desimtaine) const{
    string hex = "";
    int liekana;
    char simbolis;
    while(desimtaine != 0){
        if(desimtaine <0 ) desimtaine = (-1) * desimtaine; //jei ivedami lietuviški simboliai, jų reikšmė būna minusinė. Tai trikdo veiklai.
        liekana = desimtaine % 16;
        if (liekana < 10) simbolis = liekana + '0';
        else simbolis = liekana - 10 + 'A';
        hex = simbolis + hex;
        desimtaine /= 16;
    }
    return hex;
}

string druskyte (string input) const {
    //generavau sita koda siu ai pagalba. Ideja kodo pasakiau as, tik nelabai mokejau igyvendinti (random skaiciu bibliotekos as nelabai pazistu)
    std::seed_seq seed(input.begin(), input.end());
    std::mt19937 rng(seed);
    std::uniform_int_distribution<int> dist(0, 15);
    string result;
    for (int i = 0; i < 64; ++i) {
        int val = dist(rng);
        result += "0123456789ABCDEF"[val];//"0123456789ABCDEF" is a string containing all hexadecimal digits. [val] selects one based on the random value.
    }
    return result;
}

    // vibe coded pakeiciau originala.
    void txtfailai(const std::string &path, std::vector<std::string> &fileList) {
    namespace fs = std::filesystem;
    fileList.clear();
    try {
        for (const auto &entry : fs::directory_iterator(path)) {
            if (entry.is_regular_file() && entry.path().extension() == ".txt") {
                fileList.push_back(entry.path().filename().string());
            }
        }
    } catch (const fs::filesystem_error &e) {
        std::cout << "Error opening directory: " << e.what() << std::endl;
    }
}

string randSgen(string input, int size) { //sugeneruota su ai, nes nesigilinu i random bibliotekas
    static string similarStr; // Keeps the "panasus" string between calls

    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> dist(0, 61); // 0-9, A-Z, a-z

    auto randomChar = [&]() -> char {
        int val = dist(rng);
        if (val < 10) return '0' + val;
        else if (val < 36) return 'A' + (val - 10);
        else return 'a' + (val - 36);
    };

    if (input == "random") {
        string result;
        for (int i = 0; i < size; ++i) {
            result += randomChar();
        }
        return result;
    }
    else if (input == "panasus") {
        if (similarStr.empty() || similarStr.size() != size) {
            similarStr.clear();
            for (int i = 0; i < size; ++i) {
                similarStr += randomChar();
            }
        }
        // Change one random symbol
        uniform_int_distribution<int> idxDist(0, size - 1);
        int idx = idxDist(rng);
        char newChar;
        do {
            newChar = randomChar();
        } while (newChar == similarStr[idx]);
        similarStr[idx] = newChar;
        return similarStr;
    }
    return "";
}

public:
    std::string generateHash(const std::string &input) const override {
        string hash{};
        string salt{};
        string i2 = input;
        hashFunkcija(i2, hash, salt);
        return hash;
    }
};

#endif //MARIUSHASH_H
