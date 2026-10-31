#include "Facility.h"

#include "FacilityHashTable.h"

#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>

namespace
{
char toUpperAscii(char character)
{
    if (character >= 'a' && character <= 'z')
    {
        return static_cast<char>(character - 'a' + 'A');
    }
    return character;
}

char toLowerAscii(char character)
{
    if (character >= 'A' && character <= 'Z')
    {
        return static_cast<char>(character - 'A' + 'a');
    }
    return character;
}

std::vector<std::string> splitFields(const std::string& line)
{
    std::vector<std::string> fields;
    std::string::size_type start = 0;
    while (true)
    {
        const std::string::size_type delimiter = line.find('|', start);
        if (delimiter == std::string::npos)
        {
            fields.push_back(trimWhitespace(line.substr(start)));
            break;
        }
        fields.push_back(trimWhitespace(line.substr(start, delimiter - start)));
        start = delimiter + 1;
    }
    return fields;
}

std::string canonicalBuilding(
    const std::string& buildingName,
    const std::vector<std::string>& acceptedBuildings)
{
    for (std::size_t i = 0; i < acceptedBuildings.size(); ++i)
    {
        if (equalsIgnoreCase(buildingName, acceptedBuildings[i]))
        {
            return acceptedBuildings[i];
        }
    }
    return std::string();
}

std::string canonicalCategory(
    const std::string& category,
    const FacilityHashTable& facilities)
{
    const std::vector<Facility>& accepted = facilities.facilitiesInInsertionOrder();
    for (std::size_t i = 0; i < accepted.size(); ++i)
    {
        if (equalsIgnoreCase(category, accepted[i].category))
        {
            return accepted[i].category;
        }
    }
    return category;
}

void printFacility(const Facility& facility, std::ostream& output)
{
    output << "ID: " << facility.id
           << " | Name: " << facility.name
           << " | Building: " << facility.building
           << " | Category: " << facility.category << '\n';
}

void printLoadSummary(
    const std::string& filename,
    const FacilityLoadSummary& summary,
    const FacilityHashTable& facilities,
    std::ostream& output)
{
    output << filename << ": " << summary.accepted << " accepted, "
           << summary.rejected << " rejected; hash table capacity "
           << facilities.capacity() << ", entries " << facilities.size()
           << ", collisions " << facilities.collisionCount() << ".\n";
}
}

std::string trimWhitespace(const std::string& value)
{
    std::string::size_type first = 0;
    while (first < value.size() &&
           std::isspace(static_cast<unsigned char>(value[first])) != 0)
    {
        ++first;
    }

    std::string::size_type last = value.size();
    while (last > first &&
           std::isspace(static_cast<unsigned char>(value[last - 1])) != 0)
    {
        --last;
    }

    return value.substr(first, last - first);
}

std::string normalizeFacilityId(const std::string& id)
{
    std::string normalized = trimWhitespace(id);
    for (std::size_t i = 0; i < normalized.size(); ++i)
    {
        normalized[i] = toUpperAscii(normalized[i]);
    }
    return normalized;
}

bool isValidFacilityId(const std::string& normalizedId)
{
    if (normalizedId.size() != 4 || normalizedId[0] != 'F')
    {
        return false;
    }

    for (std::size_t i = 1; i < normalizedId.size(); ++i)
    {
        if (normalizedId[i] < '0' || normalizedId[i] > '9')
        {
            return false;
        }
    }
    return true;
}

bool equalsIgnoreCase(const std::string& left, const std::string& right)
{
    const std::string trimmedLeft = trimWhitespace(left);
    const std::string trimmedRight = trimWhitespace(right);
    if (trimmedLeft.size() != trimmedRight.size())
    {
        return false;
    }

    for (std::size_t i = 0; i < trimmedLeft.size(); ++i)
    {
        if (toLowerAscii(trimmedLeft[i]) != toLowerAscii(trimmedRight[i]))
        {
            return false;
        }
    }
    return true;
}

FacilityLoadSummary loadFacilities(
    const std::string& filename,
    const std::vector<std::string>& acceptedBuildings,
    FacilityHashTable& facilities,
    std::ostream& output)
{
    std::ifstream input(filename.c_str());
    if (!input.is_open())
    {
        FacilityLoadSummary summary = {0, 0};
        output << "Error: could not open file '" << filename << "'.\n";
        printLoadSummary(filename, summary, facilities, output);
        return summary;
    }

    return loadFacilities(input, filename, acceptedBuildings, facilities, output);
}

FacilityLoadSummary loadFacilities(
    std::istream& input,
    const std::string& filename,
    const std::vector<std::string>& acceptedBuildings,
    FacilityHashTable& facilities,
    std::ostream& output)
{
    FacilityLoadSummary summary = {0, 0};
    std::string line;
    std::size_t lineNumber = 0;

    while (std::getline(input, line))
    {
        ++lineNumber;
        const std::string trimmedLine = trimWhitespace(line);
        if (trimmedLine.empty() || trimmedLine[0] == '#')
        {
            continue;
        }

        const std::vector<std::string> fields = splitFields(line);
        std::string reason;
        if (fields.size() != 4)
        {
            reason = "expected exactly four fields";
        }
        else if (fields[0].empty() || fields[1].empty() ||
                 fields[2].empty() || fields[3].empty())
        {
            reason = "required field is empty";
        }

        const std::string normalizedId = fields.empty()
            ? std::string()
            : normalizeFacilityId(fields[0]);
        if (reason.empty() && !isValidFacilityId(normalizedId))
        {
            reason = "facility ID must be F followed by exactly three digits";
        }

        std::string building;
        if (reason.empty())
        {
            building = canonicalBuilding(fields[2], acceptedBuildings);
            if (building.empty())
            {
                reason = "unknown building '" + fields[2] + "'";
            }
        }

        Facility facility;
        if (reason.empty())
        {
            facility.id = normalizedId;
            facility.name = fields[1];
            facility.building = building;
            facility.category = canonicalCategory(fields[3], facilities);

            const FacilityInsertResult result = facilities.insert(facility);
            if (result == FacilityInsertResult::Duplicate)
            {
                reason = "duplicate facility ID '" + normalizedId + "'";
            }
            else if (result == FacilityInsertResult::InvalidId)
            {
                reason = "facility ID must be F followed by exactly three digits";
            }
        }

        if (!reason.empty())
        {
            ++summary.rejected;
            output << "Warning: " << filename << ':' << lineNumber
                   << ": " << reason << ".\n";
        }
        else
        {
            ++summary.accepted;
        }
    }

    printLoadSummary(filename, summary, facilities, output);
    return summary;
}

void printFacilities(const FacilityHashTable& facilities, std::ostream& output)
{
    const std::vector<Facility>& accepted = facilities.facilitiesInInsertionOrder();
    if (accepted.empty())
    {
        output << "No facilities loaded.\nTotal facilities: 0\n";
        return;
    }

    for (std::size_t i = 0; i < accepted.size(); ++i)
    {
        printFacility(accepted[i], output);
    }
    output << "Total facilities: " << accepted.size() << '\n';
}