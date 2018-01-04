//          Copyright John W. Wilkinson 2007 - 2014
// Distributed under the MIT License, see accompanying file LICENSE.txt

// json spirit version 4.08

#include "stdafx.h"
#include "test_utils.h"
#include <limits.h>
#include <cstdint>

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
    Object{ {"a"s, "b"s}, { "key"s, 99.86 } }
};
}

BOOST_AUTO_TEST_SUITE()

    BOOST_AUTO_TEST_CASE(can_use_object_value)
    {
        Object obj1 = {
            {"name1", "value1"}
        };
        Object obj2 = {
            {"name1", "value1"}
        };
        Object obj3 = {
            {"name3", "value3"}
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
            const InnerValue actual = Value{expected}.get_value<InnerValue>();
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
        check_get_value(Object{ {"a"s, "b"s}, { "key"s, 99.86 } });
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

BOOST_AUTO_TEST_SUITE_END()

#if 0
namespace
{
#ifdef JSON_SPIRIT_VALUE_ENABLED
    void test_obj_map_implemention()
    {
#ifdef JSON_SPIRIT_MVALUE_ENABLED
        mObject obj;

        obj[ "name 1" ] = 1;
        obj[ "name 2" ] = "two";

        assert_eq( obj.size(), 2u );

        assert_eq( obj.find( "name 1" )->second.get_int(), 1 );
        assert_eq( obj.find( "name 2" )->second.get_str(), "two" );
#endif
    }

    template< typename Int >
    void check_an_int_is_a_real( Int i, bool expected_result )
    {
        assert_eq( Value( i ).is_uint64(), expected_result );
    }

    void test_is_uint64()
    {
        check_an_int_is_a_real( 1,                            false );
        check_an_int_is_a_real( static_cast< int64_t  >( 1 ), false );
        check_an_int_is_a_real( static_cast< uint64_t >( 1 ), true );
    }

    template< typename Int >
    void check_an_int_is_a_real( Int i, double expected_result )
    {
        assert_eq( Value( i ).get_real(), expected_result );
    }

    void test_an_int_is_a_real()
    {
        check_an_int_is_a_real( -1, -1.0 );
        check_an_int_is_a_real(  0,  0.0 );
        check_an_int_is_a_real(  1,  1.0 );
        check_an_int_is_a_real( max_int64,  9223372036854775800.0 );
        check_an_int_is_a_real( max_uint64, 18446744073709552000.0 );
    }

    template< typename T >
    void check_wrong_type_exceptions( const Value& v, const string& requested_type_name, const string& actual_type_name )
    {
        try
        {
            v.get_value< T >();

            assert( false );
        }
        catch( const runtime_error& e )
        {
            ostringstream os;

            os << "get_value< " << requested_type_name << " > called on " << actual_type_name << " Value";

            assert_eq( e.what(), os.str() );
        }
    }

    template< typename T >
    void check_wrong_type_exceptions( const string& requested_type_name )
    {
        Value v;

        assert_eq( v.type(), null_type );

        check_wrong_type_exceptions< T >( v, requested_type_name, "null" );
    }

    void test_wrong_type_exceptions()
    {
        check_wrong_type_exceptions< Object >( "Object" );
        check_wrong_type_exceptions< Array >( "Array" );
        check_wrong_type_exceptions< string >( "string" );
        check_wrong_type_exceptions< bool >( "boolean");
        check_wrong_type_exceptions< boost::int64_t >( "integer" );
        check_wrong_type_exceptions< int >( "integer" );
        check_wrong_type_exceptions< double >( "real" );

        Value v( "string" );

        assert_eq( v.type(), str_type );

        check_wrong_type_exceptions< double >( v, "real", "string" );
    }
#endif

    template< class Config_type >
    class Container_constructor_runner
    {
    public:

        Container_constructor_runner()
        {
            vector< double > vd = list_of( 1.2 )( 1.3 );  test_container_constructor( vd );
            vector< int >    vi = list_of( 1 );           test_container_constructor( vi );
                             vi = list_of( 1 )( 2 );      test_container_constructor( vi );
                             vi = list_of( 1 )( 2 )( 3 ); test_container_constructor( vi );
            list< double >   ld = list_of( 1.2 )( 1.3 );  test_container_constructor( ld );
            list< int >      li = list_of( 1 );           test_container_constructor( li );
                             li = list_of( 1 )( 2 );      test_container_constructor( li );
                             li = list_of( 1 )( 2 )( 3 ); test_container_constructor( li );
        }

    private:

        typedef typename Config_type::Array_type Array_type;
        typedef typename Config_type::Value_type Value_type;

        template< class Cont >
        void test_container_constructor( const Cont& cont )
        {
            typedef typename Cont::value_type Cont_value_type;
            const Value_type val( cont.begin(), cont.end() );
            const Array_type& arr = val.get_array();
            Cont result;
            for( unsigned int i = 0; i < arr.size(); ++i )
            {
                result.push_back( arr[i].template get_value< Cont_value_type>() );
            }
            assert_eq( result, cont );
        }
    };

    void test_container_constructor()
    {
#ifdef JSON_SPIRIT_VALUE_ENABLED
       Container_constructor_runner< Config  >();
#endif
#ifdef JSON_SPIRIT_MVALUE_ENABLED
       Container_constructor_runner< mConfig >();
#endif
#if defined( JSON_SPIRIT_WVALUE_ENABLED ) && !defined( BOOST_NO_STD_WSTRING )
        Container_constructor_runner< wConfig  >();
#endif
#if defined( JSON_SPIRIT_WMVALUE_ENABLED ) && !defined( BOOST_NO_STD_WSTRING )
        Container_constructor_runner< wmConfig >();
#endif
    }

    template< class Config_type >
    class Variant_constructor_runner
    {
    public:

        Variant_constructor_runner()
        {
            test_variant_constructor< variant< int, double > >( 1.23 );
            test_variant_constructor< variant< int, double > >( 123 );
            test_variant_constructor< variant< int, double, String_type > >( to_str< String_type >( "foo" ) );
            test_variant_constructor< variant< int, double, String_type, bool > >( true );
            test_variant_constructor< variant< int, double, String_type, bool, boost::int64_t > >( boost::int64_t( 123 ) );
            test_variant_constructor< variant< int, double, String_type, bool, boost::uint64_t > >( boost::uint64_t( 123 ) );

            {
                variant< int, Null > variant = Null();
                const Value_type val( variant );
                assert( val.is_null() );
            }
            
            vector< double > vd = list_of( 1.2 )( 1.3 );   test_variant_array_constructor< double > ( vd );
            vector< int >    vi = list_of( 1 );            test_variant_array_constructor< int >( vi );
                             vi = list_of( 1 )( 2 );       test_variant_array_constructor< int >( vi );
                             vi = list_of( 1 )( 2 )( 3 );  test_variant_array_constructor< int >( vi );
            list< double >   ld = list_of( 1.2 )( 1.3 );   test_variant_array_constructor< double >( ld );
            list< int >      li = list_of( 1 );            test_variant_array_constructor< int >( li );
                             li = list_of( 1 )( 2 );       test_variant_array_constructor< int >( li );
                             li = list_of( 1 )( 2 )( 3 );  test_variant_array_constructor< int >( li );
        }

    private:

        typedef typename Config_type::String_type String_type;
        typedef typename Config_type::Array_type Array_type;
        typedef typename Config_type::Value_type Value_type;

        template< class Variant_t, typename T >
        void test_variant_constructor( const T& t )
        {
            const Variant_t variant( t );
            const Value_type val( variant );
            assert_eq( val.template get_value< T >(), t );
        }

        template< typename T, typename A, template< typename, typename > class Cont >
        void test_variant_array_constructor( const Cont< T, A >& cont )
        {
            const variant< int, Cont< T, A > > variant = cont;
            const Value_type val( variant );
            const Array_type& arr = val.get_array();
            Cont< T, A > result;
            for( unsigned int i = 0; i < arr.size(); ++i )
            {
                result.push_back( arr[i].template get_value< T >() );
            }
            assert_eq( result, cont );
        }
    };

    void test_variant_constructor()
    {
#ifdef JSON_SPIRIT_VALUE_ENABLED
        Variant_constructor_runner< Config  >();
#endif
#ifdef JSON_SPIRIT_MVALUE_ENABLED
        Variant_constructor_runner< mConfig >();

#endif
#if defined( JSON_SPIRIT_WVALUE_ENABLED ) && !defined( BOOST_NO_STD_WSTRING )
        Variant_constructor_runner< wConfig  >();
#endif
#if defined( JSON_SPIRIT_WMVALUE_ENABLED ) && !defined( BOOST_NO_STD_WSTRING )
       Variant_constructor_runner< wmConfig >();
#endif
    }
}

void json_spirit::test_value()
{
#ifdef JSON_SPIRIT_VALUE_ENABLED
    Object obj;
    Value value_str ( "value" );
    Value value_obj ( obj );
    Value value_bool( true );

    Value value_str_2 ( string( "value" ) );
    Value value_obj_2 ( obj );
    Value value_bool_2( false );

    const char* str( "value" );
    Value value_str_2b ( str );

    assert_eq( value_str, value_str );
    assert_eq( value_str, value_str_2 );
    assert_eq( value_str, value_str_2b );
    assert_eq( value_obj, value_obj );
    assert_eq( value_obj, value_obj_2 );
    assert_neq( value_str, value_obj );
    assert_neq( value_str, value_bool );

    Object obj_2;
    obj_2.push_back( Pair( "name", value_str ) );
    Value value_str_3( "xxxxx" );
    Value value_obj_3( obj_2 );

    assert_neq( value_str, value_str_3 );
    assert_neq( value_obj, value_obj_3 );

    test_obj_value();
    test_array_value();
    test_bool_value();
    test_int_value();
    test_real_value();
    test_null_value();
    test_get_value();
    test_copying();
    test_obj_map_implemention();
    test_is_uint64();
    test_an_int_is_a_real();
    test_wrong_type_exceptions();
#endif
    test_container_constructor();
    test_variant_constructor();
}
#endif
