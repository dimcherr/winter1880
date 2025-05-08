#pragma once
#include <random>
#include "tun/math.h"

namespace tun {

std::mt19937& random();
int GetRandomInt(int min, int max);
float GetRandomFloat(float min, float max);
Vec GetRandomVec(Vec min, Vec max);

template <typename T>
void shuffle(const std::vector<T>& vector) {
    std::shuffle(vector.begin(), vector.end(), random());
}

}