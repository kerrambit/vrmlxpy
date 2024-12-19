#include "Vec4f.hpp"

void vrml_proc::parser::Vec4f::Print(Printable::IndentationLevel indentationLevel) const {
    std::string indentationString = Printable::CreateIndentationString(indentationLevel);
    indentationLevel++;

    *Printable::AccessStreamPointer() << indentationString;
    *Printable::AccessStreamPointer() << "Vec4f:\n";
    *Printable::AccessStreamPointer() << Printable::CreateIndentationString(indentationLevel) << "( <" << x << "> <" << y << "> <" << z << "> <" << w << "> )" << std::endl;
}