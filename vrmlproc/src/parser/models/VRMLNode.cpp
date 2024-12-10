#include "VRMLNode.hpp"

void vrml_proc::parser::VRMLNode::Print(Printable::IndentationLevel indentation_level) const {
    std::string indentation_string = Printable::CreateIndentationString(indentation_level);
    indentation_level++;

    *Printable::AccessStreamPointer() << indentation_string;
    *Printable::AccessStreamPointer() << "VRMLNode:\n";
    if (definition_name.has_value()) {
        *Printable::AccessStreamPointer() << Printable::CreateIndentationString(indentation_level) << "DEF identifier: <" << definition_name.value() << ">\n";
    }
    *Printable::AccessStreamPointer() << Printable::CreateIndentationString(indentation_level) << "Header: <" << header << ">\n";
    *Printable::AccessStreamPointer() << Printable::CreateIndentationString(indentation_level) << "Fields:\n";
    
    for (const auto& field : fields) {
        field.Print(indentation_level + 1);
    }
}