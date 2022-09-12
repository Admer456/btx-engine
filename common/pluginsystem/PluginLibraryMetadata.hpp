
#pragma once

class IPlugin;

class PluginMetadata final
{
public:
	PluginMetadata() = default;
	PluginMetadata( const IPlugin* plugin );
	PluginMetadata( const PluginMetadata& metadata ) = default;
	PluginMetadata( PluginMetadata&& metadata ) noexcept = default;
	~PluginMetadata() = default;

	StringView	GetName() const { return pluginName; }
	StringView	GetInterfaceName() const { return implementedInterface; }

	operator bool() const { return parsedCorrectly; }
	PluginMetadata& operator=( const PluginMetadata& metadata ) = default;
	PluginMetadata& operator=( PluginMetadata&& metadata ) = default;

private:
	String		pluginName{ "unknown" };
	String		implementedInterface{ "unknown" };
	bool		parsedCorrectly{ false };
};

class PluginLibraryMetadata final
{
public:
	PluginLibraryMetadata() = default;
	PluginLibraryMetadata( const json& metadataJson, int version, const Vector<UniquePtr<IPlugin>>& plugins );

	StringView	GetName() const { return name; }
	StringView	GetAuthor() const { return author; }
	StringView	GetDescription() const { return description; }
	int			GetVersion() const { return version; }
	StringView	GetVersionDate() const { return versionDate; }

	operator bool() const { return parsedCorrectly; }

	static const PluginLibraryMetadata Invalid;

private:
	String		name{ "unknown" };
	String		author{ "unknown" };
	String		description{ "unknown" };
	int			version{ -1 };
	String		versionDate{ "unknown" };
	Vector<PluginMetadata> pluginInfo;
	bool		parsedCorrectly{ false };
};

