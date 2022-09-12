
#pragma once

class PluginRegistry;

class PluginLibrary final
{
public:
	PluginLibrary() = default;
	PluginLibrary( const PluginRegistry* registry, const json& metadataJson, Path metadataFilePath );
	PluginLibrary( const PluginLibrary& library ) = delete;
	PluginLibrary( PluginLibrary&& library ) noexcept;
	~PluginLibrary();

	bool HasPlugin( const IPlugin* plugin ) const;
	
	const PluginLibraryMetadata& GetMetadata() const
	{
		return metadata;
	}

	const Path& GetMetadataPath() const
	{
		return metadataPath;
	}

	Vector<UniquePtr<IPlugin>>& GetPlugins()
	{
		return plugins;
	}

	const Vector<UniquePtr<IPlugin>>& GetPlugins() const
	{
		return plugins;
	}

	operator bool() const 
	{
		return nullptr != registry;
	}

private:
	static Vector<UniquePtr<IPlugin>> InstantiatePlugins( const PluginRegistry* registry );

private:
	const PluginRegistry* registry{ nullptr };
	Vector<UniquePtr<IPlugin>> plugins;
	PluginLibraryMetadata metadata;
	Path metadataPath;
};
