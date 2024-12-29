#pragma once

namespace vrml_proc {
	namespace conversion_context {

		class Mergeable {
		public:
			virtual ~Mergeable() = default;

			virtual void Merge(Mergeable* other) = 0;
		};
	}
}
