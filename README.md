LCD useful parameters can be found in:
https://github.com/espressif/esp-idf/blob/master/examples/peripherals/lcd/spi_lcd_touch/main/spi_lcd_touch_example_main.c

LCD driver = ILI9341

> [!NOTE]
> chip = esp32s3 only

This app searches for an AP which SSID starts with "SOMEF" and displays its SSID and MAC address on the display.
If no device is found, a "no device is found" is displayed on the display.
Then, device is classified as ok or nok.
There are 2 branches:

### branch ok_nok_automatically:
Automatically classifies device as ok or nok. Device is ok if mac is the same as the last part of the ssid, else, device is nok.
If device is ok, its mac is written in a file: ok_list.csv, if device is nok, its mac is written in a file: nok_list.csv.
3 console commands can be typed by the user (list of commands can be seen by typing the command: help) :
last_added_device
  last added device to ok_list of nok_list

nok_list
  show list of nok_devices (content of nok_list.csv)

ok_list
  show list of ok devices (content of ok_list.csv)

restart
  software reset of the chip

### branch ok_nok_with_buttons:
After SSID and MAC are dislayed on the display, the user presses an "ok" button or a "nok" button.
If "ok" button is pressed, the device is ok and its mac is written in a file: ok_list.csv.
If "nok" button is pressed, the device is nok and its mac is written in a file: ok_list.csv.


# menuconfig parameters in skdconfig.defaults:
- flash size 4 MB
- custom partition table
- enabled fonts: montserrat 24 and montserrat 40
- enable CONFIG_ESP_CONSOLE_USB_SERIAL_JTAG
- CONFIG_CONSOLE_MAX_COMMAND_LINE_LENGTH


