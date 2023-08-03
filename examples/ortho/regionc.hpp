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

#ifndef ROOST_ORTHO_REGIONC_HPP
#define ROOST_ORTHO_REGIONC_HPP

#include "common.hpp"

namespace ortho
{
class JoinC : public SMTypes::Leaf
{
public:
    JoinC(const char* name, Ctx& ctx, SMTypes::Node* parent) : SMTypes::Leaf(name, ctx, parent)
    {
    }

    void onEntry() override;

    void createTransitionTable() override;
};

// --

class C11 : public SMTypes::Leaf
{
public:
    C11(const char* name, Ctx& ctx, SMTypes::Node* parent) : SMTypes::Leaf(name, ctx, parent)
    {
    }

    void createTransitionTable() override;
};

class C12 : public SMTypes::Leaf
{
public:
    C12(const char* name, Ctx& ctx, SMTypes::Node* parent) : SMTypes::Leaf(name, ctx, parent)
    {
    }

    void createTransitionTable() override;
};

class RegionC1 : public SMTypes::Region
{
public:
    RegionC1(const char* name, Ctx& ctx, SMTypes::Node* parent)
        : SMTypes::Region(name, ctx, parent, &m_c11),
          m_c11("C11", ctx, this),
          m_c12("C12", ctx, this),
          m_join("C1_JOIN", ctx, this)
    {
    }

    C11   m_c11;
    C12   m_c12;
    JoinC m_join;
};

// --

class C21 : public SMTypes::Leaf
{
public:
    C21(const char* name, Ctx& ctx, SMTypes::Node* parent) : SMTypes::Leaf(name, ctx, parent)
    {
    }

    void createTransitionTable() override;
};

class C22 : public SMTypes::Leaf
{
public:
    C22(const char* name, Ctx& ctx, SMTypes::Node* parent) : SMTypes::Leaf(name, ctx, parent)
    {
    }

    void createTransitionTable() override;
};

class RegionC2 : public SMTypes::Region
{
public:
    RegionC2(const char* name, Ctx& ctx, SMTypes::Node* parent)
        : SMTypes::Region(name, ctx, parent, &m_c21),
          m_c21("C21", ctx, this),
          m_c22("C22", ctx, this),
          m_join("C2_JOIN", ctx, this)
    {
    }

    C21   m_c21;
    C22   m_c22;
    JoinC m_join;
};

// --

class C31 : public SMTypes::Leaf
{
public:
    C31(const char* name, Ctx& ctx, SMTypes::Node* parent) : SMTypes::Leaf(name, ctx, parent)
    {
    }

    void createTransitionTable() override;
};

class C32 : public SMTypes::Leaf
{
public:
    C32(const char* name, Ctx& ctx, SMTypes::Node* parent) : SMTypes::Leaf(name, ctx, parent)
    {
    }

    void createTransitionTable() override;
};

class C33 : public SMTypes::Leaf
{
public:
    C33(const char* name, Ctx& ctx, SMTypes::Node* parent) : SMTypes::Leaf(name, ctx, parent)
    {
    }

    void createTransitionTable() override;
};

class RegionC3 : public SMTypes::Region
{
public:
    RegionC3(const char* name, Ctx& ctx, SMTypes::Node* parent)
        : SMTypes::Region(name, ctx, parent, &m_c31),
          m_c31("C31", ctx, this),
          m_c32("C32", ctx, this),
          m_c33("C33", ctx, this),
          m_join("C3_JOIN", ctx, this)
    {
    }

    C31   m_c31;
    C32   m_c32;
    C33   m_c33;
    JoinC m_join;
};

// --

class OrthogonalC : public SMTypes::Orthogonal
{
public:
    OrthogonalC(const char* name, Ctx& ctx, SMTypes::Node* parent)
        : SMTypes::Orthogonal(name, ctx, parent),
          m_join_count(3),
          m_g1(false),
          m_regionc1("RegionC1", ctx, this),
          m_regionc2("RegionC2", ctx, this),
          m_regionc3("RegionC3", ctx, this)
    {
    }

    int m_join_count;

    bool m_g1;

    RegionC1 m_regionc1;
    RegionC2 m_regionc2;
    RegionC3 m_regionc3;

    void onExit() override
    {
        m_join_count = 3;
    }

    void createTransitionTable() override
    {
    }

    bool allJoined()
    {
        return m_join_count <= 0;
    }

    void turnOnG1(Evt const&)
    {
        m_g1 = true;
    }
};

}  // ns: ortho

#endif  // ROOST_ORTHO_REGIONC_HPP
