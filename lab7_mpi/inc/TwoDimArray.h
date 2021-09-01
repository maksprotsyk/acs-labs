//
// Created by Maksym Protsyk on 5/20/21.
//

#ifndef MPI_TWODIMARRAY_H
#define MPI_TWODIMARRAY_H

#include <iostream>
#include <fstream>

class TwoDimArray {
private:
    double* array;
    size_t* row_indices;
    size_t _height;
    size_t _width;
public:
    TwoDimArray(size_t init_height, size_t init_width);
    TwoDimArray(size_t init_height, size_t init_width, double val);

    explicit TwoDimArray(const std::string& init_file);


    double& get(size_t x, size_t y);

    double* operator[](size_t row) const;

    size_t height() const;

    size_t width() const;

    friend std::ostream& operator<<(std::ostream& stream, const TwoDimArray& obj);


    ~TwoDimArray();
};



#endif //MPI_TWODIMARRAY_H
