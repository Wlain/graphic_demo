//
// Created by william on 2020/11/15.
//

#include "fileSystem.h"

namespace GraphicEngine
{
std::string FileSystem::getPath(const std::string& path)
{
    static builderFunc pathBuilder = getPathBuilder();
    return pathBuilder(path);
}

const std::string& FileSystem::getRoot()
{
    static std::string root("/Users/william/git/demo/graphic_demo/cross_platform_demo");
    return root;
}

builderFunc FileSystem::getPathBuilder()
{
    return (!getRoot().empty()) ? (&getPathRelativeRoot) : (&getPathRelativeBinary);
}
std::string FileSystem::getPathRelativeRoot(const std::string& path)
{
    return getRoot() + std::string("/") + path;
}
std::string FileSystem::getPathRelativeBinary(const std::string& path)
{
    return "../../" + path;
}
} // namespace GraphicEngine
