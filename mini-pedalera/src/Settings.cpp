#include "Settings.h"

const uint8_t Settings::_menu_min_options[NUMBER_OF_MENU_OPTIONS] = {1, 0};
const uint8_t Settings::_menu_max_options[NUMBER_OF_MENU_OPTIONS] = {3, 0};
const uint8_t Settings::_audio_min_options[5] = {0,0,0,0,0};
const uint8_t Settings::_audio_max_options[5] = {1,10,10,10,10};
const uint8_t* Settings::_submenu_min_options[NUMBER_OF_MENU_OPTIONS] = {nullptr, _audio_min_options};
const uint8_t* Settings::_submenu_max_options[NUMBER_OF_MENU_OPTIONS] = {nullptr, _audio_max_options};

const uint8_t Settings::_menu_option_ccs[NUMBER_OF_MENU_OPTIONS] = {25, 0};
const uint8_t Settings::_audio_ccs[5] = {20, 21, 22, 23, 24};
const uint8_t* Settings::_submenu_option_ccs[NUMBER_OF_MENU_OPTIONS] = {nullptr, _audio_ccs};

uint8_t Settings::_menu_option_values[NUMBER_OF_MENU_OPTIONS] = {Led::getLedIntensityLevel(), 0};
uint8_t Settings::_audio_values[5] = {0,0,10,0,10};
uint8_t* Settings::_submenu_option_values[NUMBER_OF_MENU_OPTIONS] = {nullptr, _audio_values};

Settings::Settings()
{
}

void Settings::init(Screen *screen, Button **buttons, const uint8_t number_of_buttons, Led **leds, const uint8_t number_of_leds)
{
  this->screen = screen;
  this->buttons = buttons;
  this->number_of_buttons = number_of_buttons;
  this->leds = leds;
  this->number_of_leds = number_of_leds;
}

void Settings::setSettingValue(uint8_t cc, uint8_t value)
{
  for (uint8_t i = 0; i < NUMBER_OF_MENU_OPTIONS; i++) {
    if (cc == Settings::_menu_option_ccs[i]) {
      Settings::_menu_option_values[i] = getPedalboardOptionValue(value, Settings::_menu_max_options[i]);
    }
    if (Settings::_submenu_option_ccs[i] == nullptr) {
      continue;
    }
    for (uint8_t j = 0; j < NUMBER_OF_SUBMENU_OPTIONS[i]; j++) {
      if (cc == Settings::_submenu_option_ccs[i][j]) {
        Settings::_submenu_option_values[i][j] = getPedalboardOptionValue(value, Settings::_submenu_max_options[i][j]);
      }
    }
  }
}

void Settings::startSettingsMode()
{
  screen->clean();
  screen->writeSettingsTitle(settings_title);
  startFlashingLeds();
}

void Settings::settingsMode()
{
  bool settings_mode = true;
  uint8_t selected_option = 0;
  uint8_t number_of_options = NUMBER_OF_MENU_OPTIONS;
  uint8_t level = 1;
  char **menu_options = (char**)MENU;
  uint8_t *option_values = _menu_option_values;
  uint8_t *min_values = (uint8_t*)_menu_min_options;
  uint8_t *max_values = (uint8_t*)_menu_max_options;
  uint8_t *option_ccs = (uint8_t*)_menu_option_ccs;
  showMenuOptions(menu_options, number_of_options, selected_option, option_values, max_values);

  while (settings_mode) {
    updateFlashingLeds();
    for (uint8_t i = 0; i < _number_of_setting_buttons; i++) {
      uint8_t action = buttons[_buttons_index[i]]->settingsChanged();
      changeOption(action, menu_options, number_of_options, selected_option, option_values, max_values);
      if (action == _select) {
        if (level == 1 && SUBMENUS[selected_option] != NULL) {
          selectSubmenu(menu_options, number_of_options, selected_option, option_values, min_values, max_values, option_ccs);
          level++;
        } else {
          editOption(
            menu_options,
            number_of_options,
            selected_option,
            level,
            option_values,
            min_values[selected_option],
            max_values[selected_option],
            option_ccs[selected_option]
          );
          showMenuOptions(menu_options, number_of_options, selected_option, option_values, max_values);
        }
      }
      exitOption(action, menu_options, number_of_options, selected_option, level, settings_mode, option_values, min_values, max_values, option_ccs);
    }
  }
}

void Settings::changeOption(uint8_t action, char **menu_options, uint8_t number_of_options, uint8_t &selected_option, uint8_t *option_values, uint8_t *max_values)
{
  if (isMenuOptionChange(action, number_of_options, selected_option)) {
    if (action == _up) {
      selected_option--;
    }
    if (action == _dn) {
      selected_option++;
    }
    showMenuOptions(menu_options, number_of_options, selected_option, option_values, max_values);
  }
}

bool Settings::isMenuOptionChange(uint8_t action, uint8_t number_of_options, uint8_t selected_option)
{
  if ((action == _up && selected_option > 0) || (action == _dn && selected_option < number_of_options - 1)) {
    return true;
  }
  return false;
}

void Settings::selectSubmenu(char **&menu_options, uint8_t &number_of_options, uint8_t &selected_option, uint8_t *&option_values, uint8_t *&min_values, uint8_t *&max_values, uint8_t *&option_ccs)
{
  screen->clean();
  screen->writeSettingsTitle(menu_options[selected_option]);
  menu_options = (char**)SUBMENUS[selected_option];
  option_values = _submenu_option_values[selected_option];
  min_values = (uint8_t*)_submenu_min_options[selected_option];
  max_values = (uint8_t*)_submenu_max_options[selected_option];
  number_of_options = NUMBER_OF_SUBMENU_OPTIONS[selected_option];
  option_ccs = (uint8_t*)_submenu_option_ccs[selected_option];
  selected_option = 0;
  showMenuOptions(menu_options, number_of_options, selected_option, option_values, max_values);
}

void Settings::editOption(char **menu_options, uint8_t number_of_options, uint8_t selected_option, uint8_t level, uint8_t *option_values, uint8_t min_value, uint8_t max_value, uint8_t option_cc)
{
  bool options_mode = true;
  uint8_t option_value = option_values[selected_option];
  uint8_t initial_option_value = option_value;
  showMenuOptionEdition(menu_options, number_of_options, selected_option, option_value);
  while (options_mode) {
    updateFlashingLeds();
    for (uint8_t i = 0; i < _number_of_setting_buttons; i++) {
      uint8_t action = buttons[_buttons_index[i]]->settingsChanged();
      if (isValueChange(action, selected_option, option_value, min_value, max_value)) {
        if (action == _up) {
          option_value++;
        }
        if (action == _dn) {
          option_value--;
        }
        if (level == 1 && selected_option == led_intensity_menu_option) {
          Led::setLedIntensityLevel(option_value);
        }
        if (option_cc) {
          usbMIDI.sendControlChange(option_cc, getGpOptionValue(option_value,max_value), _settings_midi_chanel);
        }
        showMenuOptionEdition(menu_options, number_of_options, selected_option, option_value);
      }
      if (action == _select) {
        option_values[selected_option] = option_value;
        options_mode = false;
      }
      if (action == _exit) {
        option_values[selected_option] = initial_option_value;
        if (level == 1 && selected_option == led_intensity_menu_option) {
          Led::setLedIntensityLevel(initial_option_value);
        }
        if (option_cc) {
          usbMIDI.sendControlChange(option_cc, getGpOptionValue(initial_option_value, max_value), _settings_midi_chanel);
        }
        options_mode = false;
      }
    }
  }
}

uint8_t Settings::getPedalboardOptionValue(uint8_t value, uint8_t max_value)
{
  return round((value * max_value) / 127);
}

uint8_t Settings::getGpOptionValue(uint8_t value, uint8_t max_value)
{
  return (value * 127) / max_value;
}

void Settings::exitOption(uint8_t action, char **&menu_options, uint8_t &number_of_options, uint8_t &selected_option, uint8_t &level, bool &settings_mode, uint8_t *&option_values, uint8_t *&min_values, uint8_t *&max_values, uint8_t *&option_ccs)
{
  if (action == _exit) {
    if (level > 1) {
      menu_options = (char**)MENU;
      number_of_options = NUMBER_OF_MENU_OPTIONS;
      option_values = _menu_option_values;
      min_values = (uint8_t*)_menu_min_options;
      max_values = (uint8_t*)_menu_max_options;
      option_ccs = (uint8_t*)_menu_option_ccs;
      selected_option = 0;
      level--;
      screen->clean();
      screen->writeSettingsTitle(settings_title);
      showMenuOptions(menu_options, number_of_options, selected_option, option_values, max_values);
    } else {
      settings_mode = false;
    }
  }
}

bool Settings::isValueChange(uint8_t action, uint8_t selected_option, uint8_t option_value, uint8_t min_value, uint8_t max_value)
{
  if ((action == _up && option_value < max_value) || (action == _dn && option_value > min_value)) {
    return true;
  }
  return false;
}

void Settings::startFlashingLeds()
{
  for (uint8_t i = 0; i < _number_of_leds_flashing; i++) {
    leds[_leds_index[i]]->flash(LED_FLASHING_ON, LED_FLASHING_OFF, -1);
  }
}

void Settings::updateFlashingLeds()
{
  for (uint8_t i = 0; i < _number_of_leds_flashing; i++) {
    leds[_leds_index[i]]->flashUpdate();
  }
}

void Settings::showMenuOptions(char **menu_options, uint8_t number_of_options, uint8_t selected_option, uint8_t *option_values, uint8_t *max_values)
{
  bool options_with_values[number_of_options];
  getOptionsWithValues(max_values, number_of_options, options_with_values);
  screen->showSettingOptions(menu_options, number_of_options, selected_option, option_values, options_with_values);
}

void Settings::getOptionsWithValues(uint8_t *max_values, uint8_t number_of_options, bool *options_with_values)
{
  for (uint8_t i = 0; i < number_of_options; i++) {
    options_with_values[i] = max_values[i] > 0;
  }
}

void Settings::showMenuOptionEdition(char **menu_options, uint8_t number_of_options, uint8_t selected_option, uint8_t option_value)
{
  screen->showSettingOptionEdition(menu_options, number_of_options, selected_option, option_value);
}

void Settings::exitSettingsMode()
{
  for (uint8_t i = 0; i < _number_of_leds_flashing; i++) {
    leds[_leds_index[i]]->off();
  }
}
