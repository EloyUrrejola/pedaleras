#ifndef SONG_LIST_H
#define SONG_LIST_H

#include <Arduino.h>
#include <string>
#include <vector>

class SongList
{
  public:
    static void addSongs(const std::string& list_name, const std::vector<std::string>& songs);
    static void freeSongs();
    static void setListName(const std::string& name);
    static const std::vector<std::string>& getSongList();
    static uint8_t getMaximumNumberOfSongs();
    const static std::string getCurrentSong();
    const static std::string getCurrentPart();
    static void setCurrentSongIndex(uint8_t song_index);
    static uint8_t getCurrentSongIndex();
    static void setCurrentPart(const std::string& part);
    static uint8_t getNumberOfSongs();
    
  private:
    static const int MAX_SONGS = 30;
    static std::string list_name;
    static std::vector<std::string> song_list;
    static uint8_t number_of_songs;
    static uint8_t current_song_index;
    static const uint8_t MAX_PART_LENGTH = 20;
    static std::string current_part;
};

#endif
