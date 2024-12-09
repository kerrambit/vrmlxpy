#include "VRMLField.hpp"
#include "VRMLNode.hpp"

struct Visitor {

    std::ostream *output_stream;
    Printable::IndentationLevel indentation_level;

    Visitor(std::ostream *output_stream, Printable::IndentationLevel indentation_level) : output_stream(output_stream), indentation_level(indentation_level) {}

    void operator()(const std::string& string) const {
        *output_stream << Printable::CreateIndentationString(indentation_level) << "String: <" << string << ">";
        *output_stream << std::endl;
    }

    void operator()(const Vec3fArray& array) const {
        array.Print(indentation_level);
    }

    void operator()(const Int32Array& array) const {
        array.Print(indentation_level);
    }

    void operator()(float32_t value) const {
        *output_stream << Printable::CreateIndentationString(indentation_level) << "Float32: <" << value << ">";
        *output_stream << std::endl;
    }

    void operator()(int32_t value) const {
        *output_stream << Printable::CreateIndentationString(indentation_level) << "Int32: <" << value << ">";
        *output_stream << std::endl;
    }

    void operator()(const Vec3f& vector) const {
        vector.Print(indentation_level);
    }

    void operator()(const USENode& node) const {
        node.Print(indentation_level);
    }

    void operator()(const boost::recursive_wrapper<VRMLNode>& node) const {
        node.get_pointer()->Print(indentation_level);
    }

    void operator()(const std::vector<boost::variant<boost::recursive_wrapper<VRMLNode>, boost::recursive_wrapper<USENode>>>& nodes) const {
        *output_stream << Printable::CreateIndentationString(indentation_level) << "VRMLNodeArray:\n";
        *output_stream << Printable::CreateIndentationString(indentation_level + 1) << "[\n";
        
        struct PrintVisitor {
            std::ostream* output_stream;
            Printable::IndentationLevel indentation_level;

            PrintVisitor(std::ostream* output_stream, Printable::IndentationLevel indentation_level) : output_stream(output_stream), indentation_level(indentation_level) {}

            void operator()(const boost::recursive_wrapper<VRMLNode>& node) const {
                node.get_pointer()->Print(indentation_level);
            }

            void operator()(const boost::recursive_wrapper<USENode>& node) const {
                node.get_pointer()->Print(indentation_level);
            }
        };

        for (const auto& node : nodes) {
            boost::apply_visitor(PrintVisitor(output_stream, indentation_level + 2), node);
        }

        *output_stream << Printable::CreateIndentationString(indentation_level + 1) << "]";
    }
};

void VRMLField::Print(Printable::IndentationLevel indentation_level) const {
    std::string indentation_string = Printable::CreateIndentationString(indentation_level);

    *Printable::AccessStreamPointer() << indentation_string;
    *Printable::AccessStreamPointer() << "VRMLField:\n";
    *Printable::AccessStreamPointer() << Printable::CreateIndentationString(indentation_level + 1) << "Name: <" << name << ">\n";
    *Printable::AccessStreamPointer() << Printable::CreateIndentationString(indentation_level + 1) << "Value:\n";
    boost::apply_visitor(Visitor(Printable::AccessStreamPointer(), indentation_level + 2), value);
}