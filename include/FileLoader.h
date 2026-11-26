/*
 * File: FileLoader.h
 * Purpose: Shared file path and text-line utilities for project data loading.
 * Author: Shared team work
 */

#ifndef FILELOADER_H
#define FILELOADER_H

#include <cstddef>
#include <iosfwd>
#include <string>
#include <vector>

namespace FileLoader
{
std::string resolveDataDirectory(int argc, char* argv[]);
std::string dataFilePath(const std::string& dataDirectory,
                         const std::string& filename);

bool openTextFile(const std::string& filename,
                  std::ifstream& input,
                  std::ostream& output);

std::string trimWhitespace(const std::string& value);
bool isBlankOrComment(const std::string& line);
std::vector<std::string> splitFields(const std::string& line,
                                     char delimiter = '|');
}

#endif
