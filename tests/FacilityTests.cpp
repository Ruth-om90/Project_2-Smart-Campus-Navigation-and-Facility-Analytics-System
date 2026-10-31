#include "Facility.h"
#include "FacilityHashTable.h"

#include <cassert>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

int main()
{
    FacilityHashTable table;

    Facility first = {"f001", " Room 101 ", "Library", "Library"};
    assert(table.insert(first) == FacilityInsertResult::Inserted);
    assert(table.facilitiesInInsertionOrder()[0].id == "F001");

    Facility duplicate = {"F001", "Duplicate", "Library", "Other"};
    assert(table.insert(duplicate) == FacilityInsertResult::Duplicate);
    assert(table.size() == 1);
    assert(table.collisionCount() == 0);

    Facility invalid = {"F01A", "Invalid", "Library", "Other"};
    assert(table.insert(invalid) == FacilityInsertResult::InvalidId);
    assert(table.size() == 1);

    FacilityLookupResult found = table.lookup(" f001\t");
    assert(found.status == FacilityLookupStatus::Found);
    assert(found.normalizedId == "F001");
    assert(found.facility != 0 && found.facility->name == " Room 101 ");

    FacilityLookupResult notFound = table.lookup("f999");
    assert(notFound.status == FacilityLookupStatus::NotFound);
    assert(notFound.normalizedId == "F999");

    FacilityLookupResult badFormat = table.lookup("f12");
    assert(badFormat.status == FacilityLookupStatus::InvalidFormat);

    FacilityHashTable collisionTable;
    std::string firstCollisionId;
    std::string secondCollisionId;
    for (int firstId = 0; firstId < 1000 && secondCollisionId.empty(); ++firstId)
    {
        std::ostringstream firstText;
        firstText << 'F';
        if (firstId < 100) firstText << '0';
        if (firstId < 10) firstText << '0';
        firstText << firstId;
        const std::string candidate = firstText.str();

        for (int secondId = firstId + 1; secondId < 1000; ++secondId)
        {
            std::ostringstream secondText;
            secondText << 'F';
            if (secondId < 100) secondText << '0';
            if (secondId < 10) secondText << '0';
            secondText << secondId;
            if (collisionTable.hashIndex(candidate) == collisionTable.hashIndex(secondText.str()))
            {
                firstCollisionId = candidate;
                secondCollisionId = secondText.str();
                break;
            }
        }
    }

    assert(!firstCollisionId.empty() && !secondCollisionId.empty());
    Facility collisionOne = {firstCollisionId, "First", "Library", "Library"};
    Facility collisionTwo = {secondCollisionId, "Second", "Library", "Library"};
    assert(collisionTable.insert(collisionOne) == FacilityInsertResult::Inserted);
    assert(collisionTable.insert(collisionTwo) == FacilityInsertResult::Inserted);
    assert(collisionTable.collisionCount() == 1);
    Facility duplicateCollision = {firstCollisionId, "Duplicate", "Library", "Other"};
    assert(collisionTable.insert(duplicateCollision) == FacilityInsertResult::Duplicate);
    assert(collisionTable.collisionCount() == 1);

    std::ifstream collisionInput("tests/hash-collision/data/facilities.txt");
    assert(collisionInput.is_open());
    FacilityHashTable loadedCollisionTable;
    std::ostringstream collisionOutput;
    FacilityLoadSummary collisionSummary = loadFacilities(
        collisionInput,
        "tests/hash-collision/data/facilities.txt",
        std::vector<std::string>(1, "Library"),
        loadedCollisionTable,
        collisionOutput);
    assert(collisionSummary.accepted == 2 && collisionSummary.rejected == 1);
    assert(loadedCollisionTable.collisionCount() == 1);
    assert(collisionOutput.str().find(
        "capacity 101, entries 2, collisions 1") != std::string::npos);

    std::istringstream input(
        "# facility records\r\n"
        "\r\n"
        " f010 | Room #5 | library | Library \r\n"
        "F011|Reading Room|LIBRARY|library\r\n"
        "f010|Duplicate|Library|Duplicate Category\r\n"
        "F012|Too Few Fields|Library\r\n"
        "F013||Library|Other\r\n"
        "F01X|Bad ID|Library|Other\r\n"
        "F014|Unknown Place|Unknown Building|Other\r\n");
    FacilityHashTable loaded;
    std::ostringstream output;
    const std::vector<std::string> buildings(1, "Library");
    FacilityLoadSummary summary = loadFacilities(
        input, "facilities.txt", buildings, loaded, output);

    assert(summary.accepted == 2);
    assert(summary.rejected == 5);
    assert(loaded.size() == 2);
    assert(loaded.facilitiesInInsertionOrder()[0].id == "F010");
    assert(loaded.facilitiesInInsertionOrder()[0].name == "Room #5");
    assert(loaded.facilitiesInInsertionOrder()[0].building == "Library");
    assert(loaded.facilitiesInInsertionOrder()[1].category == "Library");
    assert(output.str().find("facilities.txt:5: duplicate facility ID 'F010'") != std::string::npos);
    assert(output.str().find("facilities.txt:6: expected exactly four fields") != std::string::npos);
    assert(output.str().find("2 accepted, 5 rejected") != std::string::npos);

    std::ostringstream display;
    printFacilities(loaded, display);
    assert(display.str().find("Total facilities: 2") != std::string::npos);
}
