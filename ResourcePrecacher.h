#ifndef _INCLUDE_METAMOD_SOURCE_STUB_PLUGIN_H_
#define _INCLUDE_METAMOD_SOURCE_STUB_PLUGIN_H_

#include <ISmmPlugin.h>
#include "utlvector.h"
#include <sh_vector.h>
#include "iserver.h"
#include <ctime>
#include "igamesystemfactory.h"
#include "sourcehook.h"
#include "include/resource_precacher.h"
#include "module.h"

class IEntityResourceManifest
{
public:
	virtual void AddResource(const char*) = 0;
	virtual void AddResource(const char*, void*) = 0;
	virtual void AddResource(const char*, void*, void*, void*) = 0;
	virtual void unk_04() = 0;
	virtual void unk_05() = 0;
	virtual void unk_06() = 0;
	virtual void unk_07() = 0;
	virtual void unk_08() = 0;
	virtual void unk_09() = 0;
	virtual void unk_10() = 0;
};

class ResourcePrecacher : public ISmmPlugin, public IMetamodListener
{
public:
	bool Load(PluginId id, ISmmAPI *ismm, char *error, size_t maxlen, bool late);
	bool Unload(char *error, size_t maxlen);
	void* OnMetamodQuery(const char* iface, int* ret);
public:
	const char *GetAuthor();
	const char *GetName();
	const char *GetDescription();
	const char *GetURL();
	const char *GetLicense();
	const char *GetVersion();
	const char *GetDate();
	const char *GetLogTag();
};

class ResourcePrecacherApi : public IResourcePrecacher
{
public:
	void AddPrecache(const char* szResource);
};

extern ResourcePrecacher g_ResourcePrecacher;

PLUGIN_GLOBALVARS();

#endif //_INCLUDE_METAMOD_SOURCE_STUB_PLUGIN_H_
