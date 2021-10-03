
#pragma once

// adm-utils has all the precompiled stuff we need :)
#include "extern/adm-utils/src/Precompiled.hpp"

// Some usings & namespaces to shorten typing
namespace fs = std::filesystem;
namespace chrono = std::chrono;
using Path = fs::path;
using String = std::string;
using StringRef = std::string_view;
using File = std::fstream;

inline const String NullString = "";

// Include our interfaces tho'
#include "common/interfaces/ICommon.hpp"
#include "common/interfaces/IConsole.hpp"
#include "common/interfaces/IEngine.hpp"
#include "common/interfaces/IFileSystem.hpp"
#include "common/interfaces/IGame.hpp"

#include "common/console/CVar.hpp"

#include "common/GameMetadata.hpp"

// NO ENGINE-SPECIFIC HEADERS
// Cuz' this is used by the game library as well
