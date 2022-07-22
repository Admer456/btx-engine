
#include "common/Precompiled.hpp"
#include "PluginLibraryMetadata.hpp"

PluginMetadata::PluginMetadata( const IPlugin* plugin )
{
	if ( nullptr == plugin )
	{
		return;
	}

	if ( nullptr == plugin->GetPluginName() || nullptr == plugin->GetInterfaceName() )
	{
		return;
	}

	pluginName = plugin->GetPluginName();
	implementedInterface = plugin->GetInterfaceName();

	parsedCorrectly = true;
}

const PluginLibraryMetadata PluginLibraryMetadata::Invalid = PluginLibraryMetadata();

PluginLibraryMetadata::PluginLibraryMetadata( const json& metadataJson, int version, const Vector<UniquePtr<IPlugin>>& plugins )
	: version( version )
{
	if ( metadataJson.empty() )
	{
		return;
	}

	name = metadataJson.value( "name", "unknown" );
	author = metadataJson.value( "author", "unknown" );
	description = metadataJson.value( "description", "unknown" );
	versionDate = metadataJson.value( "versionDate", "unknown" );

	if ( plugins.empty() )
	{
		return;
	}

	for ( const auto& plugin : plugins )
	{
		pluginInfo.emplace_back( plugin.get() );
	}

	parsedCorrectly = true;
}
