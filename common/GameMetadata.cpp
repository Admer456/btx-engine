
#include "common/Precompiled.hpp"

GameMetadata::GameMetadata( Path filePath )
{
    File fileStream( filePath );
    Parse( fileStream );
}

GameMetadata::~GameMetadata()
{
    mountedGames.clear();
}

void GameMetadata::Parse( File& gameMetaFile )
{
    adm::Lexer lexer( gameMetaFile );

    std::string token;
    while ( !lexer.IsEndOfFile() )
    {
        token = lexer.Next();

        if ( token == "gameTitle" )
        {
            gameName = lexer.Next();
            continue;
        }

        if ( token == "gameDeveloper" )
        {
            gameDeveloper = lexer.Next();
            continue;
        }

        if ( token == "gamePublisher" )
        {
            gamePublisher = lexer.Next();
            continue;
        }

        if ( token == "gameVersion" )
        {
            gameVersion = lexer.Next();
            continue;
        }

        if ( token == "mount" )
        {
            mountedGames.push_back( lexer.Next() );
            continue;
        }

        //gEngine.Common.Warning( adm::format( "Unknown token '%s'", token.c_str() ) );
    }

}

StringRef GameMetadata::GetMountedGame( const size_t& index )
{
    if ( index >= GetNumMountedGames() )
    {
        return NullString;
    }

    return mountedGames.at( index );
}

size_t GameMetadata::GetNumMountedGames() const
{
    return mountedGames.size();
}
