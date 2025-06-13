#include "pch.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <cmath>
#include <cstring>
#include <random>
#include <iomanip>

using namespace std;

double calculate_entropy(const string& filename) {
    ifstream file(filename, ios::binary);
    if (!file) {
        cerr << "Ошибка открытия файла: " << filename << endl;
        return -1.0;
    }

    const size_t buffer_size = 4096;
    vector<char> buffer(buffer_size);
    map<unsigned char, long long> freq;
    long long total = 0;

    while (file) {
        file.read(buffer.data(), buffer_size);
        size_t bytes_read = file.gcount();
        total += bytes_read;

        for (size_t i = 0; i < bytes_read; ++i) {
            unsigned char uc = static_cast<unsigned char>(buffer[i]);
            freq[uc]++;
        }
    }
    file.close();

    if (total == 0) return 0.0;

    double entropy = 0.0;
    for (const auto& pair : freq) {
        double probability = static_cast<double>(pair.second) / total;
        entropy -= probability * log2(probability);
    }

    return entropy;
}

void generate_single_char_file(const string& filename, size_t size = 1000) {
    ofstream file(filename, ios::binary);
    if (!file) {
        cerr << "Ошибка создания файла: " << filename << endl;
        return;
    }
    char c = 'A';
    for (size_t i = 0; i < size; ++i) {
        file.write(&c, 1);
    }
    file.close();
}

void generate_binary_file(const string& filename, size_t size = 1000) {
    ofstream file(filename, ios::binary);
    if (!file) {
        cerr << "Ошибка создания файла: " << filename << endl;
        return;
    }

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, 1);

    for (size_t i = 0; i < size; ++i) {
        char c = dis(gen) ? '1' : '0';
        file.write(&c, 1);
    }
    file.close();
}

void generate_random_bytes_file(const string& filename, size_t size = 1000) {
    ofstream file(filename, ios::binary);
    if (!file) {
        cerr << "Ошибка создания файла: " << filename << endl;
        return;
    }

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dis(0, 255);

    for (size_t i = 0; i < size; ++i) {
        unsigned char c = static_cast<unsigned char>(dis(gen));
        file.write(reinterpret_cast<const char*>(&c), 1);
    }
    file.close();
}

int main(int argc, char* argv[]) {
    setlocale(LC_CTYPE, "rus");
    if (argc < 2) {
        cout << "=== (с) Жиляев Максим. ААМ-24 ===" << endl;
        cout << "Использование:\n"
            << "  " << argv[0] << " <filename>   - вычислить энтропию файла\n"
            << "  " << argv[0] << " generate     - сгенерировать тестовые файлы\n";
        return 1;
    }

    cout << fixed << setprecision(6);

    if (strcmp(argv[1], "generate") == 0) {
        vector<string> filenames = {
            "C://single_char.bin",
            "C://binary_data.bin",
            "C://random_bytes.bin"
        };

        generate_single_char_file(filenames[0]);
        generate_binary_file(filenames[1]);
        generate_random_bytes_file(filenames[2]);

        for (const string& filename : filenames) {
            double entropy = calculate_entropy(filename);
            if (entropy >= 0) {
                cout << "Файл: " << setw(18) << left << filename
                    << " Энтропия: " << entropy << endl;
            }
        }
    }
    else {
        string filename = argv[1];
        double entropy = calculate_entropy(filename);
        if (entropy >= 0) {
            cout << "Энтропия файла: " << entropy << endl;
        }
    }

    return 0;
}