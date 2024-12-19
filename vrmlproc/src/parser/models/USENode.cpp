#include "UseNode.hpp"

void vrml_proc::parser::UseNode::Print(Printable::IndentationLevel indentationLevel) const {
    std::string indentationString = Printable::CreateIndentationString(indentationLevel);
    indentationLevel++;

    *Printable::AccessStreamPointer() << indentationString;
    *Printable::AccessStreamPointer() << "USENode:\n";
    *Printable::AccessStreamPointer() << Printable::CreateIndentationString(indentationLevel) << "Identifier: <" << identifier << ">";
    *Printable::AccessStreamPointer() << std::endl;
}