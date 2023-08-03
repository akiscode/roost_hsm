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

#ifndef ROOST_ORTHO_REGIONA_HPP
#define ROOST_ORTHO_REGIONA_HPP

#include "common.hpp"

namespace ortho
{

class JoinA : public SMTypes::Leaf
{
public:
    JoinA(const char* name, Ctx& ctx, SMTypes::Node* parent) : SMTypes::Leaf(name, ctx, parent)
    {
    }

    void onEntry() override;

    void createTransitionTable() override;
};

// --

class A11 : public SMTypes::Leaf
{
public:
    A11(const char* name, Ctx& ctx, SMTypes::Node* parent) : SMTypes::Leaf(name, ctx, parent)
    {
    }

    void createTransitionTable() override;
};

class A12 : public SMTypes::Leaf
{
public:
    A12(const char* name, Ctx& ctx, SMTypes::Node* parent) : SMTypes::Leaf(name, ctx, parent)
    {
    }

    void createTransitionTable() override;
};

class RegionA1 : public SMTypes::Region
{
public:
    RegionA1(const char* name, Ctx& ctx, SMTypes::Node* parent)
        : SMTypes::Region(name, ctx, parent, &m_a11),
          m_a11("A11", ctx, this),
          m_a12("A12", ctx, this),
          m_join("A1_JOIN", ctx, this)
    {
    }

    A11   m_a11;
    A12   m_a12;
    JoinA m_join;
};

// ---

class A21 : public SMTypes::Leaf
{
public:
    A21(const char* name, Ctx& ctx, SMTypes::Node* parent) : SMTypes::Leaf(name, ctx, parent)
    {
    }

    void createTransitionTable() override;
};

class A22 : public SMTypes::Leaf
{
public:
    A22(const char* name, Ctx& ctx, SMTypes::Node* parent) : SMTypes::Leaf(name, ctx, parent)
    {
    }

    void createTransitionTable() override;
};

class A23 : public SMTypes::Leaf
{
public:
    A23(const char* name, Ctx& ctx, SMTypes::Node* parent) : SMTypes::Leaf(name, ctx, parent)
    {
    }

    void createTransitionTable() override;
};

class RegionA2 : public SMTypes::Region
{
public:
    RegionA2(const char* name, Ctx& ctx, SMTypes::Node* parent)
        : SMTypes::Region(name, ctx, parent, &m_a21),
          m_a21("A21", ctx, this),
          m_a22("A22", ctx, this),
          m_a23("A23", ctx, this),
          m_join("A2_JOIN", ctx, this)
    {
    }

    A21   m_a21;
    A22   m_a22;
    A23   m_a23;
    JoinA m_join;
};

// --

class OrthogonalA : public SMTypes::Orthogonal
{
public:
    OrthogonalA(const char* name, Ctx& ctx, SMTypes::Node* parent)
        : SMTypes::Orthogonal(name, ctx, parent),
          m_join_count(2),
          m_regiona1("RegionA1", ctx, this),
          m_regiona2("RegionA2", ctx, this)
    {
    }

    int m_join_count;

    RegionA1 m_regiona1;
    RegionA2 m_regiona2;

    void onExit() override
    {
        m_join_count = 2;
    }

    bool allJoined()
    {
        return m_join_count <= 0;
    }

    void createTransitionTable() override;
};

}  // ns: ortho

#endif  // ROOST_ORTHO_REGIONA_HPP
