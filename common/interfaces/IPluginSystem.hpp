
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

// I hope to God you won't actually use ~4 billion plugins
using PluginLibraryHandle = uint32_t;
constexpr PluginLibraryHandle PluginLibraryInvalid = ~0U;

class IPluginSystem
{
public:
	virtual bool Init() = 0;
	virtual void Shutdown() = 0;

	// Reads plugin metadata and loads a plugin DLL
	// One plugin library may load multiple plugins
	virtual PluginLibraryHandle LoadPluginLibrary( Path libraryPath ) = 0;

	// Gets plugin library info
	virtual const PluginLibraryMetadata& GetPluginLibraryMetadata( PluginLibraryHandle libraryHandle ) const = 0;
	virtual const PluginLibraryMetadata& GetPluginLibraryMetadata( const IPlugin* plugin ) const = 0;

	// Shuts down the plugin and unloads the DLL
	virtual void UnloadPluginLibrary( PluginLibraryHandle libraryHandle ) = 0;

	// Gets a list of plugins that implement interfaceName
	virtual const LinkedList<IPlugin*>& GetPluginList( const char* interfaceName ) const = 0;
	
	// Gets a plugin instance by name
	template<typename pluginType>
	pluginType* GetPluginByName( StringView pluginName ) const
	{
		return dynamic_cast<pluginType*>( GetPluginByNameRaw( pluginName ) );
	}

	virtual IPlugin* GetPluginByNameRaw( StringView pluginName ) const = 0;
};
