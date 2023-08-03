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

#include "join_sm.hpp"

namespace join_sm
{

// clang-format off

void Ortho1Join::onEntry()
{
    S1& s1 = *m_ctx.m_s1;

    s1.m_ortho1.join_count--;
}

void Ortho1Join::createTransitionTable()
{

    S1& s1 = *m_ctx.m_s1;

    addRow(Evt::ROOST_NONE, &s1.m_sf, ROOST_NO_ACTION, ROOST_GUARD(s1.m_ortho1.join_count <= 0));
}

void SA::createTransitionTable()
{

    S1& s1 = *m_ctx.m_s1;

    addRow(Evt::FIRST, &s1.m_ortho1.m_region1.m_join, {ROOST_ACTION(s1.m_ortho1.fireNext)}, ROOST_NO_GUARD);
}

void SB::createTransitionTable()
{
    S1& s1 = *m_ctx.m_s1;

    addRow(Evt::SECOND, &s1.m_ortho1.m_region2.m_join, {ROOST_ACTION(s1.m_ortho1.fireNext)}, ROOST_NO_GUARD);
}

void SC::createTransitionTable()
{
    S1& s1 = *m_ctx.m_s1;

    // Show the two ways of referencing guards, these two are the same
    addRow(Evt::THIRD, &s1.m_ortho1.m_region3.m_join, ROOST_NO_ACTION, ROOST_GUARD(s1.m_ortho1.isValid()));
    addRow(Evt::THIRD, &s1.m_ortho1.m_region3.m_join, ROOST_NO_ACTION, ROOST_GUARD(s1.m_ortho1.m_toggle == true));

    // This transition will "unlock" the toggle transitions above, shown with multiple actions
    addRow(Evt::FOURTH, ROOST_NO_DEST, {ROOST_ACTION(s1.m_ortho1.toggle), ROOST_ACTION(s1.m_ortho1.fireNext)}, ROOST_NO_GUARD);
}

void SF::createTransitionTable()
{
    S1& s1 = *m_ctx.m_s1;

    addRow(Evt::RESET, &s1.m_ortho1, ROOST_NO_ACTION, ROOST_NO_GUARD);
}

// clang-format on

}  // ns: join_sm
