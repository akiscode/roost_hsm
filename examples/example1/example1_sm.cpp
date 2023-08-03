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

#include "example1_sm.hpp"

namespace example1
{

void A::createTransitionTable()
{
    // clang-format off

    RootState& rs = *m_ctx.m_root;

    addRow(Evt::E1, &rs.m_b,         ROOST_NO_ACTION,          ROOST_GUARD(m_ctx.g1) );
    addRow(Evt::E1, &rs.m_b.m_e.m_g, { ROOST_ACTION(postE2) }, ROOST_NO_GUARD        );

    // clang-format on
}

void A::postE2(const Evt&)
{
    postFifo(Evt::E2);
}

void C::createTransitionTable()
{
    RootState& rs = *m_ctx.m_root;

    addRow(Evt::E3, &rs.m_b.m_e.m_f, ROOST_NO_ACTION, ROOST_NO_GUARD );
}

void RootState::createTransitionTable()
{
    // No transitions
}

}  // ns: example1
