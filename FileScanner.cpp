#include "FileScanner.h"

#include <string>

namespace
{
    bool shouldSkipDirectory(const std::filesystem::path& path)
    {
        const std::string name = path.filename().string();

        return name == ".git" || name == ".vs" || name == "build" || name == "out";
    }
}

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

    std::filesystem::recursive_directory_iterator iterator(root);
    const std::filesystem::recursive_directory_iterator end;

    while (iterator != end)
    {
        const std::filesystem::directory_entry& entry = *iterator;

        if (entry.is_directory() && shouldSkipDirectory(entry.path()))
        {
            iterator.disable_recursion_pending();
            ++iterator;
            continue;
        }

        if (entry.is_regular_file())
        {
            files.push_back(FileEntry{ entry.path(), entry.file_size() });
        }

        ++iterator;
    }

    return files;
}