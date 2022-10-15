// SPDX-FileCopyrightText: 2021-2022 Admer Šuko
// SPDX-License-Identifier: MIT

#include "common/Precompiled.hpp"

#include "console/Console.hpp"
#include "core/Core.hpp"
#include "filesystem/FileSystem.hpp"
#include "input/Input.hpp"
#include "assetmanager/ModelManager.hpp"
#include "pluginsystem/PluginSystem.hpp"

#include "elegy-rhi/DeviceManager.hpp"

#if USE_VK
#include <vulkan/vulkan.h>
#endif

#include "Engine.hpp"

namespace Utilities
{
	class BtxNvrhiMessageCallback final : public nvrhi::IMessageCallback
	{
	public:
		void Setup( IConsole* console )
		{
			Console = console;
		}

		void message( nvrhi::MessageSeverity severity, const char* messageText )
		{
			switch ( severity )
			{
			case nvrhi::MessageSeverity::Info:
				Console->DPrint( format( "[nvrhi] %s%s", PrintGreen, messageText ), 1 );
				break;
			case nvrhi::MessageSeverity::Warning:
				Console->Warning( format( "[nvrhi] %s", messageText ) );
				break;
			case nvrhi::MessageSeverity::Fatal:
				Console->Error( "====== FATAL ======" );
			case nvrhi::MessageSeverity::Error:
				Console->Error( format( "[nvrhi] %s", messageText ) );
				break;
			}
		}

	private:
		IConsole* Console;
	};

	// This gross hack is due to some lack of thought, but I'd still rather do this than #ifdefs for different platforms
	// nvrhi::app::WindowSurfaceData and WindowSurfaceData are basically the same in memory, so this is not an issue
	static nvrhi::app::WindowSurfaceData ConvertWindowSurfaceData( const WindowSurfaceData& engineWindowSurface )
	{
		return *(reinterpret_cast<const nvrhi::app::WindowSurfaceData*>(&engineWindowSurface));
	}

#if USE_VK
	// This is truly not necessary, could just use SDL_Vulkan_GetInstanceExtensions, 
	// but I'm doing this for the sake of GoldSRC Half-Life support, cuz' there's an ancient version of SDL2 in there
	static void FillRequiredVulkanExtensions( Vector<String>& extensions )
	{
		extensions.push_back( VK_KHR_SURFACE_EXTENSION_NAME );

#if ADM_PLATFORM == PLATFORM_WINDOWS

		extensions.push_back( VK_KHR_WIN32_SURFACE_EXTENSION_NAME );

#elif ADM_PLATFORM == PLATFORM_LINUX
#if ADM_USE_X11
		extensions.push_back( VK_KHR_XLIB_SURFACE_EXTENSION_NAME );
#endif

#if ADM_USE_WAYLAND
		extensions.push_back( VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME );
#endif
#endif
	}
#endif

	static void SynchroniseVideoFormat( IWindow* window, nvrhi::Format targetFormat )
	{
		WindowVideoFormat format = WindowVideoFormat::BGRA8;

		switch ( targetFormat )
		{
		case nvrhi::Format::SRGBA8_UNORM: format = WindowVideoFormat::SRGBA8; break;
		case nvrhi::Format::RGBA8_UNORM:  format = WindowVideoFormat::RGBA8;  break;
		case nvrhi::Format::SBGRA8_UNORM: format = WindowVideoFormat::SBGRA8; break;
		};

		WindowVideoMode videoMode = window->GetVideoMode();
		videoMode.format = format;
		window->SetVideoMode( videoMode );
	}

	static nvrhi::GraphicsAPI GetGraphicsApiFromArguments( const Dictionary& args )
	{
		String rendererString = args.GetString( "-gapi" );
		
		if ( rendererString == "dx12" )
		{
			return nvrhi::GraphicsAPI::D3D12;
		}
		else if ( rendererString == "dx11" )
		{
			return nvrhi::GraphicsAPI::D3D11;
		}

#if USE_VK
		return nvrhi::GraphicsAPI::VULKAN;
#else
		return nvrhi::GraphicsAPI::D3D12;
#endif
	}
}

// ============================
// Engine::InitialiseRenderer
// ============================
bool Engine::InitialiseRenderer()
{
	auto& renderFrontends = pluginSystem.GetPluginList( IRenderFrontend::Name ).GetPluginLinks();
	const size_t numRenderFrontendPlugins = renderFrontends.size();

	if ( numRenderFrontendPlugins < 1 )
	{
		console.Error( "There are no renderer plugins" );
		return false;
	}
	else if ( numRenderFrontendPlugins > 1 )
	{
		console.Error( "There are 2 or more renderer plugins, make sure you only use one" );
		return false;
	}

	renderFrontend = static_cast<IRenderFrontend*>( renderFrontends.front() );

	// Now that we've made sure there is only a single renderer plugin, and we're
	// not running in headless mode, we can create a renderer device and swapchain
	// The decision between DX12 and Vulkan is made in here, depending on the launch parameter
	if ( !CreateDeviceAndSwapchain() )
	{
		console.Error( "Failed to create device and swapchain" );
		return false;
	}

	// renderBackendManager is considered valid at this point, we can obtain the render device from it
	return renderFrontend->PostInit( renderBackendManager->GetDevice(), mainWindow );
}

// ============================
// Engine::CreateDeviceAndSwapchain
// ============================
bool Engine::CreateDeviceAndSwapchain()
{
	nvrhi::app::DeviceCreationParameters dcp;
	dcp.windowSurfaceData = Utilities::ConvertWindowSurfaceData( mainWindow->GetSurfaceData() );
	dcp.backBufferWidth = mainWindow->GetSize().x;
	dcp.backBufferHeight = mainWindow->GetSize().y;
	dcp.swapChainSampleCount = 1; // MSAA
	dcp.swapChainBufferCount = 3; // Double-buffering or, in this case, triple-buffering
	dcp.refreshRate = 60;
	// SDL2 seems to be fine with this format
	dcp.swapChainFormat = nvrhi::Format::BGRA8_UNORM;

#if USE_VK
	Utilities::FillRequiredVulkanExtensions( dcp.requiredVulkanInstanceExtensions );
#endif
	Utilities::SynchroniseVideoFormat( mainWindow, dcp.swapChainFormat );

	auto* callback = Singleton<Utilities::BtxNvrhiMessageCallback>::GetInstancePtr();
	callback->Setup( &console );
	dcp.messageCallback = callback;
	
	renderBackendManager = nvrhi::app::DeviceManager::Create( Utilities::GetGraphicsApiFromArguments( console.GetArguments() ) );
	if ( nullptr == renderBackendManager )
	{
		console.Error( "Failed to create render device" );
		return false;
	}

	if ( !renderBackendManager->CreateWindowDeviceAndSwapChain( dcp ) )
	{
		console.Error( "Failed to create swapchain" );
		return false;
	}

	return true;
}
