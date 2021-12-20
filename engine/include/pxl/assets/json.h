#pragma once

#include <pxl/containers/string.h>
#include <pxl/containers/vector.h>
#include <pxl/containers/map.h>
#include <pxl/types.h>
#include <nlohmann/nlohmann_json.hpp>


template <
    template <typename U, typename V, typename... Args> class ObjectType = std::map,
    template <typename U, typename... Args> class ArrayType = std::vector,
    class StringType = pxl::String, class BooleanType = bool,
    class NumberIntegerType = pxl::i64, class NumberUnsignedType = pxl::u64,
    class NumberFloatType = double, template <typename U> class AllocatorType = std::allocator,
    template <typename T, typename SFINAE = void> class JSONSerializer = nlohmann::adl_serializer>

using PxlCustomJson = nlohmann::basic_json<
    ObjectType, ArrayType, StringType, BooleanType, NumberIntegerType, NumberUnsignedType,
    NumberFloatType, AllocatorType, JSONSerializer>;


namespace pxl
{
    using Json = PxlCustomJson<>;

    #define PXL_JSON_SERIALIZE(Type, ...)  \
    friend void to_json(Json& nlohmann_json_j, const Type& nlohmann_json_t) { NLOHMANN_JSON_EXPAND(NLOHMANN_JSON_PASTE(NLOHMANN_JSON_TO, __VA_ARGS__)) } \
    friend void from_json(const Json& nlohmann_json_j, Type& nlohmann_json_t) { NLOHMANN_JSON_EXPAND(NLOHMANN_JSON_PASTE(NLOHMANN_JSON_FROM, __VA_ARGS__)) }
}
