#pragma once

#include <string>

namespace git {
    auto init() -> void;
    auto cat_file(std::string path) -> void;
    auto hash_object(std::string path, bool flag_w_present) -> void;
}