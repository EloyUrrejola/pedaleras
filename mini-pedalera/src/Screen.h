#ifndef SCREEN_H
#define SCREEN_H

#define DARK_GREY 0x630B

#include <Arduino.h>
#include <string>
#include <vector>
#include <TimeLib.h>
#include <Timezone.h>
#include <ctime>

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
    void writeSongAndParts();
    void writeChord(std::string chord);
    void removeChord();
    void writeSectionTitle(char *title);
    void showSettingOptions(char **menu, uint8_t number_of_options, uint8_t selected_option, uint8_t *option_values, bool *options_with_values);
    void showSettingOptionEdition(char **menu, uint8_t number_of_options, uint8_t selected_menu, uint8_t option_value);
    void writeSongList(uint8_t first_song, uint8_t song_index, int direction, bool slide, bool move);
    //void writeSongList(std::vector<std::string> songs, uint8_t selected_song_index, uint8_t number_of_songs, int direction, bool slide, bool move);
    void showNote(char *note);
    void showTuning(int8_t tuning, int8_t last_tuning);
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
    void writeParts(std::vector<std::string> part_list, uint8_t current_part);
    void writePartView(std::vector<std::string> part_list, uint8_t first_part_index, uint8_t last_part_index, uint8_t current_part_index);
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
    uint16_t calculateNeedlePosition(int8_t tuning);
    
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
    const uint16_t song_name_x     = 30;
    const uint16_t song_name_y     = 9;
    const uint8_t song_name_size  = 1;

    const ILI9341_t3_font_t song_part_font = Arial_20;
    const int song_part_number_color = ILI9488_WHITE;
    const int song_part_color  = ILI9488_CYAN;
    const int song_part_color_selected = ILI9488_YELLOW;
    const int song_part_name_height = 34;
    const uint16_t song_part_x     = 0;
    const uint16_t song_part_y     = 17;
    const uint16_t song_part_height = 260;
    const uint8_t song_part_size  = 1;

    const ILI9341_t3_font_t chord_font = LiberationSansNarrow_40_Bold;
    const int chord_bg_color = ILI9488_GREEN;
    //const int  chord_color = 0x8FCF;
    const int chord_color     = ILI9488_BLACK;
    const uint16_t chord_x     = 120;
    const uint16_t chord_y     = 12;
    const uint16_t chord_bg_x     = 239;
    const uint16_t chord_bg_y     = 99;
    const uint16_t chord_bg_w     = 260;
    const uint16_t chord_bg_h     = 66;
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

    const ILI9341_t3_font_t tuner_chord_font = LiberationSansNarrow_40_Bold;
    const int tuner_chord_font_size = 1;
    const int tuner_color_notes = ILI9488_GREEN;
    const int tuner_color_tuning = ILI9488_WHITE;
    const int tuner_color_center = ILI9488_DARKGREY;
    const int tuner_color_perfect = ILI9488_GREEN;
    const int tuner_color_out = ILI9488_RED;
    const uint16_t tuner_needle_width = 14;

    const ILI9341_t3_font_t clock_hour_font = Arial_24;
    const int clock_font_size = 1;
    const int clock_hour_color = 0xAD55;
    const int clock_background_color = ILI9488_BLACK;
    const ILI9341_t3_font_t clock_date_font = Arial_20;
    const int clock_date_color = ILI9488_YELLOW;
    const uint16_t clock_x = 394;
    const uint16_t clock_y = 4;
    const uint16_t clock_w = 86;
    const uint16_t clock_h = 28;

    const ILI9341_t3_font_t status_bar_font = LiberationSansNarrow_16_Bold;
    const int status_bar_font_size = 1;
    const uint16_t status_bar_y = 296;
    const uint16_t status_bar_h = 23;
    const uint16_t status_bar_text_y = 300;

    const int effects_mode_color = ILI9488_BLACK;
    const int effects_mode_background_color = 0x0DE8;
    const int chords_mode_color = ILI9488_BLACK;
    const int chords_mode_background_color = ILI9488_RED;
    const uint16_t buttons_mode_x = 0;
    const uint16_t buttons_mode_w = 93;

    /*const int bass_on_background_color = ILI9488_CYAN;
    const int bass_off_background_color = ILI9488_BLACK;
    const int bass_text_on_color = ILI9488_BLACK;
    const int bass_text_off_color = ILI9488_CYAN;
    const uint16_t bass_x = 82;
    const uint16_t bass_w = 60;*/

    /*const int octave_on_background_color = ILI9488_CYAN;
    const int octave_off_background_color = ILI9488_BLACK;
    const int octave_text_on_color = ILI9488_BLACK;
    const int octave_text_off_color = ILI9488_CYAN;*/
    const int octave_on_background_color = ILI9488_CYAN;
    const int octave_off_background_color = ILI9488_BLACK;
    const int octave_text_on_color = ILI9488_BLACK;
    const int octave_text_off_color = DARK_GREY;
    const uint16_t octave_x = 93;
    const uint16_t octave_w = 85;

    /*const int guitar_on_background_color = ILI9488_YELLOW;
    const int guitar_off_background_color = ILI9488_BLACK;
    const int guitar_text_on_color = ILI9488_BLACK;
    const int guitar_text_off_color = ILI9488_YELLOW;*/
    const int guitar_on_background_color = ILI9488_YELLOW;
    const int guitar_off_background_color = ILI9488_BLACK;
    const int guitar_text_on_color = ILI9488_BLACK;
    const int guitar_text_off_color = DARK_GREY;
    const uint16_t guitar_x = 178;
    const uint16_t guitar_w = 79;

    /*const int reverb_on_background_color = ILI9488_CYAN;
    const int reverb_off_background_color = ILI9488_BLACK;
    const int reverb_text_on_color = ILI9488_BLACK;
    const int reverb_text_off_color = ILI9488_CYAN;*/
    const int reverb_on_background_color = ILI9488_CYAN;
    const int reverb_off_background_color = ILI9488_BLACK;
    const int reverb_text_on_color = ILI9488_BLACK;
    const int reverb_text_off_color = DARK_GREY;
    const uint16_t reverb_x = 257;
    const uint16_t reverb_w = 86;

    /*const int midi_on_background_color = 0xF9A0;
    const int midi_off_background_color = ILI9488_BLACK;
    const int midi_text_on_color = ILI9488_BLACK;
    const int midi_text_off_color = 0xF9A0;*/
    const int midi_on_background_color = 0xF9A0;
    const int midi_off_background_color = ILI9488_BLACK;
    const int midi_text_on_color = ILI9488_BLACK;
    const int midi_text_off_color = DARK_GREY;
    const uint16_t midi_x = 432;//343;
    const uint16_t midi_w = 48;

    /*const int chorus_on_background_color = ILI9488_GREEN;
    const int chorus_off_background_color = ILI9488_BLACK;
    const int chorus_text_on_color = ILI9488_BLACK;
    const int chorus_text_off_color = ILI9488_GREEN;*/
    const int chorus_on_background_color = ILI9488_GREEN;
    const int chorus_off_background_color = ILI9488_BLACK;
    const int chorus_text_on_color = ILI9488_BLACK;
    const int chorus_text_off_color = DARK_GREY;
    const uint16_t chorus_x = 343;//391;
    const uint16_t chorus_w = 89;

    ILI9488_t3 *screen;
    void     removeMessage();
    int16_t  getCenteredXFromText(const std::string text);
    int16_t  getCenteredXFromWidth(uint16_t width);
    int16_t  getCenteredXFromTextInWidth(const std::string text, uint16_t width);
    uint16_t getTextWidth(const std::string text);
    int16_t  getAlignRightX(uint16_t width);
};

#endif