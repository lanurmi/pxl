#pragma once

#include <pxl/3rdparty/nlohmann_json.hpp>
#include <pxl/containers/string.h>

template <
    template <typename U, typename V, typename... Args> class ObjectType = std::map,
    template <typename U, typename... Args> class ArrayType = std::vector,
    class StringType = pxl::String, class BooleanType = bool,
    class NumberIntegerType = std::int64_t, class NumberUnsignedType = std::uint64_t,
    class NumberFloatType = double, template <typename U> class AllocatorType = std::allocator,
    template <typename T, typename SFINAE = void> class JSONSerializer = nlohmann::adl_serializer>

using PxlCustomJson = nlohmann::basic_json<
    ObjectType, ArrayType, StringType, BooleanType, NumberIntegerType, NumberUnsignedType,
    NumberFloatType, AllocatorType, JSONSerializer>;


namespace pxl
{
    using Json = PxlCustomJson<>;
}

/*
namespace nlohmann {
    template <>
    struct adl_serializer<pxl::String> {
        static pxl::String from_json(const json& j) {
            return pxl::String(j.get<std::string>().c_str());
        }
        static void to_json(json& j, pxl::String t) {
        }
    };
}
*/