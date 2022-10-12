// SPDX-FileCopyrightText: 2022 Admer Šuko
// SPDX-License-Identifier: MIT

#pragma once

namespace Render
{
	class Model;
}

class ModelManager : public IModelManager
{
public:
	bool				Init() override;
	void				Shutdown() override;

	// To import APIs from the engine
	void				Setup( ICore* core, IConsole* console, IPluginSystem* pluginSystem, IFileSystem* fileSystem );

	Render::IModel*		CreateModel( const Render::ModelDesc& desc ) override;
	bool				UpdateModel( Render::IModel* model, const Render::ModelDesc& desc ) override;
	void				DestroyModel( Render::IModel* model ) override;

	size_t				GetNumModels() const override;
	Render::IModel*		GetModel( uint32_t index ) const override;

private:
	// Cheaper to resize, but more fragmented this way
	// Todo: *maybe* compare the performance of
	// Vector<Render::Model> versus this
	Vector<UniquePtr<Render::Model>> models;

	ICore* Core{ nullptr };
	IConsole* Console{ nullptr };
	IPluginSystem* PluginSystem{ nullptr };
	IFileSystem* FileSystem{ nullptr };
};
