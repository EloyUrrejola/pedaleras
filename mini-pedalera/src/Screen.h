#ifndef SCREEN_H
#define SCREEN_H

#include <Arduino.h>
#include <string>
#include <vector>

#include <ILI9488_t3.h>
#include <SPI.h>

#include "SongList.h"

#include <font_Arial.h>
#include <font_ArialBold.h>
#include <font_Michroma.h>
#include <font_Georgia.h>
#include <font_Impact.h>
#include <font_LiberationSans.h>
#include <font_LiberationSansBold.h>
#include <font_LiberationSansNarrowBold.h>

#define TFT_VCC 25

class Screen
{
  public:
    Screen(ILI9488_t3 *tft);
    void begin();
    void setBacklightLevel(uint8_t backlight_level);
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
    void writeStatusBar();
    void writeStatusBarParameter(uint8_t param_index, bool state);

  private:
    void doSlide(const std::vector<std::string> songs, uint8_t first_song, uint8_t song_index, int direction);
    //void doSlide(std::vector<std::string> songs, uint8_t selected_song_index, uint8_t number_of_songs, int direction);
    void removeLastSongs();
    void removeSongs(const std::vector<std::string> songs, uint8_t first_song, int ypos);
    void writeSongs(const std::vector<std::string> songs, uint8_t first_song, uint8_t number_of_songs, uint8_t selected_index, int ypos);
    float getY(int8_t start, int8_t end, uint8_t step, float total_steps);
    void writeButtonsMode(uint8_t mode);
    void writeButtonsModeBackground(uint8_t mode);
    //void writeBassStatus(bool status);
    //void writeBassBackground(bool status);
    void writeStatusParameter(
      const char* parameter_text,
      bool status,
      int text_off_color,
      int background_off_color,
      int text_on_color,
      int background_on_color,
      uint16_t x,
      uint16_t w
    );
    
    const int INIT = 0;
    const int UP = -1;
    const int DOWN = 1;
    
    const uint8_t VISIBLE_SONGS = 10;
    const uint32_t TEMP_MESSAGE_DELAY = 2000;
    const uint32_t SLIDE_ADJUSTMENT_DELAY = 80;
    const uint8_t SLIDE_STEPS = 6;
    std::vector<std::string> last_songs;

    const int SCREEN_BG_COLOR = ILI9488_BLACK;

    const ILI9341_t3_font_t message_font = Georgia_20;
    const int message_color  = ILI9488_CYAN;
    const uint16_t message_x          = 0;
    const uint16_t message_y          = 0;
    const uint16_t message_line1_y    = 109;
    const uint16_t message_line2_y    = 149;
    const uint16_t messages_height    = 178;
    const uint8_t message_size  = 1;

    const ILI9341_t3_font_t song_name_font = Michroma_20;
    const int  song_name_color = ILI9488_WHITE;
    const uint16_t song_name_x     = 0;
    const uint16_t song_name_y     = 39;
    const uint8_t song_name_size  = 1;

    const ILI9341_t3_font_t song_part_font = Arial_20;
    const int song_part_color  = ILI9488_CYAN;
    const uint16_t song_part_x     = 0;
    const uint16_t song_part_y     = 79;
    const uint8_t song_part_size  = 1;

    const uint16_t song_and_part_height = 110;

    const ILI9341_t3_font_t chord_font = LiberationSans_32_Bold;
    const int  chord_color = 0x8FCF;
    const uint16_t chord_x     = 0;
    const uint16_t chord_y     = 149;
    const uint16_t chord_h     = 46;
    const uint8_t chord_size  = 1;

    const ILI9341_t3_font_t settings_font = Arial_20;
    const int settings_color = ILI9488_CYAN;
    const int settings_color_selected = ILI9488_YELLOW;
    const uint16_t settings_y       = 20;
    const uint16_t settings_value_x = 110;
    const uint16_t settings_value_width = 100;
    const uint16_t settings_line_height = 34;
    const uint8_t settings_size  = 1;

    const ILI9341_t3_font_t settings_song_name_font = Michroma_16;
    //const int settings_song_name_color = 0xBDF7;
    const int settings_song_name_color = ILI9488_CYAN;
    const int settings_song_name_color_selected = ILI9488_YELLOW;
    const uint16_t settings_song_name_x      = 0;
    const uint16_t settings_song_name_height = 28;
    const uint8_t settings_song_name_size  = 1;

    const ILI9341_t3_font_t tuner_chord_font = Arial_20;
    const int tuner_chord_font_size = 1;
    const int tuner_color_sides = ILI9488_BLUE;
    const int tuner_color_tuning = ILI9488_WHITE;
    const int tuner_color_center = ILI9488_BLACK;
    const uint16_t tuner_sides_width = 67;
    const uint16_t tuner_bar_width = 87;

    const ILI9341_t3_font_t clock_hour_font = Arial_20;
    const int clock_font_size = 1;
    const int clock_hour_color = ILI9488_CYAN;
    const ILI9341_t3_font_t clock_date_font = Arial_20;
    const int clock_date_color = ILI9488_YELLOW;

    const ILI9341_t3_font_t status_bar_font = LiberationSansNarrow_18_Bold;
    const int status_bar_font_size = 1;
    const uint16_t status_bar_y = 294;
    const uint16_t status_bar_h = 25;
    const uint16_t status_bar_text_y = 299;

    const int effects_mode_color = ILI9488_BLACK;
    const int effects_mode_background_color = 0x0DE8;
    const int chords_mode_color = ILI9488_BLACK;
    const int chords_mode_background_color = ILI9488_RED;
    const uint16_t buttons_mode_x = 0;
    const uint16_t buttons_mode_w = 98;

    /*const int bass_on_background_color = ILI9488_CYAN;
    const int bass_off_background_color = ILI9488_BLACK;
    const int bass_text_on_color = ILI9488_BLACK;
    const int bass_text_off_color = ILI9488_CYAN;
    const uint16_t bass_x = 82;
    const uint16_t bass_w = 60;*/

    const int octave_on_background_color = ILI9488_CYAN;
    const int octave_off_background_color = ILI9488_BLACK;
    const int octave_text_on_color = ILI9488_BLACK;
    const int octave_text_off_color = ILI9488_CYAN;
    const uint16_t octave_x = 98;
    const uint16_t octave_w = 98;

    const int guitar_on_background_color = ILI9488_YELLOW;
    const int guitar_off_background_color = ILI9488_BLACK;
    const int guitar_text_on_color = ILI9488_BLACK;
    const int guitar_text_off_color = ILI9488_YELLOW;
    const uint16_t guitar_x = 196;
    const uint16_t guitar_w = 88;

    const int reverb_on_background_color = ILI9488_CYAN;
    const int reverb_off_background_color = ILI9488_BLACK;
    const int reverb_text_on_color = ILI9488_BLACK;
    const int reverb_text_off_color = ILI9488_CYAN;
    const uint16_t reverb_x = 284;
    const uint16_t reverb_w = 100;

    const int chorus_on_background_color = ILI9488_GREEN;
    const int chorus_off_background_color = ILI9488_BLACK;
    const int chorus_text_on_color = ILI9488_BLACK;
    const int chorus_text_off_color = ILI9488_GREEN;
    const uint16_t chorus_x = 384;
    const uint16_t chorus_w = 95;

    ILI9488_t3 *screen;
    void     removeMessage();
    int16_t  getCenteredXFromText(const std::string text);
    int16_t  getCenteredXFromWidth(uint16_t width);
    int16_t  getCenteredXFromTextInWidth(const std::string text, uint16_t width);
    uint16_t getTextWidth(const std::string text);
    int16_t  getAlignRightX(uint16_t width);
};

#endif