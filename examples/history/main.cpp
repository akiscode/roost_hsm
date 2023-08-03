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

#include "history_sm.hpp"

int main(int, char**)
{
    using namespace history;

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
    backend.handleEvent(Evt::E1);
    backend.handleEvent(Evt::E1);

    backend.handleEvent(Evt::E2);

    backend.handleEvent(Evt::E4);

    backend.handleEvent(Evt::E2);

    backend.handleEvent(Evt::E3);

    return 0;
}
