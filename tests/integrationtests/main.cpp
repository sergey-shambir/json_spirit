#include "stdafx.h"

#include <boost/algorithm/string/replace.hpp>
#include <boost/test/included/unit_test_framework.hpp>
#include <boost/test/output/compiler_log_formatter.hpp>

using namespace std;

// Specifies formatting for tests log.
// Add command-line flag `--log-level=test_suite` to see full logs with formatting.
class SpecLogFormatter : public boost::unit_test::output::compiler_log_formatter
{
    void test_unit_start(std::ostream& os, boost::unit_test::test_unit const& tu) override
    {
        // Output test case name with indentation, replace '_' with ' '.
        os << string(m_indent, ' ') << boost::replace_all_copy(tu.p_name.get(), "_", " ") << endl;
        m_indent += 2;
    }

    void test_unit_finish(ostream& /*os*/, boost::unit_test::test_unit const& /*tu*/, unsigned long /*elapsed*/) override
    {
        m_indent -= 2;
    }

    int m_indent = 0;
};

boost::unit_test::test_suite* init_unit_test_suite(int /*argc*/, char* /*argv*/ [])
{
    // Replace default log formatter.
    boost::unit_test::unit_test_log.set_formatter(new SpecLogFormatter);
    // Root test suite now has name 'All tests'
    boost::unit_test::framework::master_test_suite().p_name.value = "All tests";
    return nullptr;
}

int main(int argc, char* argv[])
{
    // Run unit tests with custom initializer.
    return boost::unit_test::unit_test_main(&init_unit_test_suite, argc, argv);
}

