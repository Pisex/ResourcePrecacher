#pragma once

#define RESOURCE_PRECACHER_INTERFACE "IResourcePrecacher"

class IResourcePrecacher
{
public:
    virtual void AddPrecache(const char* szResource) = 0;
};