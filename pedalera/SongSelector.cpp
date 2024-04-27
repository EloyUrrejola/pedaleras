#include "SongSelector.h"

SongSelector::SongSelector()
{
}

void SongSelector::init(Screen *screen, Button **buttons, uint8_t number_of_buttons, Led **leds, uint8_t number_of_leds)
{
  this->screen = screen;
  this->buttons = buttons;
  this->number_of_buttons = number_of_buttons;
  this->leds = leds;
  this->number_of_leds = number_of_leds;
}

void SongSelector::startSongSelectorMode()
{
  temp_song_index = SongList::getCurrentSongIndex();
  song_list = SongList::getSongList();
  screen->clean();
  for (uint8_t i = 0; i < 4; i++) {
    leds[leds_index[i]]->flash(LED_FLASHING_ON, LED_FLASHING_OFF, -1);
  }
  showSongSelectorPanel(temp_song_index, INIT);
}

void SongSelector::songSelectorMode()
{
  bool settings_mode = true;
  while (settings_mode) {
    for (uint8_t i = 0; i < 4; i++) {
      leds[leds_index[i]]->flashUpdate();
    }
    for (uint8_t i = 0; i < number_of_buttons; i++) {
      uint8_t action = buttons[i]->settingsChanged();
      if (action == _pg_up && temp_song_index > 0) {
        temp_song_index = temp_song_index + UP;
        showSongSelectorPanel(temp_song_index, UP);
      }
      if (action == _pd_dn && temp_song_index < SongList::getNumberOfSongs() - 1) {
        temp_song_index = temp_song_index + DOWN;
        showSongSelectorPanel(temp_song_index, DOWN);
      }
      if (action == _cancel) {
        settings_mode = false;
      }
      if (action == _select) {
        if (temp_song_index != -1) {
          current_song_index = temp_song_index;
          temp_song_index = -1;
          settings_mode = false;
          usbMIDI.sendProgramChange(current_song_index, 1);
        }
      }
    }
  }
}

void SongSelector::exitSongSelectorMode()
{
  for (uint8_t i = 0; i < 4; i++) {
    leds[leds_index[i]]->off();
  }
}

void SongSelector::showSongSelectorPanel(uint8_t song_index, int direction)
{
  uint8_t first_song = 0;
  uint8_t max_number_of_visible_songs = 6;
  uint8_t number_of_songs = SongList::getNumberOfSongs();
  bool slide = false;

  if (number_of_songs < max_number_of_visible_songs) {
    first_song = 0;
  } else {
    if (direction == UP) {
      if (song_index > 3) {
        first_song = song_index - 4;
        if (song_index < number_of_songs - 2) {
          slide = true;
        }
      } else {
        first_song = 0;
      }
    }
    if (direction == DOWN) {
      uint8_t last_song = number_of_songs - 1;
      if (song_index > last_song - 4) {
        first_song = last_song - 5;
      } else {
        if (song_index > 1) {
          slide = true;
        }
        first_song = song_index - 1;
      }
    }
    if (direction == INIT) {
      uint8_t last_song = number_of_songs - 1;
      if (song_index > last_song - 4) {
        first_song = last_song - 5;
      } else {
        if (song_index > 0) {
          first_song = song_index - 1;
        } else {
          first_song = song_index;
        }
      }
    }
  }

  bool move = true;
  if (first_song == last_first_song) {
    move = false;
  }
  last_first_song = first_song;
  if (slide) {
    if (direction == DOWN) {
    }
  }

  screen->writeSongList(first_song, song_index, direction, slide, move);
}
