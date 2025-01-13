#pragma once

#include <string>
#include <iostream>

#include "Printable.hpp"
#include "VrmlProcessingExport.hpp"

namespace vrml_proc {
	namespace parser {
		struct VRMLPROCESSING_API UseNode : public Printable {

			UseNode()
				: Printable(std::cout) {}

			void Print(Printable::IndentationLevel indentationLevel) const override;

			std::string identifier;
		};
	}
}
