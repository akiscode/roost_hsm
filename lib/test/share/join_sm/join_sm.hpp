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

#ifndef ROOST_JOIN_SM_HPP
#define ROOST_JOIN_SM_HPP

#include "join_sm_common.hpp"

namespace join_sm
{

class Ortho1Join : public SMTypes::Leaf
{
public:
    Ortho1Join(const char* name, Ctx& ctx, SMTypes::Node* parent)
        : SMTypes::Leaf(name, ctx, parent)
    {
    }

    void onEntry() override;

    void createTransitionTable() override;
};

class SA : public SMTypes::Leaf
{
public:
    SA(const char* name, Ctx& ctx, SMTypes::Node* parent) : SMTypes::Leaf(name, ctx, parent)
    {
    }

    void createTransitionTable() override;
};

class Region1 : public SMTypes::Region
{
public:
    Region1(const char* name, Ctx& ctx, SMTypes::Node* parent)
        : SMTypes::Region(name, ctx, parent, &m_sa),
          m_sa("SA", ctx, this),
          m_join("SA_JOIN", ctx, this)
    {
    }

    SA         m_sa;
    Ortho1Join m_join;
};

// --

class SB : public SMTypes::Leaf
{
public:
    SB(const char* name, Ctx& ctx, SMTypes::Node* parent) : SMTypes::Leaf(name, ctx, parent)
    {
    }

    void createTransitionTable() override;
};

class Region2 : public SMTypes::Region
{
public:
    Region2(const char* name, Ctx& ctx, SMTypes::Node* parent)
        : SMTypes::Region(name, ctx, parent, &m_sb),
          m_sb("SB", ctx, this),
          m_join("SB_JOIN", ctx, this)
    {
    }

    SB m_sb;

    Ortho1Join m_join;
};

// --

class SC : public SMTypes::Leaf
{
public:
    SC(const char* name, Ctx& ctx, SMTypes::Node* parent) : SMTypes::Leaf(name, ctx, parent)
    {
    }

    void createTransitionTable() override;
};

class Region3 : public SMTypes::Region
{
public:
    Region3(const char* name, Ctx& ctx, SMTypes::Node* parent)
        : SMTypes::Region(name, ctx, parent, &m_sc),
          m_sc("SC", ctx, this),
          m_join("SC_JOIN", ctx, this)
    {
    }

    SC m_sc;

    Ortho1Join m_join;
};

class Orthogonal1 : public SMTypes::Orthogonal
{
public:
    Orthogonal1(const char* name, Ctx& ctx, SMTypes::Node* parent)
        : SMTypes::Orthogonal(name, ctx, parent),
          m_toggle(false),
          join_count(3),
          m_region1("Region1", ctx, this),
          m_region2("Region2", ctx, this),
          m_region3("Region3", ctx, this)
    {
    }

    void onExit() override
    {
        join_count = 3;
    }

    void createTransitionTable() override
    {
    }

    void fireNext(Evt const& e)
    {
        switch (e)
        {
            case Evt::FIRST:
                this->postFifo(Evt::SECOND);
                break;
            case Evt::SECOND:
                this->postFifo(Evt::THIRD);
                break;
            // Strange behavior for this one to test toggling
            case Evt::FOURTH:
                this->postFifo(Evt::THIRD);
                break;
            case Evt::THIRD:
                // Intentional fall through
            default:
                break;
        }
    }

    bool m_toggle;

    bool isValid()
    {
        return m_toggle;
    }

    // Need the signature for calling actions
    void toggle(Evt const&)
    {
        m_toggle = !m_toggle;
    }

    int join_count;

    Region1 m_region1;
    Region2 m_region2;
    Region3 m_region3;
};

class SF : public SMTypes::Leaf
{
public:
    SF(const char* name, Ctx& ctx, SMTypes::Node* parent) : SMTypes::Leaf(name, ctx, parent)
    {
    }

    void createTransitionTable() override;
};

class S1 : public SMTypes::Composite
{
public:
    S1(const char* name, Ctx& ctx, SMTypes::Node* parent)
        : SMTypes::Composite(name, ctx, parent, &m_ortho1),
          m_ortho1("Ortho1", ctx, this),
          m_sf("SF", ctx, this)
    {
    }

    void createTransitionTable() override
    {
    }

    Orthogonal1 m_ortho1;
    SF          m_sf;
};

}  // ns: join_sm

#endif  // ROOST_JOIN_SM_HPP
