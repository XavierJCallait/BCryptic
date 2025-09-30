#ifndef BUILDPASSWORDS_H
#define BUILDPASSWORDS_H

#include <vector>
#include <array>

#define ZEROS_IN_CUBIC_FX 3
#define PI 3.14159265358979323846

class BuildPasswords {
public:
    BuildPasswords();

    std::array<int, 3> generateUniqueZeros(const std::vector<unsigned char> &buffer);
    std::array<double, 2> findCriticalPoints(const std::array<int, 3> &zeros);
    double evaluateCubicFx(const double &x, const std::array<int, 3> &zeros);
    std::array<double, 2> evaluateCriticalPoints(const std::array<int, 3> &zeros);
    double findIntersectionLine(const std::array<int, 3> &zeros);
    std::array<double, 3> solveCubicEquation(const std::array<double, 4> &coefficients);
    std::array<double, 3> findIntersectionPoints(const std::array<int, 3> &zeros, double line);
    std::array<double, 3> findHorizontalLineThreeIntersections(const std::array<int, 3> &zeros);
    std::array<int, 3> getSaltIndices(const std::vector<unsigned char> &buffer);
};

#endif