#include "FileScanner.h"

#include <filesystem>
#include <vector>
#include <string>

std::vector<FileEntry> scanFiles(const std::filesystem::path& root)
{
    std::vector<FileEntry> files;

    if (!std::filesystem::exists(root))
    {
        return files;
    }

    if (!std::filesystem::is_directory(root))
    {
        return files;
    }

    for (const std::filesystem::directory_entry& entry :
         std::filesystem::recursive_directory_iterator(root))
    {
        if (entry.is_regular_file())
        {
            FileEntry file;
            file.path = entry.path();
            file.sizeBytes = entry.file_size();

            files.push_back(file);
        }
    }

    return files;
}

