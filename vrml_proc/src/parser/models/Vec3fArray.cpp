#include "Vec3fArray.hpp"

void vrml_proc::parser::Vec3fArray::Print(Printable::IndentationLevel indentationLevel) const {
    std::string indentationString = Printable::CreateIndentationString(indentationLevel);
    indentationLevel++;

    *Printable::AccessStreamPointer() << indentationString;
    *Printable::AccessStreamPointer() << "Vec3fArray (" << this << "):\n";
    *Printable::AccessStreamPointer() << Printable::CreateIndentationString(indentationLevel) << "[\n";

    for (const auto& vector : vectors) {
        vector.Print(indentationLevel + 1);
    }

    *Printable::AccessStreamPointer() << Printable::CreateIndentationString(indentationLevel) << "]";
    *Printable::AccessStreamPointer() << std::endl;
}