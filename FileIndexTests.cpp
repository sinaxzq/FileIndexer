#include "FileIndex.h"

#include "FileScanner.h"
#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

void testTokenizeSimpleWords()
{
    const std::vector<std::string> words = tokenizeLine("hello world");

    assert(words.size() == 2);
    assert(words[0] == "hello");
    assert(words[1] == "world");
}

void testTokenizeLowercasesWords()
{
    const std::vector<std::string> words = tokenizeLine("Hello SEARCH");

    assert(words.size() == 2);
    assert(words[0] == "hello");
    assert(words[1] == "search");
}

void testTokenizeSplitsOnPunctuation()
{
    const std::vector<std::string> words = tokenizeLine("Hello, C++ search!");

    assert(words.size() == 3);
    assert(words[0] == "hello");
    assert(words[1] == "c");
    assert(words[2] == "search");
}

void testTokenizeKeepsDigits()
{
    const std::vector<std::string> words = tokenizeLine("file123 version 2");

    assert(words.size() == 3);
    assert(words[0] == "file123");
    assert(words[1] == "version");
    assert(words[2] == "2");
}

void testTokenizeEmptyLine()
{
    const std::vector<std::string> words = tokenizeLine("");

    assert(words.empty());
}

void testBuildIndexStoresWordOccurrences()
{
    const std::filesystem::path testDirectory = "file_index_test_dir";
    const std::filesystem::path testFile = testDirectory / "notes.txt";

    std::filesystem::remove_all(testDirectory);
    std::filesystem::create_directory(testDirectory);

    {
        std::ofstream file(testFile);
        file << "hello search\n";
        file << "search again\n";
    }

    const std::vector<FileEntry> files = scanFiles(testDirectory);
    const FileIndex index = buildIndex(files);

    assert(index.words.at("hello").size() == 1);
    assert(index.words.at("search").size() == 2);
    assert(index.words.at("again").size() == 1);

    assert(index.words.at("search")[0].lineNumber == 1);
    assert(index.words.at("search")[1].lineNumber == 2);

    std::filesystem::remove_all(testDirectory);
}

void testSearchIndexSupportsMultipleQueryWords()
{
    FileIndex index;

    index.words["search"].push_back({"FileA.cpp", 1});
    index.words["search"].push_back({"FileA.cpp", 3});
    index.words["search"].push_back({"FileB.cpp", 2});

    index.words["file"].push_back({"FileB.cpp", 4});
    index.words["file"].push_back({"FileC.cpp", 5});

    const std::vector<RankedFileResult> results = searchIndex(index, "search file");

    assert(results.size() == 3);

    assert(results[0].matchCount == 2);
    assert(results[1].matchCount == 2);
    assert(results[2].matchCount == 1);
}

void testSearchIndexSkipsMissingQueryWords()
{
    FileIndex index;

    index.words["search"].push_back({"FileA.cpp", 1});
    index.words["search"].push_back({"FileA.cpp", 3});
    index.words["search"].push_back({"FileB.cpp", 2});

    index.words["file"].push_back({"FileB.cpp", 4});
    index.words["file"].push_back({"FileC.cpp", 5});

    const std::vector<RankedFileResult> results = searchIndex(index, "missing search file");

    assert(results.size() == 3);

    assert(results[0].matchCount == 2);
    assert(results[1].matchCount == 2);
    assert(results[2].matchCount == 1);
}

int main()
{
    testTokenizeSimpleWords();
    testTokenizeLowercasesWords();
    testTokenizeSplitsOnPunctuation();
    testTokenizeKeepsDigits();
    testTokenizeEmptyLine();
    testBuildIndexStoresWordOccurrences();
    testSearchIndexSupportsMultipleQueryWords();
    testSearchIndexSkipsMissingQueryWords();

    return 0;
}