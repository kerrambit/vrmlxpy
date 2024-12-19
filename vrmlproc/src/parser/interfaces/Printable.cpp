#include "Printable.hpp"

std::ostream& operator<<(std::ostream& os, const Printable& obj) {

    Printable::IndentationLevel indentationLevel = { 0 };
    obj.Print(indentationLevel);

    return os;

}