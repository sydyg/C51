#ifndef _KEYBOARD_H
#define _KEYBOARD_H

#ifndef _KEYBOARD_C
extern void keyscan();
extern void keydrive();
#endif

extern void KeyAction(uint8 keymap);
#endif