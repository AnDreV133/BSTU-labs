#include <iostream>
#include "Headers/AudioCollection.h"

int main()
{
    Author a;
    a.name = "puper author";
    a.positionInTop = 1;

    Track t1;
    t1.author = a;
    t1.name = "test";
    t1.duration = 400;
    t1.textOfTrack = "testtesttest";

    Track t2;
    t2.author = a;
    t2.name = "never fade away";
    t2.duration = 18342;
    t2.textOfTrack = "f arasaka";

    Album al1;
    al1.name = "empty album";
    al1.totalDuration = 0;

    al1.tracks.push_back(t2);
    al1.tracks.push_back(t1);

    AudioCollection ac1;
    ac1.add(t1);
    ac1.add(t2);
    ac1.add(al1);

    Player p;
    for (Track t : ac1.tracks)
        p.addTrackToPlayList(t);

    p.play();

    std::cout << p.getCurrentTrack().name << " -> " << p.next().name << '\n';  

    p.putAlbumToPlaylist(al1);

    std::cout << p.getCurrentTrack().name << " -> " << p.next().name;  
}
