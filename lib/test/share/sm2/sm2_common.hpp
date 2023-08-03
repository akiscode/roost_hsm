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

#ifndef ROOST_SM2_COMMON_HPP
#define ROOST_SM2_COMMON_HPP

#include "roost/state_machine.hpp"

namespace sm2
{

enum class Evt
{
    ROOST_NONE,  // Enforced by framework
    FIRST,
    SECOND,
    THIRD,
    FOURTH,
    FIFTH
};

static const char* EvtStrings[] = {"NONE", "FIRST", "SECOND", "THIRD", "FOURTH", "FIFTH"};

ROOST_ENUM_PRINT_HELPER(Evt, EvtStrings)

class RootState;

struct Ctx
{
    RootState* m_root;
};

}  // ns: sm2

// This is useful to reduce compile times (see definition in cpp file)
extern template class roost::NodeAlias<sm2::Ctx, sm2::Evt>;

namespace sm2
{

using SMTypes = roost::NodeAlias<Ctx, Evt>;

}  // ns: sm2

#endif  // ROOST_SM2_COMMON_HPP
