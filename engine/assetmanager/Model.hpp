// SPDX-FileCopyrightText: 2022 Admer Šuko
// SPDX-License-Identifier: MIT

#pragma once

namespace Assets
{
	class Model : public IModel
	{
	public:
		Model( const ModelDesc& modelDesc );
		Model( const Model& model ) = default;
		Model( Model&& model ) = default;
		~Model() = default;

		Model& operator= ( const Model& model ) = default;
		Model& operator= ( Model&& model ) = default;

		StringView			GetName() const override;
		const RenderData::Model& GetModelData() const override;

		ModelDesc&			GetDesc() override;
		const ModelDesc&	GetDesc() const override;

	private:
		ModelDesc			desc;
	};
}
