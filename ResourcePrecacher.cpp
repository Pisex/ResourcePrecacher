#include <stdio.h>
#include "ResourcePrecacher.h"
#include <fstream>

ResourcePrecacher g_ResourcePrecacher;

PLUGIN_EXPOSE(ResourcePrecacher, g_ResourcePrecacher);

IVEngineServer2* engine = nullptr;
ResourcePrecacherApi* g_pRSApi = nullptr;
IResourcePrecacher* g_pRSCore = nullptr;

int m_buildGameSessionManifestHookID;
std::vector<std::string> g_mapPrecache;

SH_DECL_HOOK1_void(IGameSystem, BuildGameSessionManifest, SH_NOATTRIB, false, const EventBuildGameSessionManifest_t&);

void Hook_BuildGameSessionManifest(const EventBuildGameSessionManifest_t& msg);

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

void ResourcePrecacherApi::AddPrecache(const char* szResource)
{
	g_mapPrecache.push_back(szResource);
}

bool ResourcePrecacher::Load(PluginId id, ISmmAPI *ismm, char *error, size_t maxlen, bool late)
{
	PLUGIN_SAVEVARS();
	GET_V_IFACE_CURRENT(GetFileSystemFactory, g_pFullFileSystem, IFileSystem, FILESYSTEM_INTERFACE_VERSION);
	GET_V_IFACE_CURRENT(GetServerFactory, g_pSource2Server, ISource2Server, SOURCE2SERVER_INTERFACE_VERSION);

	if(!LoadConfig())
	{
		Warning("Failed to load config\n");
		return false;
	}

	m_buildGameSessionManifestHookID = SH_ADD_DVPHOOK(
		IGameSystem, 
		BuildGameSessionManifest, 
		DynLibUtils::CModule(g_pSource2Server).GetVirtualTableByName("CEntityDebugGameSystem").RCast<IGameSystem*>(), 
		SH_STATIC(Hook_BuildGameSessionManifest), 
		true
	);
	g_SMAPI->AddListener( this, this );

	g_pRSApi = new ResourcePrecacherApi();
	g_pRSCore = g_pRSApi;

	return true;
}

void* ResourcePrecacher::OnMetamodQuery(const char* iface, int* ret)
{
	if (!strcmp(iface, RESOURCE_PRECACHER_INTERFACE))
	{
		*ret = META_IFACE_OK;
		return g_pRSCore;
	}

	*ret = META_IFACE_FAILED;
	return nullptr;
}

bool ResourcePrecacher::Unload(char *error, size_t maxlen)
{
    SH_REMOVE_HOOK_ID(m_buildGameSessionManifestHookID);
	return true;
}

void Hook_BuildGameSessionManifest(const EventBuildGameSessionManifest_t& msg)
{
    IEntityResourceManifest* pResourceManifest = msg.m_pResourceManifest;
    for (auto& it : g_mapPrecache)
    {
        pResourceManifest->AddResource(it.c_str());
    }
}

const char *ResourcePrecacher::GetLicense()
{
	return "Public";
}

const char *ResourcePrecacher::GetVersion()
{
	return "1.0.1";
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
	return "Pisex && komashchenko";
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
