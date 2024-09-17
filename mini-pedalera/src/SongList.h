#ifndef SONG_LIST_H
#define SONG_LIST_H

#include <Arduino.h>
#include <string>
#include <vector>

class SongList
{
  public:
    static void addSongs(const std::string& list_name, const std::vector<std::string>& songs);
    static void addParts(const std::vector<std::string>& parts);
    static void setListName(const std::string& name);
    static const std::vector<std::string>& getSongList();
    static const std::vector<std::string>& getPartList();
    static uint8_t getMaximumNumberOfSongs();
    static uint8_t getMaximumNumberOfParts();
    const static std::string getCurrentSong();
    const static std::string getCurrentPart();
    static void setCurrentSongIndex(uint8_t song_index);
    static uint8_t getCurrentSongIndex();
    static void setCurrentSongPartIndex(uint8_t part_index);
    static uint8_t getCurrentSongPartIndex();
    static void setCurrentPart(const std::string& part);
    static uint8_t getNumberOfSongs();
    
  private:
    static void freeSongs();
    static void freeParts();
    static const int MAX_SONGS = 30;
    static const int MAX_PARTS = 20;
    static std::string list_name;
    static std::vector<std::string> song_list;
    static std::vector<std::string> part_list;
    static uint8_t number_of_songs;
    static uint8_t current_song_index;
    static uint8_t current_part_index;
    static const uint8_t MAX_PART_LENGTH = 20;
    static std::string current_part;
};

#endif
