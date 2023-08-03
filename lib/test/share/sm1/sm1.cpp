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

#include "sm1.hpp"

namespace sm1
{

// clang-format off

void SM111x::createTransitionTable()
{

    RootState& rs = *m_ctx.m_root;

    addRow(Evt::FIRST, &rs.m_sm12, ROOST_NO_ACTION, ROOST_NO_GUARD);
    addRow(Evt::SECOND, ROOST_NO_DEST, {ROOST_ACTION(printSomething)}, ROOST_NO_GUARD);
}

void SM12211::createTransitionTable()
{

    RootState& rs = *m_ctx.m_root;
    addRow(Evt::ROOST_NONE, &rs.m_sm11.m_sm111, ROOST_NO_ACTION, ROOST_NO_GUARD);
}

void SM112::createTransitionTable()
{
    RootState& rs = *m_ctx.m_root;

    addRow(Evt::ROOST_NONE, &rs.m_sm11.m_sm111, ROOST_NO_ACTION, ROOST_NO_GUARD);
}

// clang-format on

}  // ns: sm1
