//
// Created by Maksym Protsyk on 3/4/21.
//

#include "tests.h"

double test_func(double x, double y) {
    double result = 0.002;
    for (int i = -2; i <= 2; i++) {
        for (int j = -2; j <= 2; j++) {
            double val = (5*(i+2) + j + 3
                          + std::pow(x - 16*j, 6)
                          + std::pow(y - 16*i, 6));
            result += 1/val;
        }
    }
    return 1/result;
}