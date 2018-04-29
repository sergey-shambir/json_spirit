#include "stdafx.h"
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>

using namespace boost::filesystem;

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

std::vector<path> get_all_json_in_dir(const path& dir)
{
    const path jsonExt{".json"};
    test_cases cases;

    for (directory_iterator it(p), end; it != end; ++it)
    {
        path item = it->path();
        if (is_regular_file(item) && item.extension() == jsonExt)
        {
            const std::string filename = path.filename().string();
            if (boost::starts_with(filename, "y_"))
            {
                cases.emplace_back();
            }
            paths.emplace_back(std::move(item));
        }
    }
    return paths;
}


}
