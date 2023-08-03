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

#include "hayai/hayai.hpp"

#include "roost/state_machine.hpp"
#include "sm1/sm1.hpp"
#include "sm2/sm2.hpp"

class SM1Bench : public ::hayai::Fixture
{
public:
    sm1::Ctx       ctx;
    sm1::RootState root{"root", ctx, nullptr};

    sm1::SMTypes::StateMachine* be;

    virtual void SetUp()
    {
        ctx.m_root = &root;

        be = new sm1::SMTypes::StateMachine("TestBackend", &root);
        be->init();
    }

    virtual void TearDown()
    {
        delete be;
    }
};

BENCHMARK_F(SM1Bench, smoke_test, 100, 100)
{
    be->handleEvent(sm1::Evt::FIRST);
}

class SM2Bench : public ::hayai::Fixture
{
public:
    sm2::Ctx       ctx;
    sm2::RootState root{"root", ctx, nullptr};

    sm2::SMTypes::StateMachine* be;

    virtual void SetUp()
    {
        ctx.m_root = &root;

        be = new sm2::SMTypes::StateMachine("TestBackend", &root);
        be->init();
        be->forceTransitionTo(&root.m_s11.m_s111.m_s1111.m_s11113.m_se);
    }

    virtual void TearDown()
    {
        delete be;
    }
};

BENCHMARK_F(SM2Bench, smoke_test, 100, 100)
{
    be->handleEvent(sm2::Evt::SECOND);
}
