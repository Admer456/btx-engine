
#pragma once

class ServerGame final : public IGame
{
public:
	bool Init() override;
	void Shutdown() override;

	void Update() override;
};
