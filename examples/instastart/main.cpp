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

/*
 * ******************************************************
 *                       READ THIS
 * ******************************************************
 *
 * This example is meant to be a quick example to grok
 * how things can be setup.
 *
 * IT IS NOT RECOMMENDED TO USE THIS SETUP FOR PRODUCTION.
 *
 * The recommended way is under the examples/quickstart
 * folder, which implements this exact same state machine
 * but structures the code appropriately.
 *
 */

#include "roost/state_machine.hpp"

enum class Evt
{
    ROOST_NONE,  // Enforced by framework
    E1,
    E2,
    E3
};

static const char* EvtStrings[] = {"NONE", "E1", "E2", "E3"};

ROOST_ENUM_PRINT_HELPER(Evt, EvtStrings)

class RootState;

// The "Context" struct, used to share global data
struct Ctx
{
    RootState* m_root;
};

using SMTypes = roost::NodeAlias<Ctx, Evt>;

class S1 : public SMTypes::Leaf
{
public:
    S1(const char* name, Ctx& ctx, SMTypes::Node* parent) : SMTypes::Leaf(name, ctx, parent)
    {
    }

    // onEntry and onExit can be optionally defined for any node
    void onEntry() override
    {
        // Do on entry action here
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

    // onEntry and onExit can be optionally defined for any node
    void onExit() override
    {
        // Do on exit action here
    }

    S1 m_s1;  // Initial State (See constructor)
    S2 m_s2;

    void createTransitionTable() override;
};

/*
 * We put the definitions of createTransitionTable() after RootState due to
 * using a forward declaration.
 *
 * Additionally, we only want to use references (not pointers) in these functions.
 * Actions and guards capture by reference, so pointers and other local variables
 * will return garbage when evaluated.
 *
 */

void S1::createTransitionTable()
{
    // We only can use references (not pointers) here!
    // See above for explanation.
    RootState& rs = *m_ctx.m_root;

    // Don't forget the & on the destination!
    addRow(Evt::E1, &rs.m_s2, ROOST_NO_ACTION, ROOST_NO_GUARD);
}

void S2::createTransitionTable()
{
    // We only can use references (not pointers) here!
    // See above for explanation.
    RootState& rs = *m_ctx.m_root;

    // Don't forget the & on the destination!
    addRow(Evt::E2, &rs.m_s1, ROOST_NO_ACTION, ROOST_NO_GUARD);
}

void RootState::createTransitionTable()
{
    // We only can use references (not pointers) here!
    // See above for explanation.
    RootState& rs = *m_ctx.m_root;

    // Don't forget the & on the destination!
    addRow(Evt::E3, &rs.m_s2, ROOST_NO_ACTION, ROOST_NO_GUARD);
}

int main(int, char**)
{
    Ctx       ctx;
    RootState root("root", ctx, nullptr);

    // Very important to set root pointer!
    ctx.m_root = &root;

    SMTypes::StateMachine backend("Backend", &root, roost::make_unique<SMTypes::PrintingSpy>());

    if (!backend.init())
    {
        std::cerr << "Could not initialize!" << std::endl;
        return -1;
    }

    backend.handleEvent(Evt::E1);
    backend.handleEvent(Evt::E2);
    backend.handleEvent(Evt::E3);

    return 0;
}
