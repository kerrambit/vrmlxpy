#include "Int32Array.hpp"

#include <ranges>

void vrml_proc::parser::Int32Array::Print(Printable::IndentationLevel indentationLevel) const {
  std::string indentationString = Printable::CreateIndentationString(indentationLevel);
  indentationLevel++;

  *Printable::AccessStreamPointer() << indentationString;
  *Printable::AccessStreamPointer() << "Int32Array (" << this << "):\n";
  *Printable::AccessStreamPointer() << Printable::CreateIndentationString(indentationLevel) << "[\n"
                                    << Printable::CreateIndentationString(indentationLevel + 1);

  for (const auto integer : integers | std::views::take(3)) {
    *Printable::AccessStreamPointer() << "<" << integer << "> ";
  }

  *Printable::AccessStreamPointer() << "\n";
  *Printable::AccessStreamPointer() << Printable::CreateIndentationString(indentationLevel) << "]";
  *Printable::AccessStreamPointer() << std::endl;
}
