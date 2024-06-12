#include "validator.h"
namespace sql {
    SqlValidator::SqlValidator() : state_(state::Start{}) {}

[[nodiscard]] bool is_valid_sql_query(const std::vector<Token> &tokens) {
        SqlValidator validator;
        for (const auto& token : tokens) {
            validator.handle(token);
        }
        return validator.is_valid();
    }

bool SqlValidator::is_valid() const {
    return std::holds_alternative<state::Valid>(state_);
}

void SqlValidator::handle(const Token &token) {
    state_ = std::visit([this, &token](auto&& arg) {
        return transition(arg, token);
    }, state_);
}

struct TransitionFromStartVisitor {
    State operator()(token::Select) const { return state::SelectStmt{}; }
    template <typename T>
    State operator()(T) const { return state::Invalid{}; }
};

State transition(state::Start, const Token &token) {
    return std::visit(TransitionFromStartVisitor{}, token.value());
}

State transition(state::SelectStmt, const Token &token) {
    if (std::holds_alternative<token::Asterisks>(token.value())) {
        return state::AllColumns{};
    }
    if (std::holds_alternative<token::Identifier>(token.value())) {
        return state::NamedColumn{};
    }
    return state::Invalid{};
}

State transition(state::AllColumns, const Token &token) {
    if (std::holds_alternative<token::From>(token.value())) {
        return state::FromClause{};
    }
    return state::Invalid{};
}

State transition(state::NamedColumn, const Token &token) {
    if (std::holds_alternative<token::Comma>(token.value())) {
        return state::MoreColumns{};
    }
    if (std::holds_alternative<token::From>(token.value())) {
        return state::FromClause{};
    }
    return state::Invalid{};
}

State transition(state::MoreColumns, const Token &token) {
    if (std::holds_alternative<token::Identifier>(token.value())) {
        return state::NamedColumn{};
    }
    return state::Invalid{};
}




State transition(state::FromClause, const Token &token) {
    if (std::holds_alternative<token::Identifier>(token.value())) {
        return state::TableName{};
    }
    return state::Invalid{};
}

State transition(state::TableName, const Token &token) {
    if (std::holds_alternative<token::Semicolon>(token.value())) {
        return state::Valid{};
    }
    return state::Invalid{};
}

State transition(state::Valid, const Token &token) {
    // Once in the Valid state, any additional tokens (other than Semicolon) should invalidate the query
    if (!std::holds_alternative<token::Semicolon>(token.value())) {
        return state::Invalid{};
    }
    return state::Valid{};
}

State transition(state::Invalid, const Token &token) {
    // Remain in the Invalid state regardless of further tokens
    return state::Invalid{};
}

} // namespace sql
