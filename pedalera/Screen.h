#ifndef SCREEN_H
#define SCREEN_H

#include <Arduino.h>
#include <string>
#include <vector>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1351.h>
#include <SPI.h>

#include "SongList.h"

#include <Fonts/MyFonts/Roboto_Condensed_24.h>
#include <Fonts/MyFonts/Roboto_Condensed_Bold_12.h>
#include <Fonts/MyFonts/Roboto_Condensed_Bold_18.h>
#include <Fonts/MyFonts/Open_Sans_Condensed_Light_18.h>
#include <Fonts/MyFonts/Roboto_Condensed_Bold_24.h>
#include <Fonts/MyFonts/Open_Sans_Condensed_Light_24.h>
#include <Fonts/MyFonts/Open_Sans_Condensed_Bold_18.h>
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSansBold18pt7b.h>

class Screen
{
  public:
    Screen(Adafruit_SSD1351 *adafruit);
    void begin();
    void clean();
    void writeTempMessage(const std::string line1, const std::string line2);
    void writeMessage(const std::string line1, const std::string line2);
    void writeSongAndPart();
    void writeChord(std::string chord);
    void removeChord();
    void writeSettingsTitle(char *title);
    void showSettingOptions(char **menu, uint8_t number_of_options, uint8_t selected_option, uint8_t *option_values, bool *options_with_values);
    void showSettingOptionEdition(char **menu, uint8_t number_of_options, uint8_t selected_menu, uint8_t option_value);
    void writeSongList(uint8_t first_song, uint8_t song_index, int direction, bool slide, bool move);
    //void writeSongList(std::vector<std::string> songs, uint8_t selected_song_index, uint8_t number_of_songs, int direction, bool slide, bool move);
    void showNote(char *note);
    void showTuning(uint8_t tuning, uint8_t last_tuning);
    void showTuningBackground();
    void showClockBackground();
    void showClock(int hours, int minutes, int seconds, int day, int month, int year);

  private:
    void doSlide(const std::vector<std::string> songs, uint8_t first_song, uint8_t song_index, int direction);
    //void doSlide(std::vector<std::string> songs, uint8_t selected_song_index, uint8_t number_of_songs, int direction);
    void removeLastSongs();
    void removeSongs(const std::vector<std::string> songs, uint8_t first_song, int ypos);
    void writeSongs(const std::vector<std::string> songs, uint8_t first_song, uint8_t number_of_songs, uint8_t selected_index, int ypos);
    float getY(int8_t start, int8_t end, uint8_t step, float total_steps);
    
    const int INIT = 0;
    const int UP = -1;
    const int DOWN = 1;
    
    const uint8_t VISIBLE_SONGS = 6;
    const uint32_t TEMP_MESSAGE_DELAY = 2000;
    const uint32_t SLIDE_ADJUSTMENT_DELAY = 80;
    const uint8_t SLIDE_STEPS = 6;
    std::vector<std::string> last_songs;

    const int OLED_Color_Black        = 0x0000;
    const int OLED_Color_Blue         = 0x001F;
    const int OLED_Color_Red          = 0xF800;
    const int OLED_Color_Green        = 0x07E0;
    const int OLED_Color_Cyan         = 0x07FF;
    const int OLED_Color_Magenta      = 0xF81F;
    const int OLED_Color_Yellow       = 0xFFE0;
    const int OLED_Color_White        = 0xFFFF;

    const int SCREEN_BG_COLOR         = OLED_Color_Black;

    const GFXfont *message_font = &Open_Sans_Condensed_Light_24;
    const int message_color  = OLED_Color_Cyan;
    const uint8_t message_x     = 0;
    const uint8_t message_line1_y     = 20;
    const uint8_t message_line2_y     = 50;
    const uint8_t message_size  = 1;

    const GFXfont *song_name_font = &Open_Sans_Condensed_Bold_18;
    const int  song_name_color = OLED_Color_White;
    const uint8_t song_name_x     = 0;
    const uint8_t song_name_y     = 20;
    const uint8_t song_name_size  = 1;

    const GFXfont *song_part_font = &Open_Sans_Condensed_Light_24;
    const int song_part_color  = OLED_Color_Cyan;
    const uint8_t song_part_x     = 0;
    const uint8_t song_part_y     = 50;
    const uint8_t song_part_size  = 1;

    const GFXfont *chord_font = &FreeSansBold18pt7b;
    const int  chord_color = 0x8FCF;
    const uint8_t chord_x     = 0;
    const uint8_t chord_y     = 100;
    const uint8_t chord_size  = 1;

    const GFXfont *settings_font = &Open_Sans_Condensed_Light_18;
    const int settings_color = OLED_Color_Cyan;
    const int settings_color_selected = OLED_Color_Yellow;
    const uint8_t settings_y       = 20;
    const uint8_t settings_value_x = 110;
    const uint8_t settings_line_height = 20;
    const uint8_t settings_size  = 1;

    const GFXfont *settings_song_name_font = &Open_Sans_Condensed_Light_18;
    //const int settings_song_name_color = 0xBDF7;
    const int settings_song_name_color = OLED_Color_Cyan;
    const int settings_song_name_color_selected = OLED_Color_Yellow;
    const uint8_t settings_song_name_x      = 0;
    const uint8_t settings_song_name_height = 21;
    const uint8_t settings_song_name_size  = 1;

    const GFXfont *tuner_chord_font = &Roboto_Condensed_24;
    const int tuner_chord_font_size = 1;
    const int tuner_color_sides = OLED_Color_Blue;
    const int tuner_color_tuning = OLED_Color_White;
    const int tuner_color_center = OLED_Color_Black;
    const uint8_t tuner_sides_width = 67;
    const uint8_t tuner_bar_width = 87;

    const GFXfont *clock_hour_font = &FreeSansBold18pt7b;
    const int clock_font_size = 1;
    const int clock_hour_color = OLED_Color_Cyan;
    const GFXfont *clock_date_font = &FreeSans9pt7b;
    const int clock_date_color = OLED_Color_Yellow;

    Adafruit_SSD1351 *screen;
    int16_t  getCenteredXFromText(const std::string text);
    int16_t  getCenteredXFromWidth(uint16_t width);
    uint16_t getTextWidth(const std::string text);
    int16_t  getAlignRightX(uint16_t width);
};

#endif