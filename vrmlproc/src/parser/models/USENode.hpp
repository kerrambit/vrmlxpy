#pragma once

#include <string>
#include <iostream>

#include "Printable.hpp"

namespace vrml_proc {
	namespace parser {
		struct USENode : public Printable {

			USENode()
				: Printable(std::cout) {}

			void Print(Printable::IndentationLevel indentation_level) const override;

			std::string identifier;
		};
	}
}
