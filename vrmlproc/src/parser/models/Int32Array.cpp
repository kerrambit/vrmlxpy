#include "Int32Array.hpp"

void vrml_proc::parser::Int32Array::Print(Printable::IndentationLevel indentation_level) const {
    std::string indentation_string = Printable::CreateIndentationString(indentation_level);
    indentation_level++;

    *Printable::AccessStreamPointer() << indentation_string;
    *Printable::AccessStreamPointer() << "Int32Array:\n";
    *Printable::AccessStreamPointer() << Printable::CreateIndentationString(indentation_level) << "[\n" << Printable::CreateIndentationString(indentation_level + 1);

    for (const auto integer : integers) {
        *Printable::AccessStreamPointer() << "<" << integer << "> ";
    }

    *Printable::AccessStreamPointer() << "\n";
    *Printable::AccessStreamPointer() << Printable::CreateIndentationString(indentation_level) << "]";
    *Printable::AccessStreamPointer() << std::endl;
}
