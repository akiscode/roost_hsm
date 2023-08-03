// Copyright (c) 2023. Akiscode
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef ROOST_LIB_COMMON_HPP
#define ROOST_LIB_COMMON_HPP

#include <cassert>
#include <fstream>
#include <memory>

#define ROOST_ENUM_PRINT_HELPER(ClassName, ArrayName)                     \
    inline const char* getStringLiteral(const ClassName& e)               \
    {                                                                     \
        size_t len = sizeof(ArrayName) / sizeof(ArrayName[0]);            \
        return roost::enumToStringHelper(e, ArrayName, len);              \
    }                                                                     \
                                                                          \
    inline std::ostream& operator<<(std::ostream& os, const ClassName& e) \
    {                                                                     \
        return os << getStringLiteral(e);                                 \
    }

#define ROOST_ASSERT(x) assert(x)

namespace roost
{

template <typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args)
{
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

template <typename E, typename = typename std::enable_if<std::is_enum<E>::value, void*>::type>
const char* enumToStringHelper(E const& e, const char* stringArr[], size_t stringArrLen)
{

    int idx = static_cast<typename std::underlying_type<E>::type>(e);

    if (idx < 0)
    {
        return "ROOST_INVALID_EVENT";
    }

    // Can safely cast here because we checked if it was less than zero
    if ((size_t)idx >= stringArrLen)
    {
        return "ROOST_INVALID_EVENT";
    }

    return stringArr[idx];
}

enum class NodeType
{
    LEAF_NODE,
    COMPOSITE_NODE,
    ORTHOGONAL_NODE,
    REGION,
    SHALLOW_HISTORY_NODE,
    DEEP_HISTORY_NODE

};  // Enum: NodeType

static const char* NodeTypeStrings[] = {"STATE_MACHINE",
                                        "LEAF_NODE",
                                        "COMPOSITE_NODE",
                                        "REGION",
                                        "SHALLOW_HISTORY_NODE",
                                        "DEEP_HISTORY_NODE"};

ROOST_ENUM_PRINT_HELPER(NodeType, NodeTypeStrings)

}  // ns: roost

#endif  // ROOST_LIB_COMMON_HPP
