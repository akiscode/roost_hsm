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

#include "history_sm.hpp"

namespace history
{

void StepB1::createTransitionTable()
{
    RootState& rs = *m_ctx.m_root;

    addRow(Evt::E1, &rs.m_processing.m_stepb.m_stepb2, ROOST_NO_ACTION, ROOST_NO_GUARD);
}

void StepA::createTransitionTable()
{

    RootState& rs = *m_ctx.m_root;

    addRow(Evt::E1, &rs.m_processing.m_stepb, ROOST_NO_ACTION, ROOST_NO_GUARD);
}

// --

void Processing::createTransitionTable()
{
    RootState& rs = *m_ctx.m_root;

    addRow(Evt::E2, &rs.m_waiting, ROOST_NO_ACTION, ROOST_NO_GUARD);
}

// --

void Waiting::createTransitionTable()
{
    RootState& rs = *m_ctx.m_root;

    // clang-format off

    addRow(Evt::E1, &rs.m_processing,                ROOST_NO_ACTION, ROOST_NO_GUARD);
    addRow(Evt::E3, &rs.m_processing.shallowHistory, ROOST_NO_ACTION, ROOST_NO_GUARD);
    addRow(Evt::E4, &rs.m_processing.deepHistory,    ROOST_NO_ACTION, ROOST_NO_GUARD);

    // clang-format on
}

}  // ns: history
