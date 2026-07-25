#include <random>

int randomInt(int maxNumber) {return rand() % (maxNumber + 1);}
float randomFloat(int maxNumber) {return (float)randomInt(maxNumber * 100) / 100;}