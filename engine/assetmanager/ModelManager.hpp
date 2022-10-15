// SPDX-FileCopyrightText: 2022 Admer Šuko
// SPDX-License-Identifier: MIT

#pragma once

#include "Model.hpp"

class ModelManager : public IModelManager
{
public:
	bool				Init() override;
	void				Shutdown() override;

	// To import APIs from the engine
	void				Setup( ICore* core, IConsole* console, IPluginSystem* pluginSystem, IFileSystem* fileSystem, IRenderFrontend* renderFrontend );

	Assets::IModel*		CreateModel( const Assets::ModelDesc& desc ) override;
	bool				UpdateModel( Assets::IModel* model, const Assets::ModelDesc& desc ) override;
	void				DestroyModel( Assets::IModel* model ) override;

	size_t				GetNumModels() const override;
	Assets::IModel*		GetModel( uint32_t index ) const override;

private:
	// Cheaper to resize, but more fragmented this way
	// Todo: *maybe* compare the performance of
	// Vector<Render::Model> versus this
	Vector<UniquePtr<Assets::Model>> models;

	ICore* Core{ nullptr };
	IConsole* Console{ nullptr };
	IPluginSystem* PluginSystem{ nullptr };
	IFileSystem* FileSystem{ nullptr };
	IRenderFrontend* Renderer{ nullptr };
};
