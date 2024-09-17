#include "SongList.h"

std::string SongList::list_name = "";
uint8_t SongList::current_song_index = 0;
uint8_t SongList::current_part_index = 0;
std::string SongList::current_part = "";
uint8_t SongList::number_of_songs = 0;
std::vector<std::string> SongList::song_list;
std::vector<std::string> SongList::part_list;

void SongList::addSongs(const std::string& list_name, const std::vector<std::string>& songs)
{
  freeSongs();

  SongList::list_name = list_name;

  song_list = songs;
  number_of_songs = songs.size();
}

void SongList::freeSongs()
{
  song_list.clear();
  number_of_songs = 0;
  current_part = "";
}

void SongList::addParts(const std::vector<std::string>& parts)
{
  freeParts();
  part_list = parts;
}

void SongList::freeParts()
{
  part_list.clear();
}

const std::vector<std::string>& SongList::getSongList()
{
  return song_list;
}

const std::vector<std::string>& SongList::getPartList()
{
  return part_list;
}

uint8_t SongList::getMaximumNumberOfSongs()
{
  return MAX_SONGS;
}

uint8_t SongList::getMaximumNumberOfParts()
{
  return MAX_PARTS;
}

const std::string SongList::getCurrentSong()
{
  return song_list[current_song_index];
}

const std::string SongList::getCurrentPart()
{
  return current_part;
}

void SongList::setCurrentSongIndex(uint8_t song_index)
{
  current_song_index = song_index;
}

uint8_t SongList::getCurrentSongIndex()
{
  return current_song_index;
}

void SongList::setCurrentSongPartIndex(uint8_t part_index)
{
  current_part_index = part_index;
}

uint8_t SongList::getCurrentSongPartIndex()
{
  return current_part_index;
}

void SongList::setCurrentPart(const std::string& part)
{
  current_part = part;
}

uint8_t SongList::getNumberOfSongs()
{
  return number_of_songs;
}
