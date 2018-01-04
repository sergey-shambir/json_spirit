//          Copyright John W. Wilkinson 2007 - 2014
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include "stdafx.h"
#include "test_utils.h"

#include "json_spirit_reader.h"
#include "json_spirit_value.h"
#include "json_spirit_writer.h"

using namespace json_spirit;
using namespace std;
using namespace boost;

// TODO: read-write-read-write-read checks: after each read JSON objects are the same
// Texts after first and second write should be same.

namespace
{
std::string SYNTAX_ONLY_TEST_DATA[] = {
    // empty object
    "{}",
    "{ }",
    "{ } ",
    "\t\t{ }  ",
    "   {}\t",

    // empty array
    "[]",
    "[ ]",
    "[ ] ",
    "\t\t[ ]  ",
    "   []\t",

    // array of numbers and string
    "[1,2,3]",
    "[ 1, -2, 3]",
    "[ 1.2, -2e6, -3e-6 ]",
    "[ 1.2, \"str\", -3e-6, { \"field\" : \"data\" } ]",
    "[" + std::to_string(INT_MIN) + "," + std::to_string(INT_MAX) + "]",
    "[" + std::to_string(LONG_LONG_MIN) + "," + std::to_string(LONG_LONG_MAX) + "]",

    // object with string key/values
    "{\"\":\"\"}",
    "{\"test\":\"123\"}",
    "{\"test\" : \"123\"}",
    "{\"testing testing testing\":\"123\"}",
    "{\"\":\"abc\"}",
    "{\"abc\":\"\"}",
    "{\"\":\"\"}",
    "{\"test1\":\"123\",\"test2\":\"456\"}",
    "{\"test1\":\"123\",\"test2\":\"456\",\"test3\":\"789\"}",

    // object with boolean value
    "{\"test\":true}",
    "{\"test\":false}",

    // object with null value
    "{\"test\":null}",

    // object with nested array
    "{\"test1\":[\"a\",\"bb\",\"cc\"]}",
    "{\"test1\":[true,false,null]}",
    "{\"test1\":[true,\"abc\",{\"a\":\"b\"},{\"d\":false},null]}",
    "{\"test1\":[1,2,-3]}",
    "{\"test1\":[1.1,2e4,-1.234e-34]}",

    // object with nested object
    "{\n"
                    "\t\"test1\":\n"
                    "\t\t{\n"
                    "\t\t\t\"test2\":\"123\",\n"
                    "\t\t\t\"test3\":\"456\"\n"
                    "\t\t}\n"
                    "}\n",
    "{\"test1\":{\"test2\":{\"test3\":\"456\"}}}",
    "{\"test1\":{\"test2\":\"123\",\"test3\":\"456\"}}",
};

std::string SYNTAX_FAIL_TEST_DATA[] = {
    // Arrays without comma separator
    "[1 2 3]",
    "[\"key\" \"value\"]",

    // Arrays with double comma separator
    "[1, 2,, 3]",
    "[\"key\",, \"value\"]",

    // Objects without comma/colon separator
    "{ \"key\": \"value\" \"anotherKey\": \"anotherValue\" }",
    "{ \"key\" \"value\", \"anotherKey\": \"anotherValue\" }",
    
    // Objects with double comma/colon separator
    "{ \"key\": \"value\",, \"anotherKey\": \"anotherValue\" }",
    "{ \"key\": \"value\", \"anotherKey\": :\"anotherValue\" }",

    // TODO: json_spirit should have both strict/nostrict parsing modes.
    // Arrays with trailing comma separator (strict JSON prohibits that)
    "[1, 2, 3,]",

    // TODO: json_spirit should fail on this test in strict mode.
    // "[1, 2, 3],",
};

template<typename Value>
class ReadTestDataBuilder
{
public:
    using Config = typename  Value::Config_type;
    using Object = typename Config::Object_type;
    using Array = typename Config::Array_type;
    using String = typename Config::String_type;

    static String get_source_1()
    {
        constexpr char source[] = R"***({
    "name 1": "value 1",
    "name 2":
    {
        "name 3": [-10, 42, 99999],
        "another name":
        {
            "name 5": null,
            "name 4": 15812
        },
        "name 7": "This is a \"heavy\" case"
    }
})***";
        return encoding_cast<String>(source);
    }
    
    static String get_source_2()
    {
        constexpr char source[] = R"***({
    "country": {
        "city": {
            "street_info": {
                "house_info": {
                    "address": "Soloviev street, 25"
                }
            }
        }
    }
})***";
        return encoding_cast<String>(source);
    }
    
    static String get_source_3()
    {
        constexpr char source[] = R"***([
    1,
    1.2,
    "john]",
    true,
    false,
    null
])***";
        return encoding_cast<String>(source);
    }
    
    static String get_source_4()
    {
        constexpr char source[] = R"***([
    1.200000000000000,
    -1.234000000000000e-123,
    1.000000000000000e-123,
    1234567890.123456,123
])***";
        return encoding_cast<String>(source);
    }

    static String get_source_5()
    {
        constexpr char source[] = R"***([
    "\t",
    "a\t",
    "\tb",
    "\t\n\\",
    "\/\r\b\f\"",
    "a\x62c",
    "\x61\x62\x63",
    "\u0061\u0062\u0063",
    "\x01\x02\x7F"
])***";
        return encoding_cast<String>(source);
    }

    static Value get_value_1()
    {
        return Value(Object{
            { key("name 1"), string("value 1") },
            { key("name 2"), Value(Object{
                { key("name 3"), array( -10, 42, 99999 ) },
                { key("another name"), Value(Object{
                    { key("name 5"), Value() },
                    { key("name 4"), Value(15812) }
                })},
                { key("name 7"), string("This is a \"heavy\" case") }
            })}
        });
    }

    static Value get_value_2()
    {
        return Value(Object{
            { key("country"), Value(Object{
                { key("city"), Value(Object{
                    { key("street_info"), Value(Object{
                        { key("house_info"), Value(Object{
                            { key("address"), string("Soloviev street, 25") }
                        })}
                    })}
                })}
            })}
        });
    }

    static Value get_value_3()
    {
        return array(1, 1.2, string("john]"), true, false, Value());
    }

    static Value get_value_4()
    {
        // TODO: following value gets rounding error when reading it from JSON: 1.234567890123456e+125
        return array(
            1.200000000000000,
            -1.234000000000000e-123,
            1.000000000000000e-123,
            1234567890.123456,
            123
        );
    }

    static Value get_value_5()
    {
        return array(
            string("\t"),
            string("a\t"),
            string("\tb"),
            string("\t\n\\"),
            string("/\r\b\f\""),
            string("abc"),
            string("abc"),
            string("abc"),
            string("\x01\x02\x7F")
        );
    }

private:
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

template<typename String>
class ReadTestHelper
{
public:
    using vConfig = typename json_spirit::Config_vector<String>;
    using mConfig = typename json_spirit::Config_map<String>;
    using vValue = typename vConfig::Value_type;
    using mValue = typename mConfig::Value_type;
    using vBuilder = ReadTestDataBuilder<vValue>;
    using mBuilder = ReadTestDataBuilder<mValue>;

    struct Case
    {
        String source;
        vValue vExpected;
        mValue mExpected;
    };

    static std::vector<Case> get_test_data()
    {
        return {
            Case{ vBuilder::get_source_1(), vBuilder::get_value_1(), mBuilder::get_value_1() },
            Case{ vBuilder::get_source_2(), vBuilder::get_value_2(), mBuilder::get_value_2() },
            Case{ vBuilder::get_source_3(), vBuilder::get_value_3(), mBuilder::get_value_3() },
            Case{ vBuilder::get_source_4(), vBuilder::get_value_4(), mBuilder::get_value_4() },
            Case{ vBuilder::get_source_5(), vBuilder::get_value_5(), mBuilder::get_value_5() }
        };
    }

    static void checkRead(const Case& data)
    {
        std::array<vValue, 6> vValues = {
            invoke_read<vValue>(data.source),
            invoke_read<vValue>(make_string_stream(data.source)),
            invoke_read<vValue>(std::begin(data.source), std::end(data.source)),
            invoke_read_or_throw<vValue>(data.source),
            invoke_read_or_throw<vValue>(make_string_stream(data.source)),
            invoke_read_or_throw<vValue>(std::begin(data.source), std::end(data.source)),
        };
        BOOST_REQUIRE_EQUAL(vValues.at(0), data.vExpected);
        BOOST_REQUIRE_EQUAL(vValues.at(1), data.vExpected);
        BOOST_REQUIRE_EQUAL(vValues.at(2), data.vExpected);
        BOOST_REQUIRE_EQUAL(vValues.at(3), data.vExpected);
        BOOST_REQUIRE_EQUAL(vValues.at(4), data.vExpected);
        BOOST_REQUIRE_EQUAL(vValues.at(5), data.vExpected);

        std::array<mValue, 6> mValues = {
            invoke_read<mValue>(data.source),
            invoke_read<mValue>(make_string_stream(data.source)),
            invoke_read<mValue>(std::begin(data.source), std::end(data.source)),
            invoke_read_or_throw<mValue>(data.source),
            invoke_read_or_throw<mValue>(make_string_stream(data.source)),
            invoke_read_or_throw<mValue>(std::begin(data.source), std::end(data.source)),
        };

        BOOST_REQUIRE_EQUAL(mValues.at(0), data.mExpected);
        BOOST_REQUIRE_EQUAL(mValues.at(1), data.mExpected);
        BOOST_REQUIRE_EQUAL(mValues.at(2), data.mExpected);
        BOOST_REQUIRE_EQUAL(mValues.at(3), data.mExpected);
        BOOST_REQUIRE_EQUAL(mValues.at(4), data.mExpected);
        BOOST_REQUIRE_EQUAL(mValues.at(5), data.mExpected);
    }
    
    template<typename Value, typename ...Args>
    static Value invoke_read(Args... arguments)
    {
        Value value{};
        BOOST_CHECK(json_spirit::read(arguments..., value));
        return value;
    }

    template<typename Value, typename ...Args>
    static Value invoke_read_or_throw(Args... arguments)
    {
        Value value{};
        BOOST_CHECK_NO_THROW(json_spirit::read_or_throw(arguments..., value));
        return value;
    }
};

template<class Value, bool shouldFail>
class SyntaxTestHelper
{
public:
    SyntaxTestHelper()
    {
        static_assert(std::is_same<Value, std::decay_t<Value>>::value, "internal error: Value isn't plain type");
    }

    template<typename String>
    void operator()(const String& source)
    {
        std::array<Value, 6> values = {
            invoke_read(source),
            invoke_read(make_string_stream(source)),
            invoke_read(std::begin(source), std::end(source)),
            invoke_read_or_throw(false, source),
            invoke_read_or_throw(false, make_string_stream(source)),
            invoke_read_or_throw(true, std::begin(source), std::end(source)),
        };

        BOOST_REQUIRE_EQUAL(values.at(0), values.at(1));
        BOOST_REQUIRE_EQUAL(values.at(0), values.at(2));
        BOOST_REQUIRE_EQUAL(values.at(0), values.at(3));
        BOOST_REQUIRE_EQUAL(values.at(0), values.at(4));
        BOOST_REQUIRE_EQUAL(values.at(0), values.at(5));
    }

private:
    template<typename ...Args>
    static Value invoke_read(Args... arguments)
    {
        Value value{};
        if (shouldFail)
        {
            BOOST_CHECK(!json_spirit::read(arguments..., value));
        }
        else
        {
            BOOST_CHECK(json_spirit::read(arguments..., value));
        }
        return value;
    }

    // TODO: JSON Spirit should never throw strings, but it does - we must fix it.
    template<typename ...Args>
    static Value invoke_read_or_throw(bool throwsString, Args... arguments)
    {
        Value value{};
        if (shouldFail)
        {
            if (throwsString)
            {
                BOOST_CHECK_THROW(json_spirit::read_or_throw(arguments..., value), std::string);
            }
            else
            {
                BOOST_CHECK_THROW(json_spirit::read_or_throw(arguments..., value), json_spirit::Error_position);
            }
        }
        else
        {
            BOOST_CHECK_NO_THROW(json_spirit::read_or_throw(arguments..., value));
        }
        return value;
    }
};
}

namespace std
{
std::ostream& operator<<(std::ostream& stream, const typename ReadTestHelper<std::string>::Case& data)
{
    stream << "Case{\n"
        << "\tsource: " << encoding_cast<std::string>(data.source) << ",\n"
        << "\tvExpected: " << data.vExpected << ",\n"
        << "\tmExpected: " << data.mExpected << "\n"
        << "}\n";
    return stream;
}

std::ostream& operator<<(std::ostream& stream, const typename ReadTestHelper<std::wstring>::Case& data)
{
    stream << "Case{\n"
        << "\tsource: " << encoding_cast<std::string>(data.source) << ",\n"
        << "\tvExpected: " << data.vExpected << ",\n"
        << "\tmExpected: " << data.mExpected << "\n"
        << "}\n";
    return stream;
}
}

BOOST_AUTO_TEST_SUITE()

    BOOST_DATA_TEST_CASE(can_parse_small_json, SYNTAX_ONLY_TEST_DATA, sourceUtf8)
    {
        SyntaxTestHelper<json_spirit::Value, false> testValue;
        testValue(sourceUtf8);

        SyntaxTestHelper<json_spirit::mValue, false> testMapValue;
        testMapValue(sourceUtf8);

        std::wstring sourceUtf16 = utf8_to_wstring(sourceUtf8);

        SyntaxTestHelper<json_spirit::wValue, false> testWideValue;
        testWideValue(sourceUtf16);

        SyntaxTestHelper<json_spirit::wmValue, false> testWideMapValue;
        testWideMapValue(sourceUtf16);
    }

    BOOST_DATA_TEST_CASE(parse_fails_on_invalid_json, SYNTAX_FAIL_TEST_DATA, sourceUtf8)
    {
        SyntaxTestHelper<json_spirit::Value, true> testValue;
        testValue(sourceUtf8);

        SyntaxTestHelper<json_spirit::mValue, true> testMapValue;
        testMapValue(sourceUtf8);

        std::wstring sourceUtf16 = utf8_to_wstring(sourceUtf8);

        SyntaxTestHelper<json_spirit::wValue, true> testWideValue;
        testWideValue(sourceUtf16);

        SyntaxTestHelper<json_spirit::wmValue, true> testWideMapValue;
        testWideMapValue(sourceUtf16);
    }

    BOOST_DATA_TEST_CASE(read_values_equal_to_expected, ReadTestHelper<std::string>::get_test_data(), data)
    {
        ReadTestHelper<std::string>::checkRead(data);
    }

BOOST_AUTO_TEST_SUITE_END()
