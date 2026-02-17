#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <zstr.hpp>


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
    void catfile(std::string path) {
        std::cerr << "Looking for object with hash: " << path << '\n';
        std::string filepath = ".git/objects/" + path.substr(0, 2) + "/" + path.substr(2);
        std::cerr << "Constructed file path: " << filepath << '\n';
        zstr::ifstream file(filepath);
        if (!file.is_open()) {
            std::cerr << "Failed to open file: " << filepath << '\n';
            return;
        }
        std::array<char, 64*1024> buffer;
        std::string file_content;
        while(file) {
            file.read(buffer.data(), buffer.size());
            std::streamsize n = file.gcount();
            if (n > 0) {
                file_content.append(buffer.data(), static_cast<size_t>(n));
            }
        }
        auto nul = file_content.find('\0');
        if (nul == std::string::npos) {
            std::cerr << "Invalid git object: missing NUL header. Decompressed bytes="
                    << file_content.size() << "\n";
            return;
        }
        std::cout << file_content.substr(nul + 1);
    }
}