#include "../Headers/AudioCollection.h"

void Player::addTrackToPlayList(Track track)
{
    playlist.push_back(track);
}

void Player::putAlbumToPlaylist(Album album)
{
    playlist.clear();
    indexCurrentTrack = 0;

    for (Track track : album.tracks)
        addTrackToPlayList(track);
}

Track Player::play()
{
    state = PLAY;

    if (indexCurrentTrack < 0)
        indexCurrentTrack = 0;
    
    return playlist[indexCurrentTrack];
}

Track Player::stop()
{
    state = STOP;

    return playlist[indexCurrentTrack];
}

Track Player::next()
{
    if (indexCurrentTrack < playlist.size() - 1)
        indexCurrentTrack++;
    
    return playlist[indexCurrentTrack];
}

Track Player::prev()
{
    if (indexCurrentTrack > 1)
        indexCurrentTrack--;

    return playlist[indexCurrentTrack];
}

Track Player::getCurrentTrack()
{
    if (indexCurrentTrack < 0
        && indexCurrentTrack >= playlist.size())
        throw;

    return playlist[indexCurrentTrack];
}