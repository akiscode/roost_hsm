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

#include "simple_history.hpp"

namespace simple_history
{

// clang-format off

void State1::createTransitionTable()
{
    RootState& rs = *m_ctx.m_root;
    addRow(Evt::FIRST,  &rs.m_state2,                ROOST_NO_ACTION, ROOST_NO_GUARD);
    addRow(Evt::FIFTH,  &rs.m_state2.shallowHistory, ROOST_NO_ACTION, ROOST_GUARD(!m_ctx.use_deep_history));
    addRow(Evt::FIFTH,  &rs.m_state2.deepHistory,    ROOST_NO_ACTION, ROOST_GUARD(m_ctx.use_deep_history));
    addRow(Evt::SECOND, this,                        ROOST_NO_ACTION, ROOST_NO_GUARD);
}
void State22::createTransitionTable()
{
    RootState& rs = *m_ctx.m_root;
    addRow(Evt::SECOND, &rs.m_state2.m_state21, ROOST_NO_ACTION, ROOST_NO_GUARD);
}

void State21::createTransitionTable()
{
    RootState& rs = *m_ctx.m_root;
    addRow(Evt::SIXTH, &rs.m_state1, ROOST_NO_ACTION, ROOST_NO_GUARD);
}

void State211::createTransitionTable()
{
    RootState& rs = *m_ctx.m_root;
    addRow(Evt::THIRD, &rs.m_state2.m_state21.m_state212, ROOST_NO_ACTION, ROOST_NO_GUARD);
}

void State212::createTransitionTable()
{
    RootState& rs = *m_ctx.m_root;
    addRow(Evt::FOURTH,  &rs.m_state2.m_state21.m_state211, ROOST_NO_ACTION, ROOST_NO_GUARD);
    addRow(Evt::SEVENTH, &rs.m_state1,                      ROOST_NO_ACTION, ROOST_NO_GUARD);
}

// clang-format on

}  // ns: simple_history
