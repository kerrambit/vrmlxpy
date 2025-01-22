#pragma once

#include <memory>
#include <sstream>

#include "Error.hpp"
#include "VrmlNode.hpp"
#include <string>

namespace vrml_proc {
	namespace traversor {
		namespace error {
			class NodeTraversorError : public vrml_proc::core::error::Error {
			public:
				NodeTraversorError()
					: m_node(GetDefaultNode()) {}

				NodeTraversorError(std::shared_ptr<vrml_proc::core::error::Error> innerError, const vrml_proc::parser::VrmlNode& errorNode)
				: m_node(errorNode) {
					SetInnerError(innerError);
				}
				const vrml_proc::parser::VrmlNode& GetErrorNode() const {
					return m_node;
				}
			protected:
				std::string GetMessageInternal() const override {
					std::ostringstream oss;
					oss << "[NodeTraversorError]: when traversing node <"
						<< m_node.header << ">, unexpected error occurred!\n";
					return oss.str();
				}
			private:
				const vrml_proc::parser::VrmlNode& m_node;

				static const vrml_proc::parser::VrmlNode& GetDefaultNode() {
					static vrml_proc::parser::VrmlNode dummyNode;
					return dummyNode;
				}
			};
		}
	}
}
