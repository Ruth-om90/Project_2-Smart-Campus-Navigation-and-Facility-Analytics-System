#ifndef FACILITY_HASH_TABLE_H
#define FACILITY_HASH_TABLE_H

#include "Facility.h"

#include <cstddef>
#include <string>
#include <vector>

enum class FacilityInsertResult
{
    Inserted,
    Duplicate,
    InvalidId
};

enum class FacilityLookupStatus
{
    Found,
    NotFound,
    InvalidFormat
};

struct FacilityLookupResult
{
    FacilityLookupStatus status;
    std::string normalizedId;
    const Facility* facility;
};

class FacilityHashTable
{
public:
    static const std::size_t DEFAULT_CAPACITY = 101;

    explicit FacilityHashTable(std::size_t capacity = DEFAULT_CAPACITY);

    FacilityInsertResult insert(Facility facility);
    FacilityLookupResult lookup(const std::string& id) const;
    const Facility* find(const std::string& id) const;

    std::size_t capacity() const;
    std::size_t size() const;
    std::size_t collisionCount() const;
    const std::vector<Facility>& facilitiesInInsertionOrder() const;

    std::size_t hashIndex(const std::string& id) const;

private:
    std::vector<std::vector<std::size_t> > buckets_;
    std::vector<Facility> facilities_;
    std::size_t collisions_;
};

#endif