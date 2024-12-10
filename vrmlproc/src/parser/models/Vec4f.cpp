#include "Vec4f.hpp"

void vrml_proc::parser::Vec4f::Print(Printable::IndentationLevel indentation_level) const {
    std::string indentation_string = Printable::CreateIndentationString(indentation_level);
    indentation_level++;

    *Printable::AccessStreamPointer() << indentation_string;
    *Printable::AccessStreamPointer() << "Vec4f:\n";
    *Printable::AccessStreamPointer() << Printable::CreateIndentationString(indentation_level) << "( <" << x << "> <" << y << "> <" << z << "> <" << w << "> )" << std::endl;
}