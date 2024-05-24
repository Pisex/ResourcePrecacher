#ifndef _INCLUDE_METAMOD_SOURCE_STUB_PLUGIN_H_
#define _INCLUDE_METAMOD_SOURCE_STUB_PLUGIN_H_

#include <ISmmPlugin.h>
#include "utlvector.h"
#include <sh_vector.h>
#include "iserver.h"
#include <ctime>
#include "igamesystemfactory.h"
#include "sdk/module.h"


class CGameSystem : public CBaseGameSystem
{
public:
	GS_EVENT(BuildGameSessionManifest);
	
	void Shutdown() override
	{
		Msg("CGameSystem::Shutdown\n");
		delete sm_Factory;
	}

	void SetGameSystemGlobalPtrs(void *pValue) override
	{
		if (sm_Factory)
			sm_Factory->SetGlobalPtr(pValue);
	}

	bool DoesGameSystemReallocate() override
	{
		return sm_Factory->ShouldAutoAdd();
	}

	static IGameSystemFactory *sm_Factory;
};

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

extern ResourcePrecacher g_ResourcePrecacher;

PLUGIN_GLOBALVARS();

#endif //_INCLUDE_METAMOD_SOURCE_STUB_PLUGIN_H_
