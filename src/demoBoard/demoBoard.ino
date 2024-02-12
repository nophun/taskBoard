
// base class GxEPD2_GFX can be used to pass references or pointers to the display instance as parameter, uses ~1.2k more code
// enable or disable GxEPD2_GFX base class
#define ENABLE_GxEPD2_GFX 0

#include <SimpleSerialShell.h>
#include <GxEPD2_BW.h>
#include <GxEPD2_3C.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include "fonts/tahoma8pt7b.h"
#include "fonts/Pockota_Bold16pt7b.h"
#include "bsp.h"

// 2.13'' EPD Module
GxEPD2_BW<GxEPD2_213_BN, GxEPD2_213_BN::HEIGHT> display(GxEPD2_213_BN(EPD_CS, EPD_DC, EPD_RST, EPD_BUSY)); // DEPG0213BN 122x250, SSD1680
// GxEPD2_3C<GxEPD2_213_Z98c, GxEPD2_213_Z98c::HEIGHT> display(GxEPD2_213_Z98c(EPD_CS, EPD_DC, EPD_RST, EPD_BUSY)); // DEPG0213BN 122x250, SSD1680

// 2.9'' EPD Module
//GxEPD2_BW<GxEPD2_290_BS, GxEPD2_290_BS::HEIGHT> display(GxEPD2_290_BS(/*CS=5*/ 5, /*DC=*/ 0, /*RST=*/ 2, /*BUSY=*/ 15)); // DEPG0290BS 128x296, SSD1680
//GxEPD2_3C<GxEPD2_290_C90c, GxEPD2_290_C90c::HEIGHT> display(GxEPD2_290_C90c(/*CS=5*/ 5, /*DC=*/ 0, /*RST=*/ 2, /*BUSY=*/ 15)); // GDEM029C90 128x296, SSD1680

const char title_str[] = "TaskBoard";
const char title2_str[] = "ISONENA";
// const char description_str[] = "Features that must be done first, so that the product can be released as soon as possible, and still satisfy the customer.";
const char description_str[] = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nulla ac purus nec eros varius aliquam. Fusce nec odio nec odio.";


void setup() {
    Serial.begin(115200);

    display.init(115200, true, 50, false);
    display.setRotation(1);

    shell.attach(Serial);
    shell.addCommand(F("task"), command_task);
    shell.addCommand(F("title"), command_title);
    shell.addCommand(F("desc"), command_description);
    shell.addCommand(F("fastpartial"), command_fastpartial);
    shell.addCommand(F("partial"), command_partial);
    shell.addCommand(F("init"), init);
    shell.addCommand(F("check"), check);
    shell.addCommand(F("off"), off);
    Serial.println("start");
    show_task(title_str);
    Serial.println("done");
    // change_title(title2_str);

    // helloWorld();
    // helloFullScreenPartialMode();
    // delay(1000);
    // if (display.epd2.hasFastPartialUpdate)
    // {
    //   showPartialUpdate();
    //   delay(1000);
    // }
    // display.hibernate();
    // init_display_content();
}

int init(int argc, char** argv) {
    display.refresh();
}

int check(int argc, char** argv) {
    display.refresh(0, 249, 1, 1);
}

int off(int argc, char** argv) {
    display.powerOff();
}

void init_display_content() {
    int16_t tbx, tby;
    uint16_t tbw, tbh;
    display.setFont(&Pockota_Bold16pt7b);
    display.getTextBounds("Title", 0, 0, &tbx, &tby, &tbw, &tbh);
    display.setFullWindow();

    display.firstPage();
    display.fillScreen(GxEPD_WHITE);

    display.setTextColor(GxEPD_BLACK);
    display.setCursor(-tbx, 23);
    display.setFont(&Pockota_Bold16pt7b);
    display.print("Title");

    display.setTextColor(GxEPD_BLACK);
    display.setCursor(0, 56);
    display.setFont(&tahoma8pt7b);
    display.print("Description");
}

void loop() {
    shell.executeIfInput();
    // Serial.println(digitalRead(EPD_BUSY));
}

String combine_string_simple(int argc, char** argv) {
    String str = "";
    int i = 1;
    for (; i < argc - 1; ++i) {
        str += argv[i];
        str += " ";
    }
    str += argv[i];
    Serial.println(str);
    return str;
}

String combine_string_advanced(int argc, char** argv) {
    int16_t tbx, tby;
    uint16_t tbw, tbh;
    display.setFont(&tahoma8pt7b);
    display.setPartialWindow(0, 32, 250, 90);
    String str = "";
    String row = "";
    int rows = 1;
    int i = 1;
    for (; i < argc; ++i) {
        /* add word to a temporary buffer */
        row += argv[i];
        /* check if new word still fits to a row */
        display.getTextBounds(row, 0, 0, &tbx, &tby, &tbw, &tbh);
        // Serial.println("Row: " + String(row) + ", " + String(tbw) + ", " + String(tbh));
        if (tbh > 16) {
            if (rows >= 4) {
                break;
            }
            /* move to next row and init temporary buffer with the latest word */
            str += "\n";
            rows++;
            row = argv[i];
        } else {
            if (i > 1) {
                str += " ";
            }
            row += " ";
        }
        str += argv[i];
    }
    // Serial.println(str);
    return str;
}

int command_task(int argc, char** argv) {
    String title = combine_string_simple(argc, argv);
    show_task(title);
    return EXIT_SUCCESS;
}

int command_title(int argc, char** argv) {
    String title = combine_string_simple(argc, argv);
    Serial.println(title);
    change_title(title);
    return EXIT_SUCCESS;
}

int command_description(int argc, char** argv) {
    String description = combine_string_advanced(argc, argv);
    change_description(description);
    return EXIT_SUCCESS;
}

int command_fastpartial(int argc, char** argv) {
    if (display.epd2.hasFastPartialUpdate) {
        showPartialUpdate();
        delay(1000);
    }
    return EXIT_SUCCESS;
}

int command_partial(int argc, char** argv) {
    helloFullScreenPartialMode();
    return EXIT_SUCCESS;
}

void show_task(const String& title) {
    int16_t tbx, tby;
    uint16_t tbw, tbh;
    display.setFont(&Pockota_Bold16pt7b);
    display.getTextBounds(title, 0, 0, &tbx, &tby, &tbw, &tbh);
    display.setFullWindow();
    display.firstPage();
    do {
        display.fillScreen(GxEPD_WHITE);

        display.setTextColor(display.epd2.hasColor ? GxEPD_RED : GxEPD_BLACK);
        display.setCursor(-tbx, 23);
        display.setFont(&Pockota_Bold16pt7b);
        display.print(title);

        display.setTextColor(GxEPD_BLACK);
        display.setCursor(0, 56);
        display.setFont(&tahoma8pt7b);
        display.print(description_str);
    }
    while (display.nextPage());
}

void change_title(const String& title) {
    int16_t tbx, tby;
    uint16_t tbw, tbh;
    display.setFont(&Pockota_Bold16pt7b);
    display.setPartialWindow(0, 0, 250, 32);
    display.getTextBounds(title, 0, 0, &tbx, &tby, &tbw, &tbh);
    Serial.println("tbx " + String(tbx) + " tby " + String(tby) + " tbw " + String(tbw) + " tbh " + String(tbh));

    display.firstPage();
    do {
        display.fillRect(0, 0, 250, 32, GxEPD_WHITE);
        display.setFont(&Pockota_Bold16pt7b);
        display.setTextColor(GxEPD_BLACK);
        display.setCursor(-tbx, 23);
        display.print(title);
    }
    while (display.nextPage());
}

void change_description(const String& description) {
    int16_t tbx, tby;
    uint16_t tbw, tbh;
    display.setFont(&tahoma8pt7b);
    display.setPartialWindow(0, 32, 250, 90);
    display.getTextBounds(description, 0, 0, &tbx, &tby, &tbw, &tbh);
    Serial.println("tbx " + String(tbx) + " tby " + String(tby) + " tbw " + String(tbw) + " tbh " + String(tbh));

    display.firstPage();
    do {
        display.fillRect(0, 32, 250, 90, GxEPD_WHITE);
        display.setFont(&tahoma8pt7b);
        display.setTextColor(GxEPD_BLACK);
        display.setCursor(-tbx, 56);
        display.print(description);
    }
    while (display.nextPage());
}

const char HelloWorld[] = "Hello World!";
const char HelloWeACtStudio[] = "WeAct Studio";

void helloWorld() {
    display.setRotation(1);
    display.setFont(&FreeMonoBold9pt7b);
    display.setTextColor(GxEPD_BLACK);
    int16_t tbx, tby; uint16_t tbw, tbh;
    display.getTextBounds(HelloWorld, 0, 0, &tbx, &tby, &tbw, &tbh);
    // center the bounding box by transposition of the origin:
    uint16_t x = ((display.width() - tbw) / 2) - tbx;
    uint16_t y = ((display.height() - tbh) / 2) - tby;
    display.setFullWindow();
    display.firstPage();
    do {
        display.fillScreen(GxEPD_WHITE);
        display.setCursor(x, y-tbh);
        display.print(HelloWorld);
        display.setTextColor(display.epd2.hasColor ? GxEPD_RED : GxEPD_BLACK);
        display.getTextBounds(HelloWeACtStudio, 0, 0, &tbx, &tby, &tbw, &tbh);
        x = ((display.width() - tbw) / 2) - tbx;
        display.setCursor(x, y+tbh);
        display.print(HelloWeACtStudio);
    }
    while (display.nextPage());
}

void helloFullScreenPartialMode() {
    //Serial.println("helloFullScreenPartialMode");
    const char fullscreen[] = "full screen update";
    const char fpm[] = "fast partial mode";
    const char spm[] = "slow partial mode";
    const char npm[] = "no partial mode";
    display.setPartialWindow(0, 0, display.width(), display.height());
    display.setRotation(1);
    display.setFont(&FreeMonoBold9pt7b);
    if (display.epd2.WIDTH < 104) display.setFont(0);
    display.setTextColor(GxEPD_BLACK);
    const char* updatemode;
    if (display.epd2.hasFastPartialUpdate)
    {
        updatemode = fpm;
    }
    else if (display.epd2.hasPartialUpdate)
    {
        updatemode = spm;
    }
    else
    {
        updatemode = npm;
    }
    // do this outside of the loop
    int16_t tbx, tby; uint16_t tbw, tbh;
    // center update text
    display.getTextBounds(fullscreen, 0, 0, &tbx, &tby, &tbw, &tbh);
    uint16_t utx = ((display.width() - tbw) / 2) - tbx;
    uint16_t uty = ((display.height() / 4) - tbh / 2) - tby;
    // center update mode
    display.getTextBounds(updatemode, 0, 0, &tbx, &tby, &tbw, &tbh);
    uint16_t umx = ((display.width() - tbw) / 2) - tbx;
    uint16_t umy = ((display.height() * 3 / 4) - tbh / 2) - tby;
    // center HelloWorld
    display.getTextBounds(title_str, 0, 0, &tbx, &tby, &tbw, &tbh);
    uint16_t hwx = ((display.width() - tbw) / 2) - tbx;
    uint16_t hwy = ((display.height() - tbh) / 2) - tby;
    display.firstPage();
    do
    {
        display.fillScreen(GxEPD_WHITE);
        display.setCursor(hwx, hwy);
        display.print(title_str);
        display.setCursor(utx, uty);
        display.print(fullscreen);
        display.setCursor(umx, umy);
        display.print(updatemode);
    }
    while (display.nextPage());
    //Serial.println("helloFullScreenPartialMode done");
}

void showPartialUpdate() {
    // some useful background
    helloWorld();
    // use asymmetric values for test
    uint16_t box_x = 10;
    uint16_t box_y = 15;
    uint16_t box_w = 70;
    uint16_t box_h = 20;
    uint16_t cursor_y = box_y + box_h - 6;
    if (display.epd2.WIDTH < 104) cursor_y = box_y + 6;
    float value = 13.95;
    uint16_t incr = display.epd2.hasFastPartialUpdate ? 1 : 3;
    display.setFont(&FreeMonoBold9pt7b);
    if (display.epd2.WIDTH < 104) display.setFont(0);
    display.setTextColor(GxEPD_BLACK);
    // show where the update box is
    for (uint16_t r = 0; r < 4; r++)
    {
        display.setRotation(r);
        display.setPartialWindow(box_x, box_y, box_w, box_h);
        display.firstPage();
        do
        {
            display.fillRect(box_x, box_y, box_w, box_h, GxEPD_BLACK);
            //display.fillScreen(GxEPD_BLACK);
        }
        while (display.nextPage());
        delay(2000);
        display.firstPage();
        do
        {
            display.fillRect(box_x, box_y, box_w, box_h, GxEPD_WHITE);
        }
        while (display.nextPage());
        delay(1000);
    }
    //return;
    // show updates in the update box
    for (uint16_t r = 0; r < 4; r++)
    {
        display.setRotation(r);
        display.setPartialWindow(box_x, box_y, box_w, box_h);
        for (uint16_t i = 1; i <= 10; i += incr)
        {
            display.firstPage();
            do
            {
                display.fillRect(box_x, box_y, box_w, box_h, GxEPD_WHITE);
                display.setCursor(box_x, cursor_y);
                display.print(value * i, 2);
            }
            while (display.nextPage());
            delay(500);
        }
        delay(1000);
        display.firstPage();
        do
        {
            display.fillRect(box_x, box_y, box_w, box_h, GxEPD_WHITE);
        }
        while (display.nextPage());
        delay(1000);
    }
}
