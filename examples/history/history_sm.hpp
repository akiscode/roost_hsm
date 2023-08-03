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

#ifndef ROOST_HISTORY_SM_HPP
#define ROOST_HISTORY_SM_HPP

#include "common.hpp"

namespace history
{

class StepB1 : public SMTypes::Leaf
{
public:
    StepB1(const char* name, Ctx& ctx, SMTypes::Node* parent) : SMTypes::Leaf(name, ctx, parent)
    {
    }

    void createTransitionTable() override;
};

class StepB2 : public SMTypes::Leaf
{
public:
    StepB2(const char* name, Ctx& ctx, SMTypes::Node* parent) : SMTypes::Leaf(name, ctx, parent)
    {
    }

    void createTransitionTable() override
    {
    }
};

class StepB : public SMTypes::Composite
{
public:
    StepB(const char* name, Ctx& ctx, SMTypes::Node* parent)
        : SMTypes::Composite(name, ctx, parent, &m_stepb1),
          m_stepb1("StepB1", ctx, this),
          m_stepb2("StepB2", ctx, this)
    {
    }

    StepB1 m_stepb1;
    StepB2 m_stepb2;

    void createTransitionTable() override
    {
    }
};

// --

class StepA : public SMTypes::Leaf
{
public:
    StepA(const char* name, Ctx& ctx, SMTypes::Node* parent) : SMTypes::Leaf(name, ctx, parent)
    {
    }

    void createTransitionTable() override;
};

class Processing : public SMTypes::Composite
{
public:
    Processing(const char* name, Ctx& ctx, SMTypes::Node* parent)
        : SMTypes::Composite(name, ctx, parent, &m_stepa),
          m_stepa("StepA", ctx, this),
          m_stepb("StepB", ctx, this)
    {
    }

    StepA m_stepa;
    StepB m_stepb;

    void createTransitionTable() override;
};

// --

class Waiting : public SMTypes::Leaf
{
public:
    Waiting(const char* name, Ctx& ctx, SMTypes::Node* parent) : SMTypes::Leaf(name, ctx, parent)
    {
    }

    void createTransitionTable() override;
};

class RootState : public SMTypes::Composite
{
public:
    RootState(const char* name, Ctx& ctx, SMTypes::Node* parent)
        : SMTypes::Composite(name, ctx, parent, &m_waiting),
          m_waiting("Waiting", ctx, this),
          m_processing("Processing", ctx, this)
    {
    }

    Waiting    m_waiting;
    Processing m_processing;

    void createTransitionTable() override
    {
    }
};

}  // ns: history

#endif  // ROOST_HISTORY_SM_HPP
