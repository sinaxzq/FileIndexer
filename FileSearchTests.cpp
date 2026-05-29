#include "FileSearch.h"

#include <cassert>
#include <filesystem>
#include <fstream>
#include <vector>

void testSearchFindsMatchingLine()
{
    const std::filesystem::path testDirectory = "file_search_test_dir";
    const std::filesystem::path testFile = testDirectory / "notes.txt";

    std::filesystem::remove_all(testDirectory);
    std::filesystem::create_directory(testDirectory);

    {
        std::ofstream file(testFile);
        file << "first line\n";
        file << "hello C++ search\n";
        file << "last line\n";
    }

    const std::vector<FileEntry> files = scanFiles(testDirectory);
    const std::vector<SearchResult> results = searchFilesForText(files, "C++");

    assert(results.size() == 1);
    assert(results[0].path.filename() == "notes.txt");
    assert(results[0].lineNumber == 2);
    assert(results[0].lineText == "hello C++ search");

    std::filesystem::remove_all(testDirectory);
}

void testSearchWithMissingQueryReturnsEmpty()
{
    const std::filesystem::path testDirectory = "file_search_test_dir";
    const std::filesystem::path testFile = testDirectory / "notes.txt";

    std::filesystem::remove_all(testDirectory);
    std::filesystem::create_directory(testDirectory);

    {
        std::ofstream file(testFile);
        file << "hello world\n";
    }

    const std::vector<FileEntry> files = scanFiles(testDirectory);
    const std::vector<SearchResult> results = searchFilesForText(files, "missing");

    assert(results.empty());

    std::filesystem::remove_all(testDirectory);
}

void testSearchWithEmptyQueryReturnsEmpty()
{
    const std::filesystem::path testDirectory = "file_search_test_dir";
    const std::filesystem::path testFile = testDirectory / "notes.txt";

    std::filesystem::remove_all(testDirectory);
    std::filesystem::create_directory(testDirectory);

    {
        std::ofstream file(testFile);
        file << "hello world\n";
    }

    const std::vector<FileEntry> files = scanFiles(testDirectory);
    const std::vector<SearchResult> results = searchFilesForText(files, "");

    assert(results.empty());

    std::filesystem::remove_all(testDirectory);
}

int main()
{
    testSearchFindsMatchingLine();
    testSearchWithMissingQueryReturnsEmpty();
    testSearchWithEmptyQueryReturnsEmpty();

    return 0;
}