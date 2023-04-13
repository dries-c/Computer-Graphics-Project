#ifndef OPENGL_PROJECT_COLOR_H
#define OPENGL_PROJECT_COLOR_H
#include <array>

class Color {
public:
    float r, g, b;
    Color(float r, float g, float b);
    [[nodiscard]] std::array<float, 3> toFloatArray() const;
};


#endif
