#ifndef FACILITY_H
#define FACILITY_H

#include <cstddef>
#include <iosfwd>
#include <string>
#include <vector>

class FacilityHashTable;

struct Facility
{
    std::string id;
    std::string name;
    std::string building;
    std::string category;
};

struct FacilityLoadSummary
{
    std::size_t accepted;
    std::size_t rejected;
};

std::string trimWhitespace(const std::string& value);
std::string normalizeFacilityId(const std::string& id);
bool isValidFacilityId(const std::string& normalizedId);
bool equalsIgnoreCase(const std::string& left, const std::string& right);

FacilityLoadSummary loadFacilities(
    const std::string& filename,
    const std::vector<std::string>& acceptedBuildings,
    FacilityHashTable& facilities,
    std::ostream& output);

FacilityLoadSummary loadFacilities(
    std::istream& input,
    const std::string& filename,
    const std::vector<std::string>& acceptedBuildings,
    FacilityHashTable& facilities,
    std::ostream& output);

void printFacilities(const FacilityHashTable& facilities, std::ostream& output);

#endif