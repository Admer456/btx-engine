
#pragma once

class ClientGame final : public IGame
{
public:
	bool Init() override;
	void Shutdown() override;

	void Update() override;
};
