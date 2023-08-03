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

#ifndef ROOST_SM1_HPP
#define ROOST_SM1_HPP

#include "sm1_common.hpp"

namespace sm1
{
class SM111x : public SMTypes::Leaf
{
public:
    SM111x(const char* name, Ctx& ctx, SMTypes::Node* parent) : SMTypes::Leaf(name, ctx, parent)
    {
    }

    void createTransitionTable() override;

    void printSomething(Evt const&)
    {
        std::cout << "Something" << std::endl;
    }
};

class SM111 : public SMTypes::Composite
{
public:
    SM111(const char* name, Ctx& ctx, SMTypes::Node* parent)
        : SMTypes::Composite(name, ctx, parent, &m_sm1111),
          m_sm1111("sm1111", ctx, this),
          m_sm1112("sm1112", ctx, this),
          m_sm1113("sm1113", ctx, this)
    {
    }

    SM111x m_sm1111;
    SM111x m_sm1112;
    SM111x m_sm1113;

    void createTransitionTable() override
    {
    }
};

class SM112 : public SMTypes::Leaf
{
public:
    SM112(const char* name, Ctx& ctx, SMTypes::Node* parent) : SMTypes::Leaf(name, ctx, parent)
    {
    }

    void createTransitionTable() override;
};

class SM11 : public SMTypes::Composite
{
public:
    SM11(const char* name, Ctx& ctx, SMTypes::Node* parent)
        : SMTypes::Composite(name, ctx, parent, &m_sm112),
          m_sm111("sm111", ctx, this),
          m_sm112("sm112", ctx, this)
    {
    }

    SM111 m_sm111;
    SM112 m_sm112;

    void createTransitionTable() override
    {
    }
};

class SM12211 : public SMTypes::Leaf
{
public:
    SM12211(const char* name, Ctx& ctx, SMTypes::Node* parent) : SMTypes::Leaf(name, ctx, parent)
    {
    }

    void createTransitionTable() override;
};

class SM1221 : public SMTypes::Composite
{
public:
    SM1221(const char* name, Ctx& ctx, SMTypes::Node* parent)
        : SMTypes::Composite(name, ctx, parent, &m_sm12211), m_sm12211("sm12211", ctx, this)
    {
    }

    SM12211 m_sm12211;

    void createTransitionTable() override
    {
    }
};

class SM122 : public SMTypes::Composite
{
public:
    SM122(const char* name, Ctx& ctx, SMTypes::Node* parent)
        : SMTypes::Composite(name, ctx, parent, &m_sm1221), m_sm1221("sm1221", ctx, this)
    {
    }

    SM1221 m_sm1221;

    void createTransitionTable() override
    {
    }
};

class SM121 : public SMTypes::Leaf
{
public:
    SM121(const char* name, Ctx& ctx, SMTypes::Node* parent) : SMTypes::Leaf(name, ctx, parent)
    {
    }

    void createTransitionTable() override
    {
    }
};

class SM12 : public SMTypes::Composite
{
public:
    SM12(const char* name, Ctx& ctx, SMTypes::Node* parent)
        : SMTypes::Composite(name, ctx, parent, &m_sm122),
          m_sm121("sm121", ctx, this),
          m_sm122("sm122", ctx, this)
    {
    }

    SM121 m_sm121;
    SM122 m_sm122;

    void createTransitionTable() override
    {
    }
};

class RootState : public SMTypes::Composite
{
public:
    RootState(const char* name, Ctx& ctx, SMTypes::Node* parent)
        : SMTypes::Composite(name, ctx, parent, &m_sm11),
          m_sm11("sm11", ctx, this),
          m_sm12("sm12", ctx, this)
    {
    }

    SM11 m_sm11;
    SM12 m_sm12;

    void createTransitionTable() override
    {
    }
};

}  // ns: sm1

#endif  // ROOST_SM1_HPP
