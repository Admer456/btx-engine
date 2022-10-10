// SPDX-FileCopyrightText: 2022 Admer Šuko
// SPDX-License-Identifier: MIT

#include "common/Precompiled.hpp"

#include "console/Console.hpp"
#include "core/Core.hpp"
#include "filesystem/FileSystem.hpp"
#include "input/Input.hpp"
#include "pluginsystem/PluginSystem.hpp"

#include "Engine.hpp"

// ============================
// Engine::Command_Mount
// 
// Implementation of the "mount"
// console command
// ============================
bool Engine::Command_Mount( const ConsoleCommandArgs& args )
{
	Engine& self = adm::Singleton<Engine>::GetInstance();

	if ( args.empty() )
	{
		self.console.Warning( "No arguments for mounting" );
		return false;
	}

	self.fileSystem.Mount( args[0] );
	return true;
}

// ============================
// Engine::Command_Quit
// 
// Likely to be called on a separate thread
// ============================
bool Engine::Command_Quit( const ConsoleCommandArgs& args )
{
	adm::Singleton<Engine>::GetInstance().shutdownRequested = true;
	return true;
}
