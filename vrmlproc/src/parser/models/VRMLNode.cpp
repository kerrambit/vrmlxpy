#include "VrmlNode.hpp"

void vrml_proc::parser::VrmlNode::Print(Printable::IndentationLevel indentationLevel) const {
    std::string indentationString = Printable::CreateIndentationString(indentationLevel);
    indentationLevel++;

    *Printable::AccessStreamPointer() << indentationString;
    *Printable::AccessStreamPointer() << "VRMLNode:\n";
    if (definitionName.has_value()) {
        *Printable::AccessStreamPointer() << Printable::CreateIndentationString(indentationLevel) << "DEF identifier: <" << definitionName.value() << ">\n";
    }
    *Printable::AccessStreamPointer() << Printable::CreateIndentationString(indentationLevel) << "Header: <" << header << ">\n";
    *Printable::AccessStreamPointer() << Printable::CreateIndentationString(indentationLevel) << "Fields:\n";
    
    for (const auto& field : fields) {
        field.Print(indentationLevel + 1);
    }
}