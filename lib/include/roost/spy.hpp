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

#ifndef ROOST_LIB_SPY_HPP
#define ROOST_LIB_SPY_HPP

#include <iostream>
#include <vector>

namespace roost
{

/*!
 * \brief Spy provides hooks into the framework when certain events happen
 *
 * The spy class allows for the user of the framework to have certain
 * callbacks executed when the framework does common state machine
 * functions.
 *
 * The two most common use cases are:
 * - Logging all entries, exits, guards and actions in a uniform manner
 * - Unit Testing
 *
 * \tparam CTX the context type
 * \tparam E the event enum class type
 */
template <typename CTX, typename E>
class Spy
{
public:
    Spy()          = default;
    virtual ~Spy() = default;

    /*!
     * \brief on_entry called whenever the framework enters a node
     *
     * \param node_name the node name that had on_entry called
     * \param ctx the reference to the context
     */
    virtual void on_entry(const char* node_name, CTX& ctx) = 0;

    /*!
     * \brief on_exit called whenever the framework exits a node
     *
     * \param node_name the node name that had on_exit called
     * \param ctx the reference to the context
     */
    virtual void on_exit(const char* node_name, CTX& ctx) = 0;

    /*!
     * \brief action called whenever an action is executed
     *
     * \param node_name the node name that had the action executed
     * \param ctx the reference to the context
     * \param e the event supplied
     * \param action_name the name of the action function being executed
     */
    virtual void action(const char* node_name, CTX& ctx, E const& e, const char* action_name) = 0;

    /*!
     * \brief guard called whenever a guard is evaluated
     *
     * \param node_name the node name that had guard evaluated
     * \param ctx the reference to the context
     * \param e the event supplied
     * \param guard_name the name of the guard being evaluated
     * \param status the return value of the guard
     */
    virtual void guard(
            const char* node_name,
            CTX&        ctx,
            E const&    e,
            const char* guard_name,
            bool        status) = 0;

    /*!
     * \brief no_transition called whenever transition was not handled by the state machine
     *
     * \param node_name the top node name
     * \param ctx the reference to the context
     * \param e the event supplied
     */
    virtual void no_transition(const char* node_name, CTX& ctx, E const& e) = 0;

    /*!
     * \brief event called whenever an event is evaluated by the state machine
     *
     * \param node_name the node name that handled the event (aka the transition source)
     * \param ctx the reference to the context
     * \param e the event supplied
     */
    virtual void event(const char* node_name, CTX& ctx, E const& e) = 0;

    /*!
     * \brief error called when an error occurs NOT associated with an event
     *
     * \param node_name the node name that experienced the error
     * \param ctx the reference to the context
     * \param main_error the main error string
     * \param sub_system_error an optional secondary error string
     */
    virtual void error(
            const char* node_name,
            CTX&        ctx,
            const char* main_error,
            const char* sub_system_error = "") = 0;

    /*!
     * \brief error called when an error occurs associated with an event
     *
     * \param node_name the node name that experienced the error
     * \param ctx the reference to the context
     * \param e the event associated with the event
     * \param main_error the main error string
     * \param sub_system_error an optional secondary error string
     */
    virtual void error(
            const char* node_name,
            CTX&        ctx,
            E const&    e,
            const char* main_error,
            const char* sub_system_error = "") = 0;

};  // Class: Spy

/*!
 * \brief PrintingSpy prints out to STDOUT
 */
template <typename CTX, typename E>
class PrintingSpy : public Spy<CTX, E>
{
public:
    PrintingSpy()          = default;
    virtual ~PrintingSpy() = default;

    void on_entry(const char* node_name, CTX&) override
    {
        std::cout << "[" << node_name << "] "
                  << "[On-Entry]" << std::endl;
    }
    void on_exit(const char* node_name, CTX&) override
    {
        std::cout << "[" << node_name << "] "
                  << "[On-Exit]" << std::endl;
    }
    void action(const char* node_name, CTX&, E const&, const char* action_name) override
    {
        std::cout << "[" << node_name << "] "
                  << "[Action: " << action_name << "]" << std::endl;
    }
    void guard(const char* node_name, CTX&, E const& e, const char* guard_name, bool status)
            override
    {
        std::cout << "[" << node_name << "] "
                  << "[Event: " << e << "] "
                  << "[Guard: " << guard_name << "] [Status: " << (status ? "True" : "False")
                  << "]" << std::endl;
    }
    void event(const char* node_name, CTX&, E const& e) override
    {
        std::cout << "[" << node_name << "] "
                  << "[Event: " << e << "]" << std::endl;
    }
    void no_transition(const char* node_name, CTX&, E const& e) override
    {
        std::cout << "[" << node_name << "] "
                  << "[No Transition: " << e << "]" << std::endl;
    }
    void error(const char* node_name, CTX&, const char* main_error, const char* sub_system_error)
            override
    {
        std::cerr << "[" << node_name << "] "
                  << "[Error] [Main Error: " << main_error
                  << "] [Sub-System Error: " << sub_system_error << "]" << std::endl;
    }

    void error(
            const char* node_name,
            CTX&,
            E const&    e,
            const char* main_error,
            const char* sub_system_error) override
    {
        std::cerr << "[" << node_name << "] "
                  << "[Error] [Event: " << e << "] [Main Error: " << main_error
                  << "] [Sub-System Error: " << sub_system_error << "]" << std::endl;
    }

};  // Class: PrintingSpy

/*!
 * \brief IErrorSpy interface that does not define error and no transition functions
 *
 * All other functions are NOOPs
 *
 */
template <typename CTX, typename E>
class IErrorSpy : public Spy<CTX, E>
{
public:
    IErrorSpy()          = default;
    virtual ~IErrorSpy() = default;

    void on_entry(const char*, CTX&) override
    {
    }
    void on_exit(const char*, CTX&) override
    {
    }
    void action(const char*, CTX&, E const&, const char*) override
    {
    }
    void guard(const char*, CTX&, E const&, const char*, bool) override
    {
    }
    void event(const char*, CTX&, E const&) override
    {
    }

    // Still have to define error and no transition!

};  // Class: IErrorSpy

/*!
 * \brief StandardErrorSpy prints out errors and no transitions to STDERR
 */
template <typename CTX, typename E>
class StandardErrorSpy : public IErrorSpy<CTX, E>
{
public:
    StandardErrorSpy()          = default;
    virtual ~StandardErrorSpy() = default;

    void no_transition(const char* node_name, CTX&, E const& e) override
    {
        std::cerr << "[" << node_name << "] "
                  << "[No Transition: " << e << "]" << std::endl;
    }
    void error(const char* node_name, CTX&, const char* main_error, const char* sub_system_error)
            override
    {
        std::cerr << "[" << node_name << "] "
                  << "[Error] [Main Error: " << main_error
                  << "] [Sub-System Error: " << sub_system_error << "]" << std::endl;
    }

    void error(
            const char* node_name,
            CTX&,
            E const&    e,
            const char* main_error,
            const char* sub_system_error) override
    {
        std::cerr << "[" << node_name << "] "
                  << "[Error] [Event: " << e << "] [Main Error: " << main_error
                  << "] [Sub-System Error: " << sub_system_error << "]" << std::endl;
    }
};  // Class: StandardErrorSpy

/*!
 * \brief TracingSpy appends OE- or OX- to node names for unit testing purposes
 */
template <typename CTX, typename E>
class TracingSpy : public Spy<CTX, E>
{
public:
    TracingSpy(std::vector<std::string>& events) : m_events(events)
    {
    }

    void on_entry(const char* node_name, CTX&) override
    {
        std::string tmp{"OE-"};
        tmp.append(node_name);
        m_events.push_back(tmp);
    }
    void on_exit(const char* node_name, CTX&) override
    {
        std::string tmp{"OX-"};
        tmp.append(node_name);
        m_events.push_back(tmp);
    }

    void action(const char*, CTX&, E const&, const char*) override
    {
    }
    void guard(const char*, CTX&, E const&, const char*, bool) override
    {
    }

    void event(const char*, CTX&, E const&) override
    {
    }

    void no_transition(const char*, CTX&, E const&) override
    {
    }

    void error(const char* node_name, CTX&, const char* main_error, const char* sub_system_error)
            override
    {
        std::cerr << "[" << node_name << "] "
                  << "[Error] [Main Error: " << main_error
                  << "] [Sub-System Error: " << sub_system_error << "]" << std::endl;
    }

    void error(
            const char* node_name,
            CTX&,
            E const&    e,
            const char* main_error,
            const char* sub_system_error) override
    {
        std::cerr << "[" << node_name << "] "
                  << "[Error] [Event: " << e << "] [Main Error: " << main_error
                  << "] [Sub-System Error: " << sub_system_error << "]" << std::endl;
    }

    std::vector<std::string>& m_events;

};  // Class: TracingSpy

/*!
 * \brief PrintingTracingSpy performs same function as TracingSpy but prints to STDOUT as well
 */
template <typename CTX, typename E>
class PrintingTracingSpy : public PrintingSpy<CTX, E>
{
public:
    PrintingTracingSpy(std::vector<std::string>& events) : PrintingSpy<CTX, E>(), m_events(events)
    {
    }

    void on_entry(const char* node_name, CTX& ctx) override
    {
        std::string tmp{"OE-"};
        tmp.append(node_name);
        m_events.push_back(tmp);

        PrintingSpy<CTX, E>::on_entry(node_name, ctx);
    }
    void on_exit(const char* node_name, CTX& ctx) override
    {
        std::string tmp{"OX-"};
        tmp.append(node_name);
        m_events.push_back(tmp);

        PrintingSpy<CTX, E>::on_exit(node_name, ctx);
    }
    void action(const char* node_name, CTX& ctx, E const& e, const char* action_name) override
    {

        PrintingSpy<CTX, E>::action(node_name, ctx, e, action_name);
    }
    void guard(const char* node_name, CTX& ctx, E const& e, const char* guard_name, bool status)
            override
    {
        PrintingSpy<CTX, E>::guard(node_name, ctx, e, guard_name, status);
    }
    void event(const char* node_name, CTX& ctx, E const& e) override
    {
        PrintingSpy<CTX, E>::event(node_name, ctx, e);
    }

    void no_transition(const char* node_name, CTX& ctx, E const& e) override
    {
        PrintingSpy<CTX, E>::no_transition(node_name, ctx, e);
    }
    void error(
            const char* node_name,
            CTX&        ctx,
            const char* main_error,
            const char* sub_system_error) override
    {
        PrintingSpy<CTX, E>::error(node_name, ctx, main_error, sub_system_error);
    }

    void error(
            const char* node_name,
            CTX&        ctx,
            E const&    e,
            const char* main_error,
            const char* sub_system_error) override
    {
        PrintingSpy<CTX, E>::error(node_name, ctx, e, main_error, sub_system_error);
    }

    std::vector<std::string>& m_events;

};  // Class: PrintingTracingSpy

}  // ns: roost

#endif  // ROOST_LIB_SPY_HPP
