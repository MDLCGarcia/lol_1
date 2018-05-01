#ifndef KEYBOARD_H__
#define KEYBOARD_H__

#include "../protocol/packet.h"

// Define mode part
#define ZERO 48
#define ONE 49
#define TWO 50
#define THREE 51
#define FOUR 52
#define FIVE 53
#define SIX 54
#define SEVEN 55
#define EIGHT 56


// Define action  part
#define A				97
#define Z				122
#define LEFTARROW			68
#define RIGHTARROW			67
#define UPARROW				65
#define DOWNARROW			66
#define Q				113
#define W				119

// Define control part
#define U				117
#define J				106
#define I				105
#define K				107
#define O				111
#define L				108

// Define exit
#define ESC 27


typedef struct KB_COMMAND
{
	char Mode;
	char Roll;
	char Pitch;
	char Yaw;
	char Lift;
}kb_command;

kb_command read_kb(char c);

#endif