#include "keyb.h"

#include <dos.h>
#include <conio.h>
#include <bios.h>

#include <stdlib.h>
#include <stdio.h>

#ifdef DJGPP
// The 32-bit compile requires DPMI functions to access the keyboard interrupt
#include <dpmi.h>
#include <go32.h>
#endif

#define KEY_BUFFER_SIZE 0x60


static u8 normalKeys [KEY_BUFFER_SIZE];
static u8 extKeys [KEY_BUFFER_SIZE];

static u8 oldNormals [KEY_BUFFER_SIZE];
static u8 oldExt [KEY_BUFFER_SIZE];

static bool normalRead [KEY_BUFFER_SIZE];
static bool extRead [KEY_BUFFER_SIZE];

static bool anyPressed;

static volatile u8 buffer = 0;

#ifdef DJGPP
#define INTERRUPT_ABI
#else
#define INTERRUPT_ABI far interrupt
#endif

#ifdef DJGPP
_go32_dpmi_seginfo oldHandler = {0};
_go32_dpmi_seginfo newHandler = {0};
#else
static void INTERRUPT_ABI (*oldHandler) (void) = NULL;
#endif

static void INTERRUPT_ABI handler() {
    u8 rawcode;
    u8 makeBreak;
    i16 scancode;

    rawcode = inp(0x60);
    makeBreak = !(rawcode & 0x80);
    scancode = rawcode & 0x7F;

    if (buffer == (u8)0xE0) {

        if(scancode < 0x60) {

            oldExt[scancode] = extKeys[scancode];
            extKeys[scancode] = makeBreak;

            if (extKeys[scancode] != oldExt[scancode]) {

                anyPressed = makeBreak;
                extRead[scancode] = false;
            }
        }
        buffer = 0;
    }
    else if (buffer >= (u8)0xE1 && buffer <= (u8)0xE2) {

        buffer = 0;
    }
    else if (rawcode >= (u8)0xE0 && rawcode <= (u8)0xE2) {

        buffer = rawcode;
    }
    else if (scancode < 0x60) {

        oldNormals[scancode] = normalKeys[scancode];
        normalKeys[scancode] = makeBreak;

        if (normalKeys[scancode] != oldNormals[scancode]) {

            anyPressed = makeBreak;
            normalRead[scancode] = false;
        }

    }

    outp(0x20, 0x20);
}


static i8 get_value_from_array(u8* arr, bool* readArr, i16 id) {

    u8 state = arr[id];
    bool read = readArr[id];
    u8 ret = 0;

    if(state == STATE_DOWN) {

        ret = read ? STATE_DOWN : STATE_PRESSED;
    }
    else if(state == STATE_UP) {

        ret = read ? STATE_UP : STATE_RELEASED;
    }
    readArr[id] = true;

    return ret;
}


void init_keyboard_listener() {

    // Use Set 1
    outp(0x60, 0xf0);
    outp(0x60, 0x1);

#ifdef DJGPP
    newHandler.pm_offset = (int) handler;
    newHandler.pm_selector = _go32_my_cs();

    _go32_dpmi_get_protected_mode_interrupt_vector(0x09, &oldHandler);
    _go32_dpmi_allocate_iret_wrapper(&newHandler);
    _go32_dpmi_set_protected_mode_interrupt_vector(0x09, &newHandler);
#else
    oldHandler = _dos_getvect(0x09);
    _dos_setvect(0x09, handler);
#endif

    anyPressed = false;
}


void reset_keyboard_listener() {

#ifdef DJGPP
    if (oldHandler.pm_offset != 0) {
        _go32_dpmi_set_protected_mode_interrupt_vector(0x09, &oldHandler);
        oldHandler.pm_offset = 0;
    }
    _go32_dpmi_free_iret_wrapper(&newHandler);
#else
     if (oldHandler != NULL) {
         _dos_setvect(0x09, oldHandler);
         oldHandler = NULL;
     }
#endif
}


void keyb_update() {

    anyPressed = false;
}


bool keyb_any_pressed() {

    return anyPressed;
}


State keyb_get_normal_key(u8 key) {

    if(key > KEY_BUFFER_SIZE)
        return STATE_UP;

    return get_value_from_array(
        normalKeys,
        normalRead, key);
}


State keyb_get_ext_key(u8 key) {

    if(key > KEY_BUFFER_SIZE)
        return STATE_UP;

    return get_value_from_array(
        extKeys,
        extRead, key);
}
