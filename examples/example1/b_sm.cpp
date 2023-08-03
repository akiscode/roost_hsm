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

#include "b_sm.hpp"

// Including this so the IDE can auto-fill RootState member variables
#include "example1_sm.hpp"

namespace example1
{

void D::createTransitionTable()
{
    RootState& rs = *m_ctx.m_root;
    addRow(Evt::ROOST_NONE, &rs.m_a, { ROOST_ACTION(m_ctx.turnOffg1) }, ROOST_NO_GUARD);
}

void B::createTransitionTable()
{
    RootState& rs = *m_ctx.m_root;
    addRow(Evt::E4, &rs.m_c, ROOST_NO_ACTION, ROOST_NO_GUARD);
}

}  // ns: example1
