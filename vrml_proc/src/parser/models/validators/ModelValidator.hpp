#pragma once

#include <memory>

#include <result.hpp>

#include "ModelValidationError.hpp"
#include "Range.hpp"
#include "VrmlProcessingExport.hpp"

namespace vrml_proc {
	namespace parser {
		namespace model {
			namespace validator {

				VRMLPROCESSING_API inline cpp::result<void, std::shared_ptr<error::ModelValidationError>> CheckVec3fIsGreaterThanZero(const Vec3f& vector) {

					core::utils::Range<float> range;
					range.SetMinimum(0.0f);

					if (!range.CheckValueInRangeExclusive(vector.x)) {
						auto error = std::make_shared<error::Vec3fOutOfRangeError>(range, vector);
						auto innerError = std::make_shared<error::NumberOutOfRangeError<float32_t>>(range, vector.x);
						error->SetInnerError(innerError);
						return cpp::fail(error);
					}

					if (!range.CheckValueInRangeExclusive(vector.y)) {
						auto error = std::make_shared<error::Vec3fOutOfRangeError>(range, vector);
						auto innerError = std::make_shared<error::NumberOutOfRangeError<float32_t>>(range, vector.y);
						error->SetInnerError(innerError);
						return cpp::fail(error);
					}

					if (!range.CheckValueInRangeExclusive(vector.z)) {
						auto error = std::make_shared<error::Vec3fOutOfRangeError>(range, vector);
						auto innerError = std::make_shared<error::NumberOutOfRangeError<float32_t>>(range, vector.z);
						error->SetInnerError(innerError);
						return cpp::fail(error);
					}

					return {};
				}
			}
		}
	}
}
