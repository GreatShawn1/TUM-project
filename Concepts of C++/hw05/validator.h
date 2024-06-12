
#pragma once

#include <variant>
#include <vector>

#include "token.h"

namespace sql {

namespace state {
/// Initial state of the FSM
struct Start {};
/// State if any transition was wrong, no matter the token, the FSM will stay in the invalid state
struct Invalid {};
/// If the sequence of tokens is correct the FSM are in the valid state, wrong tokens put the FSM
/// into the invalid state, only the semicolon is allowed.
struct Valid {};

// TODO: all new states go between here...
struct SelectStmt {};
struct AllColumns {};
struct NamedColumn {};
struct MoreColumns {};
struct FromClause {};
struct TableName {};


} // namespace state

/// variant of all possible states of our finite machine
/// TODO: Add all the possible states to the variant
using State =
    std::variant<state::Start,
    state::SelectStmt,
    state::AllColumns,
    state::NamedColumn,
    state::MoreColumns,
    state::FromClause,
    state::TableName,
    state::Invalid, 
    state::Valid>;

/// Transition from the `Start` state to the next state depending on the given
/// token
[[nodiscard]]
State transition(state::Start, const Token &token);

/// Transition from the `Valid` state to the next state depending on the given
/// token
[[nodiscard]]
State transition(state::Valid, const Token &token);

/// Transition from the `Invalid` state to the next state depending on the given
/// token
[[nodiscard]]
State transition(state::Invalid, const Token &token);

// TODO: all of the transition functions from the newly created states go
// between here...
[[nodiscard]] State transition(state::SelectStmt, const Token &token);
[[nodiscard]] State transition(state::AllColumns, const Token &token);
[[nodiscard]] State transition(state::NamedColumn, const Token &token);
[[nodiscard]] State transition(state::MoreColumns, const Token &token);
[[nodiscard]] State transition(state::FromClause, const Token &token);
[[nodiscard]] State transition(state::TableName, const Token &token);


// ... and here

/// Our finite state machine.
/// The initial state is `Start` and based on the given tokens it will move to
/// the next state. You can query if the given sequence of tokens was valid with
/// the `is_valid` member function.

class SqlValidator {
public:
    SqlValidator(); // 构造函数声明

    bool is_valid() const; // 函数声明

    void handle(const Token &token); // 函数声明

private:
    State state_;
};

bool is_valid_sql_query(const std::vector<Token> &tokens);

} // namespace sql
#pragma once
