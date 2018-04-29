//          Copyright John W. Wilkinson 2007 - 2014
// Distributed under the MIT License, see accompanying file LICENSE.txt

// json spirit version 4.08

#include "stdafx.h"
#include "json_spirit_stream_reader.h"
#include "test_utils.h"
#include <type_traits>

using namespace json_spirit;
using namespace std;
using namespace boost;

namespace
{
template<class Value>
using StringStream = std::conditional_t<
    std::is_same<typename Value::String_type, std::wstring>::value,
    std::wstringstream,
    std::stringstream>;

template<class Value>
class StreamTestHelper
{
public:
    using Stream = StringStream<Value>;
    using String = typename Value::String_type;

    StreamTestHelper(const String& source)
        : m_stream1(source)
        , m_stream2(source)
        , m_reader1(m_stream1)
        , m_reader2(m_stream2)
    {
        static_assert(std::is_same<Value, std::decay_t<Value>>::value, "internal error: Value isn't plain type");
    }

    void operator()()
    {
        Value value1{};
        Value value2{};
        BOOST_REQUIRE(m_reader1.read_next(value1));
        BOOST_REQUIRE_NO_THROW(m_reader2.read_next(value2));
        BOOST_REQUIRE_EQUAL(value1, value2);
    }

private:
    Stream m_stream1;
    Stream m_stream2;
    json_spirit::Stream_reader<Stream, Value> m_reader1;
    json_spirit::Stream_reader_thrower<Stream, Value> m_reader2;
};
}

std::string SYNTAX_TEST_DATA[] = {
    "[][1][1,2][1,2,3]",
    "[] [1] [1,2] [1,2,3]",
    " [] [1] [1,2] {\"key\":\"value\"} [1,2,3]"
};

BOOST_AUTO_TEST_SUITE()

BOOST_DATA_TEST_CASE(can_parse_json_stream, SYNTAX_TEST_DATA, sourceUtf8)
{
    StreamTestHelper<json_spirit::Value> helperUtf8(sourceUtf8);
    helperUtf8();

    StreamTestHelper<json_spirit::wValue> helperWide(utf8_to_wstring(sourceUtf8));
    helperWide();
}

BOOST_AUTO_TEST_SUITE_END()
