#include "Color.h"

/**
 * Use this constructor to create a color from RGB real values
 */
Color::Color(float r, float g, float b) {
    this->r = std::clamp(r / 255.0f, 0.0f, 1.0f);
    this->g = std::clamp(g / 255.0f, 0.0f, 1.0f);
    this->b = std::clamp(b / 255.0f, 0.0f, 1.0f);
}

std::array<float, 3> Color::toFloatArray() const {
    return {this->r, this->g, this->b};
}
