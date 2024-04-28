#ifndef SYS_EX_MESSAGE_H
#define SYS_EX_MESSAGE_H

#include <Arduino.h>
#include <sstream>
#include <string>

#include "MyClock.h"
#include "Screen.h"
#include "SongList.h"

class SysExMessage
{
  public:
    SysExMessage();
    void init(Screen *screen);
    void process(uint8_t *data, unsigned int length);
    
  private:
    Screen *screen;
    const std::string TYPE_LIST = "list";
    const std::string TYPE_SONG = "song";
    const std::string TYPE_CHORD = "chord";
    const std::string TYPE_TIME = "time";
    const char TYPE_DELIMITER = ':';
    const char LIST_DELIMITER = '%';
    const char SONG_DELIMITER = '|';

    std::string getMessage(uint8_t *data, unsigned int length);
    void replaceTildeVowels(std::string& message);
    std::string getMessageType(std::string message);
    void getSongListFromMessage(std::string message);
    void getSongAndPartFromMessage(std::string message);
    uint8_t getSongIndexFromMessage(std::string message);
    std::string getPartFromMessage(std::string message);
    void getChord(std::string message);
    std::string getChordFromMessage(std::string message);
    void getDatetime(std::string message);
    int getDatetimeFromMessage(std::string message);

    void hasNullTerminator(const char* str);
};

#endif