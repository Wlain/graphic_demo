//
// Created by william on 2020/11/15.
//

#ifndef CROSS_PLATFORM_DEMO_FILESYSTEM_H
#define CROSS_PLATFORM_DEMO_FILESYSTEM_H
#include "../utils/base.h"

namespace graphicEngine
{
typedef std::function<std::string(const std::string& path)> builderFunc;
class FileSystem
{
public:
    FileSystem() = default;
    ~FileSystem() = default;
    static std::string getPath(const std::string& path);

private:
    static const std::string& getRoot();
    static builderFunc getPathBuilder();
    static std::string getPathRelativeRoot(const std::string& path);
    static std::string getPathRelativeBinary(const std::string& path);
};
} // namespace graphicEngine

#endif //CROSS_PLATFORM_DEMO_FILESYSTEM_H
