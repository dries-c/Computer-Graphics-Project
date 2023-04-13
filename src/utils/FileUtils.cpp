#include "FileUtils.h"

#include <fstream>
#include <sstream>
#include <iostream>

std::string FileUtils::getFileContents(const char *filePath) {
    std::string contents;

    try {
        std::ifstream file(RESOURCES_PATH + filePath);

        file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        std::stringstream stream;
        stream << file.rdbuf();

        file.close();

        contents = stream.str();
    } catch (std::ifstream::failure &e) {
        std::cout << "Shader loading: " << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }

    return contents;
}
