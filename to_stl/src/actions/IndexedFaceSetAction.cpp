#include "IndexedFaceSetAction.hpp"

#include <functional>
#include <memory>

#include <result.hpp>

#include "ConversionContextActionMap.hpp"
#include "CoordConversionContext.hpp"
#include "GeometryAction.hpp"
#include "Logger.hpp"
#include "MeshConversionContext.hpp"
#include "Vec3fArray.hpp"
#include "VrmlFieldExtractor.hpp"
#include "VrmlNodeManager.hpp"
#include "VrmlNodeTraversor.hpp"

//class HelperCoordAction : public vrml_proc::action::ConversionContextAction<to_stl::conversion_context::CoordConversionContext> {
//public:
//	HelperCoordAction(const vrml_proc::traversor::handler::IndexedFaceSetHandler::IndexedFaceSetProperties& properties)
//		: m_properties(properties) {}
//
//	std::shared_ptr<to_stl::conversion_context::CoordConversionContext> Execute() {
//
//		// auto result = vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractByName<vrml_proc::parser::Vec3fArray>("point", m_properties.coord.get().fields);
//		// std::reference_wrapper<const vrml_proc::parser::Vec3fArray> data = result.value();
//		return std::make_shared<to_stl::conversion_context::CoordConversionContext>(5);
//	}
//
//private:
//	const vrml_proc::traversor::handler::IndexedFaceSetHandler::IndexedFaceSetProperties& m_properties;
//};

namespace to_stl {
	namespace action {

		IndexedFaceSetAction::IndexedFaceSetAction(const vrml_proc::traversor::handler::IndexedFaceSetHandler::IndexedFaceSetProperties& properties, bool containedByShape) :
			vrml_proc::action::GeometryAction(containedByShape), m_properties(properties) {}

		std::shared_ptr<vrml_proc::conversion_context::MeshConversionContext> IndexedFaceSetAction::Execute() {

			vrml_proc::core::logger::LogInfo("Execute IndexedFaceSetAction.", LOGGING_INFO);

			auto result = std::make_shared<vrml_proc::conversion_context::MeshConversionContext>();
			if (!m_containedByShape) {
				vrml_proc::core::logger::LogDebug("Return empty data because IndexedFaceSet node is not a child of a Shape node.", LOGGING_INFO);
				return result;
			}

			vrml_proc::parser::VrmlNodeManager manager;

			vrml_proc::action::ConversionContextActionMap<conversion_context::CoordConversionContext> map;
			/*map.AddAction("Coordinate", [this](const auto& refArgs, const auto& copyArgs) {
				if (refArgs.size() == 1 && refArgs[0].get().type() == typeid(std::reference_wrapper<const vrml_proc::parser::Vec3fArray>)) {
					auto coord = std::any_cast<std::reference_wrapper<const vrml_proc::parser::Vec3fArray>>(refArgs[0]);
					return std::make_shared<HelperCoordAction>(m_properties);
				}

				assert(false && "Invalid arguments for CoordinateAction");
				});*/

			const vrml_proc::parser::VrmlNode& node = m_properties.coord.get();
			vrml_proc::traversor::FullParsedVrmlNodeContext context = vrml_proc::traversor::FullParsedVrmlNodeContext(node, manager, false);

			// vrml_proc::traversor::VrmlNodeTraversor::Traverse<conversion_context::CoordConversionContext>(context, map);

			return result;
		}
	}
}
