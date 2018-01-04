#include "stdafx.h"
#include "test_utils.h"

#include <boost/locale/encoding.hpp>

using namespace boost::locale::conv;

//          Copyright Sergey Shambir 2017
// Distributed under the MIT License, see accompanying file LICENSE.txt

std::wstring utf8_to_wstring(const std::string& text)
{
    return utf_to_utf<wchar_t>(text, stop);
}

std::string wstring_to_utf8(const std::wstring& text)
{
    return utf_to_utf<char>(text);
}

std::stringstream make_string_stream(const std::string& source)
{
    return std::stringstream(source);
}

std::wstringstream make_string_stream(const std::wstring& source)
{
    return std::wstringstream(source);
}

std::ostream& json_spirit::operator<<(std::ostream& out, const json_spirit::Value& value)
{
    out << json_spirit::write_string(value, json_spirit::pretty_print | json_spirit::single_line_arrays);
    return out;
}

std::ostream& json_spirit::operator<<(std::ostream& out, const json_spirit::mValue& value)
{
    out << json_spirit::write_string(value, json_spirit::pretty_print | json_spirit::single_line_arrays);
    return out;
}

std::ostream& json_spirit::operator<<(std::ostream& out, const json_spirit::wValue& value)
{
    out << wstring_to_utf8(json_spirit::write_string(value, json_spirit::pretty_print | json_spirit::single_line_arrays));
    return out;
}

std::ostream& json_spirit::operator<<(std::ostream& out, const json_spirit::wmValue& value)
{
    out << wstring_to_utf8(json_spirit::write_string(value, json_spirit::pretty_print | json_spirit::single_line_arrays));
    return out;
}

std::ostream& json_spirit::operator<<(std::ostream& out, json_spirit::Value_type type)
{
    switch (type)
    {
        case json_spirit::obj_type:
            out << "Object";
            break;
        case json_spirit::array_type:
            out << "Array";
            break;
        case json_spirit::str_type:
            out << "String";
            break;
        case json_spirit::bool_type:
            out << "Boolean";
            break;
        case json_spirit::int_type:
            out << "Integer";
            break;
        case json_spirit::real_type:
            out << "Number";
            break;
        case json_spirit::null_type:
            out << "null";
            break;
    }
    return out;
}
