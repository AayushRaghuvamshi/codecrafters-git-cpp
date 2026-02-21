#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <zstr.hpp>
#include <openssl/sha.h>
#include <stdbool.h>
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
    auto init() -> void {
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
    auto cat_file(std::string path) -> void {
        std::cerr << "Looking for object with hash: " << path << '\n';
        std::string filepath = ".git/objects/" + path.substr(0, 2) + "/" + path.substr(2);
        zstr::ifstream file(filepath);
        if (!file.is_open()) {
            std::cerr << "Failed to open file: " << filepath << '\n';
            return;
        }
        std::string file_content = read_file(file);
        auto nul = file_content.find('\0');
        if (nul == std::string::npos) {
            std::cerr << "Invalid git object: missing NUL header. Decompressed bytes="
                    << file_content.size() << "\n";
            return;
        }
        std::cout << file_content.substr(nul + 1);
    }
    auto hash_object(std::string path, bool flag_w_present) -> void {
        std::cerr << "Hashing file: " << path << '\n';
        std::ifstream file(path);
        if (!file.is_open()) {  
            std::cerr << "Failed to open file: " << path << '\n';
            return;
        }       
        std::string file_content = read_file(file);
        std::string header = "blob " + std::to_string(file_content.size()) + '\0';
        std::string store_content = header + file_content;
        unsigned char hash[20];
        SHA1(reinterpret_cast<const unsigned char*>(store_content.data()), store_content.size(), hash);
        std::string hash_str;
        for (int i = 0; i < 20; i++) {
            char hex[3];
            snprintf(hex, sizeof(hex), "%02x", hash[i]);
            hash_str += hex;
        }
        std::cerr << hash_str << '\n';
        if (flag_w_present) {
            std::string dir = ".git/objects/" + hash_str.substr(0, 2);
            std::string filepath = dir + "/" + hash_str.substr(2);
            try {
                std::filesystem::create_directories(dir);
                zstr::ofstream out(filepath);
                out.write(store_content.data(), store_content.size());
                out.close();
                std::cout << hash_str << '\n';
            } catch (const std::filesystem::filesystem_error& e) {
                std::cerr << e.what() << '\n';
            }
        }
    }
}