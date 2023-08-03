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

#include "ortho_history.hpp"

namespace ortho_history
{

// clang-format off

void State0::createTransitionTable()
{
    RootState &rs = *m_ctx.m_root;

    addRow(Evt::OPEN, &rs.m_state3, ROOST_NO_ACTION, ROOST_NO_GUARD);
}

void State1::createTransitionTable()
{

    RootState &rs = *m_ctx.m_root;

    addRow(Evt::STEP, &rs.m_state0.m_state2, ROOST_NO_ACTION, ROOST_NO_GUARD);
    addRow(Evt::DIRECT, &rs.m_state0.m_state2.m_state5.m_state9, ROOST_NO_ACTION, ROOST_NO_GUARD);
}

void State6::createTransitionTable()
{

    RootState &rs = *m_ctx.m_root;

    addRow(Evt::STEP, &rs.m_state0.m_state2.m_state4.m_state8, ROOST_NO_ACTION, ROOST_NO_GUARD);
}

void State7::createTransitionTable()
{

    RootState &rs = *m_ctx.m_root;

    addRow(Evt::STEP, &rs.m_state0.m_state2.m_state5.m_state9, ROOST_NO_ACTION, ROOST_NO_GUARD);
}

void State3::createTransitionTable()
{
    RootState &rs = *m_ctx.m_root;

    addRow(Evt::CLOSE, &rs.m_state0.shallowHistory, ROOST_NO_ACTION, ROOST_GUARD(!m_ctx.use_deep_history));
    addRow(Evt::CLOSE, &rs.m_state0.deepHistory, ROOST_NO_ACTION, ROOST_GUARD(m_ctx.use_deep_history));
}

void State2::createTransitionTable()
{
    RootState &rs = *m_ctx.m_root;

    addRow(Evt::BACK, &rs.m_state0.m_state1, ROOST_NO_ACTION, ROOST_NO_GUARD);
}

// clang-format on

}  // ns: ortho_history
