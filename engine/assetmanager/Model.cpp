// SPDX-FileCopyrightText: 2022 Admer Šuko
// SPDX-License-Identifier: MIT

#include "common/Precompiled.hpp"
#include "Model.hpp"

using namespace Assets;

Model::Model( const ModelDesc& modelDesc )
	: desc( modelDesc )
{

}

StringView Model::GetName() const
{
	return desc.modelData.name;
}

const RenderData::Model& Model::GetModelData() const
{
	return desc.modelData;
}

ModelDesc& Model::GetDesc()
{
	return desc;
}

const ModelDesc& Model::GetDesc() const
{
	return desc;
}
