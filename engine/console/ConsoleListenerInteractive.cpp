
#include "Precompiled.hpp"
#include "Console.hpp"

// Currently not implemented
// TODO: add FTXUI as a dependency and use it here

// ============================
// ConsoleListenerBasic
// ============================
class ConsoleListenerInteractive final : public ConsoleListenerBase
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
void ConsoleListenerInteractive::Init( ICore* core, IConsole* console )
{
	ConsoleListenerBase::Init( core, console );
}

// ============================
// ConsoleListenerBasic::Shutdown
// ============================
void ConsoleListenerInteractive::Shutdown()
{

}

// ============================
// ConsoleListenerBasic:OnLog
// ============================
void ConsoleListenerInteractive::OnLog( const ConsoleMessage& message )
{

}

// ============================
// ConsoleListenerBasic::OnUpdate
// ============================
void ConsoleListenerInteractive::OnUpdate()
{

}

// ============================
// Console::CreateListenerTUI
// ============================
IConsoleListener* Console::CreateListenerInteractive()
{
	return new ConsoleListenerInteractive();
}
