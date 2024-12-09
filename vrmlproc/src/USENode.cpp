#include "USENode.hpp"

void USENode::Print(Printable::IndentationLevel indentation_level) const {
    std::string indentation_string = Printable::CreateIndentationString(indentation_level);
    indentation_level++;

    *Printable::AccessStreamPointer() << indentation_string;
    *Printable::AccessStreamPointer() << "USENode:\n";
    *Printable::AccessStreamPointer() << Printable::CreateIndentationString(indentation_level) << "Identifier: <" << identifier << ">";
    *Printable::AccessStreamPointer() << std::endl;
}