#include "FileSearch.h"
#include "FileScanner.h"

#include <fstream>
#include <string>
#include <vector>

namespace
{
bool isTextLikeFile(const std::filesystem::path &path)
{
    const std::string filename = path.filename().string();
    const std::string extension = path.extension().string();

    return extension == ".cpp" || extension == ".h" || extension == ".hpp" || extension == ".txt" ||
           extension == ".md" || extension == ".json" || extension == ".cmake" ||
           extension == ".yml" || extension == ".yaml" || filename == ".gitignore";
}
} // namespace

std::vector<SearchResult> searchFilesForText(const std::vector<FileEntry> &files,
                                             const std::string &query)
{
    std::vector<SearchResult> results;

    if (query.empty())
    {
        return results;
    }

    for (const FileEntry &fileEntry : files)
    {
        if (!isTextLikeFile(fileEntry.path))
            continue;

        std::ifstream file(fileEntry.path);

        if (!file)
            continue;

        std::string lineText;

        int lineNumber = 0;

        while (getline(file, lineText))
        {
            lineNumber++;
            if (lineText.find(query) != std::string::npos)
            {
                results.push_back(SearchResult{fileEntry.path, lineNumber, lineText});
            }
        }
    }

    return results;
}