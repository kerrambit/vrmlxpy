#pragma once

#include <iostream>
#include <string>
#include <cstdint>

#define ONE_INDENTATION_LEVEL_LENGTH 2

class Printable {

public:

	Printable(std::ostream& default_output_stream)
		: m_stream(&default_output_stream) {}

	virtual ~Printable() = default;

	using IndentationLevel = uint16_t;

	virtual void Print(IndentationLevel indentation_level) const = 0;

	friend std::ostream& operator<<(std::ostream& os, const Printable& obj);

	inline static std::string CreateIndentationString(IndentationLevel indentation_level) {
		return std::string(ONE_INDENTATION_LEVEL_LENGTH * indentation_level, ' ');
	}
protected:
	inline std::ostream* AccessStreamPointer() const {
		return m_stream;
	}
private:
	std::ostream* m_stream;
};
