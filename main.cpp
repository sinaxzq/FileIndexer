#include "FileScanner.h"

#include <iostream>
#include <vector>

int main(int argc , char* argv[])
{
    if (argc < 2)
    {
        std::cout << "Usage:\n";
        std::cout << "  FileIndexerApp <folder_path>\n";
        return 1;
    }

    const std::filesystem::path root = argv[1];

    const std::vector<FileEntry> files = scanFiles(root);

    std::cout << "Files found: " << files.size() << "\n";

    for (const FileEntry& file : files)
    {
        std::cout << file.path.string() << " | " << file.sizeBytes << " bytes\n";
    }

    return 0;
}