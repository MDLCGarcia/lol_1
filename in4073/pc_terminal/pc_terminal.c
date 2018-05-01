/*
Steps:
	1. Initialize joystick and communication
	2. Read inputs from joystick and keyboard
	3. Create packet
	4. Send packet (todo)
*/

#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <inttypes.h>

#include "../protocol/packet.h"
#include "keyboard.h"
//#include "joystick.h"


int run = 1;
char* value_tag = NULL;
char type_tag = 0;
Packet *pkt = NULL;


/*------------------------------------------------------------
 * console I/O
 *------------------------------------------------------------
 */
struct termios 	savetty;

void	term_initio()
{
	struct termios tty;

	tcgetattr(0, &savetty);
	tcgetattr(0, &tty);

	tty.c_lflag &= ~(ECHO|ECHONL|ICANON|IEXTEN);
	tty.c_cc[VTIME] = 0;
	tty.c_cc[VMIN] = 0;

	tcsetattr(0, TCSADRAIN, &tty);
}

void	term_exitio()
{
	tcsetattr(0, TCSADRAIN, &savetty);
}

void	term_puts(char *s)
{
	fprintf(stderr,"%s",s);
}

void	term_putchar(char c)
{
	putc(c,stderr);
}

int	term_getchar_nb()
{
        static unsigned char 	line [2];

        if (read(0,line,1)) // note: destructive read
        		return (int) line[0];

        return -1;
}

int	term_getchar()
{
        int    c;

        while ((c = term_getchar_nb()) == -1)
                ;
        return c;
}

void ExitSafe(void)
{
	//implement 
	run = 0;
}

/*------------------------------------------------------------
 * Serial I/O
 * 8 bits, 1 stopbit, no parity,
 * 115,200 baud
 *------------------------------------------------------------
 */
#include <termios.h>
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>

int serial_device = 0;
int fd_RS232;

void rs232_open(void)
{
  	char 		*name;
  	int 		result;
  	struct termios	tty;
       	fd_RS232 = open("/dev/ttyS2", O_RDWR | O_NOCTTY);  // Hardcode your serial port here, or request it as an argument at runtime

	assert(fd_RS232>=0);

  	result = isatty(fd_RS232);
  	assert(result == 1);

  	name = ttyname(fd_RS232);
  	assert(name != 0);

  	result = tcgetattr(fd_RS232, &tty);
	assert(result == 0);

	tty.c_iflag = IGNBRK; /* ignore break condition */
	tty.c_oflag = 0;
	tty.c_lflag = 0;

	tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8; /* 8 bits-per-character */
	tty.c_cflag |= CLOCAL | CREAD; /* Ignore model status + read input */

	cfsetospeed(&tty, B115200);
	cfsetispeed(&tty, B115200);

	tty.c_cc[VMIN]  = 0;
	tty.c_cc[VTIME] = 1; // added timeout

	tty.c_iflag &= ~(IXON|IXOFF|IXANY);

	result = tcsetattr (fd_RS232, TCSANOW, &tty); /* non-canonical */

	tcflush(fd_RS232, TCIOFLUSH); /* flush I/O buffer */
}


void 	rs232_close(void)
{
  	int 	result;

  	result = close(fd_RS232);
  	assert (result==0);
}


int	rs232_getchar_nb()
{
	int 		result;
	unsigned char 	c;

	result = read(fd_RS232, &c, 1);

	if (result == 0)
		return -1;

	else
	{
		assert(result == 1);
		return (int) c;
	}
}


int 	rs232_getchar()
{
	int 	c;
	while ((c = rs232_getchar_nb()) == -1)
		;
	return c;
}


int 	rs232_putchar(char c)
{
	int result;

	do {
		result = (int) write(fd_RS232, &c, 1);
	} while (result == 0);

	assert(result == 1);
	return result;
}

void send_packet(Packet packet)
{
	rs232_putchar(packet.startByte);
	rs232_putchar(packet.dataLength);
	rs232_putchar(packet.type);
	rs232_putchar(packet.valueLength);
	rs232_putchar(packet.value);
	rs232_putchar(packet.CRC);
}

/*----------------------------------------------------------------
 * main -- execute terminal
 *----------------------------------------------------------------
 */
int main(int argc, char **argv)
{
	char kb_yaw;
	char kb_pitch;
	char kb_roll;
	char kb_lift;

	char js_yaw;
	char js_pitch;
	char js_roll;
	char js_lift;

	int axis[6];
	int button[12];
	int fd = 0;

	char mode;
	js_command js_comm;
	kb_command kb_comm;

	// communication initialization
	char c = 0;

	term_puts("\nTerminal program - Embedded Real-Time Systems\n");

	term_initio();
	rs232_open();

	// display keyboard mapping
	term_puts("keyboard mapping:\n");

	term_puts("Type ^C to exit\n");

	// discard any incoming text
	while ((c = rs232_getchar_nb()) != -1)
		fputc(c, stderr);

	// joystick initialization
	//js_init(&fd);

	// send & receive
	while (run)
	{
		//read messages from the board 
		if ((c = rs232_getchar_nb()) != -1)
		{
			term_putchar(c);
		}

		// read joystick inputs
		//js_comm = read_js(&fd, &axis, &button);
		//mode = js_comm->Mode;
		// read keyboard inputs
		if ((c = term_getchar_nb()) != -1)
		{
			kb_comm = read_kb(c);	
			mode = kb_comm->Mode;
		}

		// create packet and send packet to the board
		value_tag = (char *)malloc(sizeof(char) * 1);
		*value_tag = mode;
		type_tag = T_MODE;
		pkt = Create_Packet(type_tag, 1, value_tag);
		send_packet(pkt);

		//Send Packet bytes through RS232
		printf("Testing- Type:%d\n", pkt->type);
		printf("Testing- startbyte:%d\n", pkt->startByte);
		printf("Testing- datalength:%d\n", pkt->dataLength);
		printf("Testing- value length:%d\n", pkt->valueLength);
		printf("Testing- value:%d\n", *(pkt->value));
		printf("Testing- CRC:%d\n", *(pkt->CRC));
		Destroy_Packet(pkt);
		free(packetByteStream);
	}
	term_exitio();
	rs232_close();
	term_puts("\n<exit>\n");

	return 0;
}

