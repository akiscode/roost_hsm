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

#ifndef ROOST_SIMPLE_HISTORY_HPP
#define ROOST_SIMPLE_HISTORY_HPP

#include "simple_history_common.hpp"

namespace simple_history
{

class State1 : public SMTypes::Leaf
{
public:
    State1(const char* name, Ctx& ctx, SMTypes::Node* parent) : SMTypes::Leaf(name, ctx, parent)
    {
    }

    void createTransitionTable() override;
};

class State22 : public SMTypes::Leaf
{
public:
    State22(const char* name, Ctx& ctx, SMTypes::Node* parent) : SMTypes::Leaf(name, ctx, parent)
    {
    }

    void createTransitionTable() override;
};

class State211 : public SMTypes::Leaf
{
public:
    State211(const char* name, Ctx& ctx, SMTypes::Node* parent) : SMTypes::Leaf(name, ctx, parent)
    {
    }

    void createTransitionTable() override;
};

class State212 : public SMTypes::Leaf
{
public:
    State212(const char* name, Ctx& ctx, SMTypes::Node* parent) : SMTypes::Leaf(name, ctx, parent)
    {
    }

    void createTransitionTable() override;
};

class State21 : public SMTypes::Composite
{
public:
    State21(const char* name, Ctx& ctx, SMTypes::Node* parent)
        : SMTypes::Composite(name, ctx, parent, &m_state211),
          m_state211("State211", ctx, this),
          m_state212("State212", ctx, this)
    {
    }

    State211 m_state211;
    State212 m_state212;

    void createTransitionTable() override;
};

class State2 : public SMTypes::Composite
{
public:
    State2(const char* name, Ctx& ctx, SMTypes::Node* parent)
        : SMTypes::Composite(name, ctx, parent, &m_state22),
          m_state21("State21", ctx, this),
          m_state22("State22", ctx, this)
    {
    }

    State21 m_state21;
    State22 m_state22;

    void createTransitionTable() override
    {
    }
};

class RootState : public SMTypes::Composite
{
public:
    RootState(const char* name, Ctx& ctx, SMTypes::Node* parent)
        : SMTypes::Composite(name, ctx, parent, &m_state1),
          m_state1("State1", ctx, this),
          m_state2("State2", ctx, this)
    {
    }

    State1 m_state1;
    State2 m_state2;

    void createTransitionTable() override
    {
    }
};

}  // ns: simple_history

#endif  // ROOST_SIMPLE_HISTORY_HPP
