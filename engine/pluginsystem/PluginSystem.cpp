// SPDX-FileCopyrightText: 2022 Admer Šuko
// SPDX-License-Identifier: MIT

#include "common/Precompiled.hpp"
#include "PluginSystem.hpp"

// ============================
// PluginSystem::Init
// ============================
bool PluginSystem::Init( const Vector<String>& pluginsToLoad )
{
	console->Print( "PluginSystem::Init" );

	// So GetPluginList can still return a valid list
	pluginInterfaceMap["none"] = {};

	// In cause of failure, construct an error list
	String errorString = "PluginSystem::Init: Failed to load these plugins:\n";
	errorString.reserve( 256U );
	bool failed = false;

	for ( const auto& pluginPath : pluginsToLoad )
	{
		if ( nullptr == LoadPluginLibrary( pluginPath ) )
		{
			errorString += "   * " + pluginPath + "\n";
			failed = true;
		}
	}

	if ( failed )
	{
		errorString += "Resolve this and try again.";
		console->Error( errorString.c_str() );
		return false;
	}

	console->Print( "PluginSystem::Init: Done!" );
	return true;
}

// ============================
// PluginSystem::Shutdown
// ============================
void PluginSystem::Shutdown()
{
	pluginInterfaceMap.clear();
	pluginLibraries.clear();
}

// ============================
// PluginSystem::LoadPluginLibrary
// ============================
PluginLibrary* PluginSystem::LoadPluginLibrary( Path libraryPath )
{
	// Maybe there should be a prefix system
	// console->PushPrefix( "PluginSystem::LoadPluginLibrary: " );

	String libraryPathStr = libraryPath.string();
	console->Print( adm::format( "PluginSystem::LoadPluginLibrary: loading plugin library '%s'...", libraryPathStr.c_str() ) );

	// Make sure the plugin directory exists
	auto realPath = fileSystem->GetPathTo( libraryPath, IFileSystem::Path_Directory );
	if ( !realPath )
	{
		console->Warning( adm::format( 
			"PluginSystem::LoadPluginLibrary: tried loading plugin from directory '%s', but it doesn't exist", libraryPath.c_str() ) );
		return nullptr;
	}

	// Make sure the plugin config exists
	Path libraryDirectory = realPath.value();
	Path libraryMetadataPath = libraryDirectory/"plugins.json";
	if ( !fileSystem->Exists( libraryMetadataPath, IFileSystem::Path_File, true ) )
	{
		console->Warning( adm::format( 
			"PluginSystem::LoadPluginLibrary: tried loading plugin from directory '%s', but it doesn't have a plugins.json", libraryPath.c_str() ) );
		return nullptr;
	}

	// Make sure the plugin library is new
	PluginLibrary* pluginLibrary = GetPluginLibrary( libraryMetadataPath );
	if ( nullptr != pluginLibrary )
	{
		return pluginLibrary;
	}

	// Get the JSON so we can figure out the DLL's name
	json pluginJson = adm::ParseJSON( libraryMetadataPath.string() );
	if ( pluginJson.empty() )
	{
		console->Warning( adm::format(
			"PluginSystem::LoadPluginLibrary: tried loading plugin from directory '%s', but its JSON is faulty", libraryPath.c_str() ) );
		return nullptr;
	}

	// Make sure the module is valid
	String moduleName = pluginJson.value( "moduleName", "library" );
	Path moduleDirectory = libraryDirectory/moduleName;
	Library pluginModule = Library( moduleDirectory.string() );
	if ( !pluginModule )
	{
		console->Warning( adm::format( 
			"PluginSystem::LoadPluginLibrary: tried loading plugin from directory '%s', but it doesn't have a '%s.dll' or .so", libraryPath.c_str(), moduleName.c_str() ) );
		return nullptr;
	}

	// Make sure the API is there
	auto result = pluginModule.TryExecuteFunction<PluginInterfaceFunction>( PluginInterfaceFunctionName );
	if ( !result )
	{
		console->Warning( adm::format( 
			"PluginSystem::LoadPluginLibrary: plugin '%s' has an incorrect .dll/.so file (cannot find GetPluginRegistry)", libraryPath.c_str() ) );
		return nullptr;
	}

	// Make sure the registry is valid
	PluginRegistry* registry = result.value();
	if ( nullptr == registry )
	{
		console->Warning( adm::format( 
			"PluginSystem::LoadPluginLibrary: plugin '%s' has an incorrect .dll/.so file (plugin registry is null)", libraryPath.c_str() ) );
		return nullptr;
	}

	// Make sure the registry is not empty
	if ( registry->GetFactories().empty() )
	{
		console->Warning( adm::format(
			"PluginSystem::LoadPluginLibrary: plugin library '%s' doesn't have any plugins?", libraryPath.c_str() ) );
		return nullptr;
	};

	console->Print( adm::format( "PluginSystem::LoadPluginLibrary: successfully loaded '%s'", libraryPathStr.c_str() ) );

	pluginLibraries.emplace_back( PluginLibrary( registry, pluginJson, libraryMetadataPath ), std::move( pluginModule ) );

	AddPluginsToInterfaceMap( pluginLibraries.back().pluginLibrary );

	return &pluginLibraries.back().pluginLibrary;
}

// ============================
// PluginSystem::GetPluginLibraryMetadata
// ============================
const PluginLibraryMetadata& PluginSystem::GetPluginLibraryMetadata( const IPlugin* plugin ) const
{
	for ( const auto& pluginPair : pluginLibraries )
	{
		const PluginLibrary& pluginLibrary = pluginPair.pluginLibrary;

		for ( const auto& pluginInternal : pluginLibrary.GetPlugins() )
		{
			if ( pluginInternal.get() == plugin )
			{
				return pluginLibrary.GetMetadata();
			}
		}
	}

	return PluginLibraryMetadata::Invalid;
}

// ============================
// PluginSystem::UnloadPluginLibrary
// ============================
void PluginSystem::UnloadPluginLibrary( const PluginLibrary* pluginLibrary )
{
	if ( nullptr == pluginLibrary )
	{
		return;
	}

	for ( const auto& pair : pluginLibraries )
	{
		if ( &pair.pluginLibrary == pluginLibrary )
		{
			RemovePluginsFromInterfaceMap( pair.pluginLibrary );
			pluginLibraries.remove( pair );
			break;
		}
	}
}

// ============================
// PluginSystem::ForEachPlugin
// ============================
void PluginSystem::ForEachPlugin( std::function<void( IPlugin* )> function )
{
	for ( auto& pluginLibraryElement : pluginLibraries )
	{
		for ( auto& plugin : pluginLibraryElement.pluginLibrary.GetPlugins() )
		{
			function( plugin.get() );
		}
	}
}

// ============================
// PluginSystem::GetPluginList
// ============================
PluginList& PluginSystem::GetPluginList( const char* interfaceName )
{
	auto result = pluginInterfaceMap.find( interfaceName );
	if ( result == pluginInterfaceMap.end() )
	{
		return pluginInterfaceMap.at( "none" );
	}

	return result->second;
}

// ============================
// PluginSystem::GetPluginByNameRaw
// ============================
IPlugin* PluginSystem::GetPluginByNameRaw( StringView pluginName ) const
{
	for ( const auto& pair : pluginLibraries )
	{
		for ( const auto& plugin : pair.pluginLibrary.GetPlugins() )
		{
			if ( pluginName == plugin->GetPluginName() )
			{
				return plugin.get();
			}
		}
	}

	return nullptr;
}

// ============================
// PluginSystem::AddPluginsToInterfaceMap
// ============================
void PluginSystem::AddPluginsToInterfaceMap( const PluginLibrary& library )
{
	for ( auto& plugin : library.GetPlugins() )
	{
		const char* interfaceName = plugin->GetInterfaceName();
		pluginInterfaceMap[interfaceName].GetPluginLinks().push_back( plugin.get() );
	}
}

// ============================
// PluginSystem::RemovePluginsFromInterfaceMap
// ============================
void PluginSystem::RemovePluginsFromInterfaceMap( const PluginLibrary& library )
{
	for ( const auto& plugin : library.GetPlugins() )
	{
		const char* interfaceName = plugin->GetInterfaceName();
		pluginInterfaceMap[interfaceName].GetPluginLinks().remove( plugin.get() );
	}
}

// ============================
// PluginSystem::GetPluginLibrary
// ============================
PluginLibrary* PluginSystem::GetPluginLibrary( Path metadataPath )
{
	for ( auto& pair : pluginLibraries )
	{
		if ( pair.pluginLibrary.GetMetadataPath() == metadataPath )
		{
			return &pair.pluginLibrary;
		}
	}

	return nullptr;
}
