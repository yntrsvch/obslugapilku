#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>

//informacje o kolorze
struct Color {
    int r, g, b;
    Color(int red, int green, int blue) : r(red), g(green), b(blue) {}
};

std::string colorToString(const Color& color) {
    return std::to_string(color.r) + "-" + std::to_string(color.g) + "-" + std::to_string(color.b);
}

//do odczytu pliku PBM
std::vector<Color> readPBM(const std::string& filename, int& width, int& height) {
    std::ifstream file(filename);
    std::vector<Color> colors;

    if (!file) {
        std::cerr << "Nie mozna otworzyc pliku.\n";
        return colors;
    }

    std::string magicNumber;
    file >> magicNumber;

    if (magicNumber != "P1") {
        std::cerr << "Nieprawidlowy format pliku.\n";
        return colors;
    }

    file >> width >> height;

    if (width <= 0 || height <= 0) {
        std::cerr << "Nieprawidlowe wymiary obrazu.\n";
        return colors;
    }

    int pixel;
    while (file >> pixel) {
        if (pixel == 0)
            colors.push_back(Color(255, 255, 255)); // biały kolor dla piksela czarnego
        else
            colors.push_back(Color(0, 0, 0)); // czarny kolor dla piksela białego
    }

    return colors;
}

int main() {
    std::string response;
    do {
        std::string filename;
        std::cout << "Podaj nazwe pliku: ";
        std::cin >> filename;

        int width, height;
        std::vector<Color> colors;

        if (filename.substr(filename.find_last_of(".") + 1) == "pbm") {
            colors = readPBM(filename, width, height);
        }
        else {
            std::cerr << "Nieobslugiwany format pliku.\n";
            continue;
        }

        if (!colors.empty()) {
            // mapa przechowująca liczbę wystąpień poszczególnych kolorów
            std::map<Color, int> colorCount;
            // wektor przechowujący unikalne kolory
            std::vector<Color> uniqueColors;

            // obliczanie liczby wystąpień i unikalnych kolorów
            for (const auto& color : colors) {
                colorCount[color]++;
                if (std::find(uniqueColors.begin(), uniqueColors.end(), color) == uniqueColors.end()) {
                    uniqueColors.push_back(color);
                }
            }

            //  kolor który występuje najczęściej
            auto mostCommonColor = std::max_element(colorCount.begin(), colorCount.end(),
                [](const std::pair<Color, int>& a, const std::pair<Color, int>& b) {
                    return a.second < b.second;
                });

            // informacja o obrazie
            std::cout << "\nSzerokosc obrazu: " << width << "\n";
            std::cout << "Wysokosc obrazu: " << height << "\n";
            std::cout << "Najczesciej wystepujacy kolor to " << colorToString(mostCommonColor->first)
                << " i wystapil " << mostCommonColor->second << " razy\n";
            std::cout << "Liczba unikalnych kolorow: " << uniqueColors.size() << "\n";
        }

        std::cout << "\nCzy chcesz wczytac kolejny plik (tak/nie): ";
        std::cin >> response;
    } while (response == "tak");

    return 0;
}
