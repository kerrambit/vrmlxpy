#include "VRMLField.hpp"
#include "VRMLNode.hpp"

struct Visitor {

    std::ostream *outputStream;
    Printable::IndentationLevel indentationLevel;

    Visitor(std::ostream *outputStream, Printable::IndentationLevel indentationLevel) : outputStream(outputStream), indentationLevel(indentationLevel) {}

    void operator()(const std::string& string) const {
        *outputStream << Printable::CreateIndentationString(indentationLevel) << "String: <" << string << ">";
        *outputStream << std::endl;
    }

    void operator()(const bool& boolean) const {
        *outputStream << Printable::CreateIndentationString(indentationLevel) << "Boolean: <" << std::boolalpha << boolean << ">";
        *outputStream << std::endl;
    }

    void operator()(const vrml_proc::parser::Vec3fArray& array) const {
        array.Print(indentationLevel);
    }

    void operator()(const vrml_proc::parser::Int32Array& array) const {
        array.Print(indentationLevel);
    }

    void operator()(vrml_proc::parser::float32_t value) const {
        *outputStream << Printable::CreateIndentationString(indentationLevel) << "Float32: <" << value << ">";
        *outputStream << std::endl;
    }

    void operator()(int32_t value) const {
        *outputStream << Printable::CreateIndentationString(indentationLevel) << "Int32: <" << value << ">";
        *outputStream << std::endl;
    }

    void operator()(const vrml_proc::parser::Vec3f& vector) const {
        vector.Print(indentationLevel);
    }

    void operator()(const vrml_proc::parser::Vec4f& vector) const {
        vector.Print(indentationLevel);
    }

    void operator()(const vrml_proc::parser::USENode& node) const {
        node.Print(indentationLevel);
    }

    void operator()(const boost::recursive_wrapper<vrml_proc::parser::VRMLNode>& node) const {
        node.get_pointer()->Print(indentationLevel);
    }

    void operator()(const std::vector<boost::variant<boost::recursive_wrapper<vrml_proc::parser::VRMLNode>, boost::recursive_wrapper<vrml_proc::parser::USENode>>>& nodes) const {
        *outputStream << Printable::CreateIndentationString(indentationLevel) << "VRMLNodeArray:\n";
        *outputStream << Printable::CreateIndentationString(indentationLevel + 1) << "[\n";
        
        struct PrintVisitor {
            std::ostream* outputStream;
            Printable::IndentationLevel indentationLevel;

            PrintVisitor(std::ostream* outputStream, Printable::IndentationLevel indentationLevel) : outputStream(outputStream), indentationLevel(indentationLevel) {}

            void operator()(const boost::recursive_wrapper<vrml_proc::parser::VRMLNode>& node) const {
                node.get_pointer()->Print(indentationLevel);
            }

            void operator()(const boost::recursive_wrapper<vrml_proc::parser::USENode>& node) const {
                node.get_pointer()->Print(indentationLevel);
            }
        };

        for (const auto& node : nodes) {
            boost::apply_visitor(PrintVisitor(outputStream, indentationLevel + 2), node);
        }

        *outputStream << Printable::CreateIndentationString(indentationLevel + 1) << "]";
    }
};

void vrml_proc::parser::VRMLField::Print(Printable::IndentationLevel indentationLevel) const {
    std::string indentation_string = Printable::CreateIndentationString(indentationLevel);
    *Printable::AccessStreamPointer() << indentation_string;
    *Printable::AccessStreamPointer() << "VRMLField:\n";
    *Printable::AccessStreamPointer() << Printable::CreateIndentationString(indentationLevel + 1) << "Name: <" << name << ">\n";
    *Printable::AccessStreamPointer() << Printable::CreateIndentationString(indentationLevel + 1) << "Value:\n";
    boost::apply_visitor(Visitor(Printable::AccessStreamPointer(), indentationLevel + 2), value);
}