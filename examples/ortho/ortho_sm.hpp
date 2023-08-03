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

#ifndef ROOST_ORTHO_ORTHO_SM_HPP
#define ROOST_ORTHO_ORTHO_SM_HPP

#include "regiona.hpp"
#include "regionc.hpp"

namespace ortho
{

class B : public SMTypes::Leaf
{
public:
    B(const char* name, Ctx& ctx, SMTypes::Node* parent) : SMTypes::Leaf(name, ctx, parent)
    {
    }

    void createTransitionTable() override;
};

class RootState : public SMTypes::Composite
{
public:
    RootState(const char* name, Ctx& ctx, SMTypes::Node* parent)
        : SMTypes::Composite(name, ctx, parent, &m_b),
          m_a("A", ctx, this),
          m_b("B", ctx, this),
          m_c("C", ctx, this)
    {
    }

    OrthogonalA m_a;
    B           m_b;
    OrthogonalC m_c;

    void createTransitionTable() override
    {
    }
};

}  // ns: ortho

#endif  // ROOST_ORTHO_ORTHO_SM_HPP
