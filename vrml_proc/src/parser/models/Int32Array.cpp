#include "Int32Array.hpp"

void vrml_proc::parser::Int32Array::Print(Printable::IndentationLevel indentationLevel) const {
    std::string indentationString = Printable::CreateIndentationString(indentationLevel);
    indentationLevel++;

    *Printable::AccessStreamPointer() << indentationString;
    *Printable::AccessStreamPointer() << "Int32Array:\n";
    *Printable::AccessStreamPointer() << Printable::CreateIndentationString(indentationLevel) << "[\n" << Printable::CreateIndentationString(indentationLevel + 1);

    for (const auto integer : integers) {
        *Printable::AccessStreamPointer() << "<" << integer << "> ";
    }

    *Printable::AccessStreamPointer() << "\n";
    *Printable::AccessStreamPointer() << Printable::CreateIndentationString(indentationLevel) << "]";
    *Printable::AccessStreamPointer() << std::endl;
}
