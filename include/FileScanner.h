#pragma once

#include <cstdint>
#include <filesystem>
#include <vector>

struct FileEntry
{
    std::filesystem::path path;
    std::uintmax_t sizeBytes{};
};

std::vector<FileEntry> scanFiles(const std::filesystem::path &root);