
#include "common/Precompiled.hpp"
#include "PluginLibrary.hpp"

// ============================
// PluginLibrary::ctor
// ============================
PluginLibrary::PluginLibrary( const PluginRegistry* registry, const json& metadataJson, Path metadataFilePath )
	: registry( registry ),
	plugins( std::move( InstantiatePlugins( registry ) ) ),
	metadata( metadataJson, registry ? registry->GetEngineVersion() : -1, plugins ),
	metadataPath( metadataFilePath )
{
}

// ============================
// PluginLibrary::ctor
// ============================
PluginLibrary::PluginLibrary( PluginLibrary&& library ) noexcept
{
	registry = library.registry;
	metadata = std::move( library.metadata );
	metadataPath = std::move( library.metadataPath );
	plugins = std::move( library.plugins );

	library.registry = nullptr;
}

// ============================
// PluginLibrary::dtor
// ============================
PluginLibrary::~PluginLibrary()
{
	for ( auto& plugin : plugins )
	{
		plugin->Shutdown();
	}

	plugins.clear();

	registry = nullptr;
}

// ============================
// PluginLibrary::HasPlugin
// ============================
bool PluginLibrary::HasPlugin( const IPlugin* plugin ) const
{
	for ( const auto& pluginLocal : plugins )
	{
		if ( pluginLocal.get() == plugin )
		{
			return true;
		}
	}

	return false;
}

// ============================
// PluginLibrary::InstantiatePlugin
// ============================
Vector<UniquePtr<IPlugin>> PluginLibrary::InstantiatePlugins( const PluginRegistry* registry )
{
	if ( nullptr == registry )
	{
		return {};
	}

	Vector<UniquePtr<IPlugin>> plugins;
	plugins.reserve( registry->GetFactories().size() );

	for ( const auto& factory : registry->GetFactories() )
	{
		plugins.emplace_back( factory() );
	}

	return plugins;
}
