#pragma once

#include "FileScanner.h"

#include <filesystem>
#include <string>
#include <vector>

struct SearchResult
{
    std::filesystem::path path;
    int lineNumber{};
    std::string lineText;
};

std::vector<SearchResult> searchFilesForText(const std::vector<FileEntry>& files ,
                                             const std::string& query);