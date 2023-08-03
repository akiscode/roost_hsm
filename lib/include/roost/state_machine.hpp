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

#ifndef ROOST_LIB_STATE_MACHINE_HSM_HPP
#define ROOST_LIB_STATE_MACHINE_HSM_HPP

#include "roost/alias.hpp"
#include "roost/common.hpp"
#include "roost/constants.hpp"
#include "roost/node.hpp"
#include "roost/spy.hpp"
#include "roost/transition_table.hpp"

#include <algorithm>
#include <functional>  // std::function
#include <iterator>
#include <limits>
#include <queue>
#include <string>

namespace roost
{

/*!
 * \brief IFifo represents the FIFO queue interface used by the StateMachine class
 *
 * In order to help achieve goals of reducing memory allocation unless absolutely needed,
 * this class provides a interface so that users of this library can provide a backing
 * queue that doesn't allocate (i.e. a pre-allocated ring buffer).
 *
 * The default IFifo concrete class used by the StateMachine class is the QueueFifo class,
 * which uses a std::queue backing.
 *
 * \tparam E the event enum class type
 */
template <typename E>
class IFifo
{
public:
    virtual ~IFifo() = default;

    /*!
     * \brief push pushes the event into the queue.
     *
     * This function will return true if the event was successfully
     * pushed into the queue otherwise it will return false.
     *
     * \param e the event enum class
     * \return true if pushed successfully, otherwise false
     */
    virtual bool push(E const& e) = 0;

    /*!
     * \brief empty returns true if queue is empty, otherwise false
     */
    virtual bool empty() = 0;

    /*!
     * \brief front returns the event enum class at the front of the queue
     */
    virtual E front() = 0;

    /*!
     * \brief pop_front removes the event enum class at the front of the queue
     */
    virtual void pop_front() = 0;
};

/*!
 * \brief A concrete IFifo class that uses std::queue as a backing queue
 *
 * \tparam E the event enum class type
 */
template <typename E>
class QueueFifo final : public IFifo<E>
{
private:
    std::queue<E> m_queue;

public:
    bool push(E const& e) override
    {
        m_queue.push(e);
        return true;
    }

    bool empty() override
    {
        return m_queue.empty();
    }

    E front() override
    {
        return m_queue.front();
    }

    void pop_front() override
    {
        return m_queue.pop();
    }
};

/*!
 * \brief StateMachine is responsible for publishing events and handling all user-facing functions
 *
 * This class has purview of the entire hierachical tree and provides the resources and facilities
 * to interact with the tree holistically.  It is the owner of the "Top" region, which always owns
 * all other nodes in the tree.
 *
 * After creation of the StateMachine class, the init() function must be called and return true
 * to use most of the functions that deal with the states.
 *
 * This state machine uses exclusively internal (as opposed to external) transition types as
 * defined in the UML standard.
 *
 * \tparam CTX the context type
 * \tparam E the event enum class type
 */
template <typename CTX, typename E>
class StateMachine final
{
private:
    Node<CTX, E>*
                                 m_original_node;  //!< The node which StateMachine will attach Top as a parent to
    const char*                  m_name;     //!< The name of the backend
    CTX&                         m_ctx;      //!< The context reference shared among all nodes
    std::shared_ptr<Spy<CTX, E>> m_spy;      //!< The spy associated with the StateMachine
    std::vector<Node<CTX, E>*> m_all_nodes;  //!< A vector that contains all nodes (including top)
    std::vector<Node<CTX, E>*>
                       m_on_entry_calls;  //!< A vector that holds pointers to call onEntry in reverse order
    bool               m_init;            //!< True if successfully initialized, otherwise false
    RegionNode<CTX, E> m_top;             //!< The Top node to attach to m_original_node

    //! A vector that holds all potential transitions to execute
    std::vector<TransitionTableEntry<CTX, E>> m_transitions;

    bool m_event_in_progress;  //!< True if currently handling an event in the StateMachine,
                               //!< otherwise false
    bool m_force_transition_in_progress;  //!< True if force transition in progress, otherwise
                                          //!< false
    std::unique_ptr<IFifo<E>> m_fifo;     //!< The queue that holds the events

public:
    using CTX_TYPE   = CTX;
    using EVENT_TYPE = E;

    /*!
     * \brief StateMachine constructs a state machine instance
     *
     * After constructing the StateMachine, init() still needs to be called.
     *
     * On destruction the StateMachine will call uninit()
     *
     * \param name the name of the StateMachine
     * \param attached_node the node to attach Top as a parent to
     * \param spy the spy to attach to all nodes
     * \param fifo the event queue
     */
    StateMachine(
            const char*                  name,
            Node<CTX, E>*                attached_node,
            std::shared_ptr<Spy<CTX, E>> spy  = roost::make_unique<StandardErrorSpy<CTX, E>>(),
            std::unique_ptr<IFifo<E>>    fifo = roost::make_unique<QueueFifo<E>>())
        : m_original_node(attached_node),
          m_name(name),
          m_ctx(m_original_node->m_ctx),
          m_spy(std::move(spy)),
          m_all_nodes(),
          m_on_entry_calls(),
          m_init(false),
          m_top("Top", m_ctx, nullptr, m_original_node),
          m_transitions(),
          m_event_in_progress(false),
          m_force_transition_in_progress(false),
          m_fifo(std::move(fifo))
    {
    }

    ~StateMachine()
    {
        uninit();
    }

    /*!
     * \brief init initializes and validates all nodes within the StateMachine
     *
     * The goal of this function is to do all necessary error checking and memory allocations
     * before handling events.
     *
     * This function will:
     * - Call uninit() if this function is already initialized
     * - Set the parent of the attached node (i.e. m_original_node) to top
     * - Call init() on all nodes within the StateMachine (which in turn calls
     * createTransitionTable())
     * - Pre-allocate the transition and onEntry call vectors so no allocation is needed during
     * operation
     * - Go through and make sure that the initial nodes are active
     * - Fire a completion event
     *
     * If tasks are completed successfully, then this function returns true and the StateMachine
     * is ready for operation; i.e. getInitStatus() will return True.
     *
     * Otherwise this function will return false and the StateMachine is in an undefined state;
     * i.e. getInitStatus() will return False
     *
     * It is *highly* recommended to set a Spy that at least records Errors, as this function will
     * record specific errors via the Spy.
     *
     * \return true if successful, otherwise false
     */
    bool init()
    {

        if (m_init)
        {
            uninit();
        }

        bool rval{true};

        do
        {

            m_original_node->m_parent = static_cast<Node<CTX, E>*>(&m_top);
            m_top.m_children.push_back(m_original_node);
            m_top.m_initial_child = m_original_node;

            size_t max_depth = compute_max_depth(&m_top);

            get_all_children(&m_top, m_all_nodes);

            NodeConfiguration<CTX, E> config;
            config.spy                   = m_spy;
            config.current_state_machine = this;

            // We always have the top region, so we always have 1 region
            size_t number_of_regions{1};

            for (auto& n : m_all_nodes)
            {

                if (n->m_node_type == NodeType::REGION)
                {
                    ++number_of_regions;
                }

                if (!n->init(config))
                {
                    if (m_spy)
                    {
                        m_spy->error(n->getName(), m_ctx, "Failed Init");
                    }
                    rval = false;
                }
            }

            if (!rval)
            {
                break;
            }

            // The maximum number of onEntry() calls will be the max depth of the tree
            m_on_entry_calls.reserve(max_depth);

            // Only one transition per region can "win", we can always filter down later
            m_transitions.reserve(number_of_regions);

            // Will cause the state machine to enter the initial state(s)
            m_top.construct();

            // Clear transition vector and fire new completion event
            m_transitions.clear();

            m_top.handle(m_original_node->getNoneEvt(), &m_transitions);
            processTransitions(m_original_node->getNoneEvt(), &m_transitions);

        } while (false);

        m_init = rval;
        return rval;
    }

    /*!
     * \brief uninit unitializes the StateMachine
     *
     * This function will:
     * - Set the parent of the attached node (i.e. m_original_node) to nullptr
     * - call uninit() on all nodes
     *
     * If already uninitialized, then this function does nothing
     *
     */
    void uninit()
    {

        if (!m_init)
        {
            return;
        }

        m_original_node->m_parent = nullptr;

        m_top.m_children.clear();
        m_top.m_initial_child = nullptr;

        for (auto& n : m_all_nodes)
        {
            n->uninit();
        }

        m_init = false;
    }

    /*!
     * \brief getInitStatus returns true if initialized otherwise false
     */
    bool getInitStatus() const
    {
        return m_init;
    }

    /*!
     * \brief getSCXML outputs the SCXML representation of the StateMachine to the ostream
     *
     * This function will do nothing if:
     * - This StateMachine is not initialized
     * - If an event is in progress (i.e. handleEvent() hasn't exited)
     * - If a forced transition is in progress(i.e. forceTransitionTo() hasn't exited)
     *
     * If output_transitions is set to true, transitions and history nodes will be outputed to the
     * SCXML, otherwise they won't.  It is often useful in state machines with large numbers of
     * transitions to simply see the layout of the state machine which is why this is an option.
     * Because in Roost HSM history nodes are attached to every composite state by default,
     * history nodes are hidden only if output_transitions is false.
     *
     * \param os the ostream to write to
     * \param output_transitions if true will output transitions and history nodes to the SCXML
     */
    void getSCXML(std::ostream& os, bool output_transitions = true)
    {

        if (!m_init || m_event_in_progress || m_force_transition_in_progress)
        {
            return;
        }

        os << "<scxml initial=\"" << m_original_node->getName() << "\" name=\"" << m_name
           << "\" version=\"1.0\" xmlns=\"http://www.w3.org/2005/07/scxml\">" << std::endl;

        m_original_node->getSCXML(os, output_transitions);

        os << "</scxml>" << std::endl;
    }

    /*!
     * \brief getGraphViz outputs the graphviz tree representation of the StateMachine to the
     * ostream
     *
     * This function will do nothing if:
     * - This StateMachine is not initialized
     * - If an event is in progress (i.e. handleEvent() hasn't exited)
     * - If a forced transition is in progress(i.e. forceTransitionTo() hasn't exited)
     *
     * \param os the ostream to write to
     */
    void getGraphViz(std::ostream& os)
    {

        if (!m_init || m_event_in_progress || m_force_transition_in_progress)
        {
            return;
        }

        os << "digraph " << m_name << "_graphviz { node [shape=circle]; Top [shape=rectangle]; "
           << std::endl;

        m_top.getGraphViz(os);

        os << "}" << std::endl;
    }

    /*!
     * \brief getCurrentNodes returns the currently active nodes (not the active states)
     *
     * The current node is the state that is currently pointed to in the region.  If this node
     * is an orthogonal node, then the list contains the orthogonal node as well as all nodes
     * pointed by the orthogonal regions.  This logic will happen recursively
     *
     * Note that this is different than the concept of active states.  Active states include not
     * only the current node but all parent nodes associated.
     *
     * This function will return an empty list if:
     * - This StateMachine is not initialized
     * - If an event is in progress (i.e. handleEvent() hasn't exited)
     * - If a forced transition is in progress(i.e. forceTransitionTo() hasn't exited)*
     *
     * \return a list containing all current nodes
     */
    std::vector<std::string> getCurrentNodes()
    {

        std::vector<std::string> rval;

        if (!m_init || m_event_in_progress || m_force_transition_in_progress)
        {
            return rval;
        }

        std::queue<Node<CTX, E>*> nodes;

        nodes.push(m_top.m_current_node);

        while (!nodes.empty())
        {
            Node<CTX, E>* node = nodes.front();
            nodes.pop();

            rval.push_back(node->getName());

            if (node->m_node_type == NodeType::ORTHOGONAL_NODE)
            {
                for (Node<CTX, E>* child : node->m_children)
                {
                    // This static cast is enforced by init()
                    RegionNode<CTX, E>* region = static_cast<RegionNode<CTX, E>*>(child);
                    nodes.push(region->m_current_node);
                }
            }
        }

        return rval;
    }

    /*!
     * \brief forceTransitionTo forces the StateMachine to transition to a target node
     *
     * It is NOT recommended to use this function for production purposes.  It is only meant to
     * help facilitate unit testing where one wants to place the state machine in a specific state
     * before firing an event.
     *
     * This function will cause a transition from the currently active node to the destination and
     * will use the initial child of the Top region to act as the LCA.
     *
     * *ADDITIONALLY*: Actions will not be called and no event function will fire in any spy.
     * Only onEntry and onExit functions of nodes will be called.  There will be no completion
     * events fired after the transition is completed.
     *
     * This function will do nothing if:
     * - This StateMachine is not initialized
     * - If an event is in progress (i.e. handleEvent() hasn't exited)
     * - If a forced transition is in progress(i.e. forceTransitionTo() hasn't exited)
     *
     * \param dest_node the destination to transition to
     */
    void forceTransitionTo(Node<CTX, E>* dest_node)
    {

        if (!m_init || m_event_in_progress || m_force_transition_in_progress)
        {
            return;
        }

        m_force_transition_in_progress = true;

        m_transitions.clear();

        TransitionTableEntry<CTX, E> transition = Node<CTX, E>::priv_createTransitionEntry(
                m_top.m_initial_child, dest_node, ROOST_NO_ACTION, ROOST_NO_GUARD);

        m_transitions.push_back(transition);

        // Tell process transitions to ignore the event and simply transition
        // We ignore actions and don't fire the "event" function to the spy
        // We also don't fire completion events
        processTransitions(m_top.getNoneEvt(), &m_transitions, true);

        m_force_transition_in_progress = false;
    }

    /*!
     * \brief handleEvent takes an event and fires it into the StateMachine
     *
     * This function will do nothing if:
     * - This StateMachine is not initialized
     * - If a forced transition is in progress(i.e. forceTransitionTo() hasn't exited)
     *
     *  If an event is in progress (i.e. handleEvent() hasn't exited) then calling this
     * function will enqueue the message into the queue and return
     *
     * \param e the event to fire into the state machine
     */
    void handleEvent(E const& e)
    {

        if (!m_init)
        {
            return;
        }

        // The reason we want to do this is that while forcing our way though
        // the state machine, part of our contract is that we don't want to fire
        // or handle any events while we are trying to reach our destination
        if (m_force_transition_in_progress)
        {
            return;
        }

        m_fifo->push(e);

        // The reason that we want to do this check is that other nodes
        // have the ability to fire other events within the transition
        // handlers.  Therefore, we want to simply store it within the
        // queue and wait for the current transition to terminate
        if (m_event_in_progress)
        {
            return;
        }

        m_event_in_progress = true;

        E event;

        while (!m_fifo->empty())
        {
            event = m_fifo->front();
            m_fifo->pop_front();

            m_transitions.clear();

            m_top.handle(event, &m_transitions);

            if (m_transitions.empty())
            {

                if (m_spy)
                {
                    m_spy->no_transition(m_top.getName(), m_ctx, event);
                }

                continue;
            }

            processTransitions(event, &m_transitions);
        }

        m_event_in_progress = false;
    }

    /*!
     * \brief compute_max_depth will return the maximum depth found in a Node hierarchy
     * \param node the node to start at
     * \param depth the depth to start at (Should be left at defaults)
     * \return the maximum depth found
     */
    static size_t compute_max_depth(Node<CTX, E>* node, size_t depth = 1)
    {

        if (!node)
        {
            return depth;
        }

        if (node->m_children.empty())
        {
            return depth;
        }

        size_t plusone = depth + 1;

        for (auto& c : node->m_children)
        {
            depth = std::max(depth, compute_max_depth(c, plusone));
        }

        return depth;
    }

    /*!
     * \brief get_all_children gets all nodes in a hierarchy and flattens them into a vector
     * \param node the node to start at
     * \param all_nodes the vector to push all node pointers to
     */
    static void get_all_children(Node<CTX, E>* node, std::vector<Node<CTX, E>*>& all_nodes)
    {
        if (!node)
        {
            return;
        }

        all_nodes.push_back(node);

        for (auto& c : node->m_children)
        {
            get_all_children(c, all_nodes);
        }
    }

private:
    /*!
     * \brief processTransitions an internal function that will take supplied transitons and
     * execute them
     *
     * This function performs the main execution logic of transitions.  Given a vector of
     * transitions, it processes them one at a time and can also filter out the transitions which
     * should no longer be executed according to the UML state machine rules.  This is most
     * prominent when dealing with transitions from orthogonal regions that lead outside of the
     * orthogonal node.
     *
     * There is also a "ignore_events" parameter that is used in the forceTransitionTo() function.
     * When ignore_events is true:
     *
     * - We ignore (don't fire) actions
     * - We ignore (don't fire) the event function on the spy.
     * - We don't fire completion events once we reach our destination
     * - We still enter default states
     *
     * Each of the transitions in the transitons vector were created in response to the event
     * e UNLESS ignore_events is set to true.
     *
     * \param e the event that was associated with each transition
     * \param transitions the vector of transitions to execute
     * \param ignore_events ignore the supplied event
     */
    void processTransitions(
            E const&                                   e,
            std::vector<TransitionTableEntry<CTX, E>>* transitions,
            bool                                       ignore_events = false)
    {
        E event{e};

        while (!transitions->empty())
        {
            // The current_level represents the "level" that we are operating at.
            // A level is the depth of regions within a whole state machine, starting at Top
            // Top is listed as level 1, so if this is zero, then it is not set
            u32 current_level{0};

            for (TransitionTableEntry<CTX, E> transition : *transitions)
            {

                /*
                 * While handle will make sure transitions are put into the vector for each level
                 * properly, sometimes transitions can make their way outside of their region.
                 *
                 * Once that happens we don't want a transition that was exited from firing
                 */
                if (transition.m_lca_region)
                {
                    // The lower the level, the close it is to top (which has a level of 1)

                    // If it is zero, then it isn't set (The lowest is 1 for the top region)
                    // Set the level and then continue with the current transition
                    if (current_level == 0)
                    {
                        current_level = transition.m_lca_region->getLevel();
                    }
                    // Otherwise we have to do a check on the transition
                    else if (current_level < transition.m_src_region->getLevel())
                    {
                        continue;
                    }

                    if (transition.m_lca_region->getLevel() < current_level)
                    {
                        current_level = transition.m_lca_region->getLevel();
                    }
                }

                if (m_spy && !ignore_events)
                {
                    m_spy->event(transition.m_src->getName(), m_ctx, event);
                }

                if (!ignore_events)
                {
                    // Execute all actions
                    for (auto& f : transition.m_actions)
                    {
                        f.m_action_fptr(event);
                    }
                }

                // Just an internal transition
                if (transition.m_destination == nullptr)
                {
                    continue;
                }

                if (transition.m_lca == nullptr)
                {
                    // The destination can't be non-nullptr and lca be nullptr

                    if (m_spy)
                    {
                        m_spy->error(
                                transition.m_src->getName(), m_ctx, "Transition LCA was nullptr");
                    }

                    ROOST_ASSERT(transition.m_lca != nullptr);

                    continue;
                }

                transition.m_lca_region->destructUntilNode(transition.m_lca);

                m_on_entry_calls.clear();

                Node<CTX, E>* tmp = transition.m_destination;

                while (tmp != transition.m_lca)
                {
                    m_on_entry_calls.push_back(tmp);
                    tmp = tmp->getParent();
                }

                RegionNode<CTX, E>* current_region = transition.m_lca_region;

                for (auto it = m_on_entry_calls.rbegin(); it != m_on_entry_calls.rend(); ++it)
                {
                    Node<CTX, E>* current_node = *it;

                    // Because we are forcing our way through nodes, we have to call our spy here
                    // but not on regions themselves
                    if (this->m_spy)
                    {
                        this->m_spy->on_entry(current_node->getName(), this->m_ctx);
                    }

                    current_region->m_current_node = current_node;
                    current_node->onEntry();

                    if (current_node->m_node_type == NodeType::ORTHOGONAL_NODE)
                    {

                        Node<CTX, E>* orthogonal_node = current_node;

                        ++it;

                        // If the transition ends at the orthogonal node, then we still have to
                        // initialize The children of the orthogonal node
                        if (it == m_on_entry_calls.rend())
                        {

                            for (Node<CTX, E>* child : orthogonal_node->m_children)
                            {
                                // Enforced by init()
                                RegionNode<CTX, E>* region =
                                        static_cast<RegionNode<CTX, E>*>(child);
                                region->construct();
                            }

                            // We need a break here because otherwise the iterator won't exit the
                            // for-loop
                            break;
                        }
                        else
                        {

                            ROOST_ASSERT((*it)->m_node_type == NodeType::REGION);

                            current_region = static_cast<RegionNode<CTX, E>*>(*it);

                            for (Node<CTX, E>* child : orthogonal_node->m_children)
                            {
                                // Skip the one that we are personally handling
                                if (child == current_region)
                                {
                                    continue;
                                }

                                // Enforced by init()
                                RegionNode<CTX, E>* region =
                                        static_cast<RegionNode<CTX, E>*>(child);
                                region->construct();
                            }
                        }
                    }

                    else if (current_node->m_node_type == NodeType::SHALLOW_HISTORY_NODE)
                    {

                        // We know that history nodes have no children and thus can only be called
                        // As the last node

                        // Only composite states can have history nodes
                        Node<CTX, E>* containing_composite_state = current_node->m_parent;
                        ROOST_ASSERT(
                                containing_composite_state->m_node_type ==
                                NodeType::COMPOSITE_NODE);

                        Node<CTX, E>* next_target =
                                containing_composite_state->m_last_active_child;

                        // We simply call the on entry of the last node for simple history
                        if (this->m_spy)
                        {
                            this->m_spy->on_entry(next_target->getName(), this->m_ctx);
                        }

                        next_target->onEntry();
                        current_region->m_current_node = next_target;

                        if (current_region->m_current_node->m_node_type ==
                            NodeType::ORTHOGONAL_NODE)
                        {

                            for (Node<CTX, E>* child : current_region->m_current_node->m_children)
                            {
                                // Enforced by init()
                                RegionNode<CTX, E>* region =
                                        static_cast<RegionNode<CTX, E>*>(child);
                                region->construct();
                            }
                        }

                        break;
                    }

                    else if (current_node->m_node_type == NodeType::DEEP_HISTORY_NODE)
                    {

                        // We know that history nodes have no children and thus can only be called
                        // As the last node

                        // Only composite states can have history nodes
                        Node<CTX, E>* containing_composite_state = current_node->m_parent;
                        ROOST_ASSERT(
                                containing_composite_state->m_node_type ==
                                NodeType::COMPOSITE_NODE);

                        Node<CTX, E>* next_target =
                                containing_composite_state->m_last_active_child;

                        // We simply call the on entry of the last node for simple history
                        if (this->m_spy)
                        {
                            this->m_spy->on_entry(next_target->getName(), this->m_ctx);
                        }

                        next_target->onEntry();
                        current_region->m_current_node = next_target;
                        current_region->constructFromDeepHistory();

                        break;
                    }
                }

                // We want to make sure we "drill down" after finalizing the transition
                // after all we could have ended up in a new region
                current_region->construct();
            }

            transitions->clear();

            if (!ignore_events)
            {
                event = m_top.getNoneEvt();
                m_top.handle(event, transitions);
            }
        }
    }

};  // Class: StateMachine

template <typename CTX, typename E>
class NodeAlias
{
public:
    using Node               = roost::Node<CTX, E>;
    using Leaf               = roost::LeafNode<CTX, E>;
    using Composite          = roost::CompositeNode<CTX, E>;
    using Orthogonal         = roost::OrthogonalNode<CTX, E>;
    using Region             = roost::RegionNode<CTX, E>;
    using StateMachine       = roost::StateMachine<CTX, E>;
    using Spy                = roost::Spy<CTX, E>;
    using PrintingSpy        = roost::PrintingSpy<CTX, E>;
    using TracingSpy         = roost::TracingSpy<CTX, E>;
    using PrintingTracingSpy = roost::PrintingTracingSpy<CTX, E>;
    using IErrorSpy          = roost::IErrorSpy<CTX, E>;
    using StandardErrorSpy   = roost::StandardErrorSpy<CTX, E>;
};  // Struct: NodeAlias

}  // ns: roost

#endif  // ROOST_LIB_STATE_MACHINE_HSM_HPP
