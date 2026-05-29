#include "FileScanner.h"
#include "FileSearch.h"

#include <iostream>
#include <vector>

namespace
{
void printUsage()
{
    std::cout << "Usage:\n";
    std::cout << "  FileIndexerApp scan <folder_path>\n";
    std::cout << "  FileIndexerApp search <folder_path> <query>\n";
}

void printFiles(const std::vector<FileEntry> &files)
{
    std::cout << "Files found: " << files.size() << "\n";

    for (const FileEntry &file : files)
    {
        std::cout << file.path.string() << " | " << file.sizeBytes << " bytes\n";
    }
}

void printSearchResults(const std::vector<SearchResult> &results)
{
    std::cout << "Matches found: " << results.size() << "\n";

    for (const SearchResult &result : results)
    {
        std::cout << result.path.string() << ":" << result.lineNumber << " | " << result.lineText
                  << "\n";
    }
}
} // namespace

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printUsage();
        return 1;
    }

    const std::string command = argv[1];
    const std::filesystem::path root = argv[2];

    if (command == "scan")
    {
        const std::vector<FileEntry> files = scanFiles(root);
        printFiles(files);
        return 0;
    }

    if (command == "search")
    {
        if (argc < 4)
        {
            std::cout << "Missing query\n";
            return 1;
        }

        const std::string query = argv[3];

        const std::vector<FileEntry> files = scanFiles(root);
        const std::vector<SearchResult> results = searchFilesForText(files, query);

        printSearchResults(results);
        return 0;
    }

    std::cout << "Unknown command\n";
    printUsage();
    return 1;
}