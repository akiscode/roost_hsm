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

#ifndef ROOST_LIB_NODE_HPP
#define ROOST_LIB_NODE_HPP

#include "roost/alias.hpp"
#include "roost/common.hpp"
#include "roost/spy.hpp"
#include "roost/transition_table.hpp"

#include <cstdint>
#include <cstring>
#include <fstream>
#include <map>
#include <type_traits>
#include <vector>

namespace roost
{

// Some forward declarations for friend classes

template <typename CTX, typename E, typename>
class Node;

template <typename CTX, typename E>
class StateMachine;

template <typename CTX, typename E, typename>
class LeafNode;

template <typename CTX, typename E, typename>
class CompositeNode;

template <typename CTX, typename E, typename>
class OrthogonalNode;

template <typename CTX, typename E, typename>
class RegionNode;

template <typename CTX, typename E, typename>
class ShallowHistoryNode;

template <typename CTX, typename E, typename>
class DeepHistoryNode;

/*!
 * \brief NodeConfiguration contains the config values supplied when configuring nodes
 */
template <typename CTX, typename E>
struct NodeConfiguration
{
    std::shared_ptr<Spy<CTX, E>> spy;
    StateMachine<CTX, E>*        current_state_machine;

};  // Struct: NodeConfiguration

/*!
 * \brief Node is the abstract base that all other nodes derive from
 *
 * This class is supplied two types: a Context type and a Event type
 * which must be an enum class.
 *
 * Because of the use of friend classes and sub-classing by the users of this library,
 * this class exposes three levels:
 * Public:
 * - For use by anyone
 *
 * Protected:
 * - For use by derived classes (i.e. by users of this framework)
 *
 * Private:
 * - Only for use by friend classes
 *
 * \tparam CTX the context type
 * \tparam E the event enum class type
 */
template <
        typename CTX,
        typename E,
        typename = typename std::enable_if<std::is_enum<E>::value, void*>::type>
class Node
{
public:
    using CTX_TYPE           = CTX;
    using EVENT_TYPE         = E;
    using TransitionTableMap = std::map<E, std::vector<TransitionTableEntry<CTX, E>>>;

private:
    friend class StateMachine<CTX, E>;
    friend class LeafNode<CTX, E, void*>;
    friend class CompositeNode<CTX, E, void*>;
    friend class OrthogonalNode<CTX, E, void*>;
    friend class RegionNode<CTX, E, void*>;
    friend class ShallowHistoryNode<CTX, E, void*>;
    friend class DeepHistoryNode<CTX, E, void*>;

    TransitionTableMap         m_transition_table;   //!< The map of events to transitions
    Node<CTX, E>*              m_parent;             //!< Parent pointer
    Node<CTX, E>*              m_initial_child;      //!< Initial child, may be nullptr
    Node<CTX, E>*              m_last_active_child;  //!< The last active child for history
    std::vector<Node<CTX, E>*> m_children;           //!< Children

    E m_none_event;  //!< The None or completion event (must be E::ROOST_NONE)

    NodeType m_node_type;               //!< The type of the node
    bool     m_valid_transition_table;  //!< True if transition table is valid, otherwise false
    StateMachine<CTX, E>* m_current_state_machine;  //!< Pointer to current statemachine handler

protected:
    std::shared_ptr<Spy<CTX, E>> m_spy;   //!< Pointer to spy
    const char*                  m_name;  //!< Name of node

    CTX& m_ctx;  //!< Reference to context

public:
    Node(const char* name, CTX& ctx, NodeType node_type)
        : m_transition_table(),
          m_parent(nullptr),
          m_initial_child(nullptr),
          m_last_active_child(nullptr),
          m_children(),
          m_none_event(E::ROOST_NONE),
          m_node_type(node_type),
          m_valid_transition_table(false),
          m_current_state_machine(nullptr),
          m_spy(nullptr),
          m_name(name),
          m_ctx(ctx)
    {
    }

    virtual ~Node() = default;

    // Delete copy ability
    Node(const Node&) = delete;
    Node& operator=(const Node&) = delete;

    Node(Node&& o) noexcept
        : m_transition_table(std::move(o.m_transition_table)),
          m_parent(o.m_parent),
          m_initial_child(o.m_initial_child),
          m_last_active_child(o.m_last_active_child),
          m_children(std::move(o.m_children)),
          m_none_event(o.m_none_event),
          m_node_type(o.m_node_type),
          m_valid_transition_table(o.m_valid_transition_table),
          m_current_state_machine(o.m_current_state_machine),
          m_spy(std::move(o.m_spy)),
          m_name(o.m_name),
          m_ctx(o.m_ctx)
    {
        o.m_transition_table.clear();
        o.m_parent            = nullptr;
        o.m_initial_child     = nullptr;
        o.m_last_active_child = nullptr;
        o.m_children.clear();
        //        o.m_none_event // Do nothing
        //        o.m_node_type  // Do nothing
        o.m_valid_transition_table = false;
        o.m_current_state_machine  = nullptr;
        o.m_spy                    = nullptr;
        o.m_name                   = "";
        //        o.m_ctx // Do Nothing
    }

    Node& operator=(Node&& o) noexcept
    {
        if (this != &o)
        {
            m_transition_table       = std::move(o.m_transition_table);
            m_parent                 = o.m_parent;
            m_initial_child          = o.m_initial_child;
            m_last_active_child      = o.m_last_active_child;
            m_children               = std::move(o.m_children);
            m_none_event             = o.m_none_event;
            m_node_type              = o.m_node_type;
            m_valid_transition_table = o.m_valid_transition_table;
            m_current_state_machine  = o.m_current_state_machine;
            m_spy                    = std::move(o.m_spy);
            m_name                   = o.m_name;
            m_ctx                    = o.m_ctx;

            o.m_transition_table.clear();
            o.m_parent            = nullptr;
            o.m_initial_child     = nullptr;
            o.m_last_active_child = nullptr;
            o.m_children.clear();
            //        o.m_none_event // Do nothing
            //        o.m_node_type  // Do nothing
            o.m_valid_transition_table = false;
            o.m_current_state_machine  = nullptr;
            o.m_spy                    = nullptr;
            o.m_name                   = "";
            //        o.m_ctx // Do Nothing
        }

        return *this;
    }

    const char* getName() const
    {
        return m_name;
    }

    E getNoneEvt() const
    {
        return m_none_event;
    }

    Node<CTX, E>* getParent() const
    {
        return m_parent;
    }

    NodeType getType() const
    {
        return m_node_type;
    }

    /*!
     * \brief findLca finds the lowest common ancestor (LCA) between two nodes
     *
     * The LCA is defined as being:
     *
     * The lowest (deepest) node in the tree that has both src and dst as children.
     *
     * If either one of these two (src or dst) is a common ancestor of another, then the
     * LCA would be the common ancestor.  This is for the internal transition paradigm
     * taken from the UML state machine standard (as opposed to external transition paradigm).
     *
     * If src == dst, then the LCA would be the parent of src so that the algorithm exits and
     * enters the node.
     *
     * \param src the source node
     * \param dst the destination node
     * \return a pointer to the node that is the LCA
     */
    static Node<CTX, E>* findLca(Node<CTX, E>* src, Node<CTX, E>* dst)
    {

        if (src == nullptr || dst == nullptr)
        {
            return nullptr;
        }

        if (src == dst)
        {
            // This is a special case, we want to set the LCA
            // to the parent to make sure our generic algorithm works
            return src->getParent();
        }

        Node<CTX, E>* original_src = src;

        while (dst != nullptr)
        {
            while (src != nullptr)
            {
                if (src == dst)
                {
                    return src;
                }

                src = src->getParent();
            }

            dst = dst->getParent();
            src = original_src;
        }

        // Shouldn't happen
        return nullptr;
    }

protected:
    /*!
     * \brief postFifo posts an event to the StateMachine queue while handling another event
     *
     * If you are handling an event, the semantics of state machines require that you run to
     * completion (RTC) for the current event.  However, you may still queue up events to be
     * handled while handling another by calling this function within your action or guard.
     *
     * Events will be queued in a FIFO order
     *
     * If there is no statemachine attached to this hierarchy or if init() hasn't been called
     * on the attached state machine, this function does nothing.
     *
     * \param e the event to post to the FIFO queue
     */
    void postFifo(E const& e)
    {
        if (m_current_state_machine == nullptr)
        {
            if (m_spy)
            {
                m_spy->error(
                        m_name,
                        m_ctx,
                        "Can't call postFifo without calling init() on a state machine");
            }

            return;
        }

        // This function will automatically queue events if one is in progress
        m_current_state_machine->handleEvent(e);
    }

    /*!
     * \brief addRow adds a transition table entry to the node.
     *
     * When defining createTransitionTable() in derived classes, use this function
     * to add transition table entries into the nodes entry map.
     *
     *
     * # TRANSITION PRIORITY
     *
     * Transition table entries dealing with the same event are evaluated in a top-down manner.
     *
     * Example:
     *
     * addRow(Evt::A, ..., ..., ...); // Will be evalulated first
     * addRow(Evt::A, ..., ..., ...); // Will be evaluated second
     *
     * # DESTINATION
     *
     * If the destination node is the same as the source, then the node will be exited and
     * entered.
     *
     * Anonymous transitions (transitions that don't lead to a change in the current state) are
     * defined by supplying ROOST_NO_DEST for the param.
     *
     * You can not have the destination node be of type REGION (aka an orthogonal region).
     *
     * Example:
     *
     * addRow(Evt::A, ROOST_NO_DEST, ..., ...)
     *
     * If a destination has a LCA that is an orthogonal node, then all
     * orthogonal regions will be exited and default entered.  This will
     * usually happen if you attempt to execute a transition from one orthogonal
     * region to another orthogonal region that share the same orthogonal
     * node as their immediate parent.
     *
     * # GUARDS
     *
     * Guards are evaluated for every applicable transition to determine if it is taken. If a
     * guard is evaluated to True, then the transition can proceed.  Guards can be either
     * functions or some combination of boolean logic
     *
     * To have no guard (i.e. always returns true), then supply ROOST_NO_GUARD.
     *
     * Otherwise, supply ROOST_GUARD(path_to_guard)
     *
     * Example:
     *
     * bool boolA{true};
     * bool boolB{false};
     *
     * bool func()
     * {
     *    return true;
     * }
     *
     * addRow(Evt::A, ..., ..., ROOST_NO_GUARD); // No guards for this
     * addRow(Evt::B, ..., ..., ROOST_GUARD(boolA && boolB)); // Can use boolean logic
     * addRow(Evt::B, ..., ..., ROOST_GUARD(boolA == true)); // Can use boolean logic
     * addRow(Evt::C, ..., ..., ROOST_GUARD(func())); // Can use functions too
     *
     * # ACTIONS
     *
     * Actions are functions which take in the event type as their only parameter
     * and execute when the guard is satisfied.  You can supply multiple actions
     * to be executed one after another.
     *
     * To have no action, then supply ROOST_NO_ACTION.
     *
     * Otherwise supply { ROOST_ACTION(path_to_action), ROOST_ACTION(path_to_action), ... }
     *
     * Example
     *
     * void act(E const& e)
     * {
     *     switch(e)
     *     {
     *
     *      case Evt::A:
     *         ....
     *
     *      case Evt::B:
     *         ....
     *
     *     }
     * }
     *
     * void act2(E const& )
     * {
     *   ...
     * }
     *
     * addRow(Evt::A, ..., ROOST_NO_ACTION, ...); // No action
     * addRow(Evt::A, ..., { ROOST_ACTION(act), ROOST_ACTION(act2) }, ... ); // Note the {}
     *
     * # OTHER
     *
     * Actions and guards capture variables and functions by reference and capture the this
     * pointer by value.  What this means is that do not use pointers that have their lifetime
     * defined within the createTransitionTable() function arguments to the ROOST_ACTION or
     * ROOST_GUARD macros.
     *
     * Example:
     *
     * void createTransitionTable()
     * {
     *
     *   Node<CTX,E>* local_pointer = m_ctx.m_root; // DONT USE THIS
     *   Node<CTX,E>& local_ref = *m_ctx.m_root; // USE THIS
     *
     *   addRow(Evt::A, &local_ref.m_state, ROOST_NO_ACTION, ROOST_NO_GUARD);
     * }
     *
     *
     *
     * \param e the event to handle
     * \param destination the destination node
     * \param actions the list of actions
     * \param guard the guard to evaluate
     */
    void addRow(
            E const&                           e,
            Node<CTX, E>*                      destination,
            std::vector<ActionFunctor<CTX, E>> actions,
            GuardFunctor<CTX, E>               guard)
    {

        if (destination && destination->m_node_type == NodeType::REGION)
        {

            if (m_spy)
            {
                m_spy->error(
                        m_name,
                        m_ctx,
                        "Destination can not be of type region",
                        destination->getName());
            }

            m_valid_transition_table = m_valid_transition_table && false;
            return;
        }

        TransitionTableEntry<CTX, E> entry = priv_createTransitionEntry(
                this, destination, std::move(actions), std::move(guard));

        // If destination is nullptr then it is an internal transition and
        // lca will also be a nullptr

        if (m_transition_table.count(e) == 0)
        {
            // New Entry
            m_transition_table.insert({e, std::vector<TransitionTableEntry<CTX, E>>()});
        }

        m_transition_table.at(e).push_back(std::move(entry));
        m_valid_transition_table = m_valid_transition_table && true;
    }

    /*!
     * \brief createTransitionTable create a transition table for the node
     *
     * When the StateMachine intializes each node, this function is called
     * to set the transition table entries within the node.
     *
     * Add transition table entries with the addRow() function defined above.
     *
     * # WARNING
     *
     * Actions and guards capture variables and functions by reference and capture the this
     * pointer by value.  What this means is that do not use pointers that have their lifetime
     * defined within the createTransitionTable() function arguments to the ROOST_ACTION or
     * ROOST_GUARD macros.
     *
     * Example:
     *
     * void createTransitionTable()
     * {
     *
     *   Node<CTX,E>* local_pointer = m_ctx.m_root; // DONT USE THIS
     *   Node<CTX,E>& local_ref = *m_ctx.m_root; // USE THIS
     *
     *   addRow(Evt::A, &local_ref.m_state, ROOST_NO_ACTION, ROOST_NO_GUARD);
     * }
     *
     */
    virtual void createTransitionTable() = 0;

    /*!
     * \brief onEntry called whenever the node is "entered" in the hierarchy
     */
    virtual void onEntry()
    {
    }

    /*!
     * \brief onExit called whenever the node is "exited" in the hierachy
     */
    virtual void onExit()
    {
    }

private:
    virtual void getGraphViz(std::ostream& os)
    {

        for (Node<CTX, E>* child : this->m_children)
        {
            if (child->getType() == NodeType::SHALLOW_HISTORY_NODE ||
                child->getType() == NodeType::DEEP_HISTORY_NODE)
            {
                continue;
            }

            if (child->getType() == NodeType::REGION)
            {
                os << child->getName() << " [shape=rectangle];" << std::endl;
            }

            os << this->m_name << " -> " << child->getName() << ";" << std::endl;

            if (m_initial_child)
            {
                os << m_initial_child->getName() << " [shape=doublecircle];" << std::endl;
            }

            child->getGraphViz(os);
        }
    }

    virtual void getSCXML(std::ostream& os, bool output_transitions)
    {

        os << "<state id=\"" << this->m_name << "\">" << std::endl;

        if (m_initial_child)
        {

            os << "<initial>" << std::endl;

            os << "<transition target=\"" << this->m_initial_child->getName() << "\"/>"
               << std::endl;

            os << "</initial>" << std::endl;
        }

        if (output_transitions)
        {

            auto it = this->m_transition_table.begin();

            while (it != this->m_transition_table.end())
            {

                E                                          event   = it->first;
                std::vector<TransitionTableEntry<CTX, E>>& entries = it->second;

                for (TransitionTableEntry<CTX, E>& entry : entries)
                {
                    entry.outputSCXML(event, os);
                }

                ++it;
            }
        }

        for (Node<CTX, E>* child : this->m_children)
        {
            child->getSCXML(os, output_transitions);
        }

        os << "</state>" << std::endl;
    }

    static TransitionTableEntry<CTX, E> priv_createTransitionEntry(
            Node<CTX, E>*                      src,
            Node<CTX, E>*                      dst,
            std::vector<ActionFunctor<CTX, E>> actions,
            GuardFunctor<CTX, E>               guard)
    {

        TransitionTableEntry<CTX, E> entry;
        entry.m_src         = std::move(src);
        entry.m_destination = std::move(dst);
        entry.m_actions     = std::move(actions);
        entry.m_guard       = std::move(guard);
        entry.m_lca         = findLca(entry.m_src, entry.m_destination);

        if (entry.m_lca)
        {

            if (entry.m_lca->m_node_type == NodeType::ORTHOGONAL_NODE)
            {
                // A transition that has an LCA of an orthogonal node will be transformed so that
                // the orthogonal regions are exited and then default entered

                entry.m_src         = entry.m_lca;
                entry.m_destination = entry.m_lca;

                // Force the LCA to be parent of "real" LCA so that our algorithm will call
                // On exit and then on entry of "real" LCA

                entry.m_lca = entry.m_lca->m_parent;
            }

            Node<CTX, E>* lca = entry.m_lca;

            while (lca->m_node_type != NodeType::REGION)
            {
                lca = lca->m_parent;
            }

            entry.m_lca_region = static_cast<RegionNode<CTX, E, void*>*>(lca);
        }
        else
        {
            entry.m_lca_region = nullptr;
        }

        // Important we do this after the orthogonal node lca check above
        // because we could change the m_src field in the entry
        Node<CTX, E>* tmp_src = entry.m_src;

        while (tmp_src->m_node_type != NodeType::REGION)
        {
            tmp_src = tmp_src->m_parent;
        }

        entry.m_src_region = static_cast<RegionNode<CTX, E, void*>*>(tmp_src);

        return entry;
    }

    virtual void setLastVisitedNode(Node<CTX, E>* node)
    {
        m_last_active_child = node;
    }

    virtual bool init(NodeConfiguration<CTX, E> const& config)
    {
        m_valid_transition_table = true;
        m_spy                    = config.spy;
        m_current_state_machine  = config.current_state_machine;
        m_transition_table.clear();
        createTransitionTable();

        // We use a "global" flag instead of returning from createTransitionTable()
        // because we want the addRow syntax to be cleaner

        if (!m_valid_transition_table)
        {
            return false;
        }

        return true;
    }

    virtual void uninit()
    {
        m_valid_transition_table = false;
        m_transition_table.clear();
        m_current_state_machine = nullptr;
        m_spy                   = nullptr;
    }

    // Returns false if not handled, otherwise true
    virtual bool handle(
            E const&                                   event,
            std::vector<TransitionTableEntry<CTX, E>>* transition_list)
    {
        if (m_transition_table.count(event) == 0)
        {
            // Not found
            return false;
        }

        std::vector<TransitionTableEntry<CTX, E>>& entries = m_transition_table.at(event);

        for (auto& entry : entries)
        {

            if (entry.m_guard.m_guard_fptr(event))
            {
                // Guard returned true
                transition_list->push_back(entry);

                return true;
            }
        }

        // No entries that had a good guard
        return false;
    }

};  // Class: Node

template <
        typename CTX,
        typename E,
        typename = typename std::enable_if<std::is_enum<E>::value, void*>::type>
class ShallowHistoryNode final : public Node<CTX, E>
{
private:
    friend class CompositeNode<CTX, E, void*>;

    // We want this private because only composite states can have this kind of node
    // Additionally all history nodes can not have children
    ShallowHistoryNode(const char* name, CTX& ctx, Node<CTX, E>* parent)
        : Node<CTX, E>(name, ctx, NodeType::SHALLOW_HISTORY_NODE)
    {
        this->m_parent = parent;

        if (this->m_parent)
        {
            this->m_parent->m_children.push_back(this);
        }
    }

    void createTransitionTable() override final
    {
    }

    void setLastVisitedNode(Node<CTX, E>*) override final
    {
        // We don't want to set any node, leave m_last_active_child as nullptr
    }

    void getSCXML(std::ostream& os, bool output_transitions) override
    {

        if (!output_transitions)
        {
            // If we aren't outputing transitions, why clutter up the diagram with history nodes?
            return;
        }

        os << "<history id=\"" << this->m_parent->m_name << "." << this->m_name
           << "\" type=\"shallow\"/>" << std::endl;
    }

    void getGraphViz(std::ostream&) override
    {
    }
};

template <
        typename CTX,
        typename E,
        typename = typename std::enable_if<std::is_enum<E>::value, void*>::type>
class DeepHistoryNode final : public Node<CTX, E>
{
private:
    friend class CompositeNode<CTX, E, void*>;

    // We want this private because only composite states can have this kind of node
    // Additionally all history nodes can not have children
    DeepHistoryNode(const char* name, CTX& ctx, Node<CTX, E>* parent)
        : Node<CTX, E>(name, ctx, NodeType::DEEP_HISTORY_NODE)
    {
        this->m_parent = parent;

        if (this->m_parent)
        {
            this->m_parent->m_children.push_back(this);
        }
    }

    void createTransitionTable() override final
    {
    }

    void setLastVisitedNode(Node<CTX, E>*) override final
    {
        // We don't want to set any node, leave m_last_active_child as nullptr
    }

    void getSCXML(std::ostream& os, bool output_transitions) override
    {
        if (!output_transitions)
        {
            // If we aren't outputing transitions, why clutter up the diagram with history nodes?
            return;
        }

        os << "<history id=\"" << this->m_parent->m_name << "." << this->m_name
           << "\" type=\"deep\"/>" << std::endl;
    }

    void getGraphViz(std::ostream&) override
    {
    }
};

template <
        typename CTX,
        typename E,
        typename = typename std::enable_if<std::is_enum<E>::value, void*>::type>
class LeafNode : public Node<CTX, E>
{
public:
    LeafNode(const char* name, CTX& ctx, Node<CTX, E>* parent)
        : Node<CTX, E>(name, ctx, NodeType::LEAF_NODE)
    {
        this->m_parent = parent;

        if (this->m_parent)
        {
            this->m_parent->m_children.push_back(this);
        }
    }

    virtual ~LeafNode() = default;

private:
    void setLastVisitedNode(Node<CTX, E>*) override
    {
        // We don't want to set any node, leave m_last_active_child as nullptr
    }

};  // Class: LeafNode

template <
        typename CTX,
        typename E,
        typename = typename std::enable_if<std::is_enum<E>::value, void*>::type>
class CompositeNode : public Node<CTX, E>
{
public:
    CompositeNode(const char* name, CTX& ctx, Node<CTX, E>* parent, Node<CTX, E>* initial_state)
        : Node<CTX, E>(name, ctx, NodeType::COMPOSITE_NODE),
          shallowHistory("ShallowHistory", ctx, this),
          deepHistory("DeepHistory", ctx, this)
    {
        this->m_parent            = parent;
        this->m_initial_child     = initial_state;
        this->m_last_active_child = initial_state;

        if (this->m_parent)
        {
            this->m_parent->m_children.push_back(this);
        }
    }

    virtual ~CompositeNode() = default;

    ShallowHistoryNode<CTX, E> shallowHistory;
    DeepHistoryNode<CTX, E>    deepHistory;

private:
    bool init(NodeConfiguration<CTX, E> const& config) override
    {

        // Want to set spy in init() before doing other validation
        if (!Node<CTX, E>::init(config))
        {
            return false;
        }

        if (this->m_initial_child->getParent() != this)
        {
            if (this->m_spy)
            {
                this->m_spy->error(
                        this->m_name,
                        this->m_ctx,
                        "Initial child must direct sub-state of parent");
            }
            return false;
        }

        if (this->m_initial_child->m_node_type == NodeType::SHALLOW_HISTORY_NODE ||
            this->m_initial_child->m_node_type == NodeType::DEEP_HISTORY_NODE)
        {

            if (this->m_spy)
            {
                this->m_spy->error(
                        this->m_name,
                        this->m_ctx,
                        "Can not have children nodes which are history nodes",
                        this->m_initial_child->getName());
            }

            return false;
        }

        for (Node<CTX, E>* child : this->m_children)
        {
            if (child->m_node_type == NodeType::REGION)
            {

                if (this->m_spy)
                {
                    this->m_spy->error(
                            this->m_name,
                            this->m_ctx,
                            "Can not have children nodes which are regions",
                            child->getName());
                }

                return false;
            }
        }

        return true;
    }

};  // Class: CompositeNode

template <
        typename CTX,
        typename E,
        typename = typename std::enable_if<std::is_enum<E>::value, void*>::type>
class OrthogonalNode : public Node<CTX, E>
{
public:
    OrthogonalNode(const char* name, CTX& ctx, Node<CTX, E>* parent)
        : Node<CTX, E>(name, ctx, NodeType::ORTHOGONAL_NODE)
    {
        this->m_parent = parent;

        if (this->m_parent)
        {
            this->m_parent->m_children.push_back(this);
        }
    }

    virtual ~OrthogonalNode() = default;

private:
    void setLastVisitedNode(Node<CTX, E>*) override
    {
        // We don't want to set any node, leave m_last_active_child as nullptr
    }

    // Orthogonal nodes don't have setup() because we just want to call on-entry and let the state
    // machine handle activating default nodes vs non-default nodes

    bool init(NodeConfiguration<CTX, E> const& config) override
    {

        // Want to set spy in init() before doing other validation
        if (!Node<CTX, E>::init(config))
        {
            return false;
        }

        for (Node<CTX, E>* child : this->m_children)
        {
            if (child->m_node_type != NodeType::REGION)
            {

                if (this->m_spy)
                {
                    this->m_spy->error(
                            this->m_name,
                            this->m_ctx,
                            "All children of orthogonal nodes must be regions",
                            child->getName());
                }

                return false;
            }
        }

        return true;
    }

    bool handle(E const& event, std::vector<TransitionTableEntry<CTX, E>>* transition_list)
            override
    {
        bool handled{false};

        for (Node<CTX, E>* child : this->m_children)
        {

            /*
             * It is VITAL that the order of this is maintained.  That is because
             * we must ensure that child->handle(...) is ALWAYS called for each child.
             *
             * If we made it like:
             *  handled = handled || child->handle(...)
             *
             * then due to C++ short-circuiting rules, if handled was true then
             * child-> handle(...) won't be called.
             *
             * Another way to think of this is:
             *
             * if (child->handle(event, transition_list))
             * {
             *     handled = true;
             * }
             *
             * but we can avoid branch predictor misses this way
             *
             */

            handled = child->handle(event, transition_list) || handled;
        }

        // One of the regions handled it
        if (handled)
        {
            return true;
        }

        // Otherwise lets see if we handle it
        return Node<CTX, E>::handle(event, transition_list);
    }

    void getSCXML(std::ostream& os, bool output_transitions) override
    {

        os << "<parallel id=\"" << this->m_name << "\">" << std::endl;

        if (output_transitions)
        {

            auto it = this->m_transition_table.begin();

            while (it != this->m_transition_table.end())
            {

                E                                          event   = it->first;
                std::vector<TransitionTableEntry<CTX, E>>& entries = it->second;

                for (TransitionTableEntry<CTX, E>& entry : entries)
                {
                    entry.outputSCXML(event, os);
                }

                ++it;
            }
        }

        for (Node<CTX, E>* child : this->m_children)
        {
            child->getSCXML(os, output_transitions);
        }

        os << "</parallel>" << std::endl;
    }

};  // Class: OrthogonalNode

template <
        typename CTX,
        typename E,
        typename = typename std::enable_if<std::is_enum<E>::value, void*>::type>
class RegionNode : public Node<CTX, E>
{
private:
    Node<CTX, E>* m_current_node;
    u32           m_level;

public:
    friend class StateMachine<CTX, E>;

    RegionNode(const char* name, CTX& ctx, Node<CTX, E>* parent, Node<CTX, E>* initial_state)
        : Node<CTX, E>(name, ctx, NodeType::REGION), m_current_node(this), m_level(0)
    {
        this->m_parent            = parent;
        this->m_initial_child     = initial_state;
        this->m_last_active_child = initial_state;

        if (this->m_parent)
        {
            this->m_parent->m_children.push_back(this);
        }
    }

    virtual ~RegionNode() = default;

    u32 getLevel() const
    {
        return m_level;
    }

    void onExit() override final
    {
        // No onExit for Region Nodes
    }

    void onEntry() override final
    {
        // No onEntry for region nodes
    }

    void createTransitionTable() override final
    {
        // No transitions allowed inside the region node
    }

    // Inherit getSCXML()...

private:
    bool init(NodeConfiguration<CTX, E> const& config) override
    {

        // Want to set spy in init() before doing other validation
        if (!Node<CTX, E>::init(config))
        {
            return false;
        }

        // The Top Region is level 1
        m_level           = 1;
        Node<CTX, E>* tmp = this->getParent();

        while (tmp != nullptr)
        {
            tmp = tmp->getParent();
            ++m_level;

            // This is an arbitrarily large number to act
            // as a fail safe for infinite loops.
            // If you truly have more than 1 million parent nodes
            // then you are SOL unfortunately
            if (m_level > 1000000)
            {
                if (this->m_spy)
                {
                    this->m_spy->error(this->m_name, this->m_ctx, "Can't set level");
                }
                return false;
            }
        }

        if (this->m_initial_child->getParent() != this)
        {
            if (this->m_spy)
            {
                this->m_spy->error(
                        this->m_name,
                        this->m_ctx,
                        "Initial child must direct sub-state of parent");
            }
            return false;
        }

        for (Node<CTX, E>* child : this->m_children)
        {
            if (child->m_node_type == NodeType::REGION)
            {

                if (this->m_spy)
                {
                    this->m_spy->error(
                            this->m_name,
                            this->m_ctx,
                            "Can not have children nodes which are regions",
                            child->getName());
                }

                return false;
            }
        }

        return true;
    }

    void constructFromDeepHistory()
    {

        Node<CTX, E>* current_node = this->m_current_node;

        while (current_node != nullptr)
        {

            // Next node may be nullptr, but we have to check if the current
            // Node is an orthogonal node because both orthogonal and
            // leaf nodes have m_last_active_child as nullptr

            Node<CTX, E>* next_node = current_node->m_last_active_child;

            if (current_node->m_node_type == NodeType::ORTHOGONAL_NODE)
            {
                // Orthogonal nodes have nullptr for last active childs,
                // instead activate their children

                for (Node<CTX, E>* child : current_node->m_children)
                {
                    // Enforced by init()
                    RegionNode<CTX, E>* region = static_cast<RegionNode<CTX, E>*>(child);
                    region->constructFromDeepHistory();
                }

                return;
            }

            current_node = next_node;

            if (current_node)
            {

                if (this->m_spy)
                {
                    this->m_spy->on_entry(current_node->getName(), this->m_ctx);
                }

                current_node->onEntry();
                this->m_current_node = current_node;
            }
        }
    }

    void construct()
    {

        Node<CTX, E>* current_node = m_current_node->m_initial_child;

        while (current_node != nullptr)
        {

            if (this->m_spy)
            {
                this->m_spy->on_entry(current_node->getName(), this->m_ctx);
            }

            current_node->onEntry();
            m_current_node = current_node;

            if (current_node->m_node_type == NodeType::ORTHOGONAL_NODE)
            {

                for (Node<CTX, E>* child : current_node->m_children)
                {
                    // Enforced by init
                    RegionNode<CTX, E>* region = static_cast<RegionNode<CTX, E>*>(child);
                    region->construct();
                }

                return;
            }

            current_node = m_current_node->m_initial_child;
        }
    }

    void destructUntilNode(Node<CTX, E>* node)
    {

        while (m_current_node != node)
        {

            if (m_current_node->m_node_type == NodeType::ORTHOGONAL_NODE)
            {

                for (Node<CTX, E>* child : m_current_node->m_children)
                {
                    // Enforced by init
                    RegionNode<CTX, E>* region = static_cast<RegionNode<CTX, E>*>(child);
                    region->destruct();
                }
            }

            if (this->m_spy)
            {
                this->m_spy->on_exit(m_current_node->getName(), this->m_ctx);
            }

            m_current_node->onExit();

            // Save the last active child of the region

            this->setLastVisitedNode(m_current_node);

            m_current_node = m_current_node->m_parent;

            // Also inform the composite state of the last active child
            m_current_node->setLastVisitedNode(this->m_last_active_child);
        }
    }

    void destruct()
    {
        destructUntilNode(this);
    }

    bool handle(E const& event, std::vector<TransitionTableEntry<CTX, E>>* transition_list)
            override
    {
        Node<CTX, E>* current_node = m_current_node;
        bool          rval{false};

        while (current_node != this)
        {
            rval = current_node->handle(event, transition_list);

            if (rval)
            {
                return true;
            }

            current_node = current_node->m_parent;
        }

        return false;
    }

};  // Class: RegionNode

}  // ns: roost

#endif  // ROOST_LIB_NODE_HPP
