#include "SysExMessage.h"
#include "Led.h"

SysExMessage::SysExMessage()
{
}

void SysExMessage::init(Screen *screen)
{
  this->screen = screen;
}

void SysExMessage::process(uint8_t *data, unsigned int length)
{
  std::string message = getMessage(data, length);
  replaceTildeVowels(message);

  std::string type = getMessageType(message);

  if (type == TYPE_LIST) {
    getSongListFromMessage(message);
  }
  if (type == TYPE_SONG) {
    getSongAndPartsFromMessage(message);
  }
  if (type == TYPE_CHORD) {
    getChord(message);
  }
  if (type == TYPE_TIME) {
    getDatetime(message);
  }
}

std::string SysExMessage::getMessage(uint8_t *data, unsigned int length)
{
  std::string message;
  for (unsigned int i = 1; i < (length - 1); i++) {
    message.push_back(static_cast<char>(data[i]));
  }
  return message;
}

void SysExMessage::replaceTildeVowels(std::string& message)
{
  char atilde = 'a';//char(160)
  char etilde = 'e';//char(130)
  char itilde = 'i';//char(161)
  char otilde = 'o';//char(162)
  char utilde = 'u';//char(163)
  for (size_t i = 0; i < message.length(); i++) {
    if (message[i] == '&') {
      char replacement = message[i + 1];
      switch (replacement) {
        case 'a': message[i] = atilde; break;
        case 'e': message[i] = etilde; break;
        case 'i': message[i] = itilde; break;
        case 'o': message[i] = otilde; break;
        case 'u': message[i] = utilde; break;
        default: message[i] = '&';
      }
      message.erase(i + 1, 1);
    }
  }
}

std::string SysExMessage::getMessageType(std::string message)
{
  size_t delimiter_position = message.find(TYPE_DELIMITER);
  if (delimiter_position != std::string::npos) {
    return message.substr(0, delimiter_position);
  }
  return "";
}

void SysExMessage::getSongListFromMessage(std::string message)
{
  size_t delimiter_position = message.find(TYPE_DELIMITER);
  if (delimiter_position == std::string::npos) {
    return;
  }

  delimiter_position++;
  std::string song_list_string = message.substr(delimiter_position);

  size_t list_delimiter_pos = song_list_string.find(LIST_DELIMITER);
  if (list_delimiter_pos == std::string::npos) {
    return;
  }

  std::string list_name = song_list_string.substr(0, list_delimiter_pos);
  song_list_string.erase(0, list_delimiter_pos + 1);
  const uint8_t max_songs = SongList::getMaximumNumberOfSongs();
  std::vector<std::string> song_list = getItemsFromList(song_list_string, max_songs);

  SongList::addSongs(list_name, song_list);

  if (!empty(song_list)) {
    screen->writeTempMessage(list_name.c_str(), "LOADED");
  }
}

std::vector<std::string> SysExMessage::getItemsFromList(std::string list_string, const uint8_t max_items)
{
  uint8_t item_count = 0;

  std::istringstream list_stream(list_string);
  std::string item;
  std::vector<std::string> item_list;

  while (std::getline(list_stream, item, SONG_DELIMITER) && item_count < max_items) {
    item_list.push_back(item);
    item_count++;
  }

  return item_list;
}

void SysExMessage::getSongAndPartsFromMessage(std::string message)
{
  if (SongList::getNumberOfSongs() == 0) {
    return;
  }
  
  uint8_t current_song_index = getSongIndexFromMessage(message);
  SongList::setCurrentSongIndex(current_song_index);

  uint8_t current_part_index = getSongPartIndexFromMessage(message);
  SongList::setCurrentSongPartIndex(current_part_index);

  size_t list_delimiter_pos = message.find(LIST_DELIMITER);
  if (list_delimiter_pos != std::string::npos) {
    std::string part_list_string = message.substr(list_delimiter_pos + 1);

    const uint8_t max_parts = SongList::getMaximumNumberOfParts();
    std::vector<std::string> part_list = getItemsFromList(part_list_string, max_parts);

    SongList::addParts(part_list);
  }
    
  screen->writeSongAndParts();
  Led::setStatusLeds();
}

uint8_t SysExMessage::getSongIndexFromMessage(std::string message)
{
  size_t delimiter1 = message.find(TYPE_DELIMITER);
  size_t delimiter2 = message.find('-');

  if (delimiter1 != std::string::npos) {
    if (delimiter2 != std::string::npos && delimiter2 > delimiter1) {
      std::string song_number = message.substr(delimiter1 + 1, delimiter2 - delimiter1 - 1);
      return std::stoi(song_number);
    } else {
      std::string song_number = message.substr(delimiter1 + 1);
      return std::stoi(song_number);
    }
  }
  return -1;
}

uint8_t SysExMessage::getSongPartIndexFromMessage(std::string message)
{
  size_t delimiter1 = message.find('-');
  size_t delimiter2 = message.find('%');

  if (delimiter1 != std::string::npos && delimiter2 != std::string::npos && delimiter2 > delimiter1) {
    std::string part_number = message.substr(delimiter1 + 1, delimiter2 - delimiter1 - 1);
    return std::stoi(part_number);
  }
  return -1;
}

std::string SysExMessage::getPartFromMessage(std::string message)
{
  size_t delimiter = message.find('-');
  if (delimiter != std::string::npos) {
    return message.substr(delimiter + 1);
  }
  return "";
}

void SysExMessage::getChord(std::string message)
{
  std::string chord = getChordFromMessage(message);

  if (chord != "0") {
    screen->writeChord(chord);
  } else {
    screen->removeChord();
  }
}

std::string SysExMessage::getChordFromMessage(std::string message)
{
  size_t delimiter = message.find(TYPE_DELIMITER);
  if (delimiter != std::string::npos) {
    return message.substr(delimiter + 1);
  } else {
    return "";
  }
}

void SysExMessage::getDatetime(std::string message)
{
  int current_time = getDatetimeFromMessage(message);
  MyClock::setDatetime(current_time);
}

int SysExMessage::getDatetimeFromMessage(std::string message)
{
  int datetime = 0;
  size_t delimiter = message.find(TYPE_DELIMITER);
  if (delimiter != std::string::npos) {
    std::string datetime_str = message.substr(delimiter + 1);
    datetime = std::stoi(datetime_str);
  }
  return datetime;
}
