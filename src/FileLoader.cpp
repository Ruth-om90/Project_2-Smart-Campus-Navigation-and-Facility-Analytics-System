/*
 * File: FileLoader.cpp
 * Purpose: Implements shared file path and text-line utilities.
 * Author: Shared team work
 */

#include "FileLoader.h"

#include <cctype>
#include <fstream>
#include <iostream>

namespace FileLoader
{
std::string resolveDataDirectory(int argc, char* argv[])
{
    if (argc > 1 && argv[1] != 0)
    {
        std::string directory(argv[1]);
        if (!directory.empty() && directory[directory.size() - 1] != '/' &&
            directory[directory.size() - 1] != '\\')
        {
            directory += '/';
        }
        return directory;
    }
    return "data/";
}

std::string dataFilePath(const std::string& dataDirectory,
                         const std::string& filename)
{
    std::string directory = dataDirectory;
    if (!directory.empty() && directory[directory.size() - 1] != '/' &&
        directory[directory.size() - 1] != '\\')
    {
        directory += '/';
    }
    return directory + filename;
}

bool openTextFile(const std::string& filename,
                  std::ifstream& input,
                  std::ostream& output)
{
    input.open(filename.c_str());
    if (!input.is_open())
    {
        output << "Error: could not open file '" << filename << "'. Treating it as empty.\n";
        return false;
    }
    return true;
}

std::string trimWhitespace(const std::string& value)
{
    std::size_t first = 0;
    while (first < value.size() &&
           std::isspace(static_cast<unsigned char>(value[first]) ))
    {
        ++first;
    }

    std::size_t last = value.size();
    while (last > first &&
           std::isspace(static_cast<unsigned char>(value[last - 1])))
    {
        --last;
    }

    return value.substr(first, last - first);
}

bool isBlankOrComment(const std::string& line)
{
    std::string trimmed = trimWhitespace(line);
    return trimmed.empty() || trimmed[0] == '#';
}

std::vector<std::string> splitFields(const std::string& line,
                                     char delimiter)
{
    std::vector<std::string> fields;
    std::size_t start = 0;

    while (true)
    {
        std::size_t position = line.find(delimiter, start);
        if (position == std::string::npos)
        {
            fields.push_back(trimWhitespace(line.substr(start)));
            break;
        }

        fields.push_back(trimWhitespace(line.substr(start, position - start)));
        start = position + 1;
    }

    return fields;
}
}
