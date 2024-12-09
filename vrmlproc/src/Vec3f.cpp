#include "Vec3f.hpp"

void Vec3f::Print(Printable::IndentationLevel indentation_level) const {
    std::string indentation_string = Printable::CreateIndentationString(indentation_level);
    indentation_level++;

    *Printable::AccessStreamPointer() << indentation_string;
    *Printable::AccessStreamPointer() << "Vec3f:\n";
    *Printable::AccessStreamPointer() << Printable::CreateIndentationString(indentation_level) << "( <" << x << "> <" << y << "> <" << z << "> )" << std::endl;
}