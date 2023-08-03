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

#ifndef ROOST_QUICKSTART_SM_HPP
#define ROOST_QUICKSTART_SM_HPP

#include "common.hpp"

namespace quickstart
{

class S1 : public SMTypes::Leaf
{
public:
    S1(const char* name, Ctx& ctx, SMTypes::Node* parent) : SMTypes::Leaf(name, ctx, parent)
    {
    }

    void createTransitionTable() override;
};

class S2 : public SMTypes::Leaf
{
public:
    S2(const char* name, Ctx& ctx, SMTypes::Node* parent) : SMTypes::Leaf(name, ctx, parent)
    {
    }

    void createTransitionTable() override;
};

class RootState : public SMTypes::Composite
{
public:
    RootState(const char* name, Ctx& ctx, SMTypes::Node* parent)
        : SMTypes::Composite(name, ctx, parent, &m_s1),
          m_s1("s1", ctx, this),
          m_s2("s2", ctx, this)
    {
    }

    S1 m_s1;
    S2 m_s2;

    void createTransitionTable() override;
};

}  // ns: quickstart

#endif  // ROOST_QUICKSTART_SM_HPP
