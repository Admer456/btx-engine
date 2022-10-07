// SPDX-FileCopyrightText: 2022 Admer Šuko
// SPDX-License-Identifier: MIT

#include "Precompiled.hpp"
#include "Console.hpp"

#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include "ftxui/Scroller.hpp"

using namespace ftxui;

// ============================
// ConsoleListenerInteractive
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
		return "Interactive CLI";
	}

private:
	// Handles CLI events i.e. input and scrolling
	bool ContainerEventHandler( Event e );
	void ConsumeCommand();
	void UpdateAutocomplete();

	bool IsInputValid() const;
	String GetCommandName() const;

	static const char* GenerateTimeString( const ConsoleMessage& message );
	static Element ConsoleMessageToFtxElement( const ConsoleMessage& message );

private:
	bool stopListening{ false };
	Vector<ConsoleMessage> messages{};

	// TODO: replace std::thread stuff with a job system later on
	std::thread listenerThread;
	float timeToUpdate{ 0.1f };
	// The user has entered a new command, jump to bottom to see the output
	bool jumpToBottom{ false };

	// User input string
	String userInput{ "" };
	// Element that contains the autocomplete window
	// Is updated periodically instead of every frame
	Element autocompleteElement = text( "" );

	// The screen object where everything happens
	ScreenInteractive screen = ScreenInteractive::Fullscreen();
	// Frame of the little animation in the top-right corner
	int animationFrame{ 0 };
	// Title bar on the top
	Component consoleTitleComponent{};
	// Text input bar on the bottom
	Component inputFieldComponent{};
	// Displays the actual ConsoleMessages
	Component messageFrameComponent{};
	// Parent of messageFrameComponent
	Component messageScrollerComponent{};
	// Logical container for inputFieldComponent and messageScrollerComponent
	Component containerComponent{};
	// Parent of all the above
	Component mainComponent{};
};

/*
 Rough sketch of the console:
 ----------------------------------------
 | v0.1.0 |    BurekTech X CLI    | |=| |
 |--------------------------------------|
 | 000:01:059 | Message 1               |
 | 000:01:060 | Message 2               |
 | 000:01:061 | Message 3               |
 |                                      |
 |                                      |
 |                                      |
 |                                      |
 |                                      |
 |                                      |
 |                                      |
 |--------------------------------------|
 | > [enter command here ]              |
 ----------------------------------------
*/

// ============================
// ConsoleListenerInteractive::Init
// ============================
void ConsoleListenerInteractive::Init( ICore* core, IConsole* console )
{
	ConsoleListenerBase::Init( core, console );

	consoleTitleComponent = Renderer( [&]
		{
			return hbox(
				{
					text( "v0.1.0" ),
					separatorLight(),
					text( "BurekTech X Interactive Console" ) | center | flex,
					separatorLight(),
					spinner( 18, animationFrame )
				} );
		} );

	inputFieldComponent = Input( &userInput, "[enter your command here]" );

	messageFrameComponent = Renderer( [&]
		{
			Elements consoleMessageElements{};
			for ( const auto& message : messages )
			{
				consoleMessageElements.emplace_back( ConsoleMessageToFtxElement( message ) );
			}
			return vbox( std::move( consoleMessageElements ) );
		} );
	messageScrollerComponent = Scroller( messageFrameComponent );

	containerComponent = Container::Vertical( { messageScrollerComponent, inputFieldComponent } );
	containerComponent |= CatchEvent( [&]( Event e ) 
		{ 
			return ContainerEventHandler( e );
		} );

	mainComponent = Renderer( containerComponent, [&]
		{
			return vbox(
				{
					consoleTitleComponent->Render(),

					separatorLight(),
					// A dbox allows us to draw components *over* each other
					// What is being rendered here is a vertical stack, and then
					// we render a horizontal stack over it, which is used to position the autocomplete window
					dbox(
					{
						vbox( 
						{
							messageScrollerComponent->Render(),
							filler()
						} ),
						hbox( 
						{
							// Move the autocomplete window all the way to the left
							filler(),
							vbox( 
							{
								// Move the autocomplete window to the bottom
								filler() | yflex_shrink,
								// Draw the autocomplete window
								autocompleteElement | size( WIDTH, LESS_THAN, 40 ) | size( HEIGHT, LESS_THAN, 60 )
							} ),
							// Move the autocomplete window a few characters to the left
							filler() | size( Direction::WIDTH, Constraint::EQUAL, 8 )
						} )
					} ) | flex,

					separatorLight(),
					// Draw the input bar
					hbox(
					{
						text( "> " ),
						inputFieldComponent->Render() | focus | size( HEIGHT, EQUAL, 1 )
					})
				} ) | borderDouble;
		} );

	listenerThread = std::thread( [&]
		{
			// Don't immediately render text, wait for a little bit
			std::this_thread::sleep_for( chrono::milliseconds( 100 ) );
			screen.Loop( mainComponent );
		} );
}

// ============================
// ConsoleListenerInteractive::Shutdown
// ============================
void ConsoleListenerInteractive::Shutdown()
{
	stopListening = true;
	// If this window isn't raised, it won't respond to the SDL_QUIT event
	// issued in Core::Shutdown. It may be better to do this inside Core itself
	core->GetWindows()[0]->Raise();
	screen.Post( [&]
		{
			screen.ExitLoopClosure()();
		} );
	listenerThread.join();
}

// ============================
// ConsoleListenerInteractive:OnLog
// ============================
void ConsoleListenerInteractive::OnLog( const ConsoleMessage& message )
{
	messages.push_back( message );
}

// ============================
// ConsoleListenerInteractive::OnUpdate
// ============================
void ConsoleListenerInteractive::OnUpdate()
{
	timeToUpdate -= core->DeltaTime();
	if ( timeToUpdate > 0.0f || stopListening )
	{
		return;
	}

	screen.PostEvent( Event::Custom );
	timeToUpdate = 0.1f;
}

// ============================
// ConsoleListenerInteractive::ContainerEventHandler
// 
// Called on a separate thread
// ============================
bool ConsoleListenerInteractive::ContainerEventHandler( Event e )
{
	if ( stopListening )
	{
		return true;
	}

	if ( e == Event::ArrowUp || e == Event::PageUp || e == Event::ArrowDown ||
		e == Event::PageDown || e == Event::Home || e == Event::End )
	{
		messageScrollerComponent->OnEvent( e );
		return true;
	}

	if ( e == Event::Return )
	{
		ConsumeCommand();
		jumpToBottom = true;
		return true;
	}
	else if ( e.is_character() || e == Event::Backspace || e == Event::Tab ||
		e == Event::ArrowLeft || e == Event::ArrowRight )
	{
		inputFieldComponent->OnEvent( e );
		return true;
	}

	// It's time to update
	if ( e == Event::Custom )
	{
		UpdateAutocomplete();
		animationFrame++;

		if ( jumpToBottom )
		{
			screen.PostEvent( Event::End );
			jumpToBottom = false;
		}

		return true;
	}

	return false;
}

// ============================
// ConsoleListenerInteractive::ConsumeCommand
// ============================
void ConsoleListenerInteractive::ConsumeCommand()
{
	if ( !IsInputValid() )
	{
		if ( !userInput.empty() )
		{
			console->Print( adm::format( "%sInvalid command '%s'", PrintYellow, userInput.c_str() ) );
			userInput.clear();
		}
		return;
	}

	const String commandName = GetCommandName();
	const String commandArgs = userInput.size() > commandName.size() ? userInput.substr( commandName.size() ) : "";
	console->Execute( commandName, commandArgs );
	userInput.clear();
}

// ============================
// ConsoleListenerInteractive::UpdateAutocomplete
// ============================
void ConsoleListenerInteractive::UpdateAutocomplete()
{
	if ( !IsInputValid() )
	{
		autocompleteElement = text( "" );
		return;
	}

	auto cvars = console->Search( GetCommandName() );
	if ( cvars.empty() )
	{
		autocompleteElement = window( 
			text( "Autocomplete" ), 
			text( "none" ) | center ) | color( Color::Yellow )
			| size( HEIGHT, GREATER_THAN, 8 )
			| size( WIDTH, GREATER_THAN, 16 );
		return;
	}

	// The autocomplete view is separated into variables and commands
	Element windowContent{};
	Element variableBox = text( "" );
	Element commandBox = text( "" );
	Elements variables{};
	Elements commands{};
	for ( const auto* cvar : cvars )
	{
		if ( cvar->IsCommand() )
		{
			commands.emplace_back( text( String( cvar->GetName() ) ) );
			continue;
		}

		constexpr int ValueWidth = 6;

		String cvarValue = String( cvar->GetString() );
		if ( cvarValue.size() > ValueWidth )
		{
			cvarValue = cvarValue.substr( 0, ValueWidth );
			cvarValue[ValueWidth-1] = '.';
			cvarValue[ValueWidth-2] = '.';
			cvarValue[ValueWidth-3] = '.';
		}

		const bool readOnly = cvar->GetFlags() & CVarFlags::ReadOnly;
		Element vtext = hbox( {
				text( String( cvar->GetName() ) ),
				text( readOnly ? " (read-only)" : "" ),
				filler() | xflex_shrink,
				text( ": " + String( cvar->GetString() ) ) | size( WIDTH, EQUAL, ValueWidth + 2 ),
			} );

		variables.push_back( std::move( vtext ) );
	}

	const bool hasVariables = !variables.empty();
	const bool hasCommands = !commands.empty();

	if ( hasVariables )
	{
		variableBox = vbox( std::move( variables ) ) | color( Color::Orange1 );
	}
	if ( hasCommands )
	{
		commandBox = vbox( std::move( commands ) ) | color( Color::BlueLight );
	}

	windowContent = vbox( 
		{
			std::move( variableBox ),
			separatorLight(),
			std::move( commandBox )
		} );

	autocompleteElement = window( text( "Autocomplete" ) | hcenter, std::move( windowContent ) )
		| color( Color::Yellow )
		| size( HEIGHT, GREATER_THAN, 8 )
		| size( WIDTH, GREATER_THAN, 16 );
}

// ============================
// ConsoleListenerInteractive::ConsoleMessageToFtxElement
// ============================
Element ConsoleListenerInteractive::ConsoleMessageToFtxElement( const ConsoleMessage& message )
{
	static Map<char, Color> ColourMap
	{
		{ 'r', Color::Red },
		{ 'o', Color::Orange1 },
		{ 'y', Color::Yellow },
		{ 'g', Color::GreenLight },
		{ 'b', Color::BlueLight },
		{ 'p', Color::Pink1 },
		{ 'w', Color::White },
		{ 'G', Color::GrayLight }
	};

	char currentColour = 'w';
	Elements colouredTexts{};
	ElementDecorator textColour = color( ColourMap[currentColour] );
	String string;
	for ( size_t i = 0; i < message.text.size(); i++ )
	{
		if ( message.text[i] == '$' )
		{
			if ( !string.empty() )
			{
				colouredTexts.emplace_back( text( string ) | textColour );
				string.clear();
			}

			i++;
			if ( i >= message.text.size() )
			{
				break;
			}

			currentColour = message.text[i];

			auto iterator = ColourMap.find( currentColour );
			if ( iterator == ColourMap.end() )
			{
				textColour = color( Color::White );
			}
			else
			{
				textColour = color( iterator->second );
			}

			i++;
			if ( i >= message.text.size() )
			{
				break;
			}
		}

		string += message.text[i];

		if ( i == message.text.size() - 1 )
		{
			colouredTexts.emplace_back( text( string ) | textColour );
		}
	}

	return hbox(
		{
			text( GenerateTimeString( message ) ),
			separator(),
			text( " " ),
			hbox( std::move( colouredTexts ) )
		} );
}

// ============================
// ConsoleListenerInteractive::GenerateTimeString
// ============================
const char* ConsoleListenerInteractive::GenerateTimeString( const ConsoleMessage& message )
{
	// mmm:ss.ssss 
	static char buffer[16];

	const int iTime = message.timeSubmitted;
	const int seconds = int( message.timeSubmitted ) % 60;
	const int minutes = iTime / 60;

	const float flSeconds = seconds + (message.timeSubmitted - iTime);

	sprintf( buffer, "%03i:%06.3f ", minutes, flSeconds );
	return buffer;
}

// ============================
// ConsoleListenerInteractive::IsInputValid
// ============================
bool ConsoleListenerInteractive::IsInputValid() const
{
	if ( userInput.empty() )
	{
		return false;
	}

	if ( userInput[0] == ' ' || userInput[0] == '\t' || userInput[0] == '\n' )
	{
		return false;
	}

	return true;
}

// ============================
// ConsoleListenerInteractive::GetCommandName
// ============================
String ConsoleListenerInteractive::GetCommandName() const
{
	if ( !IsInputValid() )
	{
		return "";
	}

	// Simple, one-word command
	const size_t firstSpace = userInput.find_first_of( " " );
	if ( firstSpace == String::npos )
	{
		return userInput;
	}

	return userInput.substr( 0, firstSpace );
}

// ============================
// Console::CreateListenerTUI
// ============================
IConsoleListener* Console::CreateListenerInteractive()
{
	return new ConsoleListenerInteractive();
}
