#include "utils.hpp"
#include "Logger.hpp"


std::string Utils::String::char_to_string(const char c) {
    return std::string(1, c);
}

std::string Utils::String::add_indent(const std::string& txt, const size_t n, char indent_char) {
    std::string indent_string(n, indent_char);
    std::string return_txt = indent_string;
    for (size_t i = 0; i < txt.size(); ++i) {
        return_txt += txt[i];
        if (txt[i] == '\n' && i + 1 < txt.size()) {
            return_txt += indent_string;
        }
    }
    return return_txt;
}

bool Utils::Int::is_int_in_range(const size_t num, const size_t upper_lim, const size_t lower_lim = 0) {
    if (lower_lim > upper_lim) {
        const std::string msg = 
            "lower_lim (val:" 
            + std::to_string(lower_lim)
            + ") should not be larger than upper_lim (val:" 
            + std::to_string(upper_lim)+")\n";
        throw std::runtime_error(msg.c_str());
    }
    return (num >= lower_lim && num <= upper_lim);
}

void Utils::delay_for_time_in_ms(const size_t t_in_ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(t_in_ms));
}

void Utils::clear_terminal() {
    Logger::log("clear_terminal", INFO);
    #ifdef _WIN32
        system("cls");
    #else
        #ifdef linux
            system("clear");
        #endif
    #endif
}

std::string Utils::helpers::demangle_type_name(const std::string& name) noexcept {
    const std::string class_prefix = "class ";
    if (name.find(class_prefix) == 0)
        return name.substr(class_prefix.length());
    const std::string struct_prefix = "struct ";
    if (name.find(struct_prefix) == 0)
        return name.substr(struct_prefix.length());
    return name;
}