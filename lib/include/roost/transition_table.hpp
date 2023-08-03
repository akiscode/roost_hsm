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

#ifndef ROOST_LIB_TRANSITION_TABLE_HPP
#define ROOST_LIB_TRANSITION_TABLE_HPP

#include "roost/common.hpp"

#include <functional>  // std::function
#include <vector>

#define ROOST_ACTION(x)                                     \
    roost::ActionFunctor<CTX_TYPE, EVENT_TYPE>(             \
            [&, this](EVENT_TYPE const &e) {                \
                if (m_spy)                                  \
                {                                           \
                    m_spy->action(getName(), m_ctx, e, #x); \
                }                                           \
                x(e);                                       \
            },                                              \
            #x)

#define ROOST_GUARD(x)                                           \
    roost::GuardFunctor<CTX_TYPE, EVENT_TYPE>(                   \
            [&, this](EVENT_TYPE const &e) {                     \
                bool rval = (x);                                 \
                if (m_spy)                                       \
                {                                                \
                    m_spy->guard(getName(), m_ctx, e, #x, rval); \
                }                                                \
                return rval;                                     \
            },                                                   \
            #x)

#define ROOST_NO_ACTION \
    {                   \
    }

#define ROOST_NO_GUARD \
    roost::GuardFunctor<CTX_TYPE, EVENT_TYPE>([](EVENT_TYPE const &) { return true; }, nullptr)

#define ROOST_NO_DEST nullptr

namespace roost
{

template <typename CTX, typename E, typename>
class Node;

template <typename CTX, typename E, typename>
class RegionNode;

template <typename CTX, typename E>
using ActionFunctionPtr = std::function<void(E const &)>;

template <typename CTX, typename E>
using GuardFunctionPtr = std::function<bool(E const &)>;

template <typename CTX, typename E>
struct ActionFunctor
{
    ActionFunctor(ActionFunctionPtr<CTX, E> &&action_fptr, const char *name) noexcept
        : m_action_fptr(std::move(action_fptr)), m_name(name)
    {
    }

    ActionFunctionPtr<CTX, E> m_action_fptr;
    const char *              m_name;
};  // Class: ActionFunctor

template <typename CTX, typename E>
struct GuardFunctor
{

    GuardFunctor() : m_guard_fptr(nullptr), m_name(nullptr)
    {
    }

    GuardFunctor(GuardFunctionPtr<CTX, E> &&guard_fptr, const char *name) noexcept
        : m_guard_fptr(std::move(guard_fptr)), m_name(name)
    {
    }

    GuardFunctionPtr<CTX, E> m_guard_fptr;
    const char *             m_name;
};  // Class: GuardFunctor

template <typename CTX, typename E>
struct TransitionTableEntry
{

    Node<CTX, E, void *> *             m_src;
    RegionNode<CTX, E, void *> *       m_src_region;
    Node<CTX, E, void *> *             m_destination;
    std::vector<ActionFunctor<CTX, E>> m_actions;
    GuardFunctor<CTX, E>               m_guard;
    Node<CTX, E, void *> *             m_lca;
    RegionNode<CTX, E, void *> *       m_lca_region;

    void outputSCXML(E event, std::ostream &os)
    {

        os << "<transition type=\"internal\" event=\"" << getStringLiteral(event) << "\" ";

        if (m_guard.m_name != nullptr)
        {
            os << "cond=\"" << m_guard.m_name << "\" ";
        }

        if (m_destination != nullptr)
        {

            if ((m_destination->getType() == NodeType::SHALLOW_HISTORY_NODE) ||
                m_destination->getType() == NodeType::DEEP_HISTORY_NODE)
            {
                os << "target=\"" << m_destination->getParent()->getName() << "."
                   << m_destination->getName() << "\" ";
            }
            else
            {
                os << "target=\"" << m_destination->getName() << "\" ";
            }
        }

        os << ">" << std::endl;

        if (!m_actions.empty())
        {

            os << "<script>" << std::endl;

            for (ActionFunctor<CTX, E> &action : m_actions)
            {
                os << action.m_name << "(" << event << ");" << std::endl;
            }

            os << "</script>" << std::endl;
        }

        os << "</transition>" << std::endl;
    }
};

}  // ns: roost

#endif  // ROOST_LIB_TRANSITION_TABLE_HPP
