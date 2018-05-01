#include "keyboard.h"


/*
	Description: 
		Read inputs from keyboard
	Inputs: 
		char c
	Outputs:
		a kb_command
*/

kb_command read_kb(char c)
{
	kb_command kb_c;
	switch (c)
	{
	case ESC:
		kb_c.Mode = M_PANIC;
		break;

	case ZERO:
		kb_c.Mode = M_SAFE;
		term_puts("changed mode to safe mode\n");
		break;

	case ONE:
		kb_c.Mode = M_PANIC;
		break;

	case TWO:
		kb_c.Mode = M_MANUAL;
		break;

	case THREE:
		kb_c.Mode = M_CALIBRATION;
		break;

	case FOUR:
		kb_c.Mode = M_YAWCONTROL;
		break;

	case FIVE:
		kb_c.Mode = M_FULLCONTROL;
		break;

	case SIX:
		kb_c.Mode = M_RAWMODE;
		break;

	case SEVEN:
		kb_c.Mode = M_HEIGHTCONTROL;
		break;

	case EIGHT:
		kb_c.Mode = M_WIRELESS;
		break;
	
	/*
	case 'a'://lift up
		kb_c->Lift = UP;
		printf("lift up\n");
		break;

	case 'z'://lift down
		printf("lift down\n");

		break;
	case 'q'://Yaw down
		printf("Yaw down\n");

		break;
	case 'w'://Yaw up
		printf("Yaw up\n");

		break;

	case 'u'://Yaw control P up
		printf("Yaw control P up\n");

		break;

	case 'j'://Yaw control P down
		printf("Yaw control P down\n");
		break;

	case 'i':// P1 up
		printf("P1 up\n");
		break;

	case 'k':// P1 down
		printf("P1 down\n");
		break;

	case 'o':// P2 up
		printf("P2 up\n");
		break;

	case 'l':// P2 down
		printf("P2 down\n");
		break;

	case 'D'://Left Arrow - RollUp
		printf("RollUp\n");
		break;

	case 'C'://Right Arrow - RollDown
		printf("RollDown\n");
		break;

	case 'A'://Up Arrow - PitchDown
		printf("PitchDown\n");
		break;

	case 'B'://Arrow Down - PitchUP
		printf("PitchUP\n");
		break;
	*/

	default:
		printf("Invalid Control Input\n");
		//invalid_input=1;
		break;
	}
}

