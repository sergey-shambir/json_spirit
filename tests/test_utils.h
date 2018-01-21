#pragma once
#include <string>
#include "json_spirit_value.h"
#include "json_spirit_writer_template.h"

//          Copyright Sergey Shambir 2017
// Distributed under the MIT License, see accompanying file LICENSE.txt

std::wstring utf8_to_wstring(const std::string& text);
std::string wstring_to_utf8(const std::wstring& text);

namespace detail
{
template<typename StringType>
class encoding_cast_helper
{
public:
    StringType operator()(const std::string& text);
    StringType operator()(const std::wstring& text);
};

template<>
inline std::string encoding_cast_helper<std::string>::operator()(const std::string& text)
{
    return text;
}

template<>
inline std::string encoding_cast_helper<std::string>::operator()(const std::wstring& text)
{
    return wstring_to_utf8(text);
}

template<>
inline std::wstring encoding_cast_helper<std::wstring>::operator()(const std::string& text)
{
    return utf8_to_wstring(text);
}

template<>
inline std::wstring encoding_cast_helper<std::wstring>::operator()(const std::wstring& text)
{
    return text;
}
}

template<typename StringType>
inline StringType encoding_cast(const std::string& text)
{
    detail::encoding_cast_helper<StringType> helper;
    return helper(text);
}

template<typename StringType>
inline StringType encoding_cast(const std::wstring& text)
{
    detail::encoding_cast_helper<StringType> helper;
    return helper(text);
}

std::wstringstream make_string_stream(const std::wstring& source);
std::stringstream make_string_stream(const std::string& source);

namespace json_spirit
{
std::ostream& operator<<(std::ostream& out, const json_spirit::Value& value);
std::ostream& operator<<(std::ostream& out, const json_spirit::mValue& value);
std::ostream& operator<<(std::ostream& out, const json_spirit::wValue& value);
std::ostream& operator<<(std::ostream& out, const json_spirit::wmValue& value);
std::ostream& operator<<(std::ostream& out, Value_type type);

template<class FirstT, class SecondT>
std::ostream& operator<<(std::ostream& out, std::pair<FirstT, SecondT> pair)
{
    out << "[pair first:" << pair.first << " second:" << pair.second;
    return out;
}
}
