<img align="left" width="120" height="120" src="docs/images/roost_hsm_logo.png"> 

# Roost HSM
*A (sane) alternative to Boost MSM.*

---

Need to use Hierarchical State Machines (HSMs) while programming in C++?  Don't want to use Boost MSM?  

This is the library for you.

Roost HSM is a C++11 HSM framework that is:

- Capable of large and nested state machines without long compile times
- No unnecessary memory allocations after init and no RTTI
- Designed from the ground up to be unit testable
- Can output SCXML and GraphViz for visualization

In addition to this, Roost HSM supports all your favorite HSM features:

- Easy-to-Read Transition Tables
- Arbitrary Nesting of States and State Machines
- Shallow and Deep History 
- Orthogonal (Parallel) States
- Wildcard transitions

and more!

**Contact:** <dev@akiscode.com>

![Linux Build](https://github.com/akiscode/roost_hsm/actions/workflows/build-linux.yml/badge.svg)

### Prerequisites 

1) A compiler that supports the C++11 Standard
2) CMake >= 3.5

## Build

`mkdir build; cd build; cmake ..; make -j4`


## Insta-Start Sample

**Examples folder located:**  [Here](examples/)   | **Developers Guide located:** [Here](DevGuide.md)

<img align="left" src="examples/instastart/img/instastart_sm.png ">


```bash
 
 
    ******************************************************
                          READ THIS
    ******************************************************
 
    This example is meant to be a quick example to grok
    how things can be setup.
 
    IT IS NOT RECOMMENDED TO USE THIS SETUP FOR PRODUCTION.
 
    The recommended way is under the examples/quickstart
    folder, which implements this exact same state machine
    but structures the code appropriately.
 
 
 ```
 ```c++

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
```

## CMake External Project

Add this to your current project quickly with an external project add in CMake:

```cmake
set(ROOST_HSM_PREFIX ${CMAKE_CURRENT_BINARY_DIR}/externals/roost-hsm-lib )
set(ROOST_HSM_LIB ${ROOST_HSM_PREFIX}/src/roost-hsm-lib-build/lib )
set(ROOST_HSM_INCLUDE ${ROOST_HSM_PREFIX}/src/roost-hsm-lib/lib/include )

ExternalProject_Add(
        roost-hsm-lib
        GIT_REPOSITORY "https://github.com/akiscode/roost_hsm.git"
        GIT_TAG v1.0.0
        PREFIX ${ROOST_HSM_PREFIX}
        CONFIGURE_COMMAND ${CMAKE_COMMAND} -DPRODUCTION_BUILD=ON ../roost-hsm-lib
        BUILD_COMMAND ${CMAKE_COMMAND} --build .
        INSTALL_COMMAND ""
        CMAKE_ARGS "-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}"
        BUILD_BYPRODUCTS "${ROOST_HSM_LIB}/${CMAKE_FIND_LIBRARY_PREFIXES}roosthsm.a"
)

add_library(roost-hsm STATIC IMPORTED )

add_dependencies(roost-hsm roost-hsm-lib)

include_directories("${ROOST_HSM_INCLUDE}")

set_target_properties(roost-hsm PROPERTIES
        IMPORTED_LOCATION "${ROOST_HSM_LIB}/${CMAKE_FIND_LIBRARY_PREFIXES}roosthsm.a"
)

# Usage:
# add_executable(app1 ....);
# target_link_libraries(app1 roost-hsm ...);
```

## Commercial Support

If you want commercial support, feel free to contact: <dev@akiscode.com>

## License

Unless otherwise specified, the code in this project is governed under the provisions found in LICENSE.
