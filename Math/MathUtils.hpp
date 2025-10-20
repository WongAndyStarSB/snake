#ifndef MATH_HPP
#define MATH_HPP

#include <string>
#include <stdexcept>
#include <cmath>
#include <vector>

namespace NS_math {
    const double PI = 3.14159265358979323846;

    bool isPrime(int n) {
        if (n <= 1) {
            return false;
        }
        if (n <= 3) {
            return true;
        }
        if (!(n % 2) || !(n % 3)) {
            return false;
        }
        for (int i = 5; i * i <= n; i += 6) {
            if (n % i == 0 // check for divisable by (6x-1)
                || n % (i + 2) == 0 // check for divisable by (6x+1)
            ) { 
                return false;
            }
        }
        return true;
        // 7(6x+1) 8(2x) 9(3x) 10(2x) 11(6x-1) 12(2x)
    }

    std::vector<int> getFirstPrimes(int num_of_primes) {
        std::vector<int> result(num_of_primes);
        int index = 0;
        int n = 2;
        while (index < 10) {
            bool isPrime = true;
            for (int i = 0; i < index; ++i) {
                if (!(n % result[i])) {
                    isPrime = false;
                }
            }
            if (isPrime) {
                result[index++] = n;
            }
            n++;
        }
        return result;
    }
}

#endif // MATH_HPP