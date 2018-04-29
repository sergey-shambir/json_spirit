#ifndef JSON_SPIRIT_ERROR_POSITION
#define JSON_SPIRIT_ERROR_POSITION

//          Copyright John W. Wilkinson 2007 - 2014
// Distributed under the MIT License, see accompanying file LICENSE.txt

// json spirit version 4.08

#if defined(_MSC_VER)
#pragma once
#endif

#include <stdexcept>
#include <string>

namespace json_spirit
{
// An JsonParseError exception is thrown by the "read_or_throw" functions below on finding an error.
// Note the "read_or_throw" functions are around 3 times slower than the standard functions "read"
// functions that return a bool.
class JsonParseError final : public std::runtime_error
{
public:
    JsonParseError(unsigned line, unsigned column, const std::string& reason);
    bool operator==(const JsonParseError& lhs) const;

    unsigned get_line() const;
    unsigned get_column() const;
    const std::string& get_reason() const;

private:
    static std::string format_what(unsigned line, unsigned column, const std::string& reason);

    unsigned line_;
    unsigned column_;
    std::string reason_;
};

inline JsonParseError::JsonParseError(unsigned int line, unsigned int column, const std::string& reason)
    : std::runtime_error(format_what(line, column, reason))
    , line_(line)
    , column_(column)
    , reason_(reason)
{
}

inline bool JsonParseError::operator==(const JsonParseError& lhs) const
{
    if (this == &lhs)
        return true;

    return (reason_ == lhs.reason_)
        && (line_ == lhs.line_)
        && (column_ == lhs.column_);
}

inline unsigned JsonParseError::get_line() const
{
    return line_;
}

inline unsigned JsonParseError::get_column() const
{
    return column_;
}

inline const std::string& JsonParseError::get_reason() const
{
    return reason_;
}

inline std::string JsonParseError::format_what(unsigned line, unsigned column, const std::string& reason)
{
    std::string what;
    what.append(reason);
    what.append(" at ").append(std::to_string(line)).append(":").append(std::to_string(column));
    return what;
}
}

#endif
