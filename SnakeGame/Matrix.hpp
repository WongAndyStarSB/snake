#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <vector>
#include <exception>



class UnequalRowSizeException : public std::exception {
    public:
        inline const char* what() const throw() override {
            return "Unequal number of columns in each row of the matrix.";
        }
    };
    

template <typename T>
class Matrix {
    public:
        std::vector<std::vector<T>> data;
        size_t num_of_row;
        size_t num_of_col;

        inline explicit Matrix(const size_t& arg_num_of_row, const size_t& arg_num_of_col, const T& default_val)
            : num_of_row(arg_num_of_row), num_of_col(arg_num_of_col) {
            data.resize(num_of_row, std::vector<T>(num_of_col, default_val));
        }
        inline explicit Matrix(std::initializer_list<std::initializer_list<T>> m) {
            num_of_row = m.size();
            if (num_of_row == 0) {
                num_of_col = 0;
                return;
            }

            data.reserve(num_of_row);
            size_t temp = (*m.begin()).size();
            for (const auto& row : m) {
                if (row.size() != temp) {
                    throw UnequalRowSizeException();
                }
                data.emplace_back(row.begin(), row.end());
            }

            num_of_col = temp;
        }
        inline explicit Matrix(std::vector<std::vector<T>> arg_data) : data(arg_data), num_of_row(arg_data.size()) {
            if (num_of_row == 0) {
                num_of_col = 0;
                return;
            };
            size_t temp = data[0].size();
            for (const std::vector<T>& row : data) {
                if (row.size() != temp) { 
                    throw UnequalRowSizeException();
                }
            }
            num_of_col = temp;
        }
        // copy constructor
        inline Matrix(const Matrix<T>& other) : data(other.data), num_of_row(other.num_of_row), num_of_col(other.num_of_col) {}
        // move constructor
        inline Matrix(Matrix<T>&& other) noexcept
            : data(std::move(other.data)),
            num_of_row(other.num_of_row),
            num_of_col(other.num_of_col) {
            other.num_of_row = 0;
            other.num_of_col = 0;
        }
        // move assignment operator
        inline Matrix<T>& operator=(Matrix<T>&& other) noexcept {
            if (this != &other) {
                data = std::move(other.data);
                num_of_row = other.num_of_row;
                num_of_col = other.num_of_col;
                other.num_of_row = 0;
                other.num_of_col = 0;
            }
            return *this;
        }
        // assignment operator
        inline Matrix<T>& operator=(const Matrix<T>& other) {
            if (this != &other) {
                data = other.data;
                num_of_row = other.num_of_row;
                num_of_col = other.num_of_col;
            }
            return *this;
        }

        // Overload the subscript operator [] to access elements
        inline std::vector<T>& operator[](size_t row) noexcept {
            return data[row];
        }
        inline const std::vector<T>& operator[](size_t row) const noexcept {
            return data[row];
        }
        inline std::string join_into_string(std::string line_sep = "\n", std::string elem_sep = ", ") const {
            std::string return_str = "";
            for (int r = 0; r < num_of_row; ++r) {
                for (int c = 0; c < num_of_col; ++c) {
                    if (std::is_same<T, char>::value) {
                        return_str += std::string(1, data[r][c]);
                    } else {
                        return_str += std::to_string(data[r][c]);
                    }
                    return_str += ((c == num_of_col-1)? "" : elem_sep);
                }
                if (r < num_of_row) {
                    return_str += line_sep;
                }
            }
            return return_str;
        }
        inline std::string to_string(std::string prefix = "") const {
            std::string return_val = "Matrix(\n";
            for (size_t r = 0; r < num_of_row; ++r) {
                return_val += prefix + ((r == 0)? "[[]" : " [");
                for (size_t c = 0; c < num_of_col-1; ++c) {
                    return_val += std::to_string(data[r][c]) + ", ";
                }
                return_val += std::to_string(data[r][num_of_col-1]);
                return_val += (
                    (r == num_of_row-1)? ("] ]\n"+ prefix + ")") : ("],\n")
                );
            }
            return return_val;
        }
        
        inline auto begin() noexcept { //return type: std::vector<std::vector<T>>::iterator
            return data.begin();
        }
        inline auto begin() const noexcept { //return type: std::vector<std::vector<T>>::const_iterator
            return data.begin(); 
        }

        inline auto end() noexcept { //return type: std::vector<std::vector<T>>::iterator
            return data.end();
        }
        inline auto end() const noexcept { //return type: std::vector<std::vector<T>>::const_iterator
            return data.end();
        }

        inline auto size() const noexcept { //return type: std::vector<std::vector<T>>::size_type
            return num_of_row
        }

        inline bool operator==(const Matrix<T>& other) const {
            if (num_of_row != other.num_of_row || num_of_col != other.num_of_col) {
                return false;
            }
            for (size_t r = 0; r < num_of_row; ++r) {
                for (size_t c = 0; c < num_of_col; ++c) {
                    if (data[r][c] != other.data[r][c]) {
                        return false;
                    }
                }
            }
            return true;
        }
        inline bool operator!=(const Matrix<T>& other) const {
            return !(*this == other);
        }

};



#endif