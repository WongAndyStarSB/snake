#ifndef STRING_UTILS_HPP
#define STRING_UTILS_HPP

#include <string>
#include <sstream>
#include <functional>
#include <type_traits>
#include <vector>


namespace string_utils_ns {
    std::string add_indent(
        std::string_view txt, 
        const size_t& indent_num, 
        bool add_in_first_line_too = true,
        char indent_char = ' '
    );

    std::stringstream& add_indent(
        std::stringstream& ss, 
        const size_t& indent_num, 
        const char& indent_char = ' '
    );

    template <typename ExceptionType>
    std::string common_exceptions_to_string();

    template <typename T>
    std::string to_string(const std::vector<T>& vect, bool with_prefix = true);
    
    template <typename PtrT>
    std::string convertPtrToString(PtrT ptr);

    int convertPtrToStringX64(const void* ptr, char* return_str);

    template <typename IteratorT, typename ConverterT = std::function<std::string(decltype(*std::declval<IteratorT>()))>>
    std::string iterableToStr(IteratorT begin, IteratorT end, ConverterT converter = [](const auto& item) { return std::to_string(item); });

};



#include "StringUtils.inl"


#endif // STRING_UTILS_HPP