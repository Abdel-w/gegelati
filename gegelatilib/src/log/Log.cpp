#include "log/Log.hpp"

#include <iostream>

Logger Logger::operator<<(std::ostream &(*manip)(std::ostream &)) {
    manip(*out);
    return *this;
}




