#include "Screen.h"
#include "Status.h"

Screen::Screen(ILI9488_t3 *tft)
{
  screen = tft;
}

void Screen::begin()
{
  screen->begin();
  screen->setRotation(3);
  pinMode(TFT_VCC, OUTPUT);
  setBacklightLevel(160);
}

void Screen::setBacklightLevel(uint8_t backlight_level)
{
  analogWrite(TFT_VCC, backlight_level);
}

void Screen::clean()
{
  screen->fillScreen(ILI9488_BLACK);
}

void Screen::writeTempMessage(const std::string line1, const std::string line2)
{
  clean();
  writeMessage(line1, line2);
  delay(TEMP_MESSAGE_DELAY);
  clean();
}

void Screen::removeMessage()
{
  screen->fillRect(0, message_y, ILI9488_TFTHEIGHT, messages_height, SCREEN_BG_COLOR);
}

void Screen::writeMessage(const std::string line1, const std::string line2)
{
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

void Screen::writeSongAndParts()
{
  if (SongList::getNumberOfSongs() == 0) {
    writeMessage("No", "setlist");
    return;
  }
  screen->fillRect(song_name_x, 0, song_name_width, song_name_height, SCREEN_BG_COLOR);
  screen->fillRect(0, 36, 240, song_part_height, SCREEN_BG_COLOR);

  screen->setFont(song_name_font);
  screen->setTextSize(song_name_size);
  screen->setTextWrap(false);

  const uint8_t song_number = SongList::getCurrentSongIndex() + 1;
  char song_number_text[4]; // suficiente para uint8_t (0-255)
  sprintf(song_number_text, "%d", song_number);
  
  screen->fillRoundRect(song_number_bg_x, song_number_bg_y, song_number_bg_w, song_number_bg_h, 3, song_number_bg_color);

  screen->setFont(song_number_font);
  screen->setTextColor(song_number_color);
  screen->setCursor(song_number_bg_x + getCenteredXFromTextInWidth(song_number_text, song_number_bg_w) - 1, song_number_bg_y + song_number_y_rel);
  screen->print(song_number);

  screen->setFont(song_name_font);
  screen->setTextColor(song_name_color);
  const std::string song = SongList::getCurrentSong();
  screen->setCursor(song_name_x + getCenteredXFromTextInWidth(song.c_str(), song_name_width), song_name_y);
  screen->print(song.c_str());

  std::vector<std::string> part_list = SongList::getPartList();
  writeParts(part_list, SongList::getCurrentSongPartIndex());

  time_t current_time = now();
  showClock(hour(current_time), minute(current_time), second(current_time), day(current_time), month(current_time), year(current_time));
}

void Screen::writeParts(std::vector<std::string> part_list, uint8_t current_part)
{
  screen->setFont(song_part_font);
  screen->setTextSize(song_part_size);

  uint8_t number_of_visible_parts = 7;
  uint8_t fixed_index = 3;

  uint8_t number_of_parts = part_list.size();
  uint8_t first_part_index = 0;
  uint8_t last_part_index = 0;

  if (number_of_parts <= number_of_visible_parts) {
    first_part_index = 0;
    last_part_index = number_of_parts - 1;
  } else {
    if (current_part <= fixed_index) {
      first_part_index = 0;
      last_part_index = number_of_visible_parts - 1;
    } else {
      if (current_part <= number_of_parts - (number_of_visible_parts - fixed_index)) {
        first_part_index = current_part - fixed_index;
        last_part_index = first_part_index + number_of_visible_parts - 1;
      } else {
        first_part_index = number_of_parts - number_of_visible_parts;
        last_part_index = number_of_parts - 1;
      }
    }
  }
  writePartView(part_list, first_part_index, last_part_index, current_part);
}

void Screen::writePartView(std::vector<std::string> part_list, uint8_t first_index, uint8_t last_index, uint8_t current_part)
{
  for (uint8_t i = first_index; i <= last_index; i ++) {
    int text_color = song_part_color;
    int bg_color = SCREEN_BG_COLOR;
    if (i == current_part) {
      text_color = song_part_color_selected;
      bg_color   = song_part_bg_color_selected;
    }
    screen->setTextColor(song_part_number_color);
    int y_pos = song_part_name_height * (i - first_index + 1) + song_part_y;
    screen->fillRect(song_part_x, y_pos - 4, song_part_bg_width, song_part_bg_height, bg_color);
    screen->setCursor(song_part_x, y_pos);
    screen->print(i + 1);
    screen->print(". ");
    screen->setTextColor(text_color);
    screen->print(part_list[i].c_str());
  }
}

void Screen::writeChord(std::string chord)
{
  screen->fillRoundRect(chord_bg_x + chord_shadow_w, chord_bg_y + chord_shadow_h, chord_bg_w, chord_bg_h, 8, chord_shador_color);
  screen->fillRoundRect(chord_bg_x, chord_bg_y, chord_bg_w, chord_bg_h, 8, chord_bg_color);
  screen->drawRoundRect(chord_bg_x, chord_bg_y, chord_bg_w, chord_bg_h, 8, ILI9488_LIGHTGREY);
  screen->setFont(chord_font);
  screen->setTextSize(chord_size);
  screen->setTextColor(chord_color);
  screen->setCursor(chord_bg_x + getCenteredXFromTextInWidth(chord.c_str(), chord_bg_w), chord_bg_y + chord_y);
  screen->print(chord.c_str());
}

void Screen::removeChord()
{
  screen->fillRect(chord_bg_x, chord_bg_y, chord_bg_w + chord_shadow_w, chord_bg_h + chord_shadow_h, SCREEN_BG_COLOR);
}

void Screen::writeSectionTitle(char *title)
{
  screen->setFont(settings_font);
  screen->setTextSize(settings_size);

  uint16_t width = getTextWidth(title);
  int16_t centered_x = getCenteredXFromWidth(width);

  screen->setCursor(centered_x, settings_y);
  screen->setTextColor(settings_color);
  screen->print(title);
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
      screen->fillRect(screen->width() - settings_value_width, line_height * (i + 2) + 6, settings_value_width, line_height, SCREEN_BG_COLOR);
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

  screen->fillRect(screen->width() - settings_value_width, line_height * (selected_menu + 2) + 6, settings_value_width, line_height, SCREEN_BG_COLOR);

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

int16_t Screen::getCenteredXFromTextInWidth(const std::string text, uint16_t width)
{
  uint16_t text_width = getTextWidth(text);
  int16_t centered_x = (width / 2) - floor(text_width / 2);
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
  screen->setTextColor(ILI9488_BLACK);
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
  screen->setTextColor(ILI9488_BLACK);
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
  screen->fillRect(239 - tuner_needle_width / 2, 48, tuner_needle_width, 216, tuner_color_center);
}

void Screen::showNote(char *note)
{
  screen->fillRect(0, 275, 479, 44, ILI9488_BLACK);

  screen->setFont(tuner_chord_font);
  screen->setTextSize(tuner_chord_font_size);
  screen->setTextColor(tuner_color_notes);
  screen->setCursor(getCenteredXFromText(note), 277);
  screen->print(note);
}

void Screen::showTuning(int8_t tuning, int8_t last_tuning)
{
  int16_t last_x = calculateNeedlePosition(last_tuning);
  int16_t x = calculateNeedlePosition(tuning);

  int bg_color = ILI9488_BLACK;
  int tuner_color = tuner_color_tuning;
  if (last_tuning == 0) {
    bg_color = tuner_color_center;
  }
  if (tuning == 0) {
    tuner_color = tuner_color_perfect;
  } else if (x == 0 or x == ILI9488_TFTHEIGHT - tuner_needle_width) {
    tuner_color = tuner_color_out;
  }

  screen->fillRect(last_x, 48, tuner_needle_width, 216, bg_color);
  screen->fillRect(x, 48, tuner_needle_width, 216, tuner_color);
}

uint16_t Screen::calculateNeedlePosition(int8_t tuning)
{
  int16_t x = (ILI9488_TFTHEIGHT / 2 - tuner_needle_width / 2) + (tuning * tuner_needle_width);

  if (x < 0) {
    x = 0;
  } else if (x > (ILI9488_TFTHEIGHT - tuner_needle_width)) {
    x = ILI9488_TFTHEIGHT - tuner_needle_width;
  }
  
  return x;
}

void Screen::showClockBackground()
{
  clean();
}

void Screen::showClock(int hours, int minutes, int seconds, int day, int month, int year)
{
  screen->fillRect(clock_x, clock_y, clock_w, clock_h, clock_background_color);

  screen->setFont(clock_hour_font);
  screen->setTextSize(clock_font_size);
  screen->setTextColor(clock_hour_color);
  char hour_txt[6];
  hour_txt[0] = '0' + ((hours / 10) % 10);
  hour_txt[1] = '0' + (hours % 10);
  hour_txt[2] = ':';
  hour_txt[3] = '0' + ((minutes / 10) % 10);
  hour_txt[4] = '0' + (minutes % 10);
  hour_txt[5] = 0;
  screen->setCursor(clock_x + getCenteredXFromTextInWidth(hour_txt, clock_w), clock_y);
  screen->print(hour_txt);
}

void Screen::writeStatusBarParameter(uint8_t param_index, bool state)
{
  screen->setFont(status_bar_font);
  screen->setTextSize(status_bar_font_size);
  
  if (param_index == 0) {
    writeButtonsMode(state ? 1 : 0);
  }
  if (param_index == 2) {
    writeOctaveParameter(state);
  }
  if (param_index == 3) {
    writeGuitarParameter(state);
  }
  if (param_index == 4) {
    writeReverbParameter(state);
  }
  if (param_index == 5) {
    writeMidiParameter(state);
  }
  if (param_index == 6) {
    writeChorusParameter(state);
  }
  if (param_index == 12) {
    writeBassStatus();
  }
}

void Screen::writeStatusBar()
{
  const bool* params = Status::getAll();

  screen->setFont(status_bar_font);
  screen->setTextSize(status_bar_font_size);

  writeButtonsMode(params[0] ? 1 : 0);
  writeOctaveParameter(params[2]);
  writeGuitarParameter(params[3]);
  writeReverbParameter(params[4]);
  writeMidiParameter(params[5]);
  writeChorusParameter(params[6]);
}

void Screen::writeButtonsMode(uint8_t mode)
{
  writeButtonsModeBackground(mode);
  int text_color = effects_mode_color;
  char mode_text[12] = "EFX";
  if (mode == 1) {
    text_color = chords_mode_color;
    strcpy(mode_text, "CHD");
  }
  screen->setTextColor(text_color);
  screen->setCursor(buttons_mode_x + getCenteredXFromTextInWidth(mode_text, buttons_mode_w), status_bar_text_y);
  screen->print(mode_text);
}

void Screen::writeButtonsModeBackground(uint8_t mode)
{
  int background_color = effects_mode_background_color;
  if (mode == 1) {
    background_color = chords_mode_background_color;
  }
  screen->drawLine(0, status_bar_y - 2, 479, status_bar_y - 2, background_color);
  screen->drawLine(0, status_bar_y - 1, 479, status_bar_y - 1, background_color);
  
  screen->fillRect(buttons_mode_x, status_bar_y, buttons_mode_w, status_bar_h, background_color);
}

void Screen::writeOctaveParameter(bool status)
{
  writeStatusParameter(
    "OCTAVE",
    status,
    octave_text_off_color,
    octave_off_background_color,
    octave_text_on_color,
    octave_on_background_color,
    octave_x,
    octave_w
  );
}

void Screen::writeGuitarParameter(bool status)
{
  writeStatusParameter(
    "GUITAR",
    status,
    guitar_text_off_color,
    guitar_off_background_color,
    guitar_text_on_color,
    guitar_on_background_color,
    guitar_x,
    guitar_w
  );
}

void Screen::writeReverbParameter(bool status)
{
  writeStatusParameter(
    "REVERB",
    status,
    reverb_text_off_color,
    reverb_off_background_color,
    reverb_text_on_color,
    reverb_on_background_color,
    reverb_x,
    reverb_w
  );
}

void Screen::writeChorusParameter(bool status)
{
  writeStatusParameter(
    "CHORUS",
    status,
    chorus_text_off_color,
    chorus_off_background_color,
    chorus_text_on_color,
    chorus_on_background_color,
    chorus_x,
    chorus_w
  );
}

void Screen::writeMidiParameter(bool status)
{
  writeStatusParameter(
    "MIDI",
    status,
    midi_text_off_color,
    midi_off_background_color,
    midi_text_on_color,
    midi_on_background_color,
    midi_x,
    midi_w
  );
}

void Screen::writeStatusParameter(
  const char* parameter_text,
  bool status,
  int text_off_color,
  int background_off_color,
  int text_on_color,
  int background_on_color,
  uint16_t x,
  uint16_t w
)
{
  int text_color = text_off_color;
  int background_color = background_off_color;
  if (status) {
    text_color = text_on_color;
    background_color = background_on_color;
  }

  screen->fillRect(x, status_bar_y, w, status_bar_h, background_color);
  screen->setTextColor(text_color);
  screen->setCursor(x + getCenteredXFromTextInWidth(parameter_text, w), status_bar_text_y);
  screen->print(parameter_text);
}

void Screen::writeBassStatus()
{
  int background_color = bass_status_background_off_color;
  int text_color = bass_status_off_color;
  if (Status::isBassActive()) {
    background_color = bass_status_background_on_color;
    text_color = bass_status_on_color;
  }
  screen->fillRect(bass_status_x, bass_status_y, bass_status_w, bass_status_h, background_color);

  screen->setFont(bass_status_font);
  screen->setTextSize(bass_status_font_size);
  screen->setTextColor(text_color);
  const char* bass_text = Status::getInstrument();
  screen->setCursor(bass_status_x + getCenteredXFromTextInWidth(bass_text, bass_status_w), bass_status_y + bass_status_text_y);
  screen->print(bass_text);
}
