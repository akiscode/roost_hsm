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

#include "sm2.hpp"

namespace sm2
{

// clang-format off

void S12::createTransitionTable()
{

    RootState& rs = *m_ctx.m_root;

    addRow(Evt::FIRST, &rs.m_s11.m_s112.m_s1121, ROOST_NO_ACTION, ROOST_NO_GUARD);
}

void S11211::createTransitionTable()
{
    RootState& rs = *m_ctx.m_root;

    addRow(Evt::FIRST, &rs.m_s13, ROOST_NO_ACTION, ROOST_NO_GUARD);
    addRow(Evt::SECOND, &rs.m_s11.m_s112.m_s1121.m_s11212, ROOST_NO_ACTION, ROOST_NO_GUARD);

    // Both of these transitions have an orthogonal node as an LCA, they should both result in the same things happening
    addRow(Evt::FIFTH, &rs.m_s11.m_s111.m_s1111.m_s11113.m_sf, {ROOST_ACTION(toggleFunction)}, ROOST_GUARD(m_toggle));
    addRow(Evt::FIFTH, &rs.m_s11, {ROOST_ACTION(toggleFunction)}, ROOST_GUARD(!m_toggle));
}

void S13::createTransitionTable()
{

    RootState& rs = *m_ctx.m_root;

    addRow(Evt::FIRST, &rs.m_s11.m_s111.m_s1111.m_s11112.m_sc, ROOST_NO_ACTION, ROOST_NO_GUARD);
    addRow(Evt::THIRD, ROOST_NO_DEST, ROOST_NO_ACTION, ROOST_NO_GUARD);
    addRow(Evt::FOURTH, &rs.m_s11, ROOST_NO_ACTION, ROOST_NO_GUARD);
}

void SC::createTransitionTable()
{

    RootState& rs = *m_ctx.m_root;

    addRow(Evt::SECOND, &rs.m_s11.m_s111.m_s1112.m_s11121, ROOST_NO_ACTION, ROOST_NO_GUARD);
    addRow(Evt::FOURTH, &rs.m_s13, ROOST_NO_ACTION, ROOST_NO_GUARD);
}

void SB::createTransitionTable()
{
    RootState& rs = *m_ctx.m_root;

    addRow(Evt::SECOND, &rs.m_s11.m_s111.m_s1111.m_s11111.m_sa, ROOST_NO_ACTION, ROOST_NO_GUARD);
    addRow(Evt::FOURTH, &rs.m_s11.m_s111.m_s1111.m_s11111.m_sa, ROOST_NO_ACTION, ROOST_NO_GUARD);
}

void SE::createTransitionTable()
{

    RootState& rs = *m_ctx.m_root;

    addRow(Evt::SECOND, &rs.m_s11.m_s111.m_s1112.m_s11122, ROOST_NO_ACTION, ROOST_NO_GUARD);
    addRow(Evt::FOURTH, &rs.m_s11.m_s111.m_s1112.m_s11122, ROOST_NO_ACTION, ROOST_NO_GUARD);
}
void S11121::createTransitionTable()
{
    RootState& rs = *m_ctx.m_root;

    addRow(Evt::THIRD, &rs.m_s11.m_s111.m_s1111.m_s11112.m_sc, ROOST_NO_ACTION, ROOST_NO_GUARD);
}

void S11212::createTransitionTable()
{
    RootState& rs = *m_ctx.m_root;

    addRow(Evt::FOURTH, &rs.m_s11.m_s112.m_s1121, ROOST_NO_ACTION, ROOST_NO_GUARD);
}

void S1112::createTransitionTable()
{
    RootState& rs = *m_ctx.m_root;

    addRow(Evt::SECOND, &rs.m_s11.m_s111.m_s1111, ROOST_NO_ACTION, ROOST_NO_GUARD);
}

// clang-format on

}  // ns: sm2
