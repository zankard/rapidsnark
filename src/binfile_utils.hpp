#pragma once

#include <cstddef>
#include <cstdint>
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace BinFileUtils
{

class BinFile
{

    std::unique_ptr<char[]> addr;
    std::uint64_t           size;
    std::ptrdiff_t          pos;

    class Section
    {
        char*         start;
        std::uint64_t size;

    public:
        friend BinFile;
        Section(char* _start, std::uint64_t _size)
            : start(_start)
            , size(_size){};
    };

    std::map<int, std::vector<Section>> sections;
    std::string                         type;
    std::uint32_t                       version;

    Section* readingSection;

private:
    char* data() const { return addr.get(); }

public:
    BinFile() = delete; // No default construction

    BinFile(const void* fileData, size_t fileSize, std::string expected_type,
            uint32_t maxVersion);
    // ~BinFile();

    void* getSetcionData(std::uint32_t sectionId, std::uint32_t sectionPos = 0);

    void startReadSection(std::uint32_t sectionId, std::uint32_t setionPos = 0);
    void endReadSection(bool check = true);

    void* getSectionData(std::uint32_t sectionId, std::uint32_t sectionPos = 0);
    std::uint64_t getSectionSize(std::uint32_t sectionId,
                                 std::uint32_t sectionPos = 0);

    std::uint32_t readU32LE();
    std::uint64_t readU64LE();

    void* read(uint64_t l);
};

std::unique_ptr<BinFile> openExisting(std::string filename, std::string type,
                                      uint32_t maxVersion);
} // namespace BinFileUtils
