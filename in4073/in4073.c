/*------------------------------------------------------------------
 *  in4073.c -- test QR engines and sensors
 *
 *  reads ae[0-3] uart rx queue
 *  (q,w,e,r increment, a,s,d,f decrement)
 *
 *  prints timestamp, ae[0-3], sensors to uart tx queue
 *
 *  I. Protonotarios
 *  Embedded Software Lab
 *
 *  June 2016
 *------------------------------------------------------------------
 */

#include "in4073.h"
#include <stdlib.h>

#include "keyboard.h"

int mode = 0;
char currentByte;
char msgSize;
char msgType;
bool operation = 0;
unsigned char *ptr;
unsigned counter = 0;

struct LinkedList{
	char data;
	struct LinkedList *next;
};

typedef struct LinkedList *node;



node addNode(node firstNode, char newData){
	node temp;
	temp = (node)malloc(sizeof(struct LinkedList));
	temp->data = newData;
	if (firstNode != NULL){
		temp->next = firstNode;
	}else {
		temp->next = NULL;
	}
	return temp;
}

/*------------------------------------------------------------------
 * process_key -- process command keys
 *------------------------------------------------------------------
 */
void process_key(uint8_t c)
{
	switch (c)
	{
		case ONE:
			nrf_gpio_pin_toggle(YELLOW)
			break;
		case TWO:
			nrf_gpio_pin_toggle(BLUE)
			break;
		default:
			nrf_gpio_pin_toggle(RED);
	}
}


//Enums with all the possible states and events

enum stateList	{waitMsg, checkStartByte, getMsgSize, checkMessageType, setupMsg, getMsg, checkCRC}	currentState, nextState;

enum eventList	{noEvent, byteReceived, error, timeout} currentEvent;


//Event handling functions

void changeEvent(enum eventList newEventType){
	currentEvent = newEventType;
}

enum eventList getEvent(void){
	return currentEvent;
}

void clearEvents(void){
	currentEvent = noEvent;
}

/*------------------------------------------------------------------
 * State Machine 
 *------------------------------------------------------------------
 */


void stateHandler(){
	switch(currentState){
		case waitMsg:
			if(byteReceived) {
				nextState = checkStartByte;
				break;
			}	else if (noEvent || error)	{
				nextState = waitMsg;
				changeEvent(noEvent);
				break;
			}
			break;
		case checkStartByte:
			if(currentByte != 0xFF){
				nextState = waitMsg;
			}
			nextState = getMsgSize;
			break;
		case getMsgSize:
			nextState = checkMessageType;
			msgSize = currentByte - 1;
			break;
		case checkMessageType:
			msgType = currentByte;
			break;
		case setupMsg:
			ptr = malloc(msgSize);
			counter = 0;
			nextState = getMsg;
			break;
		case getMsg:
			*(ptr+counter) = currentByte;
			counter++;
			if (counter > msgSize) {
				nextState = checkCRC;
			} else {
				nextState = getMsg;
			}
			break;
		case checkCRC:
			break;		

	}
	currentState = nextState;

}

/*------------------------------------------------------------------
 * main -- everything you need is here :)
 *------------------------------------------------------------------
 */
int main(void)
{
	uart_init();
	gpio_init();
	timers_init();
	adc_init();
	twi_init();
	imu_init(true, 100);	
	baro_init();
	spi_flash_init();
	ble_init();
	node MsgBuffer = NULL;

	demo_done = true;

	while(operation) {
		if (rx_queue.count){  //continuously check for new elements in the UART queue
			currentByte = dequeue(&rx_queue);
			MsgBuffer = addNode(MsgBuffer, currentByte);
			changeEvent(byteReceived);
			stateHandler();
		}
	}
	
	

	/*while (!demo_done)
	{
		if (rx_queue.count) process_key( dequeue(&rx_queue) );

		if (check_timer_flag()) 
		{
			if (counter++%20 == 0) nrf_gpio_pin_toggle(BLUE);

			adc_request_sample();
			read_baro();

			printf("%10ld | ", get_time_us());
			printf("%3d %3d %3d %3d | ",ae[0],ae[1],ae[2],ae[3]);
			printf("%6d %6d %6d | ", phi, theta, psi);
			printf("%6d %6d %6d | ", sp, sq, sr);
			printf("%4d | %4ld | %6ld \n", bat_volt, temperature, pressure);

			clear_timer_flag();
		}

		if (check_sensor_int_flag()) 
		{
			get_dmp_data();
			run_filters_and_control();
		}
	}*/	

	//printf("\n\t Goodbye \n\n");
	nrf_delay_ms(100);

	NVIC_SystemReset();
}

//State machine for all the states




