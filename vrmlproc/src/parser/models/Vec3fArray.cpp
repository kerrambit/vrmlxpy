#include "Vec3fArray.hpp"

void vrml_proc::parser::Vec3fArray::Print(Printable::IndentationLevel indentation_level) const {
    std::string indentation_string = Printable::CreateIndentationString(indentation_level);
    indentation_level++;

    *Printable::AccessStreamPointer() << indentation_string;
    *Printable::AccessStreamPointer() << "Vec3fArray:\n";
    *Printable::AccessStreamPointer() << Printable::CreateIndentationString(indentation_level) << "[\n";

    for (const auto& vector : vectors) {
        vector.Print(indentation_level + 1);
    }

    *Printable::AccessStreamPointer() << Printable::CreateIndentationString(indentation_level) << "]";
    *Printable::AccessStreamPointer() << std::endl;
}