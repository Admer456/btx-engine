
#pragma once

struct VideoFormatBinding
{
	using VideoFormatPair = std::pair<WindowVideoFormat, SDL_PixelFormatEnum>;

	VideoFormatBinding( std::initializer_list<VideoFormatPair> list )
	{
		formats = list;
	}

	Vector<VideoFormatPair> formats;

	SDL_PixelFormatEnum Find( WindowVideoFormat videoFormat )
	{
		for ( const auto& pair : formats )
		{
			if ( videoFormat == pair.first )
			{
				return pair.second;
			}
		}

		return SDL_PIXELFORMAT_BGRA8888;
	}

	WindowVideoFormat Find( SDL_PixelFormatEnum sdlPixelFormat )
	{
		for ( const auto& pair : formats )
		{
			if ( sdlPixelFormat == pair.second )
			{
				return pair.first;
			}
		}

		return WindowVideoFormat::SBGRA8;
	}
};
