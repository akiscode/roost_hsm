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

#include "regiona.hpp"

// Included for IDE auto-complete
#include "ortho_sm.hpp"

namespace ortho
{

void JoinA::onEntry()
{
    RootState& rs = *m_ctx.m_root;

    rs.m_a.m_join_count--;
}

void JoinA::createTransitionTable()
{

    RootState& rs = *m_ctx.m_root;

    // clang-format off

    // The below transition isn't allowed because we are directly transitioning to a region.
    // During init, Roost HSM will recognize this and fail.
    // addRow(Evt::ROOST_NONE, &rs.m_c.m_regionc2, ROOST_NO_ACTION, ROOST_GUARD( rs.m_a.allJoined() ) );
    // Instead we transition directly to the node we want.
    addRow(Evt::ROOST_NONE, &rs.m_c.m_regionc2.m_c21, ROOST_NO_ACTION, ROOST_GUARD( rs.m_a.allJoined() ) );

    // clang-format on
}

void A11::createTransitionTable()
{

    RootState& rs = *m_ctx.m_root;

    addRow(Evt::E2, &rs.m_a.m_regiona1.m_a12, ROOST_NO_ACTION, ROOST_NO_GUARD);
}

void A12::createTransitionTable()
{

    RootState& rs = *m_ctx.m_root;

    addRow(Evt::E2, &rs.m_a.m_regiona1.m_join, ROOST_NO_ACTION, ROOST_NO_GUARD);
}

// --

void A21::createTransitionTable()
{
    RootState& rs = *m_ctx.m_root;

    addRow(Evt::E2, &rs.m_a.m_regiona2.m_a22, ROOST_NO_ACTION, ROOST_NO_GUARD);
}

void A22::createTransitionTable()
{

    RootState& rs = *m_ctx.m_root;

    addRow(Evt::E2, &rs.m_a.m_regiona2.m_a23, ROOST_NO_ACTION, ROOST_NO_GUARD);
}

void A23::createTransitionTable()
{

    RootState& rs = *m_ctx.m_root;

    addRow(Evt::E2, &rs.m_a.m_regiona2.m_join, ROOST_NO_ACTION, ROOST_NO_GUARD);
}

// --

void OrthogonalA::createTransitionTable()
{
    RootState& rs = *m_ctx.m_root;

    addRow(Evt::E1, &rs.m_b, ROOST_NO_ACTION, ROOST_NO_GUARD);
}

// --

}  // ns: ortho
