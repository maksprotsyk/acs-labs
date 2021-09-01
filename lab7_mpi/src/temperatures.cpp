//
// Created by Maksym Protsyk on 5/21/21.
//
#include "temperatures.h"

void calculate_temperature(TwoDimArray* old_array_ptr, TwoDimArray* new_array_ptr, const temp_params_t& params) {

    TwoDimArray& old_array = *old_array_ptr;
    TwoDimArray& new_array = *new_array_ptr;

    double dx_sqr = params.dx * params.dx;
    double dy_sqr = params.dy * params.dy;

    // calculating temperature using recurrent formula

    for (size_t x = 1; x < old_array.width() - 1; x++) {
        for (size_t y = 1; y < old_array.height() - 1; y++) {
            new_array.get(x, y) =  old_array.get(x, y) + params.dt * params.alpha * (
                (old_array.get(x - 1, y) - 2 * (old_array.get(x, y)) + old_array.get(x + 1, y)) / dx_sqr +
                (old_array.get(x, y - 1) - 2 * (old_array.get(x, y)) + old_array.get(x, y + 1)) / dy_sqr
            );
        }
    }


}