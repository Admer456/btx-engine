
#pragma once

class FileSystem final : public IFileSystem
{
public:
	bool				Init( Path gameDirectory ) override;
	void				Shutdown() override;

	bool				Mount( Path otherGameDirectory, bool mountOthers = false ) override;

	const Path&			GetEngineDirectory() const override;
	const Path&			GetBaseDirectory() const override;
	const Path&			GetCurrentGameDirectory() const override;

	bool				Exists( Path path, const uint8_t& filterFlags ) const override;

	adm::Optional<Path>	GetPathTo( Path destination, const uint8_t& filterFlags ) const override;

	void				Setup( ICore* core, IConsole* console )
	{
		this->core = core;
		this->console = console;
	}

private:
	bool				ExistsInternal( Path path, const uint8_t& filterFlags ) const;

private:
	Path				enginePath;
	Path				basePath;
	Path				currentGamePath;
	std::vector<Path>	otherPaths; // other games, addons etc.

	ICore*				core{ nullptr };
	IConsole*			console{ nullptr };
};
