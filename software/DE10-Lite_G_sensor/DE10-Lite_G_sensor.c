/*!----------------------------------------------------------------------
   Sierra Real-Time Kernel exampel code for NiosII

  Filename      :  DE10-Lite_G_sensor.c

  Date Changed  :  2020
  Description   :  System with five periodic tasks.
  Components    :  Sierra, real-time kernel HW and SW and 5 axis ADXL345 accelerometer chip.
			       The accelerometer chip is implemented on the DE10 Liete FPGA board.

-----------------------------------------------------
  Task Description:
  The system has five tasks. One to get the value from the accelerometer, one to calculate the average values of
  the accelerometer's values, one to show the execution time, one to plot the changs on the z-axeln and an idel task.
  All the task has deadline controll. The program uses globel variables which are protected by semaphors.

  Idle will run when the other tasks are in BLOCKED state (Witing for next period).

-----------------------------------------------------------------------*/

/* Sierra Driver Includes, hardware base adresses and also C libary */
#include "altera_avalon_sierra_ker.h"
#include <altera_avalon_sierra_io.h>
#include <altera_avalon_sierra_regs.h>
#include <altera_avalon_sierra_name.h>
#include <DE10_Lite_VGA_Driver.h>
#include "draw_vga.h"
#include "system.h" // for use of SOPC base-address definitons
#include "stdio.h"
#include "io.h"
#include <alt_types.h>
#include <altera_up_avalon_accelerometer_spi.h>
//#include <altera_avalon_timer_regs.h>



// TASK, in Sierra
#define IDLE 0 				// priority 0. Not allwaoed to be in blocked state.
#define Task_ACC_filter 1 	//Lowest priority. Runs every 10 seconds
#define Task_tid 3			//Runs every second
#define Task_plot 2			//Runs every second
#define Task_ACC 4			//Highest priority. Runs every seconds



#define MAX_PIXLAR 161




// TASK STACKS. Every task has its own stack.
#define STACK_SIZE 1000
char idle_stack[STACK_SIZE];
char Task_ACC_stack[STACK_SIZE];
char Task_ACC_filter_stack[STACK_SIZE];
char Task_tid_stack[STACK_SIZE];
char Task_plot_stack[STACK_SIZE];

/*A variable should be declared volatile whenever its value could change unexpectedly.
/for exampel Global variables within a multi-threaded application*/

volatile alt_32 x_data=0,y_data=0,z_data=0;			//are used to save the values fromm accelerometer
volatile alt_32 medel_x=0, medel_y=0, medel_z=0;	//are used to save the average values
alt_up_accelerometer_spi_dev* accelerometer;


//Semaphores
#define sem1 1

/********************************************************************************************
* Read X, Y, Z values from the accelerometer once a secound and write them on the VGA monitor
* *******************************************************************************************/
void task_acc_code(void){


		/*This function initializes the period time for the calling task. The period time = 50 which gives 1s.
		 * Inititialize period time for current task.*/
		init_period_time(50); //one secound period time
		task_periodic_start_union test;

while(1)
	{
		/*This function suspends the calling task until the start of next period time. Let current task wait for next period.*/
		test=wait_for_next_period(); //Every 10 secound
		if(test.periodic_start_integer & 0x01)
			printf("deadline miss, acc\n");

		sem_take(sem1);

		/*Read and print accelerometer values
		Prototype: int alt_up_accelerometer_spi_read_x_axis(alt_up_accelerometer_spi_dev *accel_spi, alt_32 *x_axis)
		Include: <altera_up_avalon_accelerometer.h>
		Parameters: accel – the device structure
		x_axis – a pointer to the location where the x-axis data should be stored
		Returns: 0 for success
		Description: Reads the x-xis value from both registers, DATAX0 and DATAX1, and
		converts the value to a signed integer.*/

		alt_up_accelerometer_spi_read_x_axis(accelerometer,&x_data);
		alt_up_accelerometer_spi_read_y_axis(accelerometer,&y_data);
		alt_up_accelerometer_spi_read_z_axis(accelerometer,&z_data);

	/* I de tre variablerna nedan läggs summan av 10st. data för att sedan
	 * använda summan i task task_acc_filter_code för att räkna medelvärdet
	 * på accelerometers axlar.
	 */
		medel_x +=x_data;
		medel_y +=y_data;
		medel_z +=z_data;

		sem_release(sem1);

		/*writes to consol*/

		printf("acc; x: %d, y: %d, z: %d\n",(int)x_data,(int)y_data,(int)z_data);

		/*void int_print(alt_32 x_start, alt_32 y_start, int data, int data_l, alt_32 color, alt_u32 BGcolor).
		*Funktionerna nedan är till för att skriva ut på VGA-skärmen*/
		int_print(45,30,(int)x_data, 3, 0xfff, 0x000);
		int_print(45,40,(int)y_data, 3, 0xfff, 0x000);
		int_print(45,50,(int)z_data, 3, 0xfff, 0x000);


	}


}
/******************************************************************************************************
 * skriver ut X, Y, Z med medelvärdet av 10 samlingsvärden. Tasket hämtar informationen från task_acc
 * ****************************************************************************************************/
void task_acc_filter_code(void){


	/*spara summan av de 10 värden i en register och sedan när det har gått 10 sek(10värden)
	 * så delas summan med 10 och värdet visas via denna funktion. Jag tänker att koden blir effektivare
	 * än om vi går in i denna task varje sekund för att spara värden osv, eller?*/

	init_period_time(500);//10 sek

	task_periodic_start_union test;


while(1)
	{
		/*This function suspends the calling task until the start of next period time. Let current task wait for next period.*/
		test=wait_for_next_period(); //Every 10 secound
		if(test.periodic_start_integer & 0x01)
			printf("deadline miss, acc_filter\n");
		sem_take(sem1);

		/*Först räknas medelvärdet, Sedan skrivs den ut på skärmen. Sist nollas den för nästa uträkningen.*/
		medel_x = medel_x/10;
		int_print(270,30,medel_x, 3, 0xfff, 0x000);
		medel_x=0;

		medel_y = medel_y/10;
		int_print(270,40,medel_y, 3, 0xfff, 0x000);
		medel_y=0;

		medel_z=medel_z/10;
		int_print(270,50,medel_z, 4, 0xfff, 0x000);
		medel_z=0;

		sem_release(sem1);

	}


}

/*********************************************************************************************************
* skriver ut tiden (start_tid) systemet har arbetat, d.v.s. från startögonblicket när knappen trycktes ner
* *********************************************************************************************************/
void task_tid_code(void)
{


    alt_32 seconds=0,minutes=0;
	task_periodic_start_union test;

    /*This function initializes the period time for the calling task. The period time = 50 which gives 1s.
     * Inititialize period time for current task.*/
    init_period_time(50); // one secound period time

    while(1) // Loop forever
    {
		/*This function suspends the calling task until the start of next period time. Let current task wait for next period.*/
      test=wait_for_next_period(); //Every secound
	  if(test.periodic_start_integer & 0x01)
		printf("deadline miss, timer\n");

      seconds = seconds +1;
	  if(seconds<60)
	  {
		int_print(20,150,(int)minutes, 2, 0xfff, 0x000);
		char_print(70,150,':', 0xfff, 0x000);
		int_print(90,150,(int)seconds, 2, 0xfff, 0x000);
	  }
	  else
	  {
		minutes++;
		seconds=0;
		int_print(20,150,(int)minutes, 2, 0xfff, 0x000);
		char_print(70,150,':', 0xfff, 0x000);
		int_print(90,150,(int)seconds, 2, 0xfff, 0x000);

	  }

    }

}


/*********************************************************************************************************
* Ritar ut en linje som visar förändringar på Z axeln
* *********************************************************************************************************/
void task_plot_code(void){

	//arrayen som sparar värden på z-axeln som ska ritas
	int draw_pixlar[MAX_PIXLAR], var=0;

	init_period_time(50); // one secound period time

	//test är en variabeln av typen task_periodic_start_union för att hålla koll på deadline miss.
	task_periodic_start_union test;

	alt_u32 y_end;

	draw_pixlar[0]=180;//y_start. Vi startar från mitten av den fjärde kvadranten

	while(1)
	{

		//printf("	---- plot -----\n");
		test=wait_for_next_period(); //Every 10 secound
		if(test.periodic_start_integer & 0x01)
			printf("deadline miss, plot\n");

		sem_take(sem1);

		/*Funktionen nedan har jag hämtat från en Arduino webplattform
		för att mappa värden från z-axeln mellan värde 239 -121. Detta för att
		ritningen av z-axeln ska hamna i den fjärde kvadranten på skärmen
		(x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min*/
		y_end = (((z_data - (-240)) * (239 - 121) / (300 - (-240))) + 121);

		sem_release(sem1);

		//Här fylls arrayen med värden
 		if(var == 0){
			for(int i=0;i<158;i++){
				draw_pixlar[i] = y_end;
			}
			draw_pixlar[158]=y_end;
			var=1;
		}

		//Här ritas grafen bl.a.
		else{
			for(int i=0;i<158;i++){
				draw_angled_line(i+161,draw_pixlar[i],i+162,draw_pixlar[i+1], 0xaaa);//två pixlar ritas åt gången
			}

			//"wait" loop innan grafen ritas om i svart färg i nästa loop
			for(int i=0;i<100000;i++)
			{;}

			/*Grafen ritas i svart färg och samtidigt flyttas innehållet i arrayen ett steg bakåt för att fylla i med nya z-värden
			i slutet på arrayen*/
			for(int i=0;i<158;i++){
			draw_angled_line(i+161,draw_pixlar[i],i+162,draw_pixlar[i+1], 0x000);//ritar om förra grafen fast med svart färg
				draw_pixlar[i]=draw_pixlar[i+1];
			}
			draw_pixlar[158]=y_end;
			var=2;
		}

		//printf("------ plot end	-------\n");
	}
}

 /*************************************************************************************************
 *Never blocked. Idle shall only be in running or ready state,
 lowest priority and taskid 0 ska skriva ut en punkt i consolen när den exekveras
 **************************************************************************************************/
void idle_task_code(void){

    int i=0;
  while(1) // Loop forever!
   {
	  for(i=0; i<500000; i++); //
	  	 printf(".");
  }
}


/*----------------------------Main---------------------------------------*/
int main (void){

		clear_screen(0x000);

		alt_8 startbutton=1;//Knappen som ska starta systemet i kortet och på skärmen

		//nedan initieras Sierra hardware, variables, interrupt and TCB (Task control Block).
		//denna funktion finns i "altera_avalon_sierra_ker.h"
		Sierra_Initiation_HW_and_SW();
		// Get HW Version
		printf("  Sierra HW version = %d\n", sierra_HW_version());
		printf("  Sierra SW driver version = %d\n", sierra_SW_driver_version());

		/*Initialize accelerometer. alt_up_accelerometer_spi_dev* = alt_up_accelerometer_spi_open_dev(const char *name)
		Include: <altera_up_avalon_accelerometer_spi_spi.h>
		Parameters: name – the accelerometer_spi component name in Qsys.
		Returns: The corresponding device structure, or NULL if the device is not found.
		Description: Opens the accelerometer_spi device specified by name*/
		accelerometer=alt_up_accelerometer_spi_open_dev(ACCELEROMETER_SPI_0_NAME);

		/*variabeln accelerometer_available är 8 bitar unsigned som år värdet för accelerometer i fkn ovan
		* och om accelerometer_available är = NULL så har initiering av accelerometer misslyckats  */
		alt_u8 accelerometer_available=(accelerometer!=0);
		if(!accelerometer_available) {
			printf("Failed to obtain accelerometer device pointer\n");
		}


		/*Här hade jag problem med att koden jag kopierade från mina tidigare program hette (PIO_IN_KEY_BASE)
		men ska hetta PIO_BUTTONS_IN_BASE i denna kod.
		När Key0 på kortet trycks får registret PIO_BUTTONS_IN_BASE som har adressen 0x8091680 värdet 0x01.
		startbutton = 00000010 & 00000001 vilket ger startbutton=0. Annars är värdet på adressen 0x8091680 = 0x03 när ingen
		av knapparna nedtryckta*/
		while(startbutton){
			startbutton = 0x02 & IORD_8DIRECT(PIO_BUTTONS_IN_BASE, 0);
			tty_print(90,110,"Demonstration of RTOS", 0xfff, 0x000);
			tty_print(100,130,"By: Saif Saadaldin", 0xfff, 0x000);
		}


		clear_screen(0x000);

		/*Ange värden och rita den horisontella linjen*/
		alt_32 x_hline=0, y_hline=120, length_hline=320,color_hline=0xfff;
		draw_hline(x_hline,y_hline,length_hline,color_hline);

		/*Ange värden för rita den vertikala linjen*/
		alt_32 x_vline=160, y_vline=0, length_vline=240,color_vline=0xfff;
		draw_vline(x_vline,y_vline,length_vline,color_vline);

		// void tty_print(alt_32 x_start, alt_32 y_start, alt_8 *tty_string,alt_32 color, alt_u32 BGcolor)
		tty_print(25,0,"X, Y and Z value", 0xfff, 0x000);
		tty_print(175,0,"Average X, Y and Z", 0xfff, 0x000);
		tty_print(25,125,"Execution time", 0xfff, 0x000);
		//tty_print(175,125,"Plott Z", 0xfff, 0x000);

		tty_print(30,30,"x: ", 0xfff, 0x000);
		tty_print(30,40,"y: ", 0xfff, 0x000);
		tty_print(30,50,"z: ", 0xfff, 0x000);

		tty_print(165,30,"medelvarde x: ", 0xfff, 0x000);
		tty_print(165,40,"medelvarde y: ", 0xfff, 0x000);
		tty_print(165,50,"medelvarde z: ", 0xfff, 0x000);

	  /*********************************************************************
	   * Define the clock tick in the system.
	   * A register is used to set-up internal clock tick period for all timing queues in the Sierra
	   * Sierra time base register value = time tick * system frequency/1000
	   * Initialize time base register.
	   * This example     : 50 MHz system-clock
	   * Wanted tick time : 20 ms (50Hz)
	   * Formula gives    : 20 ms x 50 MHx / 1000 => 1000(dec)
	   * ******************************************************************/
		//Sets the internal clock-tick timebase for the Sierra.
		set_timebase(1000);

		/* task_create() 	TaskID, priority, taskState, taskPtr,        stackPtr,  stackSize */

		task_create(IDLE, 0, READY_TASK_STATE, idle_task_code, idle_stack, STACK_SIZE);

		task_create(Task_ACC, 4, READY_TASK_STATE, task_acc_code, Task_ACC_stack, STACK_SIZE);

		task_create(Task_tid, 3, READY_TASK_STATE, task_tid_code, Task_tid_stack, STACK_SIZE);

		task_create(Task_plot, 2, READY_TASK_STATE, task_plot_code, Task_plot_stack, STACK_SIZE);

		task_create(Task_ACC_filter, 1, READY_TASK_STATE, task_acc_filter_code, Task_ACC_filter_stack, STACK_SIZE);

		// Start the Sierra scheduler
		tsw_on(); // enable CPU irq from Sierran and now at least idle will give a irq.

		while(1) {
			// Should never end up here...!
			printf ("* ERROR! SYSTEM FAILED *\n ");
		}
}





