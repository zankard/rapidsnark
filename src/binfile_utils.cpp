#include <cassert>
#include <memory.h>
#include <stdexcept>
#include <string>
#include <system_error>

#include "binfile_utils.hpp"
#include "fileloader.hpp"

namespace BinFileUtils
{

BinFile::BinFile(const void* fileData, size_t fileSize,
                 std::string expected_type, uint32_t maxVersion)
{

    size = fileSize;
    addr = std::make_unique<char[]>(size);
    std::memcpy(addr.get(), fileData, size);

    assert(size >= 4);
    type.assign(addr.get(), 4);
    pos = 4;

    if (type != expected_type)
    {
        // free(addr);
        throw new std::invalid_argument("Invalid file type. It should be " +
                                        expected_type + " and it is " + type);
    }

    version = readU32LE();
    if (version > maxVersion)
    {
        // free(addr);
        throw new std::invalid_argument(
            "Invalid version. It should be <=" + std::to_string(maxVersion) +
            " and it is " + std::to_string(version));
    }

    std::uint32_t nSections = readU32LE();

    for (std::uint32_t i = 0; i < nSections; i++)
    {
        std::uint32_t sType = readU32LE();
        u_int64_t     sSize = readU64LE();

        if (sections.find(sType) == sections.end())
        {
            sections.insert(std::make_pair(sType, std::vector<Section>()));
        }

        sections[sType].push_back(Section(addr.get() + pos, sSize));

        pos += sSize;
    }

    pos            = 0;
    readingSection = nullptr;
}

// BinFile::~BinFile() { free(addr); }

void BinFile::startReadSection(std::uint32_t sectionId,
                               std::uint32_t sectionPos)
{

    if (sections.find(sectionId) == sections.end())
    {
        throw new std::range_error("Section does not exist: " +
                                   std::to_string(sectionId));
    }

    if (sectionPos >= sections[sectionId].size())
    {
        throw new std::range_error("Section pos too big. There are " +
                                   std::to_string(sections[sectionId].size()) +
                                   " and it's trying to access section: " +
                                   std::to_string(sectionPos));
    }

    if (readingSection != nullptr)
    {
        throw new std::range_error("Already reading a section");
    }

    pos = sections[sectionId][sectionPos].start - addr.get();

    readingSection = &sections[sectionId][sectionPos];
}

void BinFile::endReadSection(bool check)
{
    if (check)
    {
        if (data() + pos - readingSection->start != readingSection->size)
        {
            throw new std::range_error("Invalid section size");
        }
    }
    readingSection = nullptr;
}

void* BinFile::getSectionData(std::uint32_t sectionId, std::uint32_t sectionPos)
{

    if (sections.find(sectionId) == sections.end())
    {
        throw new std::range_error("Section does not exist: " +
                                   std::to_string(sectionId));
    }

    if (sectionPos >= sections[sectionId].size())
    {
        throw new std::range_error("Section pos too big. There are " +
                                   std::to_string(sections[sectionId].size()) +
                                   " and it's trying to access section: " +
                                   std::to_string(sectionPos));
    }

    return sections[sectionId][sectionPos].start;
}

u_int64_t BinFile::getSectionSize(std::uint32_t sectionId,
                                  std::uint32_t sectionPos)
{

    if (sections.find(sectionId) == sections.end())
    {
        throw new std::range_error("Section does not exist: " +
                                   std::to_string(sectionId));
    }

    if (sectionPos >= sections[sectionId].size())
    {
        throw new std::range_error("Section pos too big. There are " +
                                   std::to_string(sections[sectionId].size()) +
                                   " and it's trying to access section: " +
                                   std::to_string(sectionPos));
    }

    return sections[sectionId][sectionPos].size;
}

std::uint32_t BinFile::readU32LE()
{
    assert(pos + 4 <= size);
    std::uint32_t res;
    std::memcpy(&res, data() + pos, 4);
    pos += 4;
    return res;
}

u_int64_t BinFile::readU64LE()
{
    assert(pos + 8 <= size);
    u_int64_t res;
    std::memcpy(&res, data() + pos, 8);
    pos += 8;
    return res;
}

void* BinFile::read(u_int64_t len)
{
    void* res = data() + pos;
    pos += len;
    return res;
}

std::unique_ptr<BinFile> openExisting(std::string filename, std::string type,
                                      uint32_t maxVersion)
{

    FileLoader fileLoader(filename);

    return std::unique_ptr<BinFile>(new BinFile(
        fileLoader.dataBuffer(), fileLoader.dataSize(), type, maxVersion));
}

} // namespace BinFileUtils
