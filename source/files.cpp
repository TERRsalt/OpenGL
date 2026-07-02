#include <string>
#include <fstream>
#include <print>

std::string fileRead(std::string filePath) {
    std::ifstream file(filePath);

    if (!file.is_open()) {
        std::println("Couldn't load ”{}” into memory!", filePath);
        return "";
    }

    std::string line, fileContent;
    while (getline(file, line)) {
        fileContent.append(line);
        fileContent.append("\n");
    }

    file.close();

    return fileContent;
}