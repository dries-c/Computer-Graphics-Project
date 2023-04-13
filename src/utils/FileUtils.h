#ifndef OPENGL_PROJECT_FILEUTILS_H
#define OPENGL_PROJECT_FILEUTILS_H

#include <string>

const static std::string RESOURCES_PATH = "../resources/";

class FileUtils {
public:
    /**
     * Reads the contents of a file and returns it as a string
     * @param filePath path to the file
     * @return file contents
     */
    static std::string getFileContents(const char *filePath);
};


#endif
