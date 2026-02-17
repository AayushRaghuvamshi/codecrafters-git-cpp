#pragma once

#include <string>

namespace git {
    auto init() -> void;
    auto cat_file(std::string path) -> void;
}