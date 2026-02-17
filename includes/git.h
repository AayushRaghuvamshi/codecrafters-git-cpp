#pragma once

#include <string>

namespace git {
    void init();
    auto cat_file(std::string hash);
}