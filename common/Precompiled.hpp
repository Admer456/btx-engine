
#pragma once

// adm-utils has all the precompiled stuff we need :)
#include "extern/adm-utils/src/Precompiled.hpp"

// Some usings & namespaces to shorten typing
namespace fs = std::filesystem;
namespace chrono = std::chrono;

using namespace adm;
// Ideally all of these would be in adm::, but yea, that's something for later
using Path = fs::path;
using File = std::fstream;

inline const String NullString = "";

// Include our interfaces tho'
#include "common/interfaces/ICore.hpp"
#include "common/interfaces/IConsole.hpp"
#include "common/interfaces/IEngine.hpp"
#include "common/interfaces/IFileSystem.hpp"
#include "common/interfaces/IInput.hpp"
#include "common/interfaces/IPluginSystem.hpp"

#include "common/interfaces/plugins/IPlugin.hpp"
#include "common/interfaces/plugins/IApplication.hpp"

#include "common/console/CVar.hpp"
#include "common/pluginsystem/PluginLibraryMetadata.hpp"
#include "common/pluginsystem/PluginLibrary.hpp"
#include "common/pluginsystem/PluginList.hpp"
#include "common/EngineConfig.hpp"
#include "common/GameMetadata.hpp"

// NO ENGINE-SPECIFIC HEADERS
// Cuz' this is used by the game library as well
