/***************************************************************************
Copyright © 2023 Shell M. Shrader <shell at shellware dot com>
----------------------------------------------------------------------------
This work is free. You can redistribute it and/or modify it under the
terms of the Do What The Fuck You Want To Public License, Version 2,
as published by Sam Hocevar. See the COPYING file for more details.
****************************************************************************/
#include "main.h"

#ifdef ENABLE_DEBUG
void setExtraRemoteCommands(char c) {
  if (c == '?') {
    LOG_PRINTLN(bs.builtInRemoteCommandsMenu + "G = Get Station ID\n? = This menu\n");
  }
  if (c == 'G') {
    LOG_PRINTF("\nStation ID = [%s]\n", my_config.station_id);
  }
}
#endif

void updateExtraConfigItem(const String item, String value) {
    if (item == "station_id") {
        memset(my_config.station_id, CFG_NOT_SET, WIFI_SSID_LEN);
        if (value.length() > 0) {
            value.toCharArray(my_config.station_id, STATION_ID_LEN);
            my_config.station_id_flag = CFG_SET;
        } else {
            my_config.station_id_flag = CFG_NOT_SET;
        }
        return;
    }
}
void saveExtraConfig() {
  bs.cfg(&my_config, sizeof(my_config));
}
void updateExtraHtmlTemplateItems(String *html) {
  while (html->indexOf("{station_id}", 0) != -1) {
    html->replace("{station_id}", String(my_config.station_id));
  }
}

void setup() {
#ifdef ENABLE_DEBUG
  bs.setExtraRemoteCommands(setExtraRemoteCommands);
#endif
  bs.updateExtraConfigItem(updateExtraConfigItem);
  bs.saveExtraConfig(saveExtraConfig);
  bs.updateExtraHtmlTemplateItems(updateExtraHtmlTemplateItems);
  bs.setConfigSize(sizeof(my_config));
  bs.setup();

  // get a fresh copy of our extended config struct
  memcpy(&my_config, bs.cfg(), sizeof(my_config));

  // setup done
  LOG_PRINTLN("\nSystem Ready");
}

void loop() {
  bs.loop();

  static unsigned long lastUpdate = millis();
  if (millis() - 5000 > lastUpdate) {
    bs.updateIndexHtml();
    lastUpdate = millis();
  }
}
