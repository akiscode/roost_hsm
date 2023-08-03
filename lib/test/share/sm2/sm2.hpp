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

#ifndef ROOST_SM2_HPP
#define ROOST_SM2_HPP

#include "sm2_common.hpp"

namespace sm2
{

class S11121 : public SMTypes::Leaf
{
public:
    S11121(const char* name, Ctx& ctx, SMTypes::Node* parent) : SMTypes::Leaf(name, ctx, parent)
    {
    }

    void createTransitionTable() override;
};

class S11122 : public SMTypes::Leaf
{
public:
    S11122(const char* name, Ctx& ctx, SMTypes::Node* parent) : SMTypes::Leaf(name, ctx, parent)
    {
    }

    void createTransitionTable() override
    {
    }
};

class S1112 : public SMTypes::Composite
{
public:
    S1112(const char* name, Ctx& ctx, SMTypes::Node* parent)
        : SMTypes::Composite(name, ctx, parent, &m_s11122),
          m_s11121("s11121", ctx, this),
          m_s11122("s11122", ctx, this)
    {
    }

    S11121 m_s11121;
    S11122 m_s11122;

    void createTransitionTable() override;
};

// --

class SA : public SMTypes::Leaf
{
public:
    SA(const char* name, Ctx& ctx, SMTypes::Node* parent) : SMTypes::Leaf(name, ctx, parent)
    {
    }

    void createTransitionTable() override
    {
    }
};

class SB : public SMTypes::Leaf
{
public:
    SB(const char* name, Ctx& ctx, SMTypes::Node* parent) : SMTypes::Leaf(name, ctx, parent)
    {
    }

    void createTransitionTable() override;
};

class SC : public SMTypes::Leaf
{
public:
    SC(const char* name, Ctx& ctx, SMTypes::Node* parent) : SMTypes::Leaf(name, ctx, parent)
    {
    }

    void createTransitionTable() override;
};

class SD : public SMTypes::Leaf
{
public:
    SD(const char* name, Ctx& ctx, SMTypes::Node* parent) : SMTypes::Leaf(name, ctx, parent)
    {
    }

    void createTransitionTable() override
    {
    }
};

class SE : public SMTypes::Leaf
{
public:
    SE(const char* name, Ctx& ctx, SMTypes::Node* parent) : SMTypes::Leaf(name, ctx, parent)
    {
    }

    void createTransitionTable() override;
};

class SF : public SMTypes::Leaf
{
public:
    SF(const char* name, Ctx& ctx, SMTypes::Node* parent) : SMTypes::Leaf(name, ctx, parent)
    {
    }

    void createTransitionTable() override
    {
    }
};

class S11113 : public SMTypes::Region
{
public:
    S11113(const char* name, Ctx& ctx, SMTypes::Node* parent)
        : SMTypes::Region(name, ctx, parent, &m_se), m_se("se", ctx, this), m_sf("sf", ctx, this)
    {
    }

    SE m_se;
    SF m_sf;
};

class S11111 : public SMTypes::Region
{
public:
    S11111(const char* name, Ctx& ctx, SMTypes::Node* parent)
        : SMTypes::Region(name, ctx, parent, &m_sb), m_sa("sa", ctx, this), m_sb("sb", ctx, this)
    {
    }

    SA m_sa;
    SB m_sb;
};

class S11112 : public SMTypes::Region
{
public:
    S11112(const char* name, Ctx& ctx, SMTypes::Node* parent)
        : SMTypes::Region(name, ctx, parent, &m_sd), m_sc("sc", ctx, this), m_sd("sd", ctx, this)
    {
    }

    SC m_sc;
    SD m_sd;
};

class S1111 : public SMTypes::Orthogonal
{
public:
    S1111(const char* name, Ctx& ctx, SMTypes::Node* parent)
        : SMTypes::Orthogonal(name, ctx, parent),
          m_s11111("s11111", ctx, this),
          m_s11112("s11112", ctx, this),
          m_s11113("s11113", ctx, this)

    {
    }

    S11111 m_s11111;
    S11112 m_s11112;
    S11113 m_s11113;

    void createTransitionTable() override
    {
    }
};

class S111 : public SMTypes::Region
{
public:
    S111(const char* name, Ctx& ctx, SMTypes::Node* parent)
        : SMTypes::Region(name, ctx, parent, &m_s1112),
          m_s1111("s1111", ctx, this),
          m_s1112("s1112", ctx, this)
    {
    }

    S1111 m_s1111;
    S1112 m_s1112;
};

// --

class S11211 : public SMTypes::Leaf
{
public:
    S11211(const char* name, Ctx& ctx, SMTypes::Node* parent)
        : SMTypes::Leaf(name, ctx, parent), m_toggle(false)
    {
    }

    void createTransitionTable() override;

    bool m_toggle;

    void toggleFunction(Evt const&)
    {
        m_toggle = !m_toggle;
    }
};

class S11212 : public SMTypes::Leaf
{
public:
    S11212(const char* name, Ctx& ctx, SMTypes::Node* parent) : SMTypes::Leaf(name, ctx, parent)
    {
    }

    void createTransitionTable() override;
};

class S1121 : public SMTypes::Composite
{
public:
    S1121(const char* name, Ctx& ctx, SMTypes::Node* parent)
        : SMTypes::Composite(name, ctx, parent, &m_s11211),
          m_s11211("s11211", ctx, this),
          m_s11212("s11212", ctx, this)
    {
    }

    S11211 m_s11211;
    S11212 m_s11212;

    void createTransitionTable() override
    {
    }
};

class S112 : public SMTypes::Region
{
public:
    S112(const char* name, Ctx& ctx, SMTypes::Node* parent)
        : SMTypes::Region(name, ctx, parent, &m_s1121), m_s1121("s1121", ctx, this)
    {
    }

    S1121 m_s1121;
};

// --

class S11 : public SMTypes::Orthogonal
{
public:
    S11(const char* name, Ctx& ctx, SMTypes::Node* parent)
        : SMTypes::Orthogonal(name, ctx, parent),
          m_s111("s111", ctx, this),
          m_s112("s112", ctx, this)
    {
    }

    void createTransitionTable() override
    {
    }

    S111 m_s111;
    S112 m_s112;
};

class S12 : public SMTypes::Leaf
{
public:
    S12(const char* name, Ctx& ctx, SMTypes::Node* parent) : SMTypes::Leaf(name, ctx, parent)
    {
    }

    void createTransitionTable() override;
};

class S13 : public SMTypes::Leaf
{
public:
    S13(const char* name, Ctx& ctx, SMTypes::Node* parent) : SMTypes::Leaf(name, ctx, parent)
    {
    }

    void createTransitionTable() override;
};

class RootState : public SMTypes::Composite
{
public:
    RootState(const char* name, Ctx& ctx, SMTypes::Node* parent)
        : SMTypes::Composite(name, ctx, parent, &m_s12),
          m_s11("s11", ctx, this),
          m_s12("s12", ctx, this),
          m_s13("s13", ctx, this)
    {
    }

    S11 m_s11;
    S12 m_s12;
    S13 m_s13;

    void createTransitionTable() override
    {
    }
};

}  // ns: sm2

#endif  // ROOST_SM2_HPP
