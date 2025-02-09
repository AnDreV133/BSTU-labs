#include "../Headers/AudioCollection.h"

void AudioCollection::add(Track track)
{
    tracks.push_back(track);
}

void AudioCollection::add(Album album)
{
    albums.push_back(album);
}