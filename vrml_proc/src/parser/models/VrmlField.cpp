#include "VrmlField.hpp"
#include "VrmlNode.hpp"

struct Visitor {
  std::ostream* outputStream;
  Printable::IndentationLevel indentationLevel;

  Visitor(std::ostream* outputStream, Printable::IndentationLevel indentationLevel)
      : outputStream(outputStream), indentationLevel(indentationLevel) {}

  void operator()(const std::string& string) const {
    *outputStream << Printable::CreateIndentationString(indentationLevel) << "String (" << &string << "): <" << string
                  << ">";
    *outputStream << std::endl;
  }

  void operator()(const bool& boolean) const {
    *outputStream << Printable::CreateIndentationString(indentationLevel) << "Boolean (" << &boolean << "): <"
                  << std::boolalpha << boolean << ">";
    *outputStream << std::endl;
  }

  void operator()(const vrml_proc::parser::Vec2fArray& array) const { array.Print(indentationLevel); }

  void operator()(const vrml_proc::parser::Vec3fArray& array) const { array.Print(indentationLevel); }

  void operator()(const vrml_proc::parser::Int32Array& array) const { array.Print(indentationLevel); }

  void operator()(vrml_proc::parser::float32_t value) const {
    *outputStream << Printable::CreateIndentationString(indentationLevel) << "Float32 (" << &value << "): <" << value
                  << ">";
    *outputStream << std::endl;
  }

  void operator()(int32_t value) const {
    *outputStream << Printable::CreateIndentationString(indentationLevel) << "Int32 (" << &value << "): <" << value
                  << ">";
    *outputStream << std::endl;
  }

  void operator()(const vrml_proc::parser::Vec2f& vector) const { vector.Print(indentationLevel); }

  void operator()(const vrml_proc::parser::Vec3f& vector) const { vector.Print(indentationLevel); }

  void operator()(const vrml_proc::parser::Vec4f& vector) const { vector.Print(indentationLevel); }

  void operator()(const vrml_proc::parser::UseNode& node) const { node.Print(indentationLevel); }

  void operator()(const boost::recursive_wrapper<vrml_proc::parser::VrmlNode>& node) const {
    node.get_pointer()->Print(indentationLevel);
  }

  void operator()(
      const std::vector<boost::variant<boost::recursive_wrapper<vrml_proc::parser::VrmlNode>,
                                       boost::recursive_wrapper<vrml_proc::parser::UseNode>>>& nodes) const {
    *outputStream << Printable::CreateIndentationString(indentationLevel) << "VRMLNodeArray (" << &nodes << "):\n";
    *outputStream << Printable::CreateIndentationString(indentationLevel + 1) << "[\n";

    struct PrintVisitor {
      std::ostream* outputStream;
      Printable::IndentationLevel indentationLevel;

      PrintVisitor(std::ostream* outputStream, Printable::IndentationLevel indentationLevel)
          : outputStream(outputStream), indentationLevel(indentationLevel) {}

      void operator()(const boost::recursive_wrapper<vrml_proc::parser::VrmlNode>& node) const {
        node.get_pointer()->Print(indentationLevel);
      }

      void operator()(const boost::recursive_wrapper<vrml_proc::parser::UseNode>& node) const {
        node.get_pointer()->Print(indentationLevel);
      }
    };

    for (const auto& node : nodes) {
      boost::apply_visitor(PrintVisitor(outputStream, indentationLevel + 2), node);
    }

    *outputStream << Printable::CreateIndentationString(indentationLevel + 1) << "]";
  }
};

void vrml_proc::parser::VrmlField::Print(Printable::IndentationLevel indentationLevel) const {
  std::string indentation_string = Printable::CreateIndentationString(indentationLevel);
  *Printable::AccessStreamPointer() << indentation_string;
  *Printable::AccessStreamPointer() << "VRMLField (" << this << "):\n";
  *Printable::AccessStreamPointer() << Printable::CreateIndentationString(indentationLevel + 1) << "Name: <" << name
                                    << ">\n";
  *Printable::AccessStreamPointer() << Printable::CreateIndentationString(indentationLevel + 1) << "Value:\n";
  boost::apply_visitor(Visitor(Printable::AccessStreamPointer(), indentationLevel + 2), value);
}