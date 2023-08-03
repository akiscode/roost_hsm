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

#ifndef ROOST_EXAMPLE1_E_SM_HPP
#define ROOST_EXAMPLE1_E_SM_HPP

#include "common.hpp"

namespace example1
{

class F : public SMTypes::Leaf
{
public:
    F(const char* name, Ctx& ctx, SMTypes::Node* parent) : SMTypes::Leaf(name, ctx, parent)
    {
    }

    void createTransitionTable() override;
};

class G : public SMTypes::Leaf
{
public:
    G(const char* name, Ctx& ctx, SMTypes::Node* parent) : SMTypes::Leaf(name, ctx, parent)
    {
    }

    void createTransitionTable() override;
};

class E : public SMTypes::Composite
{
public:
    E(const char* name, Ctx& ctx, SMTypes::Node* parent)
        : SMTypes::Composite(name, ctx, parent, &m_f), m_f("F", ctx, this), m_g("G", ctx, this)
    {
    }

    F m_f;
    G m_g;

    void createTransitionTable() override;
};

}  // ns: example1

#endif  // ROOST_EXAMPLE1_E_SM_HPP
