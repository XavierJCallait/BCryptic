#include <sodium.h>
#include <set>
#include <string>
#include <cmath>
#include <stdexcept>
#include <algorithm>

#include "BuildPasswords.h"

BuildPasswords::BuildPasswords() {
    
}

/*
* Generates three unique numbers in the range [1, 1000] as the zeros for cubic function
*/
std::array<int, 3> BuildPasswords::generateUniqueZeros(const std::vector<unsigned char> &buffer) {
    std::set<int> zeros;

    for (int i = 0; i < ZEROS_IN_CUBIC_FX; i++) {
        int candidate = 0;
        unsigned long long iteration = 0;
        do {
            std::string message = "index_" + std::to_string(i) + "_iteration_" + std::to_string(iteration);
            
            unsigned char mac[crypto_auth_hmacsha256_BYTES];
            crypto_auth_hmacsha256_state state;

            crypto_auth_hmacsha256_init(&state, buffer.data(), buffer.size());
            crypto_auth_hmacsha256_update(&state, reinterpret_cast<const unsigned char *>(message.data()), message.size());
            crypto_auth_hmacsha256_final(&state, mac);

            candidate = ((mac[0] << 8) | (mac[1])) % 1000 + 1;
            iteration++;
        } while (zeros.count(candidate));
        zeros.insert(candidate);
    }

    std::array<int, 3> result{};
    size_t index = 0;
    for (int element : zeros) {
        if (index < result.size()) {
            result[index++] = element;
        }
    }

    return result;
}

/*
* Finds x-values of where the maxima and minima are to find the region where a line intersects three times
*/
std::array<double, 2> BuildPasswords::findCriticalPoints(const std::array<int, 3> &zeros) {
    std::array<double, 2> result{};

    long long sum = zeros[0] + zeros[1] + zeros[2];
    long long pairProducts = zeros[0] * zeros[1] + zeros[0] * zeros[2] + zeros[1] * zeros[2];

    long long discriminant = 4 * sum * sum - 12 * pairProducts;
    double sqrtDiscriminant = std::sqrt(discriminant);

    double point1 = (2 * sum - sqrtDiscriminant) / 6.0;
    double point2 = (2 * sum + sqrtDiscriminant) / 6.0;

    result[0] = std::min(point1, point2);
    result[1] = std::max(point1, point2);

    return result;
}

/*
* Evaluates the cubic function using factored form
*/
double BuildPasswords::evaluateCubicFx(const double &x, const std::array<int, 3> &zeros) {
    return (x - zeros[0]) * (x - zeros[1]) * (x - zeros[2]);
}

/*
* Given the x-values of critical points, calculates the y-values of those points
*/
std::array<double, 2> BuildPasswords::evaluateCriticalPoints(const std::array<int, 3> &zeros) {
    std::array<double, 2> result{};
    std::array<double, 2> criticalPoints = findCriticalPoints(zeros);
    double point1 = evaluateCubicFx(criticalPoints[0], zeros);
    double point2 = evaluateCubicFx(criticalPoints[1], zeros);
    
    result[0] = std::min(point1, point2);
    result[1] = std::max(point1, point2);

    return result;
}

/*
* Calculates the horizontal intersection line by taking the average of the y-values of critical points
*/
double BuildPasswords::findIntersectionLine(const std::array<int, 3> &zeros) {
    std::array<double, 2> points = evaluateCriticalPoints(zeros);
    return (points[0] + points[1]) / 2.0;
}

std::array<double, 3> BuildPasswords::solveCubicEquation(const std::array<double, 4> &coefficients) {
    double a = coefficients[1];
    double b = coefficients[2];
    double c = coefficients[3];

    double p = b - a * a / 3.0;
    double q = (2.0 * a * a * a) / 27.0 - (a * b) / 3.0 + c;

    double discriminant = (q * q) / 4.0 + (p * p * p) / 27.0;
    if (discriminant > 1e-12) {
        throw std::runtime_error("Cubic does not have 3 real roots (numerical issue)");
    }

    double phi = std::acos((-q / 2.0) / std::sqrt(-(p * p * p) / 27.0));
    double r = 2.0 * std::sqrt(-p / 3.0);

    std::array<double, 3> roots{};
    for (int k = 0; k < 3; k++) {
        roots[k] = r * std::cos((phi + 2.0 * PI * k) / 3.0) - a / 3.0;
    }

    return roots;
}

/*
* Finds the x-values of the intersection points between the cubic polynomial and line
*/
std::array<double, 3> BuildPasswords::findIntersectionPoints(const std::array<int, 3> &zeros, double line) {
    double a = -(zeros[0] + zeros[1] + zeros[2]);
    double b = zeros[0]*zeros[1] + zeros[0]*zeros[2] + zeros[1]*zeros[2];
    double c = -(zeros[0]*zeros[1]*zeros[2]) - line;
    std::array<double, 4> coefficients = {1.0, a, b, c};
    return solveCubicEquation(coefficients);
}

/*
* Given the unique zeros, it returns the x-values of the intersection points
*/
std::array<double, 3> BuildPasswords::findHorizontalLineThreeIntersections(const std::array<int, 3> &zeros) {
    if (zeros[0] == zeros[1] || zeros[0] == zeros[2] || zeros[1] == zeros[2]) {
        throw std::runtime_error("Zeros must be distinct");
    }

    double intersectionLine = findIntersectionLine(zeros);

    return findIntersectionPoints(zeros, intersectionLine);
}

/*
* Maps the x-value intersection points to the range [1, 10] 
*/
std::array<int, 3> BuildPasswords::getSaltIndices(const std::vector<unsigned char> &buffer) {
    std::array<int, 3> zeros = generateUniqueZeros(buffer);
    std::array<double, 3> intersectPoints = findHorizontalLineThreeIntersections(zeros);
    std::array<int, 3> salts{};

    std::transform(intersectPoints.begin(), intersectPoints.end(), salts.begin(),
                   [](double x) { return static_cast<int>(std::fmod(std::llround(x), 10) + 1); });

    return salts;
}