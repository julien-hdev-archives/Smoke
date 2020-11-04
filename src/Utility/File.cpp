#include "File.hpp"

#include <fstream>
#include <qlogging.h>

std::string MyFile::LoadToString(const std::string& filepath) {
    // Thanks to https://stackoverflow.com/questions/2602013/read-whole-ascii-file-into-c-stdstring
    std::string res;
    std::ifstream stream(filepath);
    if (!stream.is_open()) {
        qFatal("[MyFile::LoadToString] Failed to open file '%s'", filepath.c_str());
        return "";
    }
    stream.seekg(0, std::ios::end);
    res.reserve(stream.tellg());
    stream.seekg(0, std::ios::beg);
    res.assign(
        (std::istreambuf_iterator<char>(stream)),
        std::istreambuf_iterator<char>()
    );
    stream.close();
    return res;
}