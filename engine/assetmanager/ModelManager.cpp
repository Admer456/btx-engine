// SPDX-FileCopyrightText: 2022 Admer Šuko
// SPDX-License-Identifier: MIT

#include "common/Precompiled.hpp"
#include "Model.hpp"
#include "ModelManager.hpp"

using namespace Render;

bool ModelManager::Init()
{
	return true;
}

void ModelManager::Shutdown()
{
	models.clear();
}

void ModelManager::Setup( ICore* core, IConsole* console, IPluginSystem* pluginSystem, IFileSystem* fileSystem )
{
	Core = core;
	Console = console;
	PluginSystem = pluginSystem;
	FileSystem = fileSystem;
}

IModel* ModelManager::CreateModel( const ModelDesc& desc )
{
	// TODO: Try seeing if there are any same models first
	// We could do this by hashing the names of models and doing a quick check on them

	// TODO: Load model from file (ModelDesc::modelPath) and so with plugins
	if ( !desc.modelPath.empty() )
	{
		Console->Error( "Model loading from files is not yet implemented" );
		return nullptr;
	}

	// TODO: Deferred model loading if it's from a file and shouldStream is true

	return models.emplace_back( new Model( desc ) ).get();
}

bool ModelManager::UpdateModel( IModel* model, const ModelDesc& desc )
{
	// TODO: Implement
	// TODO: Alert the render frontend about the model update to regenerate vertex buffers
	return false;
}

void ModelManager::DestroyModel( Render::IModel* model )
{
	for ( auto it = models.begin(); it != models.end(); it++ )
	{
		if ( it->get() == model )
		{
			models.erase( it );
			return;
		}
	}

	Console->Warning( "Attempted to destroy an invalid model" );
}

size_t ModelManager::GetNumModels() const
{
	return models.size();
}

IModel* ModelManager::GetModel( uint32_t index ) const
{
	if ( index >= models.size() )
	{
		return nullptr;
	}

	return models.at( index ).get();
}
