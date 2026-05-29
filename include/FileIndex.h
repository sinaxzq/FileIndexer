#include "FileScanner.h"

#include <filesystem>
#include <string>
#include <unordered_map>
#include <vector>

struct IndexedOccurrence
{
    std::filesystem::path path;
    int lineNumber{};
};

struct FileIndex
{
    std::unordered_map<std::string, std::vector<IndexedOccurrence>> words;
};

std::vector<std::string> tokenizeLine(const std::string &line);

FileIndex buildIndex(const std::vector<FileEntry> &files);

struct RankedFileResult
{
    std::filesystem::path path;
    int matchCount{};
};

std::vector<RankedFileResult> searchIndex(const FileIndex &index, const std::string &query);