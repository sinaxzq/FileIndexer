#include "FileScanner.h"

#include <cassert>
#include <filesystem>
#include <fstream>
#include <vector>

void testScanMissingDirectoryReturnsEmpty()
{
    const std::vector<FileEntry> files = scanFiles("definitely_missing_directory_12345");

    assert(files.empty());
}

void testScanDirectoryFindsRegularFile()
{
    const std::filesystem::path testDirectory = "file_scanner_test_dir";
    const std::filesystem::path testFile = testDirectory / "note.txt";

    std::filesystem::remove_all(testDirectory);
    std::filesystem::create_directory(testDirectory);

    {
        std::ofstream file(testFile);
        file << "hello";
    }

    const std::vector<FileEntry> files = scanFiles(testDirectory);

    assert(files.size() == 1);
    assert(files[0].path.filename() == "note.txt");
    assert(files[0].sizeBytes == 5);

    std::filesystem::remove_all(testDirectory);
}

int main()
{
    testScanMissingDirectoryReturnsEmpty();
    testScanDirectoryFindsRegularFile();

    return 0;
}