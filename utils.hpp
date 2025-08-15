#ifndef UTILS_HPP
#define UTILS_HPP

#include <vector>
#include <string>
#include <unordered_map>
#include <thread>
#include <chrono>
#include <typeinfo>
#include <tuple>
#include <array>


namespace Utils {
    namespace helpers {
        std::string demangle_type_name(const std::string& name) noexcept;
    }

    namespace String {
        std::string char_to_string(const char c);
        std::string add_indent(const std::string& txt, const size_t n, char indent_char = ' ');
    }
    namespace Int {
        bool is_int_in_range(const size_t num, const size_t upper_lim, const size_t lower_lim);
    }
    namespace Vector {
        template <typename T>
        static bool have_same_elements(const std::vector<T>& vect_1, const std::vector<T>& vect_2, bool hashable = false) {
            if (vect_1.size() != vect_2.size()) {
                return false;
            }
            std::vector<T> vect_2_copy(vect_2);
            for (const T& vect_1_elem : vect_1) {
                auto it = std::find(vect_2_copy.begin(), vect_2_copy.end(), vect_1_elem);
                if (it == vect_2_copy.end()) {
                    return false;
                } else {
                    vect_2_copy.erase(it);
                }
            }
            return true;
        }

    }
    namespace Time {
        void delay_for_time_in_ms(const size_t& t_in_ms);
        std::tm get_current_time();
        std::array<int, 3> get_current_hour_min_sec();
        unsigned int time_minus_get_seconds(std::array<int, 3> start, std::array<int, 3> end);

        template<typename Callable, typename... Args>
        long long duration_used_in_function(Callable func, Args&&... args) {
            auto start_time = std::chrono::high_resolution_clock::now();
            func(std::forward<Args>(args)...);
            auto end_time = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
            return duration.count();
        }
    }
    void clear_terminal();
    
    template <typename T>
        std::string get_class_name(const T& obj) noexcept {
            return Utils::helpers::demangle_type_name(typeid(obj).name());
        }
        template <typename T>
        std::string get_class_name() noexcept {
            return Utils::helpers::demangle_type_name(typeid(T).name());
        }

        

        
}










#endif // UTILS_HPP
// End of file