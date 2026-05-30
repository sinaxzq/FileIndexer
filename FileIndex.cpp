#include "FileIndex.h"
#include "FileScanner.h"
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>

bool isAsciiLetterOrDigit(char ch)
{
    return (ch >= '0' && ch <= '9') || (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
}

char toLowerAscii(char ch)
{
    if (ch >= 'A' && ch <= 'Z')
    {
        return ch - 'A' + 'a';
    }

    return ch;
}

std::vector<std::string> tokenizeLine(const std::string &line)
{

    std::vector<std::string> words;
    std::string currentWord;

    for (char ch : line)
    {
        if (isAsciiLetterOrDigit(ch))
        {
            currentWord.push_back(toLowerAscii(ch));
        }
        else if (!currentWord.empty())
        {
            words.push_back(currentWord);
            currentWord.clear();
        }
    }

    if (!currentWord.empty())
    {
        words.push_back(currentWord);
    }

    return words;
}

FileIndex buildIndex(const std::vector<FileEntry> &files)
{
    FileIndex index;

    for (const FileEntry &fileEntry : files)
    {
        std::ifstream file(fileEntry.path);

        if (!file)
            continue;

        int lineNumber = 0;
        std::string line;

        while (std::getline(file, line))
        {
            lineNumber++;
            const std::vector<std::string> words = tokenizeLine(line);
            for (const auto &word : words)
            {
                index.words[word].push_back({fileEntry.path, lineNumber});
            }
        }
    }

    return index;
}

std::vector<RankedFileResult> searchIndex(const FileIndex &index, const std::string &query)
{
    std::vector<RankedFileResult> results;

    std::vector<std::string> words = tokenizeLine(query);

    if (words.empty())
        return results;

    std::unordered_map<std::string, int> matchCounts;

    for (const auto &word : words)
    {
        const auto it = index.words.find(word);

        if (it == index.words.end())
            continue;

        for (const IndexedOccurrence &occurence : it->second)
        {
            ++matchCounts[occurence.path.string()];
        }
    }

    for (const auto &[pathText, count] : matchCounts)
    {
        results.push_back(RankedFileResult{pathText, count});
    }

    std::sort(results.begin(), results.end(),
              [](const RankedFileResult &left, const RankedFileResult &right) {
                  return left.matchCount > right.matchCount;
              });

    return results;
}

std::vector<WordFrequency> getTopWords(const FileIndex &index, int limit)
{
    std::vector<WordFrequency> frequencies;

    if (limit <= 0)
        return frequencies;

    for (const auto &[word, occurrences] : index.words)
    {
        frequencies.push_back(WordFrequency{word, static_cast<int>(occurrences.size())});
    }

    std::sort(frequencies.begin(), frequencies.end(),
              [](const WordFrequency &left, const WordFrequency &right) {
                  return left.count > right.count;
              });

    if (frequencies.size() > static_cast<std::size_t>(limit))
    {
        frequencies.resize(static_cast<std::size_t>(limit));
    }

    return frequencies;
}