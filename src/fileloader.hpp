#pragma once

#include <cstddef>
#include <string>

namespace BinFileUtils
{

class FileLoader
{
public:
    FileLoader(const std::string& fileName);
    ~FileLoader();

    void*  dataBuffer() { return addr; }
    size_t dataSize() const { return size; }

private:
    void*       addr;
    std::size_t size;
    int         fd;
};

} // namespace BinFileUtils
