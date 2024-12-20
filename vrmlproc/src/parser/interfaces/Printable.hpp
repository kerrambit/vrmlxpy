#pragma once

#include <iostream>
#include <string>
#include <cstdint>

#include "VrmlProcessingExport.hpp"

#define ONE_INDENTATION_LEVEL_LENGTH 2

class VRMLPROCESSING_API Printable {

public:

	Printable(std::ostream& defaultOutputStream)
		: m_stream(&defaultOutputStream) {}

	virtual ~Printable() = default;

	using IndentationLevel = uint16_t;

	virtual void Print(IndentationLevel indentationLevel) const = 0;

	VRMLPROCESSING_API friend std::ostream& operator<<(std::ostream& os, const Printable& obj);

	inline static std::string CreateIndentationString(IndentationLevel indentationLevel) {
		return std::string(ONE_INDENTATION_LEVEL_LENGTH * indentationLevel, ' ');
	}
protected:
	inline std::ostream* AccessStreamPointer() const {
		return m_stream;
	}
private:
	std::ostream* m_stream;
};
