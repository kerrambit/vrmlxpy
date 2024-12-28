#pragma once

#include <optional>
#include <string>
#include <type_traits>
#include <typeinfo>

#include <boost/variant.hpp>

#include "VrmlField.hpp"

template <typename T>
struct ExtractorVisitor;

template <typename T>
struct VariantVisitor;

namespace vrml_proc {
	namespace traversor {
		namespace utils {
			namespace VrmlFieldExtractor {

				template <typename T>
				static std::optional<T> ExtractByName(const std::string& name, const std::vector<vrml_proc::parser::VrmlField>& fields) {
					for (const auto& field : fields) {
						if (field.name == name) {
							ExtractorVisitor<T> visitor;
							return boost::apply_visitor(visitor, field.value);
						}
					}
					return {};
				}

                template <typename T>
                static std::optional<T> ExtractFromVariant(const boost::variant<boost::recursive_wrapper<vrml_proc::parser::VrmlNode>, boost::recursive_wrapper<vrml_proc::parser::UseNode>>& variant) {

                    VariantVisitor<T> visitor;
                    return boost::apply_visitor(visitor, variant);
                }
			}
		}
	}
}

template <typename T>
struct ExtractorVisitor : public boost::static_visitor<std::optional<T>> {

    std::optional<T> operator()(const std::string& value) const {
        std::cout << "I am here string" << std::endl;
        if constexpr (std::is_same<T, std::string>::value) {
            return value;
        }
        return {};
    }

    std::optional<T> operator()(bool value) const {
        std::cout << "I am here bool" << std::endl;
        if constexpr (std::is_same<T, bool>::value) {
            return value;
        }
        return {};
    }

    std::optional<T> operator()(const vrml_proc::parser::Vec3fArray& value) const {
        std::cout << "I am here vec3f array" << std::endl;
        if (value.vectors.size() == 0) {
            if constexpr (std::is_same<T, vrml_proc::parser::Vec3fArray>::value) {
                return value;
            }
            else if constexpr (std::is_same<T, vrml_proc::parser::Int32Array>::value) {
                return T{};
            }
            else if constexpr (std::is_same<T, std::vector<boost::variant<boost::recursive_wrapper<vrml_proc::parser::VrmlNode>, boost::recursive_wrapper<vrml_proc::parser::UseNode>>>>::value) {
                return T{};
            }
            else {
                return {};
            }
        }

        if constexpr (std::is_same<T, vrml_proc::parser::Vec3fArray>::value) {
            return value;
        }
        return {};
    }

    std::optional<T> operator()(const vrml_proc::parser::Int32Array& value) const {
        std::cout << "I am here intarray" << std::endl;
        if constexpr (std::is_same<T, vrml_proc::parser::Int32Array>::value) {
            return value;
        }
        return {};
    }

    std::optional<T> operator()(float value) const {
        std::cout << "I am here float" << std::endl;
        if constexpr (std::is_same<T, float>::value) {
            return value;
        }
        return {};
    }

    std::optional<T> operator()(int32_t value) const {
        std::cout << "int32" << std::endl;
        if constexpr (std::is_same<T, int32_t>::value) {
            return value;
        }
        return {};
    }

    std::optional<T> operator()(const vrml_proc::parser::Vec3f& value) const {
        std::cout << "vec3f" << std::endl;
        if constexpr (std::is_same<T, vrml_proc::parser::Vec3f>::value) {
            return value;
        }
        return {};
    }

    std::optional<T> operator()(const vrml_proc::parser::Vec4f& value) const {
        std::cout << "I am here vec4f" << std::endl;
        if constexpr (std::is_same<T, vrml_proc::parser::Vec4f>::value) {
            return value;
        }
        return {};
    }

    std::optional<T> operator()(const vrml_proc::parser::UseNode& value) const {
        std::cout << "I am here usenode" << std::endl;
        if constexpr (std::is_same<T, vrml_proc::parser::UseNode>::value) {
            return value;
        }
        return {};
    }

    std::optional<T> operator()(const boost::recursive_wrapper<vrml_proc::parser::VrmlNode>& node) const {
        std::cout << "I am here vrmlnode (recursive wrapper)" << std::endl;

        if constexpr (std::is_same<T, boost::recursive_wrapper<vrml_proc::parser::VrmlNode>>::value) {
            return node;
        }

        return {};
    }

    std::optional<T> operator()(const vrml_proc::parser::VrmlNode& node) const {
        std::cout << "I am here vrmlnode (non-recursive)" << std::endl;

        if constexpr (std::is_same<T, vrml_proc::parser::VrmlNode>::value) {
            return node;
        }

        if constexpr (std::is_same<T, boost::recursive_wrapper<vrml_proc::parser::VrmlNode>>::value) {
            return boost::recursive_wrapper<vrml_proc::parser::VrmlNode>(node);
        }

        return {};
    }


    std::optional<T> operator()(const std::vector<boost::variant<boost::recursive_wrapper<vrml_proc::parser::VrmlNode>, boost::recursive_wrapper<vrml_proc::parser::UseNode>>>& value) const {
        std::cout << "I am her arraay" << std::endl;
        if constexpr (std::is_same<T, std::vector<boost::variant<boost::recursive_wrapper<vrml_proc::parser::VrmlNode>, boost::recursive_wrapper<vrml_proc::parser::UseNode>>>>::value) {
            return value;
        }
        return {};
    }

    template <typename U>
    std::optional<T> operator()(const U&) const {
        std::cout << "Type mismatch: expected '" << typeid(T).name()
            << "' but got '" << typeid(U).name() << "'" << std::endl;
        return {};
    }
};


template <typename T>
struct VariantVisitor : public boost::static_visitor<std::optional<T>> {

    std::optional<T> operator()(const vrml_proc::parser::UseNode& node) const {
        std::cout << "I am here usenode" << std::endl;
        if constexpr (std::is_same<T, vrml_proc::parser::UseNode>::value) {
            return node;
        }

        if constexpr (std::is_same<T, boost::recursive_wrapper<vrml_proc::parser::UseNode>>::value) {
            return boost::recursive_wrapper<vrml_proc::parser::UseNode>(node);
        }

        return {};
    }

    std::optional<T> operator()(const boost::recursive_wrapper<vrml_proc::parser::UseNode>& node) const {
        std::cout << "I am here udenode (recursive wrapper)" << std::endl;

        if constexpr (std::is_same<T, boost::recursive_wrapper<vrml_proc::parser::UseNode>>::value) {
            return node;
        }

        return {};
    }

    std::optional<T> operator()(const boost::recursive_wrapper<vrml_proc::parser::VrmlNode>& node) const {
        std::cout << "I am here vrmlnode (recursive wrapper)" << std::endl;

        if constexpr (std::is_same<T, boost::recursive_wrapper<vrml_proc::parser::VrmlNode>>::value) {
            return node;
        }

        return {};
    }

    std::optional<T> operator()(const vrml_proc::parser::VrmlNode& node) const {
        std::cout << "I am here vrmlnode (non-recursive)" << std::endl;

        if constexpr (std::is_same<T, vrml_proc::parser::VrmlNode>::value) {
            return node;
        }

        if constexpr (std::is_same<T, boost::recursive_wrapper<vrml_proc::parser::VrmlNode>>::value) {
            return boost::recursive_wrapper<vrml_proc::parser::VrmlNode>(node);
        }

        return {};
    }

    template <typename U>
    std::optional<T> operator()(const U&) const {
        std::cout << "Type mismatch: expected '" << typeid(T).name()
            << "' but got '" << typeid(U).name() << "'" << std::endl;
        return {};
    }
};
