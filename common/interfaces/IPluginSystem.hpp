
#pragma once

class IPlugin;
class PluginList;
class PluginLibrary;
class PluginLibraryMetadata;

class IPluginSystem
{
public:
	virtual bool Init( const Vector<String>& pluginsToLoad ) = 0;
	virtual void Shutdown() = 0;

	// Reads plugin metadata and loads a plugin DLL
	// One plugin library may load multiple plugins
	// @param libraryPath: directory of the metadata file
	// @returns nullptr if the path is not valid or if it's missing
	// needed files, otherwise returns a valid PluginLibrary
	virtual PluginLibrary* LoadPluginLibrary( Path library ) = 0;

	// Gets plugin library info
	virtual const PluginLibraryMetadata& GetPluginLibraryMetadata( const IPlugin* plugin ) const = 0;

	// Shuts down the plugin and unloads the DLL
	// After this, libraryHandle is no longer valid
	virtual void UnloadPluginLibrary( const PluginLibrary* pluginLibrary ) = 0;

	// Execute 'function' for each plugin
	virtual void ForEachPlugin( std::function<void( IPlugin* )> function ) = 0;

	// Execute 'function' for each plugin that implements pluginType
	// Does not work with downcasting, so IPlugin won't pick up any plugins, use ForEachPlugin for that
	template<typename pluginType>
	void ForEachPluginOfType( std::function<void( pluginType* )> function )
	{
		GetPluginList( pluginType::Name ).ForEach<pluginType>( function );
	}

	// Gets a list of plugins that implement interfaceName
	virtual PluginList& GetPluginList( const char* interfaceName ) = 0;
	
	// Gets a plugin instance by name
	template<typename pluginType>
	pluginType* GetPluginByName( StringView pluginName ) const
	{
		return dynamic_cast<pluginType*>( GetPluginByNameRaw( pluginName ) );
	}

	virtual IPlugin* GetPluginByNameRaw( StringView pluginName ) const = 0;
};
