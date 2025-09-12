#ifndef MATH_MATRIX_HPP
#define MATH_MATRIX_HPP


#include <array>
#include <vector>
#include <string>
#include <stdexcept>

#include "../Logger/Logger.hpp"

namespace NS_math {
#include "MathUtils.hpp"


template <size_t RowN, size_t ColN>
class Matrix { // fixed sized Matrix with elem type using double
    
    static_assert(RowN > 0, "Matrix: RowN cannot be 0");
    static_assert(ColN > 0, "Matrix: ColN cannot be 0");

    public:
        std::array<std::array<double, ColN>, RowN> data;
    public:
        double m_precision = 1e-6;
    public:
        inline constexpr std::pair<size_t, size_t> size() const { return std::make_pair(RowN, ColN); }
        inline constexpr size_t num_of_row() const { return RowN; }
        inline constexpr size_t num_of_col() const { return ColN; }

        constexpr Matrix(std::initializer_list<std::initializer_list<double>>& arg_data); // using initializer_list, don't use explicit
        
        constexpr explicit Matrix(const std::array<std::array<double, ColN>, RowN>& arg_data);

        static Matrix<RowN, 1> createFromArrRow(const std::array<double, RowN>& arg_arr);
        static Matrix<1, ColN> createFromArrCol(const std::array<double, ColN>& arg_arr);
        constexpr explicit Matrix(const double& default_val = 0.0);
        explicit Matrix(const std::vector<std::vector<double>>& arg_data);

        Matrix(const Matrix<RowN, ColN>&) = default;
        Matrix(Matrix<RowN, ColN>&&) noexcept = default;

        Matrix<RowN, ColN>& operator=(const Matrix<RowN, ColN>& other);
        inline Matrix<RowN, ColN>& operator=(Matrix<RowN, ColN>&& other) noexcept = default;

        Matrix<RowN, ColN>& operator=(const std::array<std::array<double, ColN>, RowN>& other);
        Matrix<RowN, ColN>& operator=(const std::vector<std::vector<double>>& arg_data);

        Matrix<RowN, ColN>& assign_with_initializer_list(const std::initializer_list<std::initializer_list<double>> arg_data);
        Matrix<RowN, ColN>& assign_with_vector(const std::vector<std::vector<double>>& arg_data);

        bool operator==(const Matrix<RowN, ColN>& other) const;
        bool operator!=(const Matrix<RowN, ColN>& other) const;

        std::array<double, ColN>& operator[](size_t index);
        const std::array<double, ColN>& operator[](size_t index) const;

        bool is_size_equal(size_t row_num, size_t col_num) const;

        const std::array<std::array<double, ColN>, RowN>& get_data_constReference() const;
        std::array<std::array<double, ColN>, RowN>& get_data_reference();
        std::array<std::array<double, ColN>, RowN> get_data_copy() const;
        
        std::array<double, RowN> get_column(size_t col_index) const;

        /**
         * @brief Extracts a sub-matrix from the current matrix.
         *
         * @tparam SubRowN Number of rows in the sub-matrix
         * @tparam SubColN Number of columns in the sub-matrix
         * @param start_row Starting row index in the parent matrix
         * @param start_col Starting column index in the parent matrix
         * @return Matrix<SubRowN, SubColN> The extracted sub-matrix
         *
         * @throws std::out_of_range If start_row or start_col are out of bounds,
         *         or if the sub-matrix dimensions exceed the parent matrix bounds.
         */
        template <size_t SubRowN, size_t SubColN>
        Matrix<SubRowN, SubColN> get_sub_matrix(size_t start_row, size_t start_col) const;

        // negative operator
        Matrix<RowN, ColN> operator-() const;

        Matrix<RowN, ColN> operator+(const Matrix<RowN, ColN>& other) const;
        Matrix<RowN, ColN> operator-(const Matrix<RowN, ColN>& other) const;

        /**
         * @brief Matrix multiplication operator.
         * 
         * Multiplies this matrix with another matrix of compatible dimensions.
         * The number of columns in this matrix (ColN) must match the number of rows in the other matrix.
         * 
         * @param other The right-hand side matrix to multiply with
         * @return Resulting matrix of size RowN x oColN
         */
        template <size_t oColN>
        Matrix<RowN, oColN> operator*(const Matrix<ColN, oColN>& other) const;

        // scalar mul
        Matrix<RowN, ColN> operator*(double scalar) const;
        // scalar div
        Matrix<RowN, ColN> operator/(double scalar) const;
        
        Matrix<RowN, ColN>& operator+=(const Matrix<RowN, ColN>& other) noexcept;

        Matrix<RowN, ColN>& operator-=(const Matrix<RowN, ColN>& other) noexcept;

        Matrix<RowN, ColN>& operator*=(double scalar) noexcept;

        Matrix<RowN, ColN>& operator/=(double scalar);
        
        Matrix<ColN, RowN> transpose() const;


        inline std::string to_string(
            bool add_prefix = true, 
            std::string elem_sep = ", ", 
            std::string line_sep = "], \n [", 
            std::string front = "[[", 
            std::string end = "]]"
        ) const;

        inline Matrix<RowN, ColN> T() const { return transpose(); }
        inline static constexpr Matrix<RowN, ColN> O() { return Matrix<RowN, ColN>(0); }

    private:
        
        void log(const std::string& func_name, const std::string& message, const Logger::LogLevel& log_level, bool add_timestamp) {
            Logger::log(
                "Matrix<" + std::to_string(RowN) + ", " 
                    + std::to_string(ColN) + ">::" 
                    + func_name, 
                message, 
                log_level, 
                add_timestamp
            );
        }

        template <typename ExceptionType = std::runtime_error>
        [[noreturn]] void log_and_throw(const std::string& func_name, const std::string& message) {
            Logger::log_and_throw<ExceptionType>(
                "Matrix<" + std::to_string(RowN) + ", " 
                    + std::to_string(ColN) + ">::" 
                    + func_name, 
                message);
        }

}; // class Matrix

} // namespace Math

#include "Matrix.inl"

#endif // MATH_MATRIX_HPP