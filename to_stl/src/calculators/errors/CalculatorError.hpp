#pragma once

#include <memory>
#include <sstream>
#include <string>

#include "Error.hpp"

namespace to_stl::calculator::error {

	class CalculatorError : public vrml_proc::core::error::Error {
	protected:
		virtual std::string GetMessageInternal() const {
			return "[CalculatorError]";
		}
	};

	class PropertiesError : public vrml_proc::core::error::Error {
	public:
		PropertiesError(std::shared_ptr<vrml_proc::core::error::Error> innerError) {
			SetInnerError(innerError);
		}
		PropertiesError() = default;
	protected:
		virtual std::string GetMessageInternal() const {
			return "[PropertiesError]: some of the properties passed to the calculator are not valid!\n";
		}
	};

	class BoxCalculatorError : public CalculatorError {
	public:
		BoxCalculatorError(std::shared_ptr<vrml_proc::core::error::Error> innerError) {
			SetInnerError(innerError);
		}
		BoxCalculatorError() = default;
	protected:
		std::string GetMessageInternal() const override {
			std::ostringstream stream;
			stream << CalculatorError::GetMessageInternal() << "[BoxCalculator]: error occured!\n";
			return stream.str();
		}
	};
}
