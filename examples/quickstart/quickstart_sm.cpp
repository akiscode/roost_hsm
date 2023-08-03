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

#include "quickstart_sm.hpp"

namespace quickstart
{

// clang-format off

void S1::createTransitionTable()
{
    // We only can use references (not pointers) here!
    RootState &rs = *m_ctx.m_root;

    // Don't forget the & on the destination!
    addRow(Evt::E1, &rs.m_s2, ROOST_NO_ACTION, ROOST_NO_GUARD);
}

void S2::createTransitionTable()
{
    // We only can use references (not pointers) here!
    RootState &rs = *m_ctx.m_root;

    // Don't forget the & on the destination!
    addRow(Evt::E2, &rs.m_s1, ROOST_NO_ACTION, ROOST_NO_GUARD);
}

void RootState::createTransitionTable()
{
    // We only can use references (not pointers) here!
    RootState &rs = *m_ctx.m_root;

    // Don't forget the & on the destination!
    addRow(Evt::E3, &rs.m_s2, ROOST_NO_ACTION, ROOST_NO_GUARD);
}

// clang-format on

}  // ns: quickstart
