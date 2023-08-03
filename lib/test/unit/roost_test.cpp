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

#include <gtest/gtest.h>
#include <iostream>

#include "join_sm/join_sm.hpp"
#include "ortho_history/ortho_history.hpp"
#include "roost/state_machine.hpp"
#include "simple_history/simple_history.hpp"
#include "sm1/sm1.hpp"
#include "sm2/sm2.hpp"

class RoostTestFixture : public ::testing::Test
{
    void SetUp() override
    {
    }

    void TearDown() override
    {
    }
};

TEST_F(RoostTestFixture, event_test)
{
    using namespace sm1;

    Ctx       ctx;
    RootState root("root", ctx, nullptr);
    ctx.m_root = &root;

    std::vector<std::string>      actual_states;
    std::shared_ptr<SMTypes::Spy> spy = std::make_shared<SMTypes::TracingSpy>(actual_states);

    SMTypes::StateMachine be("TestBackend", &root, std::move(spy));
    ASSERT_TRUE(be.init());

    std::vector<std::string> expected_states = {
            "OE-root", "OE-sm11", "OE-sm112", "OX-sm112", "OE-sm111", "OE-sm1111"};

    ASSERT_EQ(actual_states, expected_states);

    actual_states.clear();

    be.handleEvent(Evt::SECOND);
    be.handleEvent(Evt::FIRST);
    be.handleEvent(Evt::THIRD);
    be.handleEvent(Evt::FIRST);

    expected_states = {"OX-sm1111",  "OX-sm111",   "OX-sm11",    "OE-sm12",   "OE-sm122",
                       "OE-sm1221",  "OE-sm12211", "OX-sm12211", "OX-sm1221", "OX-sm122",
                       "OX-sm12",    "OE-sm11",    "OE-sm111",   "OE-sm1111", "OX-sm1111",
                       "OX-sm111",   "OX-sm11",    "OE-sm12",    "OE-sm122",  "OE-sm1221",
                       "OE-sm12211", "OX-sm12211", "OX-sm1221",  "OX-sm122",  "OX-sm12",
                       "OE-sm11",    "OE-sm111",   "OE-sm1111"

    };

    ASSERT_EQ(actual_states, expected_states);
}

TEST_F(RoostTestFixture, depth_test)
{
    using namespace sm1;
    Ctx       ctx;
    RootState root("root", ctx, nullptr);
    ctx.m_root = &root;

    ASSERT_EQ(SMTypes::StateMachine::compute_max_depth(&root), (size_t)5);
}

TEST_F(RoostTestFixture, lca_test)
{
    using namespace sm1;
    Ctx       ctx;
    RootState root("root", ctx, nullptr);
    ctx.m_root = &root;

    ASSERT_EQ(
            SMTypes::Node::findLca(
                    &root.m_sm11.m_sm111.m_sm1111, &root.m_sm12.m_sm122.m_sm1221.m_sm12211),
            &root);

    // The following 3 tests should return nullptr
    ASSERT_FALSE(SMTypes::Node::findLca(&root, nullptr));
    ASSERT_FALSE(SMTypes::Node::findLca(nullptr, nullptr));
    ASSERT_FALSE(SMTypes::Node::findLca(nullptr, &root));

    ASSERT_EQ(SMTypes::Node::findLca(&root.m_sm12.m_sm121, &root.m_sm12.m_sm122), &root.m_sm12);
    ASSERT_EQ(
            SMTypes::Node::findLca(&root.m_sm12.m_sm122, &root.m_sm12.m_sm122.m_sm1221),
            &root.m_sm12.m_sm122);

    ASSERT_EQ(
            SMTypes::Node::findLca(&root.m_sm11.m_sm111.m_sm1111, &root.m_sm11.m_sm112),
            &root.m_sm11);

    // A special test for "self-transition".  We need the LCA to be "one above" the node for our
    // generic algorithm to work
    ASSERT_EQ(SMTypes::Node::findLca(&root.m_sm11, &root.m_sm11), &root);

    ASSERT_EQ(SMTypes::Node::findLca(&root.m_sm11.m_sm111.m_sm1111, &root.m_sm11), &root.m_sm11);

    ASSERT_EQ(SMTypes::Node::findLca(&root.m_sm11, &root.m_sm11.m_sm111.m_sm1111), &root.m_sm11);
}

TEST_F(RoostTestFixture, orthogonal_smoke_test)
{
    using namespace sm2;
    Ctx       ctx;
    RootState root("s1", ctx, nullptr);
    ctx.m_root = &root;

    std::vector<std::string>      actual_states;
    std::shared_ptr<SMTypes::Spy> spy = std::make_shared<SMTypes::TracingSpy>(actual_states);

    SMTypes::StateMachine be("TestBackend", &root, std::move(spy));

    std::vector<std::string> current_nodes  = be.getCurrentNodes();
    std::vector<std::string> expected_nodes = {};

    ASSERT_EQ(current_nodes, expected_nodes);

    ASSERT_TRUE(be.init());

    std::vector<std::string> expected_states = {"OE-s1", "OE-s12"};

    ASSERT_EQ(actual_states, expected_states);
    actual_states.clear();

    current_nodes  = be.getCurrentNodes();
    expected_nodes = {"s12"};

    ASSERT_EQ(current_nodes, expected_nodes);

    be.handleEvent(Evt::FIRST);

    expected_states = {"OX-s12", "OE-s11", "OE-s1112", "OE-s11122", "OE-s1121", "OE-s11211"};

    ASSERT_EQ(actual_states, expected_states);
    actual_states.clear();

    current_nodes  = be.getCurrentNodes();
    expected_nodes = {"s11", "s11122", "s11211"};

    ASSERT_EQ(current_nodes, expected_nodes);

    be.handleEvent(Evt::FIRST);

    expected_states = {"OX-s11122", "OX-s1112", "OX-s11211", "OX-s1121", "OX-s11", "OE-s13"};

    ASSERT_EQ(actual_states, expected_states);
    actual_states.clear();

    be.handleEvent(Evt::FIRST);

    expected_states = {
            "OX-s13", "OE-s11", "OE-s1121", "OE-s11211", "OE-s1111", "OE-sb", "OE-se", "OE-sc"};

    ASSERT_EQ(actual_states, expected_states);
    actual_states.clear();

    be.handleEvent(Evt::SECOND);

    expected_states = {"OX-sb",
                       "OE-sa",
                       "OX-sa",
                       "OX-sc",
                       "OX-se",
                       "OX-s1111",
                       "OE-s1112",
                       "OE-s11121",
                       "OX-s11211",
                       "OE-s11212"};

    ASSERT_EQ(actual_states, expected_states);
    actual_states.clear();

    be.handleEvent(Evt::THIRD);
    expected_states = {"OX-s11121", "OX-s1112", "OE-s1111", "OE-sb", "OE-se", "OE-sc"};

    ASSERT_EQ(actual_states, expected_states);
    actual_states.clear();

    be.handleEvent(Evt::FOURTH);
    expected_states = {"OX-sb",
                       "OE-sa",
                       "OX-sa",
                       "OX-sc",
                       "OX-se",
                       "OX-s1111",
                       "OX-s11212",
                       "OX-s1121",
                       "OX-s11",
                       "OE-s13"};

    ASSERT_EQ(actual_states, expected_states);
    actual_states.clear();

    be.handleEvent(Evt::THIRD);
    expected_states = {};

    ASSERT_EQ(actual_states, expected_states);
    actual_states.clear();

    // Do it again to make sure we didn't move

    be.handleEvent(Evt::THIRD);
    expected_states = {};

    ASSERT_EQ(actual_states, expected_states);
    actual_states.clear();

    be.handleEvent(Evt::FOURTH);
    expected_states = {"OX-s13", "OE-s11", "OE-s1112", "OE-s11122", "OE-s1121", "OE-s11211"};

    ASSERT_EQ(actual_states, expected_states);
    actual_states.clear();

    be.handleEvent(Evt::FIFTH);
    expected_states = {"OX-s11122",
                       "OX-s1112",
                       "OX-s11211",
                       "OX-s1121",
                       "OX-s11",
                       "OE-s11",
                       "OE-s1112",
                       "OE-s11122",
                       "OE-s1121",
                       "OE-s11211"};

    ASSERT_EQ(actual_states, expected_states);
    actual_states.clear();

    be.handleEvent(Evt::FIFTH);
    expected_states = {"OX-s11122",
                       "OX-s1112",
                       "OX-s11211",
                       "OX-s1121",
                       "OX-s11",
                       "OE-s11",
                       "OE-s1112",
                       "OE-s11122",
                       "OE-s1121",
                       "OE-s11211"};

    ASSERT_EQ(actual_states, expected_states);
    actual_states.clear();
}

TEST_F(RoostTestFixture, join_smoke_test)
{
    using namespace join_sm;
    Ctx ctx;
    S1  s1("s1", ctx, nullptr);
    ctx.m_s1 = &s1;

    std::vector<std::string>      actual_states;
    std::shared_ptr<SMTypes::Spy> spy = std::make_shared<SMTypes::TracingSpy>(actual_states);

    SMTypes::StateMachine be("TestBackend", &s1, std::move(spy));
    ASSERT_TRUE(be.init());

    std::vector<std::string> expected_states = {"OE-s1", "OE-Ortho1", "OE-SA", "OE-SB", "OE-SC"};

    ASSERT_EQ(actual_states, expected_states);
    actual_states.clear();

    std::vector<std::string> current_nodes  = be.getCurrentNodes();
    std::vector<std::string> expected_nodes = {"Ortho1", "SA", "SB", "SC"};

    ASSERT_EQ(current_nodes, expected_nodes);

    be.handleEvent(Evt::FIRST);

    expected_states = {"OX-SA", "OE-SA_JOIN", "OX-SB", "OE-SB_JOIN"};

    ASSERT_EQ(actual_states, expected_states);
    actual_states.clear();

    be.handleEvent(Evt::FOURTH);

    expected_states = {"OX-SC",
                       "OE-SC_JOIN",
                       "OX-SA_JOIN",
                       "OX-SB_JOIN",
                       "OX-SC_JOIN",
                       "OX-Ortho1",
                       "OE-SF"};

    ASSERT_EQ(actual_states, expected_states);
    actual_states.clear();

    current_nodes  = be.getCurrentNodes();
    expected_nodes = {"SF"};

    ASSERT_EQ(current_nodes, expected_nodes);
}

TEST_F(RoostTestFixture, join_sm_force_transition_test)
{
    using namespace join_sm;
    Ctx ctx;
    S1  s1("s1", ctx, nullptr);
    ctx.m_s1 = &s1;

    std::vector<std::string>      actual_states;
    std::shared_ptr<SMTypes::Spy> spy = std::make_shared<SMTypes::TracingSpy>(actual_states);

    SMTypes::StateMachine be("TestBackend", &s1, std::move(spy));
    ASSERT_TRUE(be.init());

    std::vector<std::string> current_nodes  = be.getCurrentNodes();
    std::vector<std::string> expected_nodes = {"Ortho1", "SA", "SB", "SC"};

    ASSERT_EQ(current_nodes, expected_nodes);

    be.forceTransitionTo(&s1);

    current_nodes  = be.getCurrentNodes();
    expected_nodes = {"Ortho1", "SA", "SB", "SC"};

    ASSERT_EQ(current_nodes, expected_nodes);

    be.forceTransitionTo(&s1.m_ortho1.m_region1.m_sa);

    current_nodes  = be.getCurrentNodes();
    expected_nodes = {"Ortho1", "SA", "SB", "SC"};

    ASSERT_EQ(current_nodes, expected_nodes);

    be.forceTransitionTo(&s1.m_sf);

    current_nodes  = be.getCurrentNodes();
    expected_nodes = {"SF"};

    ASSERT_EQ(current_nodes, expected_nodes);

    be.forceTransitionTo(&s1.m_ortho1);

    current_nodes  = be.getCurrentNodes();
    expected_nodes = {"Ortho1", "SA", "SB", "SC"};

    ASSERT_EQ(current_nodes, expected_nodes);
}

TEST_F(RoostTestFixture, shallow_history_smoke_test)
{
    using namespace simple_history;
    Ctx       ctx;
    RootState root("RootState", ctx, nullptr);
    ctx.m_root = &root;

    std::vector<std::string>      actual_states;
    std::shared_ptr<SMTypes::Spy> spy = std::make_shared<SMTypes::TracingSpy>(actual_states);

    SMTypes::StateMachine be("TestBackend", &root, std::move(spy));
    ASSERT_TRUE(be.init());
    be.handleEvent(Evt::FIRST);

    std::vector<std::string> current_nodes  = be.getCurrentNodes();
    std::vector<std::string> expected_nodes = {"State22"};
    ASSERT_EQ(current_nodes, expected_nodes);

    be.forceTransitionTo(&root.m_state1);

    current_nodes  = be.getCurrentNodes();
    expected_nodes = {"State1"};
    ASSERT_EQ(current_nodes, expected_nodes);

    actual_states.clear();

    be.handleEvent(Evt::SECOND);
    be.handleEvent(Evt::FIRST);
    be.handleEvent(Evt::SECOND);
    be.handleEvent(Evt::THIRD);
    be.handleEvent(Evt::SEVENTH);

    std::vector<std::string> expected_states = {"OX-State1",
                                                "OE-State1",
                                                "OX-State1",
                                                "OE-State2",
                                                "OE-State22",
                                                "OX-State22",
                                                "OE-State21",
                                                "OE-State211",
                                                "OX-State211",
                                                "OE-State212",
                                                "OX-State212",
                                                "OX-State21",
                                                "OX-State2",
                                                "OE-State1"};

    ASSERT_EQ(actual_states, expected_states);
    actual_states.clear();

    current_nodes  = be.getCurrentNodes();
    expected_nodes = {"State1"};
    ASSERT_EQ(current_nodes, expected_nodes);

    be.handleEvent(Evt::FIFTH);

    expected_states = {
            "OX-State1", "OE-State2", "OE-ShallowHistory", "OE-State21", "OE-State211"};

    ASSERT_EQ(actual_states, expected_states);
    actual_states.clear();

    current_nodes  = be.getCurrentNodes();
    expected_nodes = {"State211"};
    ASSERT_EQ(current_nodes, expected_nodes);
}

TEST_F(RoostTestFixture, shallow_history_default_history_test)
{
    using namespace simple_history;
    Ctx       ctx;
    RootState root("RootState", ctx, nullptr);
    ctx.m_root = &root;

    std::vector<std::string>      actual_states;
    std::shared_ptr<SMTypes::Spy> spy = std::make_shared<SMTypes::TracingSpy>(actual_states);

    SMTypes::StateMachine be("TestBackend", &root, std::move(spy));
    ASSERT_TRUE(be.init());
    be.handleEvent(Evt::FIFTH);

    std::vector<std::string> expected_states = {"OE-RootState",
                                                "OE-State1",
                                                "OX-State1",
                                                "OE-State2",
                                                "OE-ShallowHistory",
                                                "OE-State22"};

    ASSERT_EQ(actual_states, expected_states);
    actual_states.clear();

    std::vector<std::string> current_nodes  = be.getCurrentNodes();
    std::vector<std::string> expected_nodes = {"State22"};
    ASSERT_EQ(current_nodes, expected_nodes);
}

TEST_F(RoostTestFixture, deep_history_smoke_test)
{
    using namespace simple_history;
    Ctx       ctx;
    RootState root("RootState", ctx, nullptr);
    ctx.m_root = &root;

    std::vector<std::string>      actual_states;
    std::shared_ptr<SMTypes::Spy> spy = std::make_shared<SMTypes::TracingSpy>(actual_states);

    SMTypes::StateMachine be("TestBackend", &root, std::move(spy));
    ASSERT_TRUE(be.init());

    ctx.use_deep_history = true;

    for (int i = 0; i < 3; i++)
    {

        actual_states.clear();

        be.handleEvent(Evt::FIRST);
        be.handleEvent(Evt::SECOND);
        be.handleEvent(Evt::THIRD);
        be.handleEvent(Evt::SEVENTH);

        std::vector<std::string> expected_states = {"OX-State1",
                                                    "OE-State2",
                                                    "OE-State22",
                                                    "OX-State22",
                                                    "OE-State21",
                                                    "OE-State211",
                                                    "OX-State211",
                                                    "OE-State212",
                                                    "OX-State212",
                                                    "OX-State21",
                                                    "OX-State2",
                                                    "OE-State1"};

        ASSERT_EQ(actual_states, expected_states);
        actual_states.clear();

        std::vector<std::string> current_nodes  = be.getCurrentNodes();
        std::vector<std::string> expected_nodes = {"State1"};
        ASSERT_EQ(current_nodes, expected_nodes);

        be.handleEvent(Evt::FIFTH);

        expected_states = {
                "OX-State1", "OE-State2", "OE-DeepHistory", "OE-State21", "OE-State212"};

        ASSERT_EQ(actual_states, expected_states);
        actual_states.clear();

        current_nodes  = be.getCurrentNodes();
        expected_nodes = {"State212"};
        ASSERT_EQ(current_nodes, expected_nodes);
        be.handleEvent(Evt::SEVENTH);
    }

    actual_states.clear();

    std::vector<std::string> current_nodes  = be.getCurrentNodes();
    std::vector<std::string> expected_nodes = {"State1"};
    ASSERT_EQ(current_nodes, expected_nodes);

    be.handleEvent(Evt::FIRST);
    be.handleEvent(Evt::SECOND);

    std::vector<std::string> expected_states = {
            "OX-State1", "OE-State2", "OE-State22", "OX-State22", "OE-State21", "OE-State211"};

    ASSERT_EQ(actual_states, expected_states);
    actual_states.clear();
}

TEST_F(RoostTestFixture, deep_history_default_history_test)
{
    using namespace simple_history;
    Ctx       ctx;
    RootState root("RootState", ctx, nullptr);
    ctx.m_root           = &root;
    ctx.use_deep_history = true;

    std::vector<std::string>      actual_states;
    std::shared_ptr<SMTypes::Spy> spy = std::make_shared<SMTypes::TracingSpy>(actual_states);

    SMTypes::StateMachine be("TestBackend", &root, std::move(spy));
    ASSERT_TRUE(be.init());
    be.handleEvent(Evt::FIFTH);

    std::vector<std::string> expected_states = {"OE-RootState",
                                                "OE-State1",
                                                "OX-State1",
                                                "OE-State2",
                                                "OE-DeepHistory",
                                                "OE-State22"};

    ASSERT_EQ(actual_states, expected_states);
    actual_states.clear();

    std::vector<std::string> current_nodes  = be.getCurrentNodes();
    std::vector<std::string> expected_nodes = {"State22"};
    ASSERT_EQ(current_nodes, expected_nodes);
}

TEST_F(RoostTestFixture, ortho_history_shallow_test)
{
    using namespace ortho_history;
    Ctx       ctx;
    RootState root("RootState", ctx, nullptr);
    ctx.m_root           = &root;
    ctx.use_deep_history = false;

    std::vector<std::string>      actual_states;
    std::shared_ptr<SMTypes::Spy> spy = std::make_shared<SMTypes::TracingSpy>(actual_states);

    SMTypes::StateMachine be("TestBackend", &root, std::move(spy));
    ASSERT_TRUE(be.init());

    std::vector<std::string> expected_states = {"OE-RootState", "OE-State0", "OE-State1"};

    ASSERT_EQ(actual_states, expected_states);
    actual_states.clear();

    std::vector<std::string> current_nodes  = be.getCurrentNodes();
    std::vector<std::string> expected_nodes = {"State1"};
    ASSERT_EQ(current_nodes, expected_nodes);

    be.handleEvent(Evt::STEP);
    be.handleEvent(Evt::STEP);

    expected_states = {"OX-State1",
                       "OE-State2",
                       "OE-State6",
                       "OE-State7",
                       "OX-State6",
                       "OE-State8",
                       "OX-State7",
                       "OE-State9"};
    ASSERT_EQ(actual_states, expected_states);
    actual_states.clear();

    current_nodes  = be.getCurrentNodes();
    expected_nodes = {"State2", "State8", "State9"};
    ASSERT_EQ(current_nodes, expected_nodes);

    be.handleEvent(Evt::OPEN);

    expected_states = {"OX-State8", "OX-State9", "OX-State2", "OX-State0", "OE-State3"};
    ASSERT_EQ(actual_states, expected_states);
    actual_states.clear();

    current_nodes  = be.getCurrentNodes();
    expected_nodes = {"State3"};
    ASSERT_EQ(current_nodes, expected_nodes);

    be.handleEvent(Evt::CLOSE);

    expected_states = {
            "OX-State3", "OE-State0", "OE-ShallowHistory", "OE-State2", "OE-State6", "OE-State7"};
    ASSERT_EQ(actual_states, expected_states);
    actual_states.clear();

    current_nodes  = be.getCurrentNodes();
    expected_nodes = {"State2", "State6", "State7"};
    ASSERT_EQ(current_nodes, expected_nodes);

    be.handleEvent(Evt::BACK);
    be.handleEvent(Evt::OPEN);
    be.handleEvent(Evt::CLOSE);

    expected_states = {"OX-State6",
                       "OX-State7",
                       "OX-State2",
                       "OE-State1",
                       "OX-State1",
                       "OX-State0",
                       "OE-State3",
                       "OX-State3",
                       "OE-State0",
                       "OE-ShallowHistory",
                       "OE-State1"};
    ASSERT_EQ(actual_states, expected_states);
    actual_states.clear();

    current_nodes  = be.getCurrentNodes();
    expected_nodes = {"State1"};
    ASSERT_EQ(current_nodes, expected_nodes);

    be.handleEvent(Evt::DIRECT);

    expected_states = {"OX-State1", "OE-State2", "OE-State6", "OE-State9"};
    ASSERT_EQ(actual_states, expected_states);
    actual_states.clear();

    current_nodes  = be.getCurrentNodes();
    expected_nodes = {"State2", "State6", "State9"};
    ASSERT_EQ(current_nodes, expected_nodes);

    be.handleEvent(Evt::OPEN);
    be.handleEvent(Evt::CLOSE);

    expected_states = {"OX-State6",
                       "OX-State9",
                       "OX-State2",
                       "OX-State0",
                       "OE-State3",
                       "OX-State3",
                       "OE-State0",
                       "OE-ShallowHistory",
                       "OE-State2",
                       "OE-State6",
                       "OE-State7"};
    ASSERT_EQ(actual_states, expected_states);
    actual_states.clear();

    current_nodes  = be.getCurrentNodes();
    expected_nodes = {"State2", "State6", "State7"};
    ASSERT_EQ(current_nodes, expected_nodes);
}

TEST_F(RoostTestFixture, ortho_history_deep_test)
{
    using namespace ortho_history;
    Ctx       ctx;
    RootState root("RootState", ctx, nullptr);
    ctx.m_root           = &root;
    ctx.use_deep_history = true;

    std::vector<std::string>      actual_states;
    std::shared_ptr<SMTypes::Spy> spy = std::make_shared<SMTypes::TracingSpy>(actual_states);

    SMTypes::StateMachine be("TestBackend", &root, std::move(spy));
    ASSERT_TRUE(be.init());

    std::vector<std::string> expected_states = {"OE-RootState", "OE-State0", "OE-State1"};

    ASSERT_EQ(actual_states, expected_states);
    actual_states.clear();

    std::vector<std::string> current_nodes  = be.getCurrentNodes();
    std::vector<std::string> expected_nodes = {"State1"};
    ASSERT_EQ(current_nodes, expected_nodes);

    be.handleEvent(Evt::STEP);
    be.handleEvent(Evt::STEP);

    expected_states = {"OX-State1",
                       "OE-State2",
                       "OE-State6",
                       "OE-State7",
                       "OX-State6",
                       "OE-State8",
                       "OX-State7",
                       "OE-State9"};
    ASSERT_EQ(actual_states, expected_states);
    actual_states.clear();

    current_nodes  = be.getCurrentNodes();
    expected_nodes = {"State2", "State8", "State9"};
    ASSERT_EQ(current_nodes, expected_nodes);

    be.handleEvent(Evt::OPEN);

    expected_states = {"OX-State8", "OX-State9", "OX-State2", "OX-State0", "OE-State3"};
    ASSERT_EQ(actual_states, expected_states);
    actual_states.clear();

    current_nodes  = be.getCurrentNodes();
    expected_nodes = {"State3"};
    ASSERT_EQ(current_nodes, expected_nodes);

    be.handleEvent(Evt::CLOSE);

    expected_states = {
            "OX-State3", "OE-State0", "OE-DeepHistory", "OE-State2", "OE-State8", "OE-State9"};
    ASSERT_EQ(actual_states, expected_states);
    actual_states.clear();

    current_nodes  = be.getCurrentNodes();
    expected_nodes = {"State2", "State8", "State9"};
    ASSERT_EQ(current_nodes, expected_nodes);

    be.handleEvent(Evt::BACK);
    be.handleEvent(Evt::OPEN);
    be.handleEvent(Evt::CLOSE);

    expected_states = {"OX-State8",
                       "OX-State9",
                       "OX-State2",
                       "OE-State1",
                       "OX-State1",
                       "OX-State0",
                       "OE-State3",
                       "OX-State3",
                       "OE-State0",
                       "OE-DeepHistory",
                       "OE-State1"};
    ASSERT_EQ(actual_states, expected_states);
    actual_states.clear();

    current_nodes  = be.getCurrentNodes();
    expected_nodes = {"State1"};
    ASSERT_EQ(current_nodes, expected_nodes);

    be.handleEvent(Evt::DIRECT);

    expected_states = {"OX-State1", "OE-State2", "OE-State6", "OE-State9"};
    ASSERT_EQ(actual_states, expected_states);
    actual_states.clear();

    current_nodes  = be.getCurrentNodes();
    expected_nodes = {"State2", "State6", "State9"};
    ASSERT_EQ(current_nodes, expected_nodes);

    be.handleEvent(Evt::OPEN);
    be.handleEvent(Evt::CLOSE);

    expected_states = {"OX-State6",
                       "OX-State9",
                       "OX-State2",
                       "OX-State0",
                       "OE-State3",
                       "OX-State3",
                       "OE-State0",
                       "OE-DeepHistory",
                       "OE-State2",
                       "OE-State6",
                       "OE-State9"};
    ASSERT_EQ(actual_states, expected_states);
    actual_states.clear();

    current_nodes  = be.getCurrentNodes();
    expected_nodes = {"State2", "State6", "State9"};
    ASSERT_EQ(current_nodes, expected_nodes);
}
