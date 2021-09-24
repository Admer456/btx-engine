
class FileSystem final : public IFileSystem
{
public:
	void				Init( Path gameDirectory ) override;

	void				Mount( Path otherGameDirectory, bool mountOthers = false ) override;

	const Path&			GetBaseDirectory() const override;
	const Path&			GetCurrentGameDirectory() const override;

	bool				Exists( Path, const uint8_t& filterFlags ) const override;

private:
	bool				ExistsInternal( Path path, const uint8_t& filterFlags ) const;

private:
	String				baseDirectory;
	Path				basePath;
	String				currentGameDirectory;
	Path				currentGamePath;
	std::vector<Path>	otherPaths; // other games, addons etc.
};
