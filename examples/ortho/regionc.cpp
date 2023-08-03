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

#include "regionc.hpp"

// Included for IDE auto-complete
#include "ortho_sm.hpp"

namespace ortho
{

void JoinC::onEntry()
{
    RootState& rs = *m_ctx.m_root;

    rs.m_c.m_join_count--;
}

void JoinC::createTransitionTable()
{

    RootState& rs = *m_ctx.m_root;

    // clang-format off

    addRow(Evt::ROOST_NONE, &rs.m_b, ROOST_NO_ACTION, ROOST_GUARD( rs.m_c.allJoined() ) );

    // clang-format on
}

// --

void C11::createTransitionTable()
{
    RootState& rs = *m_ctx.m_root;

    // clang-format off

    addRow(Evt::ROOST_NONE, &rs.m_c.m_regionc1.m_c12, { ROOST_ACTION( rs.m_c.turnOnG1 ) }, ROOST_NO_GUARD );

    // clang-format on
}

void C12::createTransitionTable()
{
    RootState& rs = *m_ctx.m_root;

    // clang-format off

    addRow(Evt::E4, &rs.m_c.m_regionc1.m_c11,  ROOST_NO_ACTION, ROOST_NO_GUARD );
    addRow(Evt::E5, &rs.m_c.m_regionc1.m_join, ROOST_NO_ACTION, ROOST_NO_GUARD );

    // clang-format on
}

// --

void C21::createTransitionTable()
{

    RootState& rs = *m_ctx.m_root;

    // clang-format off

    addRow(Evt::E3, &rs.m_c.m_regionc2.m_c22, ROOST_NO_ACTION,  ROOST_GUARD( rs.m_c.m_g1 ) );

    // clang-format on
}

void C22::createTransitionTable()
{

    RootState& rs = *m_ctx.m_root;

    // clang-format off

    addRow(Evt::E3, &rs.m_c.m_regionc2.m_join, ROOST_NO_ACTION, ROOST_NO_GUARD );
    addRow(Evt::E4, &rs.m_b,                   ROOST_NO_ACTION, ROOST_NO_GUARD );

    // clang-format on
}

// --

void C31::createTransitionTable()
{

    RootState& rs = *m_ctx.m_root;

    addRow(Evt::E3, &rs.m_c.m_regionc3.m_c32, ROOST_NO_ACTION, ROOST_NO_GUARD);
}

void C32::createTransitionTable()
{
    RootState& rs = *m_ctx.m_root;

    addRow(Evt::ROOST_NONE, &rs.m_c.m_regionc3.m_c33, ROOST_NO_ACTION, ROOST_NO_GUARD);
}

void C33::createTransitionTable()
{
    RootState& rs = *m_ctx.m_root;

    // clang-format off

    addRow(Evt::E3, &rs.m_c.m_regionc3.m_join, ROOST_NO_ACTION, ROOST_NO_GUARD);
    addRow(Evt::E4, &rs.m_a.m_regiona2.m_a23,  ROOST_NO_ACTION, ROOST_NO_GUARD);

    // clang-format on
}

}  // ns: ortho
