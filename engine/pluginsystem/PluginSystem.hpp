
#pragma once

// Replacement for std::pair<Library, PluginLibrary> for
// less ambiguity (first vs. second in std::pair)
struct PluginLibraryPair
{
	PluginLibraryPair( PluginLibrary&& pluginLibrary, Library&& pluginModule )
		: pluginLibrary( std::move( pluginLibrary ) ),
		pluginModule( std::move( pluginModule ) )
	{
	}

	// Constructed first, destructed last
	Library pluginModule;
	// Constructed last, destructed first
	PluginLibrary pluginLibrary;

	// Needed for std::list
	bool operator==( const PluginLibraryPair& pair ) const
	{
		return &pluginLibrary == &pair.pluginLibrary &&
			&pluginModule == &pair.pluginModule;
	}
};

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

	void ForEachPlugin( std::function<void( IPlugin* )> function ) override;

	PluginList& GetPluginList( const char* interfaceName ) override;

	IPlugin* GetPluginByNameRaw( StringView pluginName ) const override;

private:
	void AddPluginsToInterfaceMap( const PluginLibrary& library );
	void RemovePluginsFromInterfaceMap( const PluginLibrary& library );

	PluginLibrary* GetPluginLibrary( Path metadataPath );

private:
	LinkedList<PluginLibraryPair> pluginLibraries;

	// pluginInterfaceMap["IApplication"] would give us
	// all plugins that implement IApplication for example
	Map<StringView, PluginList> pluginInterfaceMap;

	ICore* core{ nullptr };
	IConsole* console{ nullptr };
	IFileSystem* fileSystem{ nullptr };
};
