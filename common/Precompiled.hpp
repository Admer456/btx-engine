
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

// Render backend interfaces
#include <nvrhi/nvrhi.h>

// NO ENGINE-SPECIFIC HEADERS
// Cuz' this is used by the game library as well
// Include our interfaces tho'

#include "common/console/CVar.hpp"
#include "common/core/IWindow.hpp"

#include "common/pluginsystem/PluginLibraryMetadata.hpp"
#include "common/pluginsystem/PluginLibrary.hpp"
#include "common/pluginsystem/PluginList.hpp"

#include "common/rendering/Common.hpp"
#include "common/rendering/IRenderBatch.hpp"
#include "common/rendering/IRenderEntity.hpp"
#include "common/rendering/IRenderLight.hpp"
#include "common/rendering/IRenderView.hpp"
#include "common/rendering/IRenderVolume.hpp"

#include "common/EngineConfig.hpp"
#include "common/GameMetadata.hpp"

#include "common/interfaces/plugins/IPlugin.hpp"
#include "common/interfaces/plugins/IApplication.hpp"

#include "common/interfaces/ICore.hpp"
#include "common/interfaces/IConsole.hpp"
#include "common/interfaces/IConsoleListener.hpp"
#include "common/interfaces/IEngine.hpp"
#include "common/interfaces/IFileSystem.hpp"
#include "common/interfaces/IInput.hpp"
#include "common/interfaces/IMaterialManager.hpp"
#include "common/interfaces/IModelManager.hpp"
#include "common/interfaces/IPluginSystem.hpp"
#include "common/interfaces/IRenderFrontend.hpp"
