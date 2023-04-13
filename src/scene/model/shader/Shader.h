#ifndef OPENGL_PROJECT_SHADER_H
#define OPENGL_PROJECT_SHADER_H

#include <string>
#include "glm/mat4x4.hpp"

class Shader {
private:
    unsigned int ID;
    static void checkCompileErrors(unsigned int shader, const std::string& type);
public:
    Shader(const char* vertexPath, const char* fragmentPath);
    void bind() const;
    static void unbind() ;
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    void setMat4(const std::string &name, glm::mat4 value) const;
};


#endif
