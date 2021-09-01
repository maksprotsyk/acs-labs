//
// Created by Maksym Protsyk on 5/21/21.
//

#include "TwoDimArray.h"


TwoDimArray::TwoDimArray(size_t init_height, size_t init_width): _height(init_height), _width(init_width) {
    array = new double[_height * _width];
    row_indices = new size_t[_height];

    // calculating row start indices for faster element accessing
    size_t index = 0;
    for (size_t i = 0; i < _height; i++) {
        row_indices[i] = index;
        index += _width;
    }

}


TwoDimArray::TwoDimArray(size_t init_height, size_t init_width, double val): TwoDimArray(init_height, init_width) {
    for (size_t i = 0; i < _height * _width; i++) {
        array[i] = val;
    }
}


TwoDimArray::TwoDimArray(const std::string& init_file) {
    // open file
    std::ifstream input(init_file);
    if (!input.is_open()) {
        throw std::logic_error("Invalid filename");
    }

    // count lines in the file
    long lines = std::count(std::istreambuf_iterator<char>(input),
                            std::istreambuf_iterator<char>(), '\n') + 1;
    _height = lines;
    _width = lines;
    array = new double[_height * _width];

    // return to the start of the file
    input.clear();
    input.seekg(0, std::ios::beg);

    // fill the array
    for (int i = 0; i < _height * _width; ++i) {
        input >> array[i];
    }

    row_indices = new size_t[_height];
    size_t index = 0;
    for (size_t i = 0; i < _height; i++) {
        row_indices[i] = index;
        index += _width;
    }
}


double& TwoDimArray::get(size_t x, size_t y) {
    if (x >= width() || y >= height()) {
        throw std::runtime_error("Array index out of range");
    }
    return array[row_indices[y] + x];
}


double* TwoDimArray::operator[](size_t row) const {
    return array + row_indices[row];
}


size_t TwoDimArray::height() const {
    return _height;
}


size_t TwoDimArray::width() const {
    return _width;
}


TwoDimArray::~TwoDimArray() {
    delete[] array;
    delete[] row_indices;
}


std::ostream& operator<<(std::ostream& stream, const TwoDimArray& obj) {
    stream.precision(5);
    for (size_t i = 0; i < obj.height(); i++) {
        double* row_ptr = obj[i];
        for (size_t j = 0; j < obj.width(); j++) {
            stream << row_ptr[j] << " ";
        }
        stream << "\n";
    }
    return stream;
}
