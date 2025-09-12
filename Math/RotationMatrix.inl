#ifndef MATH_ROTATION_MATRIX_INL
#define MATH_ROTATION_MATRIX_INL


#include "MathUtils.hpp"
#include "RotationMatrix.hpp"
#include "Angle.hpp"
#include "Matrix.hpp"
#include "SquareMatrix.hpp"
#include "Logger.hpp"

#include <cmath>

using namespace NS_math;

template <size_t N>
inline RotationMatrix<N>::RotationMatrix(
    Angle rad
) : m_angle(rad), 
    SquareMatrix(
        { {std::cos(rad), -std::sin(rad)}, 
          {std::sin(rad),  std::cos(rad)}  }
    ) 
{}
template <size_t N>
inline RotationMatrix<N>::RotationMatrix(
    std::initializer_list<std::initializer_list<double>> rotation_matrix
) : SquareMatrix(rotation_matrix) {
    if (!isRotationMatrix(*this)) {
        log_and_throw<std::invalid_argument>(
            "RotationMatrix(std::initializer_list<std::initializer_list<double>> rotation_matrix)", 
            "rotation_matrix is not a valid rotation matrix"
        );
    }
}

template <size_t N>
template <size_t ColN>
NS_math::Matrix<N, ColN> RotationMatrix<N>::operator*(
    const NS_math::Matrix<N, ColN>& mat
) {
    return SquareMatrix<N>::operator*(mat);
}


template <size_t N>
Vector<N> RotationMatrix<N>::operator*(
    const Vector<N>& vect
) const {
    if (N < 5) {
        return mul_without_parallelization(vect);
    }
    // Number of threads for parallelization
    const uint8_t num_of_threads = std::thread::hardware_concurrency();
    
    if (num_of_threads == 0) { // Validate thread count
        return mul_without_parallelization(vect);
    }
    
    Vector<N> result = Vector<N>::zeroVector();
    
    std::vector<std::thread> threads;
    std::vector<Vector<N>> partial_results (num_of_threads, Vector<N>::zeroVector());
    
    auto computePartialResult = [&](int start, int end, Vector<N>& partial_result) {
        for (size_t i = start; i < end; ++i) {
            for (size_t j = 0; j < N; ++j) {
                partial_result[i] += data[i][j] * vect[j];
            }
        }
    };
    
    try {
        // Divide the workload into chunks for each thread
        int chunk_size = N / num_of_threads;
        for (int t = 0; t < num_of_threads; ++t) {
            int start = t * chunk_size;
            // The last thread handles its chunk plus any remainder
            int end = (t == num_of_threads - 1) ? N : (t + 1) * chunk_size;
            
            threads.emplace_back(computePartialResult, start, end, std::ref(partial_results[t]));
        }
        
        // Wait for all threads to complete
        for (std::thread& thread : threads) {
            thread.join();
        }
        
        // Combine partial results to form the final result
        for (int t = 0; t < num_of_threads; ++t) {
            result += partial_results[t];
        }
    } catch (const std::exception& e) {
        // Log the exception and fallback to non-parallelized multiplication
        log("operator*", 
            "Threading failed: " + std::string(e.what()) + ". Falling back to non-parallelized multiplication.", 
            Logger::LogLevel::WARNING_LOW);
        return mul_without_parallelization(vect);
    }
    
    return result;
}
template <size_t N>
Vector<N> RotationMatrix<N>::mul_without_parallelization(
    const Vector<N>& vect
) const {
    Vector<N> result = Vector<N>::zeroVector();
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < N; ++j) {
            result[i] += data[i][j] * vect[j];
        }
    }
    return result;
}

template <size_t N>
SquareMatrix<N> RotationMatrix<N>::operator*(
    const SquareMatrix<N>& sq_mat
) {
    return SquareMatrix<N>::operator*(sq_mat);
}


template <size_t N>
inline bool RotationMatrix<N>::isRotationMatrix(
    const SquareMatrix<N>& mat
) {
    if constexpr (N == 1) {
        log("is_matrix_rotationMatrix", 
            "weird usage: determining if a matrix of size *1x1* is a rotation matrix", 
            Logger::LogLevel::WARNING_LOW
        );
        if (std::abs(mat[0][0] - 1) > m_precision) {
            return false;
        } else {
            return true;
        }
    } else {
        if (std::abs(mat.det() - 1) > m_precision) {
            return false;
        }
        if constexpr (N == 2) {
            // { {cos, -sin}, 
            //   {sin, cos}  }
            if (std::abs(mat[0][0] - mat[1][1]) > m_precision // topLeft != bottomRight
                || std::abs(mat[1][0] + mat[0][1]) > m_precision // bottomLeft != negative topRight
                || std::abs(mat[0][0] * mat[0][0] + mat[1][0] * mat[1][0] - 1) > m_precision // cos^2 + sin^2 != 1
            ) {
                return false;
            }
            return true;
        } else {
            if (mat.transpose() == mat.inverse()) {
                return true;
            } else {
                return false;
            }
        }
    }
    throw std::exception("UNREACHABLE");
}


template <size_t N>
inline void RotationMatrix<N>::log(
    const std::string& where,
    const std::string& what,
    const Logger::LogLevel& lev
) {
    Logger::log(
        "RotationMatrix::" + where, 
        what,
        lev
    );
}

template <size_t N>
template <typename ExceptionType>
inline void RotationMatrix<N>::log_and_throw(
    const std::string& where,
    const std::string& what
) {
    Logger::log_and_throw(
        "RotationMatrix::" + where, 
        what
    );
}

#endif // MATH_ROTATION_MATRIX_INL