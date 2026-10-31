#include "FacilityHashTable.h"

#include <stdexcept>

FacilityHashTable::FacilityHashTable(std::size_t capacity)
    : buckets_(capacity), collisions_(0)
{
    if (capacity == 0)
    {
        throw std::invalid_argument("Facility hash table capacity must be greater than zero.");
    }
}

FacilityInsertResult FacilityHashTable::insert(Facility facility)
{
    facility.id = normalizeFacilityId(facility.id);
    if (!isValidFacilityId(facility.id))
    {
        return FacilityInsertResult::InvalidId;
    }

    const std::size_t index = hashIndex(facility.id);
    std::vector<std::size_t>& bucket = buckets_[index];
    for (std::size_t i = 0; i < bucket.size(); ++i)
    {
        if (facilities_[bucket[i]].id == facility.id)
        {
            return FacilityInsertResult::Duplicate;
        }
    }

    if (!bucket.empty())
    {
        ++collisions_;
    }

    facilities_.push_back(facility);
    bucket.push_back(facilities_.size() - 1);
    return FacilityInsertResult::Inserted;
}

FacilityLookupResult FacilityHashTable::lookup(const std::string& id) const
{
    const std::string normalizedId = normalizeFacilityId(id);
    if (!isValidFacilityId(normalizedId))
    {
        return FacilityLookupResult{FacilityLookupStatus::InvalidFormat, normalizedId, 0};
    }

    const std::vector<std::size_t>& bucket = buckets_[hashIndex(normalizedId)];
    for (std::size_t i = 0; i < bucket.size(); ++i)
    {
        const Facility& facility = facilities_[bucket[i]];
        if (facility.id == normalizedId)
        {
            return FacilityLookupResult{FacilityLookupStatus::Found, normalizedId, &facility};
        }
    }

    return FacilityLookupResult{FacilityLookupStatus::NotFound, normalizedId, 0};
}

const Facility* FacilityHashTable::find(const std::string& id) const
{
    const FacilityLookupResult result = lookup(id);
    return result.status == FacilityLookupStatus::Found ? result.facility : 0;
}

std::size_t FacilityHashTable::capacity() const
{
    return buckets_.size();
}

std::size_t FacilityHashTable::size() const
{
    return facilities_.size();
}

std::size_t FacilityHashTable::collisionCount() const
{
    return collisions_;
}

const std::vector<Facility>& FacilityHashTable::facilitiesInInsertionOrder() const
{
    return facilities_;
}

std::size_t FacilityHashTable::hashIndex(const std::string& id) const
{
    const std::string normalizedId = normalizeFacilityId(id);
    std::size_t hash = 0;
    for (std::size_t i = 0; i < normalizedId.size(); ++i)
    {
        hash = (hash * 31 + static_cast<unsigned char>(normalizedId[i])) % buckets_.size();
    }
    return hash;
}