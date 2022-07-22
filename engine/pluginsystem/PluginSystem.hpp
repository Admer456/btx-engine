
#pragma once

class PluginSystem final : public IPluginSystem
{
public:
	bool Init( const Vector<String>& pluginsToLoad ) override;
	void Shutdown() override;

	void Setup( ICore* core, IConsole* console, IFileSystem* fileSystem )
	{
		this->core = core;
		this->console = console;
		this->fileSystem = fileSystem;
	}

	PluginLibrary* LoadPluginLibrary( Path libraryPath ) override;

	const PluginLibraryMetadata& GetPluginLibraryMetadata( const IPlugin* plugin ) const override;

	void UnloadPluginLibrary( const PluginLibrary* pluginLibrary ) override;

	PluginList& GetPluginList( const char* interfaceName ) override;

	IPlugin* GetPluginByNameRaw( StringView pluginName ) const override;

private:
	void AddPluginsToInterfaceMap( const PluginLibrary& library );
	void RemovePluginsFromInterfaceMap( const PluginLibrary& library );

	PluginLibrary* GetPluginLibrary( Path metadataPath );

private:
	//LinkedList<Library> pluginModules;
	//LinkedList<PluginLibrary> pluginLibraries;
	
	LinkedList<std::pair<Library, PluginLibrary>> pluginLibraries;

	// pluginInterfaceMap["IApplication"] would give us
	// all plugins that implement IApplication for example
	Map<StringView, PluginList> pluginInterfaceMap;

	ICore* core{ nullptr };
	IConsole* console{ nullptr };
	IFileSystem* fileSystem{ nullptr };
};
