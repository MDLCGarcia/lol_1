#include "joystick.h"

/*
	Description:
		Joystick initialization
	Inputs:
		* fd
	Outputs:
		None
*/
void js_init(int* fd)
{
	if ((*fd = open(JS_DEV, O_RDONLY)) < 0)
	{
		perror("jyinit");
		exit(1);
	}

	/* non-blocking mode
	*/
	fcntl(*fd, F_SETFL, O_NONBLOCK);
}


/*
	Description: 
		Read inputs from joystick
	Inputs:
		* fd, * axis, * button
	Outputs:
		a js_command
*/
js_command read_js(int* fd, int* axis, int* button)
{
	struct js_event js;
	js_command js_c;

	/* check up on JS
	*/
	while (read(*fd, &js, sizeof(struct js_event)) ==
		sizeof(struct js_event)) {

		/* register data
		*/
		// fprintf(stderr,".");
		switch (js.type & ~JS_EVENT_INIT) {
		case JS_EVENT_BUTTON:
			*button[js.number] = js.value;
			break;
		case JS_EVENT_AXIS:
			*axis[js.number] = js.value;
			break;
		}
	}

	if (errno != EAGAIN)
	{
		perror("\njs: error reading (EAGAIN)");
		exit(1);
	}

	// Switch the mode
	if (*button[0])
		js_c.Mode = M_SAFE;
	if (*button[1])
		js_c.Mode = M_PANIC;
	if (*button[2])
		js_c.Mode = M_MANUAL;
	if (*button[3])
		js_c.Mode = M_CALIBRATION;
	if (*button[4])
		js_c.Mode = M_YAWCONTROL;
	if (*button[5])
		js_c.Mode = M_FULLCONTROL;
	if (*button[6])
		js_c.Mode = M_RAWMODE;
	if (*button[7])
		js_c.Mode = M_HEIGHTCONTROL;
	if (*button[8])
		js_c.Mode = M_WIRELESS;

	/*
	// roll
	if (*axis(0))
	{
		js_c->Roll = *axis(0);
	}

	// pitch
	if (*axis(1))
	{
		js_c->Pitch = *axis(1);
	}

	// yaw
	if (*axis(2))
	{
		js_c->Yaw = *axis(2);
	}

	// lift
	if (*axis(3))
	{
		js_c->Lift = *axis(3);
	}
	*/

	return js_c;
}