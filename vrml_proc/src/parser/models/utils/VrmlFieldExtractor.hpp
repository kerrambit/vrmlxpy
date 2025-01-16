#pragma once

#include <optional>
#include <string>
#include <type_traits>
#include <typeinfo>

#include <boost/variant.hpp>

#include <result.hpp>

#include "VrmlField.hpp"
#include "VrmlNodeManager.hpp"
#include "TypeToString.hpp"

template <typename T>
struct ExtractorVisitor;

template <typename T>
struct VariantVisitor;

namespace vrml_proc {
    namespace parser {
        namespace model {
            namespace utils {
                namespace VrmlFieldExtractor {

                    static bool IsNamePresent(const std::string& name, const std::vector<vrml_proc::parser::VrmlField>& fields) {
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
                    static cpp::result<T, ExtractByNameError> ExtractByNameExtended(const std::string& name, const std::vector<vrml_proc::parser::VrmlField>& fields, std::string& invalidType) {
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
                    static cpp::result<T, ExtractByNameError> ExtractByName(const std::string& name, const std::vector<vrml_proc::parser::VrmlField>& fields) {
                        std::string invalidType;
                        return ExtractByNameExtended<T>(name, fields, invalidType);
                    }

                    enum class ExtractVrmlNodeError {
                        FieldNotFound,
                        ValidationError,
                        UnknownUseNode
                    };


                    static cpp::result<vrml_proc::parser::VrmlNode&, ExtractVrmlNodeError> ExtractVrmlNodeExtended(const std::string& name, const std::vector<vrml_proc::parser::VrmlField>& fields, const vrml_proc::parser::VrmlNodeManager& manager, std::string& invalidType, std::string& useId) {
                        if (IsNamePresent(name, fields)) {

                            std::string error = "";
                            auto vrmlNode = ExtractByNameExtended<vrml_proc::parser::VrmlNode>(name, fields, error);
                            if (vrmlNode.has_value()) {
                                return vrmlNode.value();
                            }

                            auto useNode = ExtractByName<vrml_proc::parser::UseNode>(name, fields);
                            if (useNode.has_value()) {
                                auto managerFound = manager.GetDefinitionNode(useNode.value().identifier);
                                if (managerFound != nullptr) {
                                    return *managerFound;
                                }
                                else {
                                    useId = useNode.value().identifier;
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

                    static cpp::result<vrml_proc::parser::VrmlNode&, ExtractVrmlNodeError> ExtractVrmlNode(const std::string& name, const std::vector<vrml_proc::parser::VrmlField>& fields, const vrml_proc::parser::VrmlNodeManager& manager) {
                        std::string invalidType; std::string useId;
                        return ExtractVrmlNodeExtended(name, fields, manager, invalidType, useId);
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
}

template <typename T>
struct ExtractorVisitor : public boost::static_visitor<cpp::result<T, std::optional<std::string>>> {

    cpp::result<T, std::optional<std::string>> operator()(const std::string& value) const {
        //std::cout << "I am here string" << std::endl;
        if constexpr (std::is_same<T, std::string>::value) {
            return value;
        }
        return cpp::fail(std::optional<std::string>{});
    }

    cpp::result<T, std::optional<std::string>> operator()(bool value) const {
        //std::cout << "I am here bool" << std::endl;
        if constexpr (std::is_same<T, bool>::value) {
            return value;
        }
        return cpp::fail(std::optional<std::string>{});
    }

    cpp::result<T, std::optional<std::string>> operator()(const vrml_proc::parser::Vec3fArray& value) const {
        //std::cout << "I am here vec3f array" << std::endl;
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
                return cpp::fail(std::optional<std::string>{});
            }
        }

        if constexpr (std::is_same<T, vrml_proc::parser::Vec3fArray>::value) {
            return value;
        }
        return cpp::fail(std::optional<std::string>{});
    }

    cpp::result<T, std::optional<std::string>> operator()(const vrml_proc::parser::Int32Array& value) const {
        //std::cout << "I am here intarray" << std::endl;
        if constexpr (std::is_same<T, vrml_proc::parser::Int32Array>::value) {
            return value;
        }
        return cpp::fail(std::optional<std::string>{});
    }

    cpp::result<T, std::optional<std::string>> operator()(float value) const {
        //std::cout << "I am here float" << std::endl;
        if constexpr (std::is_same<T, float>::value) {
            return value;
        }
        return cpp::fail(std::optional<std::string>{});
    }

    cpp::result<T, std::optional<std::string>> operator()(int32_t value) const {
        //std::cout << "int32" << std::endl;
        if constexpr (std::is_same<T, int32_t>::value) {
            return value;
        }
        return cpp::fail(std::optional<std::string>{});
    }

    cpp::result<T, std::optional<std::string>> operator()(const vrml_proc::parser::Vec3f& value) const {
        //std::cout << "vec3f" << std::endl;
        if constexpr (std::is_same<T, vrml_proc::parser::Vec3f>::value) {
            return value;
        }
        return cpp::fail(std::optional<std::string>{});
    }

    cpp::result<T, std::optional<std::string>> operator()(const vrml_proc::parser::Vec4f& value) const {
        //std::cout << "I am here vec4f" << std::endl;
        if constexpr (std::is_same<T, vrml_proc::parser::Vec4f>::value) {
            return value;
        }
        return cpp::fail(std::optional<std::string>{});
    }

    cpp::result<T, std::optional<std::string>> operator()(const vrml_proc::parser::UseNode& value) const {
        //std::cout << "I am here usenode" << std::endl;
        if constexpr (std::is_same<T, vrml_proc::parser::UseNode>::value) {
            return value;
        }
        return cpp::fail(std::optional<std::string>{});
    }

    /**
     * @todo Maybe I will need to create also std::optional<T> operator()(const boost::recursive_wrapper<vrml_proc::parser::UseNode>& node) const;
     * @see See the same fucntion for VrmlNode.
     */

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


    cpp::result<T, std::optional<std::string>> operator()(const std::vector<boost::variant<boost::recursive_wrapper<vrml_proc::parser::VrmlNode>, boost::recursive_wrapper<vrml_proc::parser::UseNode>>>& value) const {
        //std::cout << "I am her arraay" << std::endl;
        if constexpr (std::is_same<T, std::vector<boost::variant<boost::recursive_wrapper<vrml_proc::parser::VrmlNode>, boost::recursive_wrapper<vrml_proc::parser::UseNode>>>>::value) {
            return value;
        }
        return cpp::fail(std::optional<std::string>{});
    }

    template <typename U>
    cpp::result<T, std::optional<std::string>> operator()(const U&) const {
        std::cout << "Type mismatch: expected '" << typeid(T).name()
            << "' but got '" << typeid(U).name() << "'" << std::endl;

        return cpp::fail(std::optional<std::string>(vrml_proc::core::utils::TypeToString(T)));
    }
};


template <typename T>
struct VariantVisitor : public boost::static_visitor<std::optional<T>> {

    std::optional<T> operator()(const vrml_proc::parser::UseNode& node) const {
        //std::cout << "I am here usenode" << std::endl;
        if constexpr (std::is_same<T, vrml_proc::parser::UseNode>::value) {
            return node;
        }

        if constexpr (std::is_same<T, boost::recursive_wrapper<vrml_proc::parser::UseNode>>::value) {
            return boost::recursive_wrapper<vrml_proc::parser::UseNode>(node);
        }

        return {};
    }

    std::optional<T> operator()(const boost::recursive_wrapper<vrml_proc::parser::UseNode>& node) const {
        //std::cout << "I am here udenode (recursive wrapper)" << std::endl;

        if constexpr (std::is_same<T, boost::recursive_wrapper<vrml_proc::parser::UseNode>>::value) {
            return node;
        }

        return {};
    }

    std::optional<T> operator()(const boost::recursive_wrapper<vrml_proc::parser::VrmlNode>& node) const {
        //std::cout << "I am here vrmlnode (recursive wrapper)" << std::endl;

        if constexpr (std::is_same<T, boost::recursive_wrapper<vrml_proc::parser::VrmlNode>>::value) {
            return node;
        }

        return {};
    }

    std::optional<T> operator()(const vrml_proc::parser::VrmlNode& node) const {
        //std::cout << "I am here vrmlnode (non-recursive)" << std::endl;

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
