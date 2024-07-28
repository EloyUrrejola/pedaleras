#include "SongSelector.h"

SongSelector::SongSelector()
{
}

void SongSelector::init(Screen *screen, Button **buttons, uint8_t number_of_buttons)
{
  this->screen = screen;
  this->buttons = buttons;
  this->number_of_buttons = number_of_buttons;
}

void SongSelector::startSongSelectorMode()
{
  temp_song_index = SongList::getCurrentSongIndex();
  song_list = SongList::getSongList();
  screen->clean();
  Led::startFlashingLeds(leds_index, number_of_leds_flashing);
  showSongSelectorPanel(temp_song_index, INIT);
}

void SongSelector::songSelectorMode()
{
  bool settings_mode = true;
  while (settings_mode) {
    Led::updateFlashingLeds(leds_index, number_of_leds_flashing);
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
}

void SongSelector::showSongSelectorPanel(uint8_t song_index, int direction)
{
  uint8_t first_song = 0;
  uint8_t number_of_songs = SongList::getNumberOfSongs();
  bool slide = false;

  if (number_of_songs < max_number_of_visible_songs) {
    first_song = 0;
  } else {
    if (direction == UP) {
      if (song_index > (max_number_of_visible_songs - 3)) {
        first_song = song_index - (max_number_of_visible_songs - 2);
        if (song_index < number_of_songs - 2) {
          slide = true;
        }
      } else {
        first_song = 0;
      }
    }
    if (direction == DOWN) {
      uint8_t last_song = number_of_songs - 1;
      if (song_index > last_song - (max_number_of_visible_songs - 2)) {
        first_song = last_song - (max_number_of_visible_songs - 1);
      } else {
        if (song_index > 1) {
          slide = true;
        }
        first_song = song_index - 1;
      }
    }
    if (direction == INIT) {
      uint8_t last_song = number_of_songs - 1;
      if (song_index > last_song - (max_number_of_visible_songs - 2)) {
        first_song = last_song - (max_number_of_visible_songs - 1);
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

  screen->writeSongList(first_song, song_index, direction, slide, move);
}
