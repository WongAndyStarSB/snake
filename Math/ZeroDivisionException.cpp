#include "ZeroDivisionException.hpp"

#include <string_view>
#include "../Logger/Logger.hpp"

namespace NS_math {
    ZeroDivisionException::ClassInitializer::ClassInitializer() {
        Logger::addTypeStringBond<ZeroDivisionException>("Math::ZeroDivisionException");
    }

    ZeroDivisionException::ClassInitializer ZeroDivisionException::s_initializer;

    ZeroDivisionException::ZeroDivisionException(const char* message)
        : m_msg((message && *message) ? message : s_default_msg) 
    {}
    ZeroDivisionException::ZeroDivisionException(std::string_view message)
        : m_msg(message.empty() ? s_default_msg : message) 
    {}
    const char* ZeroDivisionException::what() const throw() {
        return m_msg.c_str();
    }
}