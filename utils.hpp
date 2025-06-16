#ifndef UTILS_HPP
#define UTILS_HPP

#include <vector>
#include <string>
#include <unordered_map>
#include <thread>
#include <chrono>
#include <typeinfo>


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
        static std::string to_string(const std::vector<T>& vect, bool with_prefix = true) {
            std::string return_str = ((with_prefix)? ("vector<"+get_class_name<T>()+">[") : ("[")); 
            if (vect.empty()) {
                return return_str+"-empty-]";
            }
            try {
                for (size_t i = 0; i < vect.size(); ++i) {
                    return_str += vect[i].to_string(false) + ((i == vect.size()-1)? "]" : ", ");
                }
            } catch (const std::exception& e) {
                std::string msg = "Utils::vector::to_string error: " + std::string(e.what());
                Logger::log(msg, ERROR);
                throw std::runtime_error(msg);
            } catch (...) {
                std::string msg = "Utils::vector::to_string error: unknown exception";
                Logger::log(msg, ERROR);
                throw std::runtime_error(msg);
            }
            return return_str;
        }
        
        template <typename T>
        static bool have_same_elements(const std::vector<T>& vect_1, const std::vector<T>& vect_2, bool hashable = false) {
            Logger::log("Utils::vector::have_same_elements check_started", INFO);
            if (vect_1.size() != vect_2.size()) {
                Logger::log("Utils::vector::have_same_elements returned false", INFO);
                return false;
            }
            std::vector<T> vect_2_copy(vect_2);
            for (const T& vect_1_elem : vect_1) {
                auto it = std::find(vect_2_copy.begin(), vect_2_copy.end(), vect_1_elem);
                if (it == vect_2_copy.end()) {
                    Logger::log("Utils::have_same_elements returned false", INFO);
                    return false;
                } else {
                    vect_2_copy.erase(it);
                }
            }
            Logger::log("Utils::vector::have_same_elements returned true", INFO);
            return true;
        }

    }
    void delay_for_time_in_ms(const size_t t_in_ms);
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