#include "stdafx.h"
#include <boost/algorithm/string.hpp>
#include <boost/dll/runtime_symbol_info.hpp>
#include <boost/filesystem.hpp>
#include <boost/test/data/monomorphic.hpp>
#include <fstream>
#include <iomanip>

#include "json_spirit_reader.h"
#include "json_spirit_value.h"
#include "json_spirit_writer_template.h"

using namespace boost::filesystem;
namespace data = boost::unit_test::data;

namespace std
{
ostream& operator<<(ostream& out, const vector<boost::filesystem::path>& values)
{
    out << "[";
    bool addComma = false;
    for (auto&& value : values)
    {
        if (addComma)
        {
            out << ", ";
        }
        addComma = true;
        out << std::quoted(value.string());
    }
    out << "]";
    return out;
}

ostream& operator<<(ostream& out, const json_spirit::Value& value)
{
    out << json_spirit::write_string(value, json_spirit::pretty_print | json_spirit::single_line_arrays);
    return out;
}
}

namespace
{
struct test_cases
{
    // parse must be successful.
    std::vector<path> correct_files;
    // parse must fail.
    std::vector<path> broken_files;
    // parse may either fail or be successful, there are no strict requirement in JSON RFC documents
    std::vector<path> undefined_files;
};

test_cases get_all_json_in_dir(const path& dir)
{
    const path jsonExt{ ".json" };
    test_cases cases;

    for (directory_iterator it(dir), end; it != end; ++it)
    {
        const path item = it->path();
        if (is_regular_file(item) && item.extension() == jsonExt)
        {
            const std::string filename = item.filename().string();
            if (boost::starts_with(filename, "y_"))
            {
                cases.correct_files.push_back(item);
            }
            else if (boost::starts_with(filename, "n_"))
            {
                cases.broken_files.push_back(item);
            }
            else if (boost::starts_with(filename, "i_") || boost::starts_with(filename, "t_"))
            {
                cases.undefined_files.push_back(item);
            }
        }
    }
    return cases;
}

test_cases& get_test_cases()
{
    boost::filesystem::path exe_dir = boost::dll::program_location().parent_path();
    static test_cases cases = get_all_json_in_dir(exe_dir / "integration_tests_data");
    return cases;
}

std::string read_whole_file(const path& filepath)
{
    std::ifstream input(filepath.native());
    input.exceptions(std::ios::badbit);
    if (!input.is_open())
    {
        throw std::runtime_error("cannot open " + filepath.string());
    }

    const auto input_size = static_cast<size_t>(file_size(filepath));

    std::string content;
    content.reserve(input_size);
    std::copy(std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>(), std::back_inserter(content));

    if (input_size != content.size())
    {
        throw std::runtime_error("cannot read whole file " + filepath.string());
    }

    return content;
}
}

BOOST_AUTO_TEST_SUITE(json_parser_tests)

BOOST_DATA_TEST_CASE(can_parse_correct_files, data::make(get_test_cases().correct_files), filepath)
{
    const std::string content = read_whole_file(filepath);
    BOOST_REQUIRE(!content.empty());
    json_spirit::Value value;
    bool succeed = json_spirit::read(content, value);
    BOOST_CHECK(succeed);
}

BOOST_DATA_TEST_CASE(fails_to_parse_broken_files, data::make(get_test_cases().broken_files), filepath)
{
	const std::string content = read_whole_file(filepath);
    json_spirit::Value value;
	BOOST_CHECK_NO_THROW(json_spirit::read(content, value));
	// TODO: add BOOST_CHECK(!succeed);
}

BOOST_DATA_TEST_CASE(does_not_throw_on_undefined_files, data::make(get_test_cases().undefined_files), filepath)
{
    const std::string content = read_whole_file(filepath);
    BOOST_REQUIRE(!content.empty());
    json_spirit::Value value;
    BOOST_CHECK_NO_THROW(json_spirit::read(content, value));
}

// FIXME: after first and second rewrite JSON must be the same, but it isn't
#if 0
BOOST_DATA_TEST_CASE(correct_files_a_same_after_two_writes, data::make(get_test_cases().correct_files), filepath)
{
	const std::string content = read_whole_file(filepath);

	json_spirit::Value value_generation_1;
	json_spirit::Value value_generation_2;

	std::string json_generation_2;
	std::string json_generation_3;

	{
		BOOST_CHECK(json_spirit::read(content, value_generation_1));
		json_generation_2 = json_spirit::write_string(value_generation_1);
	}

	{
		BOOST_CHECK(json_spirit::read(json_generation_2, value_generation_2));
		json_generation_3 = json_spirit::write_string(value_generation_2);
	}

	BOOST_CHECK_EQUAL(value_generation_1, value_generation_2);
	BOOST_CHECK_EQUAL(json_generation_2, json_generation_3);
}
#endif

BOOST_AUTO_TEST_SUITE_END()
