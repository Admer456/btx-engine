// SPDX-FileCopyrightText: 2022 Admer Šuko
// SPDX-License-Identifier: MIT

#pragma once

class FileSystem final : public IFileSystem
{
public:
	bool				Init( Path gameDirectory, Path engineDirectory ) override;
	void				Shutdown() override;

	bool				Mount( Path otherGameDirectory, bool mountOthers = false ) override;

	const				GameMetadata& GetCurrentGameMetadata() const;
	const				Vector<GameMetadata>& GetMountedGameMetadatas() const;

	const Path&			GetEngineDirectory() const override;
	const Path&			GetBaseDirectory() const override;
	const Path&			GetCurrentGameDirectory() const override;

	bool				Exists( Path path, const uint8_t& filterFlags, bool noMountedDirectories ) const override;

	Optional<Path>		GetPathTo( Path destination, const uint8_t& filterFlags, bool noMountedDirectories ) const override;

	void				Setup( ICore* core, IConsole* console )
	{
		this->core = core;
		this->console = console;
	}

private:
	bool				MountInternal( Path otherGameDirectory, bool mountOthers, bool mountingMainGame, bool mountingEngine );
	bool				ExistsInternal( Path path, const uint8_t& filterFlags ) const;

private:
	Path				enginePath;
	Path				basePath;
	Path				currentGamePath;
	Vector<Path>		otherPaths; // other games, addons etc.

	GameMetadata		gameMetadata;
	Vector<GameMetadata> otherMetadatas;

	ICore*				core{ nullptr };
	IConsole*			console{ nullptr };
};
