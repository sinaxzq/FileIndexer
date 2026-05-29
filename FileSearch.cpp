#include "FileSearch.h"
#include "FileScanner.h"

#include <fstream>
#include <string>
#include <vector>


std::vector<SearchResult> searchFilesForText(const std::vector<FileEntry>& files ,
                                             const std::string& query)
{
    std::vector<SearchResult> results;
    
    if (query.empty())
    {
        return results;
    }

    for (const FileEntry& fileEntry : files)
    {
        std::ifstream file(fileEntry.path);
            
        if (!file) continue;

        std::string lineText;

        int lineNumber = 0;

        while (getline(file , lineText))
        {
            lineNumber++;
            if (lineText.find(query) != std::string::npos)
            {
                results.push_back(SearchResult{ fileEntry.path, lineNumber, lineText });
            }
        }
    }

    return results;
}