//
// Created by william on 2020/11/15.
//

#ifndef CROSS_PLATFORM_DEMO_FILESYSTEM_H
#define CROSS_PLATFORM_DEMO_FILESYSTEM_H

namespace GraphicEngine
{
class FileSystem
{
public:
    enum class StreamMode
    {
        READ = 1,
        WRITE = 2
    };
    enum class DialogMode
    {
        OPEN,
        SAVE
    };
    FileSystem() = default;
    ~FileSystem();

};
} // namespace GraphicEngine

#endif //CROSS_PLATFORM_DEMO_FILESYSTEM_H
