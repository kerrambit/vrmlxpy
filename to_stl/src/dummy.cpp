#include "dummy.hpp"

#include <iostream>

#include <Logger.hpp>

namespace to_stl {
	void Start() {
		vrml_proc::core::logger::Log("hello", vrml_proc::core::logger::Level::Debug, "fff", 15, "fewfwf");
	}
}

