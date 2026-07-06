#include <random>
#include <print>

#include <GLFW/glfw3.h>

#include "random.hpp"

int randomInt(int maxNumber) {return rand()%(maxNumber+1);}
float randomFloat(int maxNumber) {return (float)randomInt(maxNumber*100)/100;}

std::map<std::string, float> randomRgbColor() {
    std::map<std::string, float> randomColor = {
        {"red", randomFloat(1)},
        {"green", randomFloat(1)},
        {"blue", randomFloat(1)}
    };

    std::println("Changed the color to RGB({}, {}, {})", randomColor["red"], randomColor["green"], randomColor["blue"]);

    return randomColor;
}
