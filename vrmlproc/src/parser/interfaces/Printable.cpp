#include "Printable.hpp"

std::ostream& operator<<(std::ostream& os, const Printable& obj) {

    Printable::IndentationLevel indentation_level = { 0 };
    obj.Print(indentation_level);

    return os;

}