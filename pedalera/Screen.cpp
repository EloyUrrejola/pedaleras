#include "Screen.h"

Screen::Screen(Adafruit_SSD1351 *adafruit)
{
  screen = adafruit;
}

void Screen::begin()
{
  screen->begin();
}

void Screen::clean()
{
  screen->fillScreen(OLED_Color_Black);
}

void Screen::writeTempMessage(const std::string line1, const std::string line2)
{
  writeMessage(line1, line2);
  delay(TEMP_MESSAGE_DELAY);
  writeSongAndPart();
}

void Screen::writeMessage(const std::string line1, const std::string line2)
{
  screen->fillRect(0, 0, 128, 56, SCREEN_BG_COLOR);

  screen->setFont(message_font);
  screen->setTextSize(message_size);
  screen->setTextWrap(false);

  int16_t centered_x = getCenteredXFromText(line1);

  screen->setTextColor(message_color);
  screen->setCursor(centered_x, message_line1_y);
  screen->print(line1.c_str());

  centered_x = getCenteredXFromText(line2);

  screen->setCursor(centered_x, message_line2_y);
  screen->print(line2.c_str());
}

void Screen::writeSongAndPart()
{
  if (SongList::getNumberOfSongs() == 0) {
    writeMessage("No", "setlist");
    return;
  }

  const std::string song = SongList::getCurrentSong();
  const std::string part = SongList::getCurrentPart();

  screen->fillRect(0, 0, 128, 56, SCREEN_BG_COLOR);

  screen->setFont(song_name_font);
  screen->setTextSize(song_name_size);
  screen->setTextWrap(false);

  int16_t centered_x = getCenteredXFromText(song);

  screen->setTextColor(song_name_color);
  screen->setCursor(centered_x, song_name_y);
  screen->print(song.c_str());

  screen->setFont(song_part_font);
  screen->setTextSize(song_part_size);

  centered_x = getCenteredXFromText(part);

  screen->setTextColor(song_part_color);
  screen->setCursor(centered_x, song_part_y);
  screen->print(part.c_str());
}

void Screen::writeChord(std::string chord)
{
  screen->fillRect(0, 71, 128, 50, SCREEN_BG_COLOR);

  screen->setFont(chord_font);
  screen->setTextSize(chord_size);

  int16_t centered_x = getCenteredXFromText(chord);

  screen->setTextColor(chord_color);
  screen->setCursor(centered_x, chord_y);
  screen->print(chord.c_str());
}

void Screen::removeChord()
{
  screen->fillRect(0, 71, 128, 50, SCREEN_BG_COLOR);
}

void Screen::writeSettingsTitle(char *title)
{
  screen->fillRect(0, 0, 128, 50, SCREEN_BG_COLOR);

  screen->setFont(settings_font);
  screen->setTextSize(settings_size);

  uint16_t width = getTextWidth(title);
  int16_t centered_x = getCenteredXFromWidth(width);

  screen->setCursor(centered_x, settings_y);
  screen->setTextColor(settings_color);
  screen->print(title);

  screen->drawLine(centered_x, 26, centered_x + width, 26, settings_color);
}

void Screen::showSettingOptions(char **menu, uint8_t number_of_options, uint8_t selected_menu, uint8_t *option_values, bool *options_with_values)
{
  uint8_t line_height = settings_line_height;
  for (uint8_t i = 0; i < number_of_options; i++) {
    screen->setFont(settings_font);
    screen->setTextSize(settings_size);
    screen->setCursor(0, line_height * (i + 2) + 6);
    if (i == selected_menu) {
      screen->setTextColor(settings_color_selected);
    } else {
      screen->setTextColor(settings_color);
    }
    screen->print(menu[i]);
    
    if (options_with_values[i]) {
      screen->fillRect(settings_value_x, line_height * (i + 1) + 6, 128 - settings_value_x, line_height, SCREEN_BG_COLOR);
      char str_option_value[4];
      sprintf(str_option_value, "%u", option_values[i]);
      uint16_t width = getTextWidth(str_option_value);
      screen->setCursor(getAlignRightX(width), line_height * (i + 2) + 6);
      screen->setTextColor(settings_color);
      screen->print(str_option_value);
    }
  }
}

void Screen::showSettingOptionEdition(char **menu, uint8_t number_of_options, uint8_t selected_menu, uint8_t option_value)
{
  uint8_t line_height = settings_line_height;
  screen->setFont(settings_font);
  screen->setTextSize(settings_size);
  screen->setTextColor(settings_color);
  screen->setCursor(0, line_height * (selected_menu + 2) + 6);
  screen->print(menu[selected_menu]);

  screen->fillRect(settings_value_x, line_height * (selected_menu + 1) + 6, 128 - settings_value_x, line_height, SCREEN_BG_COLOR);

  char str_option_value[4];
  sprintf(str_option_value, "%u", option_value);
  uint16_t width = getTextWidth(str_option_value);
  screen->setCursor(getAlignRightX(width), line_height * (selected_menu + 2) + 6);
  screen->setTextColor(settings_color_selected);
  screen->print(str_option_value);
}

int16_t Screen::getCenteredXFromText(const std::string text)
{
  uint16_t width = getTextWidth(text);
  int16_t centered_x = (screen->width() / 2) - floor(width / 2);
  return centered_x > 0 ? centered_x : 0;
}

int16_t Screen::getCenteredXFromWidth(uint16_t width)
{
  int16_t centered_x = (screen->width() / 2) - floor(width / 2);
  return centered_x > 0 ? centered_x : 0;
}

uint16_t Screen::getTextWidth(const std::string text)
{
  int16_t x, y;
  uint16_t w, h;
  screen->getTextBounds(text.c_str(), 0, 0, &x, &y, &w, &h);
  return w;
}

int16_t Screen::getAlignRightX(uint16_t width)
{
  int16_t align_right_x = screen->width() - width - 1;
  return align_right_x > 0 ? align_right_x : 0;
}

void Screen::writeSongList(uint8_t first_song, uint8_t song_index, int direction, bool slide, bool move)
{
  const std::vector<std::string> songs = SongList::getSongList();
  screen->setTextColor(settings_song_name_color);
  if (slide) {
    doSlide(songs, first_song, song_index, direction);
  } else {
    if (direction == INIT || move) {
      clean();
    }
    writeSongs(songs, first_song, VISIBLE_SONGS, song_index - first_song, 0);
    if (!move) {
      delay(SLIDE_ADJUSTMENT_DELAY);
    }
  }
}

void Screen::doSlide(const std::vector<std::string> songs, uint8_t first_song, uint8_t song_index, int direction)
{
  int8_t start = 0;
  int8_t end = 0;
  uint8_t selected_index = song_index - first_song;

  if (direction == DOWN) {
    end = -settings_song_name_height;
    first_song --;
    selected_index ++;
  }
  if (direction == UP) {
    start = -settings_song_name_height;
  }
  
  float ypos = getY(start, end, 0, SLIDE_STEPS);

  for (uint8_t step = 1; step < SLIDE_STEPS; step ++) {
    if (step == 1) {
      removeLastSongs();
    } else {
      removeSongs(songs, first_song, ypos);
    }
    ypos = getY(start, end, step, SLIDE_STEPS);
    writeSongs(songs, first_song, VISIBLE_SONGS + 1, selected_index, ypos);
  }
  removeSongs(songs, first_song, ypos);
  if (direction == DOWN) {
    first_song ++;
    selected_index --;
  }
  writeSongs(songs, first_song, VISIBLE_SONGS, selected_index, 0);
}

float Screen::getY(int8_t start, int8_t end, uint8_t step, float total_steps)
{
  return start + step * (end - start) / total_steps;
}

void Screen::removeLastSongs()
{
  screen->setFont(settings_song_name_font);
  screen->setTextSize(settings_song_name_size);
  screen->setTextWrap(false);
  screen->setTextColor(OLED_Color_Black);
  for (uint8_t i = 0; i < last_songs.size(); i++) {
    screen->setCursor(0, settings_song_name_height * (i + 1));
    screen->print(last_songs[i].c_str());
  }
}

void Screen::removeSongs(const std::vector<std::string> songs, uint8_t first_song, int ypos)
{
  screen->setFont(settings_song_name_font);
  screen->setTextSize(settings_song_name_size);
  screen->setTextWrap(false);
  screen->setTextColor(OLED_Color_Black);
  for (uint8_t i = 0; i < VISIBLE_SONGS + 1; i ++) {
    screen->setCursor(0, settings_song_name_height * (i + 1) + ypos);
    screen->print(songs[i + first_song].c_str());
  }
}

void Screen::writeSongs(const std::vector<std::string> songs, uint8_t first_song, uint8_t number_of_songs, uint8_t selected_index, int ypos)
{
  screen->setFont(settings_song_name_font);
  screen->setTextSize(settings_song_name_size);
  screen->setTextWrap(false);
  uint8_t number_of_songs_to_print = songs.size() < number_of_songs ? songs.size() : number_of_songs;
  for (uint8_t i = 0; i < number_of_songs_to_print; i ++) {
    if (i == selected_index) {
      screen->setTextColor(settings_song_name_color_selected);
    } else {
      screen->setTextColor(settings_song_name_color);
    }
    screen->setCursor(0, settings_song_name_height * (i + 1) + ypos);
    screen->print(songs[i + first_song].c_str());
  }

  last_songs.clear();
  for (uint8_t i = 0; i < number_of_songs_to_print; i++) {
    last_songs.push_back(songs[i + first_song]);
  }
}

void Screen::showTuningBackground()
{
  screen->fillRect(50, 0, tuner_sides_width, 128, tuner_color_sides);
  screen->fillRect(50, 60, tuner_sides_width, 8, tuner_color_center);
}

void Screen::showNote(char *note)
{
  screen->fillRect(0, 28, 28, 18, OLED_Color_Black);

  screen->setFont(tuner_chord_font);
  screen->setTextSize(tuner_chord_font_size);
  screen->setTextColor(chord_color);
  screen->setCursor(0, 46);
  screen->print(note);
}

void Screen::showTuning(uint8_t tuning, uint8_t last_tuning)
{
  if (last_tuning < 60 || last_tuning > 67) {
    screen->fillRect(40, 127 - last_tuning - 1, tuner_bar_width, 2, OLED_Color_Black);
    screen->fillRect(50, 127 - last_tuning - 1, tuner_sides_width, 2, tuner_color_sides);
  } else {
    screen->fillRect(40, 127 - last_tuning - 1, tuner_bar_width, 2, OLED_Color_Black);
  }

  screen->fillRect(40, 127 - tuning - 1, tuner_bar_width, 2, tuner_color_tuning);
}

void Screen::showClockBackground()
{
  clean();
}

void Screen::showClock(int hours, int minutes, int seconds, int day, int month, int year)
{
  clean();
  screen->setFont(clock_hour_font);
  screen->setTextSize(clock_font_size);
  screen->setTextColor(clock_hour_color);
  screen->setCursor(20, 65);
  char hour_txt[6];
  hour_txt[0] = '0' + ((hours / 10) % 10);
  hour_txt[1] = '0' + (hours % 10);
  hour_txt[2] = ':';
  hour_txt[3] = '0' + ((minutes / 10) % 10);
  hour_txt[4] = '0' + (minutes % 10);
  hour_txt[5] = 0;
  screen->print(hour_txt);

  screen->setFont(clock_date_font);
  screen->setTextSize(clock_font_size);
  screen->setTextColor(clock_date_color);
  screen->setCursor(18, 90);
  char date_txt[11];
  date_txt[0] = '0' + ((day / 10) % 10);
  date_txt[1] = '0' + (day % 10);
  date_txt[2] = '/';
  date_txt[3] = '0' + ((month / 10) % 10);
  date_txt[4] = '0' + (month % 10);
  date_txt[5] = '/';
  date_txt[6] = '0' + ((year / 1000) % 10);
  date_txt[7] = '0' + ((year / 100) % 10);
  date_txt[8] = '0' + ((year / 10) % 10);
  date_txt[9] = '0' + (year % 10);
  date_txt[10] = 0;
  screen->print(date_txt);
}
