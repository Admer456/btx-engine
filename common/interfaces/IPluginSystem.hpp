
#pragma once

class IPlugin;

struct PluginMetadata
{
	String pluginName;
	String implementedInterface;
};

struct PluginLibraryMetadata
{
	String name;
	String author;
	String description;
	int version;
	String versionDate;
	Vector<PluginMetadata> pluginInfo;
};

template<typename pluginType>
using PluginList = LinkedList<pluginType>;

class IPluginSystem
{
public:
	virtual bool Init() = 0;
	virtual void Shutdown() = 0;

	// Reads plugin metadata and loads a plugin DLL
	// One plugin library may load multiple plugins
	virtual int LoadPluginLibrary( Path libraryPath ) = 0;

	// Gets plugin library info
	virtual const PluginLibraryMetadata& GetPluginLibraryMetadata( const int& libraryHandle ) const = 0;
	virtual const PluginLibraryMetadata& GetPluginLibraryMetadata( const IPlugin* plugin ) const = 0;

	// Shuts down the plugin and unloads the DLL
	virtual void UnloadPluginLibrary( const int& libraryHandle ) = 0;

	// Gets a plugin instance by name
	template<typename pluginType>
	pluginType* GetPluginByName( StringView pluginName, int minimumVersion ) const
	{
		return dynamic_cast<pluginType*>( GetPluginByNameRaw( pluginName, minimumVersion ) );
	}

	virtual IPlugin* GetPluginByNameRaw( StringView pluginName, int minimumVersion ) const = 0;
};
