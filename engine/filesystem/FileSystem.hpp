
#pragma once

class FileSystem final : public IFileSystem
{
public:
	void				Init( Path gameDirectory ) override;
	void				Shutdown() override;

	void				Mount( Path otherGameDirectory, bool mountOthers = false ) override;

	const Path&			GetBaseDirectory() const override;
	const Path&			GetCurrentGameDirectory() const override;

	bool				Exists( Path, const uint8_t& filterFlags ) const override;

	void				Setup( ICore* core, IConsole* console )
	{
		this->core = core;
		this->console = console;
	}

private:
	bool				ExistsInternal( Path path, const uint8_t& filterFlags ) const;

private:
	Path				basePath;
	Path				currentGamePath;
	std::vector<Path>	otherPaths; // other games, addons etc.

	ICore*				core{ nullptr };
	IConsole*			console{ nullptr };
};
