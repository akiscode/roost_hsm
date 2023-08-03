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

#ifndef ROOST_ORTHO_HISTORY_HPP
#define ROOST_ORTHO_HISTORY_HPP

#include "ortho_history_common.hpp"

namespace ortho_history
{

class State1 : public SMTypes::Leaf
{
public:
    State1(const char* name, Ctx& ctx, SMTypes::Node* parent) : SMTypes::Leaf(name, ctx, parent)
    {
    }

    void createTransitionTable() override;
};

class State3 : public SMTypes::Leaf
{
public:
    State3(const char* name, Ctx& ctx, SMTypes::Node* parent) : SMTypes::Leaf(name, ctx, parent)
    {
    }

    void createTransitionTable() override;
};

class State6 : public SMTypes::Leaf
{
public:
    State6(const char* name, Ctx& ctx, SMTypes::Node* parent) : SMTypes::Leaf(name, ctx, parent)
    {
    }

    void createTransitionTable() override;
};

class State7 : public SMTypes::Leaf
{
public:
    State7(const char* name, Ctx& ctx, SMTypes::Node* parent) : SMTypes::Leaf(name, ctx, parent)
    {
    }

    void createTransitionTable() override;
};

class State8 : public SMTypes::Leaf
{
public:
    State8(const char* name, Ctx& ctx, SMTypes::Node* parent) : SMTypes::Leaf(name, ctx, parent)
    {
    }

    void createTransitionTable() override
    {
    }
};

class State9 : public SMTypes::Leaf
{
public:
    State9(const char* name, Ctx& ctx, SMTypes::Node* parent) : SMTypes::Leaf(name, ctx, parent)
    {
    }

    void createTransitionTable() override
    {
    }
};

class State4 : public SMTypes::Region
{
public:
    State4(const char* name, Ctx& ctx, SMTypes::Node* parent)
        : SMTypes::Region(name, ctx, parent, &m_state6),
          m_state6("State6", ctx, this),
          m_state8("State8", ctx, this)
    {
    }

    State6 m_state6;
    State8 m_state8;
};

class State5 : public SMTypes::Region
{
public:
    State5(const char* name, Ctx& ctx, SMTypes::Node* parent)
        : SMTypes::Region(name, ctx, parent, &m_state7),
          m_state7("State7", ctx, this),
          m_state9("State9", ctx, this)
    {
    }

    State7 m_state7;
    State9 m_state9;
};

class State2 : public SMTypes::Orthogonal
{
public:
    State2(const char* name, Ctx& ctx, SMTypes::Node* parent)
        : SMTypes::Orthogonal(name, ctx, parent),
          m_state4("State4", ctx, this),
          m_state5("State5", ctx, this)
    {
    }

    State4 m_state4;
    State5 m_state5;

    void createTransitionTable() override;
};

class State0 : public SMTypes::Composite
{
public:
    State0(const char* name, Ctx& ctx, SMTypes::Node* parent)
        : SMTypes::Composite(name, ctx, parent, &m_state1),
          m_state1("State1", ctx, this),
          m_state2("State2", ctx, this)
    {
    }

    State1 m_state1;
    State2 m_state2;

    void createTransitionTable() override;
};

class RootState : public SMTypes::Composite
{
public:
    RootState(const char* name, Ctx& ctx, SMTypes::Node* parent)
        : SMTypes::Composite(name, ctx, parent, &m_state0),
          m_state0("State0", ctx, this),
          m_state3("State3", ctx, this)
    {
    }

    State0 m_state0;
    State3 m_state3;

    void createTransitionTable() override
    {
    }
};

}  // ns: ortho_history

#endif  // ROOST_ORTHO_HISTORY_HPP
