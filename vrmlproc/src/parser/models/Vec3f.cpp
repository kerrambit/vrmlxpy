#include "Vec3f.hpp"

void vrml_proc::parser::Vec3f::Print(Printable::IndentationLevel indentationLevel) const {
    std::string indentationString = Printable::CreateIndentationString(indentationLevel);
    indentationLevel++;

    *Printable::AccessStreamPointer() << indentationString;
    *Printable::AccessStreamPointer() << "Vec3f:\n";
    *Printable::AccessStreamPointer() << Printable::CreateIndentationString(indentationLevel) << "( <" << x << "> <" << y << "> <" << z << "> )" << std::endl;
}