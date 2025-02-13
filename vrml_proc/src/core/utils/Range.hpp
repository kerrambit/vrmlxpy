#pragma once

#include "Comparable.hpp"

#include "VrmlProcessingExport.hpp"

namespace vrml_proc {
	namespace core {
		namespace utils {

			template <contract::Comparable T>
			struct VRMLPROCESSING_API Range {

				Range(T min, T max)
					: min(min), max(max) {}

				Range()
					: min(-std::numeric_limits<T>::infinity()), max(std::numeric_limits<T>::infinity()) {}

				void SetMinimum(T min) {
					this->min = min;
				}

				void SetMaximum(T max) {
					this->max = max;
				}

				bool CheckValueInRangeExclusive(T value) {
					return !(value <= min || value >= max);
				}

				bool CheckValueInRangeInclusive(T value) {
					return !(value < min || value > max);
				}

				friend std::ostream& operator<<(std::ostream& os, const Range& range) {
					return os << "Range: { min: <" << range.min << ">, max: <" << range.max << "> }";
				}

				T max;
				T min;
			};
		}
	}
}
