#pragma once

#include <GxEPD2_BW.h>
#include <GxEPD2_3C.h>

static constexpr int EPD_CS     = 1;
static constexpr int EPD_DC     = 3;
static constexpr int EPD_RST    = 0;
static constexpr int EPD_BUSY   = 2;

#define COLOR_DISPLAY 1
#define ENABLE_GxEPD2_GFX 0

#if COLOR_DISPLAY
    using EPDPANEL = GxEPD2_213_Z98c;   // GDEY0213Z98 122x250, SSD1680, (FPC-A002 20.04.08)
    using EPDTYPE = GxEPD2_3C<EPDPANEL, EPDPANEL::HEIGHT>;
#else
    using EPDPANEL = GxEPD2_213_BN; // DEPG0213BN  122x250, SSD1680, (FPC-7528B), TTGO T5 V2.4.1, V2.3.1
    using EPDTYPE = GxEPD2_BW<EPDPANEL, EPDPANEL::HEIGHT>;
#endif