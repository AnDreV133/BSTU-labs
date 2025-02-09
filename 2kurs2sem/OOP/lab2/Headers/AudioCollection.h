#ifndef AudioCollection_h
#define AudioCollection_h

#include <string>
#include <list>
#include <vector>

struct Author
{
    std::string name;
    int positionInTop;
};

struct Track
{
    Author author;
    std::string name;
    int duration;
    std::string textOfTrack;
};

struct Album
{
    std::list<Track> tracks;
    std::string name;
    int totalDuration;
};

struct AudioCollection
{
    std::list<Track> tracks;
    std::list<Album> albums;
    void add(Track track);
    void add(Album album);
};

enum PlayerState {
    PLAY,
    STOP
};

struct Player
{
    AudioCollection audioCollection;
    std::vector<Track> playlist;
    int indexCurrentTrack;
    PlayerState state;
    void addTrackToPlayList(Track track);
    void putAlbumToPlaylist(Album album);
    Track getCurrentTrack();
    Track play();
    Track stop();
    Track next();
    Track prev();
};

#endif
