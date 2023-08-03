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

#ifndef ROOST_EXAMPLE1_SM_HPP
#define ROOST_EXAMPLE1_SM_HPP

#include "b_sm.hpp"
#include "common.hpp"

namespace example1
{

class A : public SMTypes::Leaf
{
public:
    A(const char* name, Ctx& ctx, SMTypes::Node* parent) : SMTypes::Leaf(name, ctx, parent)
    {
    }

    void createTransitionTable() override;

    void postE2(Evt const&);
};

class C : public SMTypes::Leaf
{
public:
    C(const char* name, Ctx& ctx, SMTypes::Node* parent) : SMTypes::Leaf(name, ctx, parent)
    {
    }

    void createTransitionTable() override;
};

class RootState : public SMTypes::Composite
{
public:
    RootState(const char* name, Ctx& ctx, SMTypes::Node* parent)
        : SMTypes::Composite(name, ctx, parent, &m_a),
          m_a("A", ctx, this),
          m_b("B", ctx, this),
          m_c("C", ctx, this)
    {
    }

    A m_a;
    B m_b;
    C m_c;

    void createTransitionTable() override;
};

}  // ns: example1

#endif  // ROOST_EXAMPLE1_SM_HPP
