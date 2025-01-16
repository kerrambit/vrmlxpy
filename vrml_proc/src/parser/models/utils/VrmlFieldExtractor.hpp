#pragma once

#include <optional>
#include <string>
#include <type_traits>
#include <typeinfo>
#include  <functional>

#include <boost/variant.hpp>

#include <result.hpp>

#include "VrmlField.hpp"
#include "VrmlNodeManager.hpp"
#include "TypeToString.hpp"
#include "VrmlProcessingExport.hpp"

template <typename T>
struct ExtractorVisitor;

template <typename T>
struct VariantVisitor;

namespace vrml_proc {
    namespace parser {
        namespace model {
            namespace utils {
                namespace VrmlFieldExtractor {

                    VRMLPROCESSING_API inline bool IsNamePresent(const std::string& name, const std::vector<vrml_proc::parser::VrmlField>& fields) {
                        for (const auto& field : fields) {
                            if (field.name == name) {
                                return true;
                            }
                        }
                        return false;
                    }

                    enum class ExtractByNameError {
                        FieldNotFound,
                        ValidationError
                    };

                    template <typename T>
                    VRMLPROCESSING_API inline cpp::result<std::reference_wrapper<const T>, ExtractByNameError> ExtractByNameExtended(const std::string& name, const std::vector<vrml_proc::parser::VrmlField>& fields, std::string& invalidType) {
                        for (const auto& field : fields) {
                            if (field.name == name) {

                                ExtractorVisitor<T> visitor;
                                auto result = boost::apply_visitor(visitor, field.value);

                                if (result.has_value()) {
                                    return result.value();

                                }
                                else {
                                    if (result.error().has_value()) {
                                        invalidType = result.error().value();
                                    }
                                    return cpp::fail(ExtractByNameError::ValidationError);
                                }
                            }
                        }
                        return cpp::fail(ExtractByNameError::FieldNotFound);
                    }

                    template <typename T>
                    VRMLPROCESSING_API inline cpp::result<std::reference_wrapper<const T>, ExtractByNameError> ExtractByName(const std::string& name, const std::vector<vrml_proc::parser::VrmlField>& fields) {
                        std::string invalidType;
                        return ExtractByNameExtended<T>(name, fields, invalidType);
                    }

                    enum class ExtractVrmlNodeError {
                        FieldNotFound,
                        ValidationError,
                        UnknownUseNode
                    };


                    VRMLPROCESSING_API inline cpp::result<std::reference_wrapper<const vrml_proc::parser::VrmlNode>, ExtractVrmlNodeError> ExtractVrmlNodeExtended(const std::string& name, const std::vector<vrml_proc::parser::VrmlField>& fields, const vrml_proc::parser::VrmlNodeManager& manager, std::string& invalidType, std::string& useId) {
                        if (IsNamePresent(name, fields)) {

                            std::string error = "";
                            auto vrmlNode = ExtractByNameExtended<vrml_proc::parser::VrmlNode>(name, fields, error);
                            if (vrmlNode.has_value()) {
                                return vrmlNode.value();
                            }

                            auto useNode = ExtractByName<vrml_proc::parser::UseNode>(name, fields);
                            if (useNode.has_value()) {
                                auto managerFound = manager.GetDefinitionNode(useNode.value().get().identifier);
                                if (managerFound != nullptr) {
                                    return *managerFound;
                                }
                                else {
                                    useId = useNode.value().get().identifier;
                                    return cpp::fail(ExtractVrmlNodeError::UnknownUseNode);
                                }
                            }
                            
                            invalidType = error;
                            return cpp::fail(ExtractVrmlNodeError::ValidationError);
                        }
                        else {
                            return cpp::fail(ExtractVrmlNodeError::FieldNotFound);
                        }
                    }

                    VRMLPROCESSING_API inline cpp::result<std::reference_wrapper<const vrml_proc::parser::VrmlNode>, ExtractVrmlNodeError> ExtractVrmlNode(const std::string& name, const std::vector<vrml_proc::parser::VrmlField>& fields, const vrml_proc::parser::VrmlNodeManager& manager) {
                        std::string invalidType; std::string useId;
                        return ExtractVrmlNodeExtended(name, fields, manager, invalidType, useId);
                    }

                    template <typename T>
                    VRMLPROCESSING_API inline std::optional<T> ExtractFromVariantExtended(const boost::variant<boost::recursive_wrapper<vrml_proc::parser::VrmlNode>, boost::recursive_wrapper<vrml_proc::parser::UseNode>>& variant, std::string& invalidType) {
                        VariantVisitor<T> visitor;
                        auto result = boost::apply_visitor(visitor, variant);
                        if (result.has_value()) {
                            return result.value();
                        }

                        if (result.error().has_value()) {
                            invalidType = result.error().value();
                        }

                        return {};
                    }

                    template <typename T>
                    VRMLPROCESSING_API inline std::optional<T> ExtractFromVariant(const boost::variant<boost::recursive_wrapper<vrml_proc::parser::VrmlNode>, boost::recursive_wrapper<vrml_proc::parser::UseNode>>& variant) {
                        std::string invalidType;
                        return ExtractFromVariantExtended<T>(variant, invalidType);
                    }
                }
            }
        }
    }
}

template <typename T>
struct ExtractorVisitor : public boost::static_visitor<cpp::result<std::reference_wrapper<const T>, std::optional<std::string>>> {

    cpp::result<std::reference_wrapper<const T>, std::optional<std::string>> operator()(const std::string& value) const {
        std::cout << "I am here string" << std::endl;
        if constexpr (std::is_same<T, std::string>::value) {
            return std::cref(value);
        }
        return cpp::fail(std::optional<std::string>(vrml_proc::core::utils::TypeToString<std::string>()));
    }

    cpp::result<std::reference_wrapper<const T>, std::optional<std::string>> operator()(bool value) const {
        std::cout << "I am here bool" << std::endl;
        if constexpr (std::is_same<T, bool>::value) {
            return std::cref(value);
        }
        return cpp::fail(std::optional<std::string>(vrml_proc::core::utils::TypeToString<bool>()));
    }

    cpp::result<std::reference_wrapper<const T>, std::optional<std::string>> operator()(const vrml_proc::parser::Vec3fArray& value) const {
        std::cout << "I am here vec3f array" << std::endl;
        if (value.vectors.size() == 0) {
            if constexpr (std::is_same<T, vrml_proc::parser::Vec3fArray>::value) {
                return std::cref(value);
            }
            else if constexpr (std::is_same<T, vrml_proc::parser::Int32Array>::value) {
                static const T empty_value{};  // Local static object
                return std::cref(empty_value);
            }
            else if constexpr (std::is_same<T, std::vector<boost::variant<boost::recursive_wrapper<vrml_proc::parser::VrmlNode>, boost::recursive_wrapper<vrml_proc::parser::UseNode>>>>::value) {
                static const T empty_value{};  // Local static object
                return std::cref(empty_value);
            }
            else {
                return cpp::fail(std::optional<std::string>{});
            }
        }

        if constexpr (std::is_same<T, vrml_proc::parser::Vec3fArray>::value) {
            return std::cref(value);
        }
        return cpp::fail(std::optional<std::string>(vrml_proc::core::utils::TypeToString<vrml_proc::parser::Vec3fArray>()));
    }

    cpp::result<std::reference_wrapper<const T>, std::optional<std::string>> operator()(const vrml_proc::parser::Int32Array& value) const {
        std::cout << "I am here intarray" << std::endl;
        std::cout << "Type of result.value(): " << typeid(value).name() << std::endl;
        if (value.integers.size() == 0) {
            if constexpr (std::is_same<T, vrml_proc::parser::Int32Array>::value) {
                return std::cref(value);
            }
            else if constexpr (std::is_same<T, vrml_proc::parser::Vec3fArray>::value) {
                static const T empty_value{};  // Local static object
                return std::cref(empty_value);
            }
            else if constexpr (std::is_same<T, std::vector<boost::variant<boost::recursive_wrapper<vrml_proc::parser::VrmlNode>, boost::recursive_wrapper<vrml_proc::parser::UseNode>>>>::value) {
                static const T empty_value{};  // Local static object
                return std::cref(empty_value);
            }
            else {
                return cpp::fail(std::optional<std::string>{});
            }
        }

        if constexpr (std::is_same<T, vrml_proc::parser::Int32Array>::value) {
            return std::cref(value);
        }
        return cpp::fail(std::optional<std::string>(vrml_proc::core::utils::TypeToString<vrml_proc::parser::Int32Array>()));
    }

    cpp::result<std::reference_wrapper<const T>, std::optional<std::string>> operator()(float value) const {
        std::cout << "I am here float" << std::endl;
        if constexpr (std::is_same<T, float>::value) {
            return std::cref(value);
        }
        return cpp::fail(std::optional<std::string>(vrml_proc::core::utils::TypeToString<float>()));
    }

    cpp::result<std::reference_wrapper<const T>, std::optional<std::string>> operator()(int32_t value) const {
        std::cout << "int32" << std::endl;
        if constexpr (std::is_same<T, int32_t>::value) {
            return std::cref(value);
        }
        return cpp::fail(std::optional<std::string>(vrml_proc::core::utils::TypeToString<int32_t>()));
    }

    cpp::result<std::reference_wrapper<const T>, std::optional<std::string>> operator()(const vrml_proc::parser::Vec3f& value) const {
        std::cout << "vec3f" << std::endl;
        if constexpr (std::is_same<T, vrml_proc::parser::Vec3f>::value) {
            return std::cref(value);
        }
        return cpp::fail(std::optional<std::string>(vrml_proc::core::utils::TypeToString<vrml_proc::parser::Vec3f>()));
    }

    cpp::result<std::reference_wrapper<const T>, std::optional<std::string>> operator()(const vrml_proc::parser::Vec4f& value) const {
        std::cout << "I am here vec4f" << std::endl;
        if constexpr (std::is_same<T, vrml_proc::parser::Vec4f>::value) {
            return std::cref(value);
        }
        return cpp::fail(std::optional<std::string>(vrml_proc::core::utils::TypeToString<vrml_proc::parser::Vec4f>()));
    }

    cpp::result<std::reference_wrapper<const T>, std::optional<std::string>> operator()(const vrml_proc::parser::UseNode& value) const {
        std::cout << "I am here usenode" << std::endl;
        if constexpr (std::is_same<T, vrml_proc::parser::UseNode>::value) {
            return std::cref(value);
        }
        return cpp::fail(std::optional<std::string>(vrml_proc::core::utils::TypeToString<vrml_proc::parser::UseNode>()));
    }

    /**
     * @todo Maybe I will need to create also std::optional<T> operator()(const boost::recursive_wrapper<vrml_proc::parser::UseNode>& node) const;
     * @see See the same fucntion for VrmlNode.
     */

    cpp::result<std::reference_wrapper<const T>, std::optional<std::string>> operator()(const boost::recursive_wrapper<vrml_proc::parser::VrmlNode>& node) const {
        std::cout << "I am here vrmlnode (recursive wrapper)" << std::endl;

        if constexpr (std::is_same<T, boost::recursive_wrapper<vrml_proc::parser::VrmlNode>>::value) {
            return std::cref(node);
        }

        return cpp::fail(std::optional<std::string>(vrml_proc::core::utils::TypeToString<vrml_proc::parser::VrmlNode>()));
    }

    cpp::result<std::reference_wrapper<const T>, std::optional<std::string>> operator()(const vrml_proc::parser::VrmlNode& node) const {
        std::cout << "I am here vrmlnode (non-recursive)" << std::endl;

        if constexpr (std::is_same<T, vrml_proc::parser::VrmlNode>::value) {
            return std::cref(node);
        }

        if constexpr (std::is_same<T, boost::recursive_wrapper<vrml_proc::parser::VrmlNode>>::value) {
            return std::cref(boost::recursive_wrapper<vrml_proc::parser::VrmlNode>(node));
        }

        return cpp::fail(std::optional<std::string>(vrml_proc::core::utils::TypeToString<vrml_proc::parser::VrmlNode>()));
    }

    cpp::result<std::reference_wrapper<const T>, std::optional<std::string>> operator()(const std::vector<boost::variant<boost::recursive_wrapper<vrml_proc::parser::VrmlNode>, boost::recursive_wrapper<vrml_proc::parser::UseNode>>>& value) const {
        std::cout << "I am her arraay" << std::endl;
        if constexpr (std::is_same<T, std::vector<boost::variant<boost::recursive_wrapper<vrml_proc::parser::VrmlNode>, boost::recursive_wrapper<vrml_proc::parser::UseNode>>>>::value) {
            return std::cref(value);
        }
        return cpp::fail(std::optional<std::string>(vrml_proc::core::utils::TypeToString<std::vector<boost::variant<boost::recursive_wrapper<vrml_proc::parser::VrmlNode>, boost::recursive_wrapper<vrml_proc::parser::UseNode>>>>()));
    }

    template <typename U>
    cpp::result<std::reference_wrapper<const T>, std::optional<std::string>> operator()(const U&) const {
        std::cout << "Type mismatch: expected '" << typeid(T).name()
            << "' but got '" << typeid(U).name() << "'" << std::endl;

        return cpp::fail(std::optional<std::string>(vrml_proc::core::utils::TypeToString(U)));
    }
};


template <typename T>
struct VariantVisitor : public boost::static_visitor<cpp::result<T, std::optional<std::string>>> {

    cpp::result<T, std::optional<std::string>> operator()(const vrml_proc::parser::UseNode& node) const {
        //std::cout << "I am here usenode" << std::endl;
        if constexpr (std::is_same<T, vrml_proc::parser::UseNode>::value) {
            return node;
        }

        if constexpr (std::is_same<T, boost::recursive_wrapper<vrml_proc::parser::UseNode>>::value) {
            return boost::recursive_wrapper<vrml_proc::parser::UseNode>(node);
        }

        return cpp::fail(std::optional<std::string>{});
    }

    cpp::result<T, std::optional<std::string>> operator()(const boost::recursive_wrapper<vrml_proc::parser::UseNode>& node) const {
        //std::cout << "I am here udenode (recursive wrapper)" << std::endl;

        if constexpr (std::is_same<T, boost::recursive_wrapper<vrml_proc::parser::UseNode>>::value) {
            return node;
        }

        return cpp::fail(std::optional<std::string>{});
    }

    cpp::result<T, std::optional<std::string>> operator()(const boost::recursive_wrapper<vrml_proc::parser::VrmlNode>& node) const {
        //std::cout << "I am here vrmlnode (recursive wrapper)" << std::endl;

        if constexpr (std::is_same<T, boost::recursive_wrapper<vrml_proc::parser::VrmlNode>>::value) {
            return node;
        }

        return cpp::fail(std::optional<std::string>{});
    }

    cpp::result<T, std::optional<std::string>> operator()(const vrml_proc::parser::VrmlNode& node) const {
        //std::cout << "I am here vrmlnode (non-recursive)" << std::endl;

        if constexpr (std::is_same<T, vrml_proc::parser::VrmlNode>::value) {
            return node;
        }

        if constexpr (std::is_same<T, boost::recursive_wrapper<vrml_proc::parser::VrmlNode>>::value) {
            return boost::recursive_wrapper<vrml_proc::parser::VrmlNode>(node);
        }

        return cpp::fail(std::optional<std::string>{});
    }

    template <typename U>
    cpp::result<T, std::optional<std::string>> operator()(const U&) const {
        std::cout << "Type mismatch: expected '" << typeid(T).name()
            << "' but got '" << typeid(U).name() << "'" << std::endl;
        return cpp::fail(std::optional<std::string>(vrml_proc::core::utils::TypeToString(U)));
    }
};
