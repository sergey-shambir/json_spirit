//          Copyright John W. Wilkinson 2007 - 2014
// Distributed under the MIT License, see accompanying file LICENSE.txt

// json spirit version 4.08

#include "stdafx.h"
#include "test_utils.h"
#include <cstdint>
#include <limits.h>

using namespace json_spirit;
using namespace std;
using namespace std::literals;
using namespace boost;

namespace
{
const Value COPY_TEST_DATA[] = {
    Value(),
    true,
    false,
    -1,
    0,
    29,
    2'000'000'001,
    -2'000'000'001,
    -1009.28,
    std::numeric_limits<uint64_t>::max(),
    ""s,
    "42"s,
    "text with space and\nlinebreak"s,
    Array{ 1, "a" },
    Object{ { "a"s, "b"s }, { "key"s, 99.86 } }
};

const std::pair<Value, bool> IS_UINT64_TEST_DATA[] = {
    { Value(false), false },
    { Value(true), false },
    { Value(""), false },
    { Value("hello"), false },
    { Value("0"), false },
    { Value(0.0), false },
    { Value(std::numeric_limits<double>::quiet_NaN()), false },
    { Value(1.0), false },
    { Value(1.2), false },
    { Value(1), false },
    { Value(int64_t(1)), false },
    { Value(uint64_t(1)), true }
};

const std::pair<Value, double> GET_REAL_TEST_DATA[] = {
    { Value(-1), -1.0 },
    { Value(0), 0.0 },
    { Value(1), 1.0 },
    { Value(42), 42.0 },
    { Value((std::numeric_limits<int64_t>::max)()), 9223372036854775800.0 },
    { Value((std::numeric_limits<uint64_t>::max)()), 18446744073709552000.0 },
};
}

BOOST_AUTO_TEST_SUITE()

BOOST_AUTO_TEST_CASE(can_use_object_value)
{
    Object obj1 = {
        { "name1", "value1" }
    };
    Object obj2 = {
        { "name1", "value1" }
    };
    Object obj3 = {
        { "name3", "value3" }
    };
    Value value1{ obj1 };
    Value value2{ obj2 };
    Value value3{ obj3 };

    BOOST_CHECK_EQUAL(value1.type(), obj_type);
    BOOST_CHECK_EQUAL(value2.type(), obj_type);
    BOOST_CHECK_EQUAL(value3.type(), obj_type);

    BOOST_CHECK_EQUAL(value1, value2);
    BOOST_CHECK_EQUAL(value1.get_obj(), obj2);
    BOOST_CHECK_EQUAL(obj1, value2.get_obj());

    BOOST_CHECK_NE(value1, value3);
    BOOST_CHECK_NE(obj1, obj3);
}

BOOST_AUTO_TEST_CASE(can_use_array_value)
{
    Array array1 = { 1, "2" };
    Array array2 = { 1, "2" };
    Array array3 = { 1, "X" };
    Value value1 = array1;
    Value value2 = array2;
    Value value3 = array3;

    BOOST_CHECK_EQUAL(value1.type(), array_type);
    BOOST_CHECK_EQUAL(value1, value2);
    BOOST_CHECK_EQUAL(value1.get_array(), array2);

    BOOST_CHECK_NE(value1, value3);
    BOOST_CHECK_NE(array1, array3);
}

BOOST_AUTO_TEST_CASE(can_use_bool_value)
{
    Value value1(true);
    Value value2(true);
    Value value3(false);
    Value value4(false);

    BOOST_CHECK_EQUAL(value1.type(), bool_type);
    BOOST_CHECK_EQUAL(value1, value2);
    BOOST_CHECK_EQUAL(value3, value4);
    BOOST_CHECK_NE(value1, value3);
    BOOST_CHECK(value1.get_bool());
    BOOST_CHECK(!value3.get_bool());
}

BOOST_AUTO_TEST_CASE(can_use_int32_value)
{
    Value value1(42);
    Value value2(42);
    Value value3(std::numeric_limits<int32_t>::max());
    Value value4(std::numeric_limits<int32_t>::min());

    BOOST_CHECK_EQUAL(value1.type(), int_type);
    BOOST_CHECK_EQUAL(value3.type(), int_type);
    BOOST_CHECK_EQUAL(value1, value2);
    BOOST_CHECK_EQUAL(value1.get_int(), 42);
    BOOST_CHECK_EQUAL(value2.get_int(), 42);
    BOOST_CHECK_EQUAL(value3.get_int(), std::numeric_limits<int32_t>::max());
    BOOST_CHECK_EQUAL(value4.get_int(), std::numeric_limits<int32_t>::min());

    BOOST_CHECK_NE(value1, value3);
    BOOST_CHECK_NE(value3, value4);
}

BOOST_AUTO_TEST_CASE(can_use_int64_value)
{
    Value value1(42);
    Value value2(std::numeric_limits<int64_t>::max());
    Value value3(std::numeric_limits<int64_t>::min());

    BOOST_CHECK_EQUAL(value2.type(), int_type);
    BOOST_CHECK_EQUAL(value3.type(), int_type);
    BOOST_CHECK_EQUAL(value1.get_int64(), int64_t(42));
    BOOST_CHECK_EQUAL(value2.get_int64(), std::numeric_limits<int64_t>::max());
    BOOST_CHECK_EQUAL(value3.get_int64(), std::numeric_limits<int64_t>::min());

    BOOST_CHECK_NE(value1, value2);
    BOOST_CHECK_NE(value2, value3);
    BOOST_CHECK_NE(value1, value3);
}

BOOST_AUTO_TEST_CASE(can_use_uint64_value)
{
    Value value1(42);
    Value value2(std::numeric_limits<uint64_t>::max());
    Value value3(std::numeric_limits<uint64_t>::min());

    BOOST_CHECK_EQUAL(value2.type(), int_type);
    BOOST_CHECK_EQUAL(value3.type(), int_type);
    BOOST_CHECK_EQUAL(value1.get_uint64(), uint64_t(42));
    BOOST_CHECK_EQUAL(value2.get_uint64(), std::numeric_limits<uint64_t>::max());
    BOOST_CHECK_EQUAL(value3.get_uint64(), std::numeric_limits<uint64_t>::min());

    BOOST_CHECK_NE(value1, value2);
    BOOST_CHECK_NE(value2, value3);
    BOOST_CHECK_NE(value1, value3);
}

BOOST_AUTO_TEST_CASE(can_use_double_and_float_values)
{
    Value value1(42.3);
    Value value2(std::numeric_limits<float>::max());
    Value value3(std::numeric_limits<float>::min());
    Value value4(std::numeric_limits<double>::max());
    Value value5(std::numeric_limits<double>::min());

    BOOST_CHECK_EQUAL(value1.type(), real_type);
    BOOST_CHECK_EQUAL(value2.type(), real_type);
    BOOST_CHECK_EQUAL(value3.type(), real_type);
    BOOST_CHECK_EQUAL(value4.type(), real_type);
    BOOST_CHECK_EQUAL(value5.type(), real_type);

    BOOST_CHECK_EQUAL(value1, Value(42.3));
    BOOST_CHECK_EQUAL(value2, Value(std::numeric_limits<float>::max()));
    BOOST_CHECK_EQUAL(value3, Value(std::numeric_limits<float>::min()));
    BOOST_CHECK_EQUAL(value4, Value(std::numeric_limits<double>::max()));
    BOOST_CHECK_EQUAL(value5, Value(std::numeric_limits<double>::min()));

    BOOST_CHECK_EQUAL(value1.get_real(), 42.3);
    BOOST_CHECK_EQUAL(value2.get_real(), double(std::numeric_limits<float>::max()));
    BOOST_CHECK_EQUAL(value3.get_real(), double(std::numeric_limits<float>::min()));
    BOOST_CHECK_EQUAL(value4.get_real(), std::numeric_limits<double>::max());
    BOOST_CHECK_EQUAL(value5.get_real(), std::numeric_limits<double>::min());

    BOOST_CHECK_NE(value1, value2);
    BOOST_CHECK_NE(value1, value3);
    BOOST_CHECK_NE(value1, value4);
    BOOST_CHECK_NE(value1, value5);
}

BOOST_AUTO_TEST_CASE(can_use_null_value)
{
    Value value1;
    Value value2;
    Value value3(false);

    BOOST_CHECK_EQUAL(value1.type(), null_type);
    BOOST_CHECK_EQUAL(value2.type(), null_type);
    BOOST_CHECK_EQUAL(value1, value2);
    BOOST_CHECK_NE(value1, value3);

    BOOST_CHECK(value1.is_null());
    BOOST_CHECK(!value3.is_null());
}

BOOST_AUTO_TEST_CASE(can_get_inner_value_by_type)
{
    auto check_get_value = [](auto&& expected) {
        using InnerValue = std::decay_t<decltype(expected)>;
        const InnerValue actual = Value{ expected }.get_value<InnerValue>();
        BOOST_CHECK_EQUAL(actual, expected);
    };
    check_get_value(true);
    check_get_value(false);

    check_get_value(-1);
    check_get_value(0);
    check_get_value(29);
    check_get_value(2'001);
    check_get_value(-2'001);
    check_get_value(2'000'001);
    check_get_value(-2'000'001);
    check_get_value(2'000'000'001);
    check_get_value(-2'000'000'001);
    check_get_value(std::numeric_limits<uint64_t>::max());
    check_get_value(std::numeric_limits<int64_t>::max());
    check_get_value(std::numeric_limits<int32_t>::max());
    check_get_value(std::numeric_limits<uint64_t>::min());
    check_get_value(std::numeric_limits<int64_t>::min());
    check_get_value(std::numeric_limits<int32_t>::min());

    check_get_value(0.0);
    check_get_value(29.0);
    check_get_value(1.3);
    check_get_value(-1009.28);
    check_get_value(double(std::numeric_limits<float>::max()));
    check_get_value(std::numeric_limits<double>::max());
    check_get_value(double(std::numeric_limits<float>::min()));
    check_get_value(std::numeric_limits<double>::min());

    check_get_value(""s);
    check_get_value("42"s);
    check_get_value("hello"s);
    check_get_value("text with space and\nlinebreak"s);

    check_get_value(Array{ 1, "a" });
    check_get_value(Object{ { "a"s, "b"s }, { "key"s, 99.86 } });
}

BOOST_DATA_TEST_CASE(can_copy_value, COPY_TEST_DATA, value)
{
    const auto type = value.type();
    Value copy = value;
    Value anotherCopy;
    anotherCopy = copy;

    BOOST_CHECK_EQUAL(value, copy);
    BOOST_CHECK_EQUAL(value.type(), type);
    BOOST_CHECK_EQUAL(value, anotherCopy);
}

BOOST_AUTO_TEST_CASE(can_use_map_for_objets)
{
    mObject object;
    object["key 1"] = 1;
    object["key 2"] = "another value";
    object["key 1"] = "hello";

    BOOST_CHECK_EQUAL(object.size(), size_t(2));
    BOOST_CHECK_THROW(object.find("key 1")->second.get_int(), std::exception);
    BOOST_CHECK_EQUAL(object.find("key 1")->second.get_str(), "hello");
    BOOST_CHECK_EQUAL(object.find("key 2")->second.get_str(), "another value");
}

BOOST_DATA_TEST_CASE(can_check_if_is_uint64, IS_UINT64_TEST_DATA, data)
{
    BOOST_CHECK_EQUAL(data.first.is_uint64(), data.second);
}

BOOST_DATA_TEST_CASE(can_get_real_from_int, GET_REAL_TEST_DATA, data)
{
    BOOST_CHECK_EQUAL(data.first.get_real(), data.second);
}

BOOST_AUTO_TEST_CASE(can_construct_from_initializer_lists)
{
// TODO: fix initializer lists constructor
#if 0
        {
            Value value = {{ "hello" }};
            BOOST_CHECK_EQUAL(value.type(), array_type);
            BOOST_CHECK_EQUAL(value.get_array().size(), 1);
            BOOST_CHECK_EQUAL(value.get_array().at(0).get_str(), "hello");
        }
        {
            Value value = { "hello", "world" };
            BOOST_CHECK_EQUAL(value.type(), array_type);
            BOOST_CHECK_EQUAL(value.get_array().size(), 2);
            BOOST_CHECK_EQUAL(value.get_array().at(0).get_str(), "hello");
            BOOST_CHECK_EQUAL(value.get_array().at(1).get_str(), "world");
        }
#endif
    {
        Value value = Array{ { "hello" } };
        BOOST_CHECK_EQUAL(value.type(), array_type);
        BOOST_CHECK_EQUAL(value.get_array().size(), 1u);
        BOOST_CHECK_EQUAL(value.get_array().at(0).get_str(), "hello");
    }
    {
        Value value = Array{ "hello", "world" };
        BOOST_CHECK_EQUAL(value.type(), array_type);
        BOOST_CHECK_EQUAL(value.get_array().size(), 2u);
        BOOST_CHECK_EQUAL(value.get_array().at(0).get_str(), "hello");
        BOOST_CHECK_EQUAL(value.get_array().at(1).get_str(), "world");
    }
    {
        Value value = Array{ "hello", "world", "again" };
        BOOST_CHECK_EQUAL(value.type(), array_type);
        BOOST_CHECK_EQUAL(value.get_array().size(), 3u);
        BOOST_CHECK_EQUAL(value.get_array().at(0).get_str(), "hello");
        BOOST_CHECK_EQUAL(value.get_array().at(1).get_str(), "world");
        BOOST_CHECK_EQUAL(value.get_array().at(2).get_str(), "again");
    }
    {
        Value value = Array{ Array{ "hello" } };
        BOOST_CHECK_EQUAL(value.type(), array_type);
        BOOST_CHECK_EQUAL(value.get_array().size(), 1u);
        BOOST_CHECK_EQUAL(value.get_array().at(0).get_array(), Array{ "hello" });
    }
    {
        Value value = Value{ Array{ "hello" } };
        BOOST_CHECK_EQUAL(value.type(), array_type);
        BOOST_CHECK_EQUAL(value.get_array().size(), 1u);
        BOOST_CHECK_EQUAL(value.get_array().at(0).get_str(), "hello");
    }
    {
        Value value = Array{ Value{ "hello" } };
        BOOST_CHECK_EQUAL(value.type(), array_type);
        BOOST_CHECK_EQUAL(value.get_array().size(), 1u);
        BOOST_CHECK_EQUAL(value.get_array().at(0).get_str(), "hello");
    }
    {
        Value value = Array{ Array{ "hello" }, "world" };
        BOOST_CHECK_EQUAL(value.type(), array_type);
        BOOST_CHECK_EQUAL(value.get_array().size(), 2u);
        BOOST_CHECK_EQUAL(value.get_array().at(0).get_array(), Array{ "hello" });
        BOOST_CHECK_EQUAL(value.get_array().at(1).get_str(), "world");
    }
}

BOOST_AUTO_TEST_SUITE_END()
