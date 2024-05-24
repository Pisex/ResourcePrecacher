#include <stdio.h>
#include "ResourcePrecacher.h"
#include <fstream>

ResourcePrecacher g_ResourcePrecacher;

PLUGIN_EXPOSE(ResourcePrecacher, g_ResourcePrecacher);

std::vector<std::string> g_mapPrecache;

CBaseGameSystemFactory **CBaseGameSystemFactory::sm_pFirst = nullptr;

CGameSystem g_GameSystem;
IGameSystemFactory *CGameSystem::sm_Factory = nullptr;

bool InitGameSystems()
{
	DynLibUtils::CModule libserver("server");

	void* UTIL_InitGameSystems = libserver.FindPattern("4C 8B 35 ? ? ? ? 4D 85 F6 75 2D E9 ? ? ? ? 0F 1F 40 00 48 8B 05").RCast< decltype(UTIL_InitGameSystems) >();
	uint8 *ptr = (uint8*)UTIL_InitGameSystems + 3;
	uint32 offset = *(uint32*)ptr;
	CBaseGameSystemFactory::sm_pFirst = (CBaseGameSystemFactory **)(ptr + 4 + offset);
	CGameSystem::sm_Factory = new CGameSystemStaticFactory<CGameSystem>("ResourcePrecacher_GameSystem", &g_GameSystem);
	return true;
}

GS_EVENT_MEMBER(CGameSystem, BuildGameSessionManifest)
{
	IEntityResourceManifest *pResourceManifest = msg->m_pResourceManifest;
	for (auto& it : g_mapPrecache)
	{
		pResourceManifest->AddResource(it.c_str());
	}
}

bool LoadConfig()
{
	g_mapPrecache.clear();
	KeyValues* g_kvSettings = new KeyValues("Models");
	const char *pszPath = "addons/configs/resource_precacher.ini";

	if (!g_kvSettings->LoadFromFile(g_pFullFileSystem, pszPath))
	{
		Warning("Failed to load %s\n", pszPath);
		return false;
	}
	FOR_EACH_VALUE(g_kvSettings, pValue)
	{
		g_mapPrecache.push_back(pValue->GetString(nullptr, nullptr));
	}
	return true;
}

bool ResourcePrecacher::Load(PluginId id, ISmmAPI *ismm, char *error, size_t maxlen, bool late)
{
	PLUGIN_SAVEVARS();
	GET_V_IFACE_CURRENT(GetFileSystemFactory, g_pFullFileSystem, IFileSystem, FILESYSTEM_INTERFACE_VERSION);

	if(!LoadConfig())
	{
		Warning("Failed to load config\n");
		return false;
	}

	if (!InitGameSystems()) {
		Warning("Failed to initialize game systems\n");
		return false;
	}
	g_SMAPI->AddListener( this, this );
	return true;
}

bool ResourcePrecacher::Unload(char *error, size_t maxlen)
{
	return true;
}

const char *ResourcePrecacher::GetLicense()
{
	return "Public";
}

const char *ResourcePrecacher::GetVersion()
{
	return "1.0";
}

const char *ResourcePrecacher::GetDate()
{
	return __DATE__;
}

const char *ResourcePrecacher::GetLogTag()
{
	return "[RP]";
}

const char *ResourcePrecacher::GetAuthor()
{
	return "Pisex";
}

const char *ResourcePrecacher::GetDescription()
{
	return "";
}

const char *ResourcePrecacher::GetName()
{
	return "ResourcePrecacher";
}

const char *ResourcePrecacher::GetURL()
{
	return "https://discord.com/invite/g798xERK5Y";
}
