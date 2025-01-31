#include "Vec2f.hpp"

void vrml_proc::parser::Vec2f::Print(Printable::IndentationLevel indentationLevel) const {
    std::string indentationString = Printable::CreateIndentationString(indentationLevel);
    indentationLevel++;

    *Printable::AccessStreamPointer() << indentationString;
    *Printable::AccessStreamPointer() << "Vec2f (" << this << "):\n";
    *Printable::AccessStreamPointer() << Printable::CreateIndentationString(indentationLevel) << "( <" << u << "> <" << v << "> )" << std::endl;
}