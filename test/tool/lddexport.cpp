/**
 * The MIT License
 *
 * Copyright 2022 Krishna sssky307@163.com
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

/**
 * @file lddexport.cpp
 * 
 * @brief Export dll files from ldd.
 * 
 * ldd is a tool used to print shared library dependencies,
 * and this tool is used to export dll files from ldd's output.
 */

#include <iostream>
#include <string>
#include <set>
#include <regex>
#include <filesystem>

using FileAndFilePath = std::pair<std::string, std::string>;
using FileAndFilePathSet = std::set<FileAndFilePath>;

FileAndFilePath GetFileAndFilePath(const std::string& line)
{
    static const std::regex re("[ \t]*([^ \t]+)[ \t]*=>[ \t]*([^ \t]+).*");
    std::smatch match;

    if (std::regex_match(line, match, re))
    {
        return {match[1].str(), match[2].str()};
    }

    return {};
}

FileAndFilePathSet GetFileAndFilePathSet()
{
    FileAndFilePathSet fileAndFilePathSet;
    std::string line;
    while(std::getline(std::cin, line))
    {
        fileAndFilePathSet.insert(GetFileAndFilePath(line));
    }
    return fileAndFilePathSet;
}

void SaveFileSet(const FileAndFilePathSet& fileAndFilePathSet, std::string newPath)
{
    if(fileAndFilePathSet.empty()) return;
    if(!newPath.empty() && newPath.back() == '/') newPath.pop_back();
    if(newPath.empty()) return;

    if(newPath != "." || newPath != ".." || newPath != "/")
    {
        std::filesystem::create_directories(newPath);  
    }

    for(auto& pair: fileAndFilePathSet)
    {
        //std::filesystem::copy_file(pair.second, newPath + "/" + pair.first);
        std::string cmd ="cp " + pair.second + " " + newPath + "/" + pair.first;
        std::system(cmd.c_str());
    }
}

void work(const std::string& newPath)
{
    SaveFileSet(GetFileAndFilePathSet(), newPath);
}

int ShowHelp()
{
    static const char* help = R"(lddexport --out <directory>

Pipe in the output of ldd, export dlls to the directory.
This tool is used in MSYS2 environment.

Example:
  ldd foo.exe | lddexport --out dlls
)";

    std::cout << help;

    return 0;
}

int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        return ShowHelp();
    }
    else if(std::string("--out") == argv[1] 
            || std::string("-out") == argv[1] 
            || std::string("-o") == argv[1])
    {
        work(argv[2]);
    }

    return 0;
}