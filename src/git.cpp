#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <zstr.hpp>
#include "../includes/git.h"

namespace {
    std::string read_file(std::istream& in) {
        std::string content;
        std::array<char, 64*1024> buffer;
        while(in) {
            in.read(buffer.data(), buffer.size());
            std::streamsize n = in.gcount();
            if (n > 0) {
                content.append(buffer.data(), static_cast<size_t>(n));
            }
        }
        return content;
    }
}


namespace git {
    void init() {
        try {
            std::filesystem::create_directory(".git");
            std::filesystem::create_directory(".git/objects");
            std::filesystem::create_directory(".git/refs");
    
            std::ofstream headFile(".git/HEAD");
            if (headFile.is_open()) {
                headFile << "ref: refs/heads/main\n";
                headFile.close();
            } else {
                std::cerr << "Failed to create .git/HEAD file.\n";
            }
    
            std::cout << "Initialized git directory\n";
        } catch (const std::filesystem::filesystem_error& e) {
            std::cerr << e.what() << '\n';
        }
    }
    void cat_file(std::string path) {
        std::cerr << "Looking for object with hash: " << path << '\n';
        std::string filepath = ".git/objects/" + path.substr(0, 2) + "/" + path.substr(2);
        zstr::ifstream file(filepath);
        if (!file.is_open()) {
            std::cerr << "Failed to open file: " << filepath << '\n';
            return;
        }
        std::array<char, 64*1024> buffer;
        std::string file_content = read_file(file);
        auto nul = file_content.find('\0');
        if (nul == std::string::npos) {
            std::cerr << "Invalid git object: missing NUL header. Decompressed bytes="
                    << file_content.size() << "\n";
            return;
        }
        std::cout << file_content.substr(nul + 1);
    }
}