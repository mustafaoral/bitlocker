#include <Arduino.h>
#include <Adafruit_TinyUSB.h>
#include <SPI.h>
#include "pin.h"

// USB HID keycodes
static const uint8_t KEY_ENTER = 0x28;
static const uint8_t KEY_BKSP = 0x2A;

static const uint8_t EMPTY_REPORT[8] = {0};

// Boot keyboard report descriptor
uint8_t const desc_hid_report[] = {TUD_HID_REPORT_DESC_KEYBOARD()};

Adafruit_USBD_HID usb_hid;

void pressKey(uint8_t keycode)
{
    uint8_t report[8] = {0, 0, keycode, 0, 0, 0, 0, 0};
    usb_hid.sendReport(0, report, sizeof(report));
    delay(50);
    usb_hid.sendReport(0, EMPTY_REPORT, sizeof(EMPTY_REPORT));
    delay(50);
}

void clear()
{
    for (int i = 0; i < 18; i++)
    {
        pressKey(KEY_BKSP);
    }
}

void sendPIN()
{
    for (size_t i = 0; i < sizeof(PIN_KEYS); i++)
    {
        pressKey(PIN_KEYS[i]);
    }

    pressKey(KEY_ENTER);
}

void setup()
{
    usb_hid.setBootProtocol(HID_ITF_PROTOCOL_KEYBOARD);
    usb_hid.setPollInterval(2);
    usb_hid.setReportDescriptor(desc_hid_report, sizeof(desc_hid_report));
    usb_hid.begin();

    while (!TinyUSBDevice.mounted())
    {
        delay(10);
    }

    delay(1000);

    clear();
    sendPIN();
}

void loop() {}
