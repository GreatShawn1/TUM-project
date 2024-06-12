#pragma once

#include <string>
#include <variant>
#ifndef HEADER_NAME_H
#define HEADER_NAME_H


namespace sql {

namespace token {
    // Token struct for 'SELECT' keyword
    struct Select {};

    // Token struct for '*' (all columns)
    struct Asterisks {};

    // Token struct for ',' (comma)
    struct Comma {};

    // Token struct for 'FROM' keyword
    struct From {};

    // Token struct for ';' (semicolon)
    struct Semicolon {};

    // Token struct for identifiers (like table or column names)
    struct Identifier {
        std::string name;
    };

    // ...（添加任何其他必需的令牌类型）
} // namespace token

// Class representing a token for our simplified SQL select clause.
class Token {
public:
    // Using std::variant to hold any type of token
    using token_type = std::variant<
        token::Select, 
        token::Asterisks, 
        token::Comma, 
        token::From, 
        token::Semicolon, 
        token::Identifier
        // ...（添加任何其他必需的令牌类型）
    >;

    // Declare the constructor, definition will be in token.cpp
    Token(token_type value);

    // Getter for the underlying variant
    [[nodiscard]] token_type value() const;

private:
    token_type value_;
};

} // namespace sql
#pragma once
#endif // HEADER_NAME_H

