#ifndef MATH_ROTATION_MATRIX
#define MATH_ROTATION_MATRIX


#include "MathUtils.hpp"
#include "SquareMatrix.hpp"
#include "Matrix.hpp"
#include "Angle.hpp"
#include "Vector.hpp"
#include "../Logger/Logger.hpp"

namespace Math {

template <size_t N>
class RotationMatrix : protected SquareMatrix<N> {

    static_assert(N > 0, "RotationMatrix: N(size) must be positive");

    private:
        Angle m_angle;

    public:
        double m_precision = 1e-6;
    
    private:
        void log(const std::string& where, const std::string& what, const Logger::LogLevel& lev);

        template <typename ExceptionType>
        [[noreturn]] void log_and_throw(const std::string& where, const std::string& what);

    public:
        explicit RotationMatrix(Angle rad);
        
        RotationMatrix(std::initializer_list<std::initializer_list<double>> rotation_matrix);

        inline RotationMatrix(const RotationMatrix<N>& other) = default;
        inline RotationMatrix(RotationMatrix<N>&& other) = default;

        inline Angle getAngle() const { return m_angle; }
        inline const Angle& getAngleConstReference() const { return m_angle; }
        inline Angle& getAngleReference() { return m_angle; }

        inline Angle& setAngle(const Angle& rad) { m_angle = rad; }

        template <size_t ColN>
        Matrix<N, ColN> operator*(const Matrix<N, ColN>& mat);

        Vector<N> operator*(const Vector<N>& vect) const;
        Vector<N> mul_without_parallelization(const Vector<N>& vect) const;
        

        SquareMatrix<N> operator*(const SquareMatrix<N>& sq_mat);
    
    public:
        static bool isRotationMatrix(const SquareMatrix<N>& mat);
    

}; // class RotationMatrix
}

#endif // MATH_ROTATION_MATRIX