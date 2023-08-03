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

#ifndef ROOST_JOIN_SM_COMMON_HPP
#define ROOST_JOIN_SM_COMMON_HPP

#include "roost/state_machine.hpp"

namespace join_sm
{

enum class Evt
{
    ROOST_NONE,  // Enforced by framework
    FIRST,
    SECOND,
    THIRD,
    FOURTH,
    RESET
};

static const char* EvtStrings[] = {"NONE", "FIRST", "SECOND", "THIRD", "FOURTH", "RESET"};

ROOST_ENUM_PRINT_HELPER(Evt, EvtStrings)

class S1;

struct Ctx
{
    S1* m_s1;
};

}  // ns: join_sm

extern template class roost::NodeAlias<join_sm::Ctx, join_sm::Evt>;

namespace join_sm
{

using SMTypes = roost::NodeAlias<Ctx, Evt>;

}  // ns: join_sm

#endif  // ROOST_JOIN_SM_COMMON_HPP
