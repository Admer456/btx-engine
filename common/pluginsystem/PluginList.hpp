
#pragma once

class PluginList final
{
public:
	PluginList() = default;
	PluginList( LinkedList<IPlugin*>&& plugins ) 
		: pluginLinks( std::move( plugins ) ) 
	{
	}
	PluginList( const PluginList& list ) = default;
	PluginList( PluginList&& list ) = default;
	~PluginList() = default;

	PluginList& operator=( PluginList&& ) = default;

	// Makes sure that all plugins in the list are convertible to pluginType
	template<typename pluginType>
	bool IsAllOfType() const
	{
		for ( const auto& plugin : pluginLinks )
		{
			if ( nullptr == dynamic_cast<pluginType*>(plugin) )
			{
				return false;
			}
		}

		return true;
	}

	// Safety warning:
	// Plugins in the list are not guaranteed to be of
	// pluginType, you need to make sure beforehand
	template<typename pluginType>
	void ForEach( std::function<void(pluginType*)> function )
	{
		for ( auto& plugin : pluginLinks )
		{
			function( static_cast<pluginType*>( plugin ) );
		}
	}

	LinkedList<IPlugin*>& GetPluginLinks()
	{
		return pluginLinks;
	}

	const LinkedList<IPlugin*>& GetPluginLinks() const
	{
		return pluginLinks;
	}

private:
	LinkedList<IPlugin*> pluginLinks;
};
