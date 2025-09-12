#pragma once
#ifndef MATH_MATRIX_INL
#define MATH_MATRIX_INL


#include "MathUtils.hpp"
#include "Matrix.hpp"


namespace NS_math {

// constructors

template <size_t RowN, size_t ColN>
inline constexpr Matrix<RowN, ColN>::Matrix( // using initializer_list, don't use explicit
    std::initializer_list<std::initializer_list<double>>& arg_data
) {
    try {
        assign_with_initializer_list(arg_data);
    } catch (std::exception& e) {
        log_and_throw<Logger::SeeAbove>(
            "Matrix(std::initializer_list<std::initializer_list<double>> arg_data)", 
            e.what()
        );
    }
}

template <size_t RowN, size_t ColN>
inline constexpr Matrix<RowN, ColN>::Matrix(
    const std::array<std::array<double, ColN>, RowN>& arg_data
) : data(arg_data) 
{}

template <size_t RowN, size_t ColN>
inline Matrix<RowN, 1> Matrix<RowN, ColN>::createFromArrRow(
    const std::array<double, RowN>& arg_arr
) {
    static_assert(
        ColN == 1, 
        "Matrix(const std::array<double, RowN>&): "
            "Only valid for column vectors (ColN == 1)"
    );
    for (size_t r = 0; r < RowN; ++r) {
        data[r][0] = arg_arr[r];
    }
}

template <size_t RowN, size_t ColN>
inline Matrix<1, ColN> Matrix<RowN, ColN>::createFromArrCol(
    const std::array<double, ColN>& arg_arr
) {
    static_assert(
        RowN == 1, 
        "Matrix(const std::array<double, ColN>&): " 
            "Only valid for row vectors (RowN == 1)"
    );
    data = arg_arr;
}

template <size_t RowN, size_t ColN>
inline constexpr Matrix<RowN, ColN>::Matrix(
    const double& default_val
) {
    for (auto it = data.begin(); it != data.end(); ++it) {
        for (auto jt = it->begin(); jt != it->end(); ++jt) {
            (*jt) = default_val;
        }
    }
}

template <size_t RowN, size_t ColN>
inline Matrix<RowN, ColN>::Matrix(
    const std::vector<std::vector<double>>& arg_data
) {
    try {
        assign_with_vector(arg_data);
    } catch (std::exception& e) {
        log_and_throw<Logger::SeeAbove>("Matrix", e.what());
    }
}

// assignment operators

template <size_t RowN, size_t ColN> \
inline \
Matrix<RowN, ColN>& \
Matrix<RowN, ColN>::operator=(const Matrix<RowN, ColN>& other) {
    if (this == &other) { // self-assignment check
        return *this;  
    }
    data = other.data;
    return *this;
}

template <size_t RowN, size_t ColN> \
inline \
Matrix<RowN, ColN>& \
Matrix<RowN, ColN>::operator=(const std::array<std::array<double, ColN>, RowN>& other) {
    data = other;
    return *this;
}

template <size_t RowN, size_t ColN> \
inline \
Matrix<RowN, ColN>& \
Matrix<RowN, ColN>::operator=(const std::vector<std::vector<double>>& arg_data) {
    try {
        return assign_with_vector(arg_data);
    } catch (std::invalid_argument& e) {
        log_and_throw<Logger::SeeAbove>(
            "operator=", e.what());
        throw;
    }
}

// assign functions

template <size_t RowN, size_t ColN> \
inline \
Matrix<RowN, ColN>& \
Matrix<RowN, ColN>::assign_with_initializer_list(const std::initializer_list<std::initializer_list<double>> arg_data) {
    if (arg_data.size() != RowN) {
        log_and_throw<std::invalid_argument>(
            "assign_with_initializer_list", 
            "arg_data must have the same size as RowN");
        throw;
    }
    for (std::initializer_list<double>* ptr = arg_data.begin(); ptr != arg_data.end(); ++ptr) {
        if ((*ptr).size() != ColN) {
            log_and_throw<std::invalid_argument>(
                "assign_with_initializer_list", 
                "all rows of arg_data must have the same size as ColN");
            throw;
        }
    }
    data = arg_data;
    return *this;
}

template <size_t RowN, size_t ColN> \
inline \
Matrix<RowN, ColN>& \
Matrix<RowN, ColN>::assign_with_vector(const std::vector<std::vector<double>>& arg_data) {
    if (arg_data.size() != RowN) {
        log_and_throw<std::invalid_argument>(
            "assign_with_vector", 
            "arg_data must have the same size as RowN");
        throw;
    }
    for (auto it = arg_data.cbegin(); it != arg_data.cend(); ++it) {
        if ((*it).size() != ColN) {
            log_and_throw<std::invalid_argument>(
                "assign_with_vector", 
                "all rows of arg_data must have the same size as ColN");
            throw;
        }
    }
    for (size_t r = 0; r < RowN; ++r) {
        for (size_t c = 0; c < ColN; ++c) {
            data[r][c] = arg_data[r][c];
        }
    }
    return *this;
}

// comparision operators

template <size_t RowN, size_t ColN> \
inline \
bool Matrix<RowN, ColN>::operator==(const Matrix<RowN, ColN>& other) const {
    for (size_t r = 0; r < RowN; ++r) {
        for (size_t c = 0; c < ColN; ++c) {
            if (std::abs(data[r][c] - other[r][c]) > m_precision) {
                return false;
            }
        }
    }
    return true;
}

template <size_t RowN, size_t ColN> \
inline \
bool Matrix<RowN, ColN>::operator!=(const Matrix<RowN, ColN>& other) const {
    return !operator==(other);
}

// operator[]

template <size_t RowN, size_t ColN> \
inline \
std::array<double, ColN>& \
Matrix<RowN, ColN>::operator[](size_t index) {
    if (index >= RowN) {
        log_and_throw<std::out_of_range>(
            "operator[](size_t index)", 
            "index(" + std::to_string(index) + ") out of range (0 to RowN-1)"
        );
        throw;
    }
    return data[index];
}


template <size_t RowN, size_t ColN> \
inline \
const std::array<double, ColN>& \
Matrix<RowN, ColN>::operator[](size_t index) const {
    if (index >= RowN) {
        log_and_throw<std::out_of_range>(
            "operator[](size_t index) const", 
            "index(" + std::to_string(index) + ") out of range (0 to RowN-1)"
        );
        throw;
    }
    return data[index];
}

// is_size_equal

template <size_t RowN, size_t ColN> \
inline \
bool Matrix<RowN, ColN>::is_size_equal(size_t row_num, size_t col_num) const {
    return row_num == RowN && col_num == ColN;
}

// getters

template <size_t RowN, size_t ColN>
inline const std::array<std::array<double, ColN>, RowN>& Matrix<RowN, ColN>::get_data_constReference() const {
    return data;
}
template <size_t RowN, size_t ColN>
inline std::array<std::array<double, ColN>, RowN>& Matrix<RowN, ColN>::get_data_reference() {
    return data;
}
template <size_t RowN, size_t ColN>
inline std::array<std::array<double, ColN>, RowN> Matrix<RowN, ColN>::get_data_copy() const {
    return data;
}

template <size_t RowN, size_t ColN>
inline std::array<double, RowN> Matrix<RowN, ColN>::get_column(size_t col_index) const {
    if (col_index >= ColN) {
        log_and_throw<std::out_of_range>(
            "get_column(size_t col_index) const", 
            "col_index out of range (0 to ColN-1)"
        );
        throw;
    }
    std::array<double, RowN> column;
    for (size_t i = 0; i < RowN; ++i) {
        column[i] = data[i][col_index];
    }
    return column;
}

template <size_t RowN, size_t ColN>
template <size_t SubRowN, size_t SubColN>
inline Matrix<SubRowN, SubColN> Matrix<RowN, ColN>::get_sub_matrix(size_t start_row, size_t start_col) const
{
    if (start_row >= RowN)
    {
        log_and_throw<std::out_of_range>(
            "get_sub_matrix(size_t start_row, size_t start_col, size_t end_row = RowN, size_t end_col = ColN) const",
            "start_row(" + std::to_string(start_row) + ") cannot be >= RowN");
        throw;
    }
    if (start_col >= ColN) {
        log_and_throw<std::out_of_range>(
            "get_sub_matrix(size_t start_row, size_t start_col, size_t end_row = RowN, size_t end_col = ColN) const",
            "start_col(" + std::to_string(start_col) + ") cannot be >= ColN"
        );
        throw;
    }
    
    if (start_row + SubRowN > RowN) {
        log_and_throw<std::out_of_range>(
            "get_sub_matrix(size_t start_row, size_t start_col, size_t end_row = RowN, size_t end_col = ColN) const",
            "start_row(" + std::to_string(start_col) 
                + ") + SubRowN(" + std::to_string(SubRowN) 
                + ") cannot be > RowN"
        );
        throw;
    }
    if (start_col + SubColN > ColN) {
        log_and_throw<std::out_of_range>(
            "get_sub_matrix(size_t start_row, size_t start_col, size_t end_row = RowN, size_t end_col = ColN) const",
            "start_col(" + std::to_string(start_col) 
                + ") + SubColN(" + std::to_string(SubColN) 
                + ") cannot be > ColN"
        );
        throw;
    }
    
    // main logic
    Matrix<SubRowN, SubColN> result;
    for (size_t r = 0; r < SubRowN; ++r) {
        for (size_t c = 0; c < SubColN; ++c) {
            result[r][c] = data[start_row + r][start_col + c];
        }
    }
    return result;
}

// calculation operators

template <size_t RowN, size_t ColN>
inline Matrix<RowN, ColN> Matrix<RowN, ColN>::operator-() const { 
    Matrix<RowN, ColN> result;
    for (size_t r = 0; r < RowN; ++r) {
        for (size_t c = 0; c < ColN; ++c) {
            result[r][c] = -data[r][c];
        }
    }
    return result;
}

template <size_t RowN, size_t ColN>
inline Matrix<RowN, ColN> Matrix<RowN, ColN>::operator+(const Matrix<RowN, ColN>& other) const {
    Matrix<RowN, ColN> result;
    for (size_t r = 0; r < RowN; ++r) {
        for (size_t c = 0; c < ColN; ++c) {
            result[r][c] = data[r][c] + other[r][c];
        }
    }
    return result;
}

template <size_t RowN, size_t ColN>
inline Matrix<RowN, ColN> Matrix<RowN, ColN>::operator-(const Matrix<RowN, ColN>& other) const {
    Matrix<RowN, ColN> result;
    for (size_t r = 0; r < RowN; ++r) {
        for (size_t c = 0; c < ColN; ++c) {
            result[r][c] = data[r][c] - other[r][c];
        }
    }
    return result;
}

template <size_t RowN, size_t ColN>
template <size_t oColN>
inline Matrix<RowN, oColN> Matrix<RowN, ColN>::operator*(const Matrix<ColN, oColN>& other) const {
    Matrix<RowN, oColN> result;
    double tmp;
    for (size_t rr = 0; rr < RowN; ++rr) {
        for (size_t rc = 0; rc < oColN; ++rc) {
            tmp = 0;
            for (size_t i = 0; i < ColN; ++i) {
                tmp += data[rr][i] * other[i][rc];
            }
            result[rr][rc] = tmp;
        }
    }
    return result;
}


template <size_t RowN, size_t ColN>
inline Matrix<RowN, ColN> Matrix<RowN, ColN>::operator*(double scalar) const {
    Matrix<RowN, ColN> result;
    for (size_t r = 0; r < RowN; ++r) {
        for (size_t c = 0; c < ColN; ++c) {
            result[r][c] = data[r][c] * scalar;
        }
    }
    return result;
}

template <size_t RowN, size_t ColN>
inline Matrix<RowN, ColN> Matrix<RowN, ColN>::operator/(double scalar) const {
    if (scalar == 0) {
        log_and_throw(
            "operator/(double scalar) const", 
            "scalar cannot be 0");
        throw;
    }
    Matrix<RowN, ColN> result;
    for (size_t r = 0; r < RowN; ++r) {
        for (size_t c = 0; c < ColN; ++c) {
            result[r][c] = data[r][c] / scalar;
        }
    }
    return result;
}

template <size_t RowN, size_t ColN>
inline Matrix<RowN, ColN>& Matrix<RowN, ColN>::operator+=(const Matrix<RowN, ColN>& other) noexcept {
    for (size_t r = 0; r < RowN; ++r) {
        for (size_t c = 0; c < ColN; ++c) {
            data[r][c] += other[r][c];
        }
    }
    return *this;
}

template <size_t RowN, size_t ColN>
inline Matrix<RowN, ColN>& Matrix<RowN, ColN>::operator-=(const Matrix<RowN, ColN>& other) noexcept {
    for (size_t r = 0; r < RowN; ++r) {
        for (size_t c = 0; c < ColN; ++c) {
            data[r][c] -= other[r][c];
        }
    }
    return *this;
}
template <size_t RowN, size_t ColN>
inline Matrix<RowN, ColN>& Matrix<RowN, ColN>::operator*=(double scalar) noexcept {
    for (size_t r = 0; r < RowN; ++r) {
        for (size_t c = 0; c < ColN; ++c) {
            data[r][c] *= scalar;
        }
    }
    return *this;
}

template <size_t RowN, size_t ColN>
inline Matrix<RowN, ColN>& Matrix<RowN, ColN>::operator/=(double scalar) {
    if (scalar == 0) {
        log_and_throw(
            "operator/=(double scalar)", 
            "scalar cannot be 0");
        throw;
    }
    for (size_t r = 0; r < RowN; ++r) {
        for (size_t c = 0; c < ColN; ++c) {
            data[r][c] /= scalar;
        }
    }
    return *this;
}

template <size_t RowN, size_t ColN>
inline Matrix<ColN, RowN> Matrix<RowN, ColN>::transpose() const {
    if (RowN == 1 && ColN == 1) {
        return Matrix<1, 1>(data[0][0]);
    }
    Matrix<ColN, RowN> result;
    for (size_t r = 0; r < RowN; ++r) {
        for (size_t c = 0; c < ColN; ++c) { 
            result[c][r] = data[r][c];
        }
    }
    return result;
}

template <size_t RowN, size_t ColN>
inline std::string Matrix<RowN, ColN>::to_string(
    bool add_prefix, 
    std::string elem_sep, 
    std::string line_sep, 
    std::string front, 
    std::string end
) const {
    std::string result;
    result += (add_prefix? "Matrix(\n" : "(\n") + front;
    for (size_t r = 0; r < RowN; ++r) {
        for (size_t c = 0; c < ColN; ++c) {
            result += 
                std::to_string(data[r][c]) 
                + (c < ColN - 1? elem_sep : "");
        }
        result += (r < RowN - 1? line_sep : end);
    }
    result += "\n)";
    return result;
}

} // namespace Math

#endif // MATH_MATRIX_INL