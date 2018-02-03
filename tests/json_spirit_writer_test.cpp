//          Copyright John W. Wilkinson 2007 - 2014
// Distributed under the MIT License, see accompanying file LICENSE.txt

// json spirit version 4.08

#include "stdafx.h"
#include "test_utils.h"
#include "json_spirit_writer_template.h"
#include <cstdint>
#include <limits>
#include <sstream>

using namespace json_spirit;
using namespace std;
using namespace boost;

constexpr int64_t max_int64   = (std::numeric_limits<int64_t>::max)();
constexpr uint64_t max_uint64 = (std::numeric_limits<uint64_t>::max)();

using ValueTypes = boost::mpl::list<Value, mValue, wValue, wmValue>;
using ValuesVectorBased = boost::mpl::list<Value, mValue, wValue, wmValue>;
using ValueVectorTypes = boost::mpl::list<Value, wValue>;
using ValueMapTypes = boost::mpl::list<mValue, wmValue>;

template<class Value>
void checkWriteString(const Value& value, const std::string& expected, int flags = json_spirit::none)
{
    auto actual = encoding_cast<std::string>(write_string(value, flags));
    BOOST_CHECK_EQUAL(actual, expected);
}

template<class Value>
void checkWriteStream(const Value& value, const std::string& expected, int flags = json_spirit::none)
{
    using Char = typename Value::String_type::value_type;

    basic_ostringstream<Char> stream;
    stream << std::hex; // the code should handle this, i.e. output decimal
    write_stream(value, stream, flags);

    auto actual = encoding_cast<std::string>(stream.str());
    BOOST_CHECK_EQUAL(actual, expected);
}

template<class Value>
void checkWrite(const Value& value, const std::string& expected, int flags = json_spirit::none)
{
    checkWriteString(value, expected, flags);
    checkWriteStream(value, expected, flags);
}

template<class T>
class Builder
{
public:
    using Value = T;
    using String = typename T::String_type;
    using Array = typename T::Array;

    template<class V>
    static Value value(V&& v)
    {
        return Value(std::forward<V>(v));
    }

    static Value null()
    {
        return Value();
    }

    static String key(const std::string& str)
    {
        return encoding_cast<String>(str);
    }

    static Value string(const std::string& str)
    {
        return Value(encoding_cast<String>(str));
    }
    
    template <typename ...Args>
    static Value array(Args... args)
    {
        return Value(Array{Value(args)...});
    }
};

BOOST_AUTO_TEST_SUITE()
    BOOST_AUTO_TEST_CASE_TEMPLATE(can_print_empty_object, T, ValueTypes)
    {
        T value = typename T::Object{};
        checkWrite(value, "{}");
        checkWrite(value, "{\n}", pretty_print);
    }
    
    BOOST_AUTO_TEST_CASE_TEMPLATE(can_print_empty_array, T, ValueTypes)
    {
        T value = typename T::Array{};
        checkWrite(value, "[]");
        checkWrite(value, "[\n]", pretty_print);
    }

    BOOST_AUTO_TEST_CASE_TEMPLATE(can_print_object_with_one_entry, T, ValueTypes)
    {
        using B = Builder<T>;
        T value = typename T::Object{
             { B::key("hello"), B::string("world") }
        };
        checkWrite(value, R"***({"hello":"world"})***");
    }

    BOOST_AUTO_TEST_CASE_TEMPLATE(can_print_vector_object_with_two_entries, T, ValueVectorTypes)
    {
        using B = Builder<T>;
        T value = typename T::Object{
             { B::key("int"), B::value(24) },
             { B::key("hello"), B::string("world") }
        };
        checkWrite(value, R"***({"int":24,"hello":"world"})***");
    }

    BOOST_AUTO_TEST_CASE_TEMPLATE(can_print_map_object_with_two_entries, T, ValueMapTypes)
    {
        using B = Builder<T>;
        T value = typename T::Object{
             { B::key("in\"t"), B::value(24) },
             { B::key("he\'llo"), B::string("world") }
        };
        checkWrite(value, R"***({"he'llo":"world","in\"t":24})***");
    }

    BOOST_AUTO_TEST_CASE_TEMPLATE(can_print_array, T, ValueTypes)
    {
        using B = Builder<T>;
        // TODO: precision lost when printing 2.4 - it prints 2.39999...
        T value = B::array(B::string(""), B::string("\""), B::string("abcd\""), B::string("28"), 28, 0, -1, 2.5);
        checkWrite(value, R"***(["","\"","abcd\"","28",28,0,-1,2.5])***");

        T value2 = B::array(true, false, true);
        checkWrite(value2, R"***([true,false,true])***");
    }

    BOOST_AUTO_TEST_CASE_TEMPLATE(can_print_nested_array, T, ValueTypes)
    {
        using B = Builder<T>;
        using Object = typename T::Object;

        T value = B::array(B::array(B::string("nested")), 101);
        checkWrite(value, R"***([["nested"],101])***");

        T value2 = B::array(B::array(B::array(B::array(B::array()))));
        checkWrite(value2, R"***([[[[[]]]]])***");
        
        T value3 = B::array(B::array(B::array()),B::array(B::array()));
        checkWrite(value3, R"***([[[]],[[]]])***");

        T value4 = Object{
            { B::key("arr"), B::array(24, B::string("20")) },
            { B::key("obj"), Object{} }
        };
        checkWrite(value4, R"***({"arr":[24,"20"],"obj":{}})***");
    }
    
    BOOST_AUTO_TEST_CASE_TEMPLATE(can_print_nested_object, T, ValueTypes)
    {
        using B = Builder<T>;
        using Object = typename T::Object;

        T value = Object{
            { B::key("obj"), Object{} },
            { B::key("obj2"), Object{ { B::key("/name"), B::string("/value") } } }
        };
        checkWrite(value, R"***({"obj":{},"obj2":{"/name":"/value"}})***");
    }

    BOOST_AUTO_TEST_CASE_TEMPLATE(can_print_object_with_bool_pairs, T, ValueTypes)
    {
        using B = Builder<T>;
        using Object = typename T::Object;
        T value = Object{
            { B::key("name_1"), B::value(true) },
            { B::key("name_2"), B::value(false) },
            { B::key("name_3"), B::value(true) }
        };
        checkWrite(value, R"***({"name_1":true,"name_2":false,"name_3":true})***");
    }
    
    BOOST_AUTO_TEST_CASE_TEMPLATE(can_print_object_with_int_pairs, T, ValueTypes)
    {
        using B = Builder<T>;
        using Object = typename T::Object;
        T value = Object{
            { B::key("name_1"), B::value(1111) },
            { B::key("name_2"), B::value((std::numeric_limits<int32_t>::max)()) },
            { B::key("name_3"), B::value((std::numeric_limits<int64_t>::max)()) }
        };
        checkWrite(value, R"***({"name_1":1111,"name_2":2147483647,"name_3":9223372036854775807})***");
    }
    
    BOOST_AUTO_TEST_CASE_TEMPLATE(can_print_object_with_real_pairs, T, ValueTypes)
    {
        using B = Builder<T>;
        using Object = typename T::Object;
        T value = Object{
            { B::key("name_1"), B::value(0.0) },
            { B::key("name_2"), B::value(1.234567890123456789e-108) },
            { B::key("name_3"), B::value(-1234567890.123456789) },
            { B::key("name_4"), B::value(-1.2e-126) }
        };
        checkWrite(value, R"***({"name_1":0,"name_2":1.2345678901234567e-108,"name_3":-1234567890.1234567,"name_4":-1.2e-126})***");
    }
    
    BOOST_AUTO_TEST_CASE_TEMPLATE(can_print_object_with_null_pairs, T, ValueTypes)
    {
        using B = Builder<T>;
        using Object = typename T::Object;
        T value = Object{
            { B::key("name_1"), B::null() },
            { B::key("name_2"), B::null() },
            { B::key("name_3"), B::null() }
        };
        checkWrite(value, R"***({"name_1":null,"name_2":null,"name_3":null})***");
    }

    // TODO: add tests for remove_trailing_zeros
    // TODO: add tests for escaping Unicode
    // TODO: add tests for escaping non-printable ASCII (with codes 0..31)
    // TODO: add tests for doubles precision
BOOST_AUTO_TEST_SUITE_END()
