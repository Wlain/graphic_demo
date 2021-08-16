//
// Created by william on 2021/8/15.
//

#ifndef CROSS_PLATFORM_DEMO_RESOURCECACHE_H
#define CROSS_PLATFORM_DEMO_RESOURCECACHE_H

class Device;
class ImageView;

class ResourceCache
{
public:
    ResourceCache(Device &device);
    void clear();
private:
    Device &m_device;
};

#endif //CROSS_PLATFORM_DEMO_RESOURCECACHE_H
