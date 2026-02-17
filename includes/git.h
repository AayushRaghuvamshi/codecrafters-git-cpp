#pragma once

#include <string>

namespace git {
    bool init();
    std::string cat_file(std::string filepath);
}