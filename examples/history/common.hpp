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

#ifndef ROOST_HISTORY_COMMON_HPP
#define ROOST_HISTORY_COMMON_HPP

#include "roost/state_machine.hpp"

namespace history
{
enum class Evt
{
    ROOST_NONE,  // Enforced by framework
    E1,
    E2,
    E3,
    E4
};

static const char* EvtStrings[] = {"NONE", "E1", "E2", "E3", "E4"};

ROOST_ENUM_PRINT_HELPER(Evt, EvtStrings)

class RootState;

struct Ctx
{
    RootState* m_root;
};

}  // ns: history

// Use explicit template instantiation to help with compile times, see common.cpp
// for other key part
extern template class roost::NodeAlias<history::Ctx, history::Evt>;

namespace history
{

using SMTypes = roost::NodeAlias<Ctx, Evt>;

}  // ns: history

#endif  // ROOST_HISTORY_COMMON_HPP
