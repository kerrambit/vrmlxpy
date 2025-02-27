#include "Vec2fArray.hpp"

#include <ranges>

void vrml_proc::parser::Vec2fArray::Print(Printable::IndentationLevel indentationLevel) const {
    std::string indentationString = Printable::CreateIndentationString(indentationLevel);
    indentationLevel++;

    *Printable::AccessStreamPointer() << indentationString;
    *Printable::AccessStreamPointer() << "Vec2fArray (" << this << "):\n";
    *Printable::AccessStreamPointer() << Printable::CreateIndentationString(indentationLevel) << "[\n";

    for (const auto& vector : vectors | std::views::take(3)) {
        vector.Print(indentationLevel + 1);
    }

    *Printable::AccessStreamPointer() << Printable::CreateIndentationString(indentationLevel) << "]";
    *Printable::AccessStreamPointer() << std::endl;
}