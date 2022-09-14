
#include "Precompiled.hpp"
#include "Console.hpp"

// Currently not implemented
// TODO: add FTXUI as a dependency and use it here

// ============================
// ConsoleListenerBasic
// ============================
class ConsoleListenerTUI final : public ConsoleListenerBase
{
public:
	void Init( ICore* core, IConsole* console ) override;
	void Shutdown() override;

	void OnLog( const ConsoleMessage& message ) override;
	void OnUpdate() override;

	const char* GetName() override
	{
		return "BurekTech X CLI";
	}
};

// ============================
// ConsoleListenerBasic::Init
// ============================
void ConsoleListenerTUI::Init( ICore* core, IConsole* console )
{
	ConsoleListenerBase::Init( core, console );
}

// ============================
// ConsoleListenerBasic::Shutdown
// ============================
void ConsoleListenerTUI::Shutdown()
{

}

// ============================
// ConsoleListenerBasic:OnLog
// ============================
void ConsoleListenerTUI::OnLog( const ConsoleMessage& message )
{

}

// ============================
// ConsoleListenerBasic::OnUpdate
// ============================
void ConsoleListenerTUI::OnUpdate()
{

}

// ============================
// Console::CreateListenerTUI
// ============================
IConsoleListener* Console::CreateListenerTUI()
{
	return new ConsoleListenerTUI();
}
