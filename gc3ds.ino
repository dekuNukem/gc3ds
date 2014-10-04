#define DATA_PIN 7
#define CPAD_X_PIN 5
#define CPAD_Y_PIN 6
#include <EEPROM.h>
#include "Nintendo.h"
#include "nokia_lcd.h"
#include "ds_pindef.h"
#include "setting_menu.h"
#include <stdlib.h>

void setup()
{
	ds_pin_init();
	map_init();
	pinMode(CPAD_X_PIN, OUTPUT);
	pinMode(CPAD_Y_PIN, OUTPUT);	
	analogWrite(CPAD_X_PIN, 30);
	analogWrite(CPAD_Y_PIN, 200);
	lcd.clear();
	lcd.gotoXY(0, 0);
	lcd.print("waiting for");
	lcd.gotoXY(0, 2);
	lcd.print("GameCube");
	lcd.gotoXY(0, 4);
	lcd.print("controller");

	// wait for gc controller 
	while(Gamecube.begin(DATA_PIN) == false);

	// go into settings menu if gc controllerstart button is held down
	if(Gamecube.read(false) && Gamecube.report.start == 1)
		do_menu();

	lcd.clear();
	lcd.gotoXY(0, 0);
	lcd.print("controller");
	lcd.gotoXY(0, 1);
	lcd.print("found");
	/* 
		switching timer0 prescaler to 1 so that PWM frequency on pin 5 and 6
		is 62.5KHz, making it easier to filter.
		for details see: http://playground.arduino.cc/Main/TimerPWMCheatsheet
	*/
	TCCR0B = TCCR0B & 0b11111000 | 0x01;
}

void map_button(char gc_index)
{
	// find the corresponding 3DS index, given gc controller index
	char ds_index = ds_button_map[gc_index];

	if(ds_index < 0 || ds_index > 7)
		return;
	
	// if any of its mappings is 1, press that button
	for(int i = 0; i < 10; i++)
		if(ds_button_map[i] == ds_index && gc_report[i])
		{
			button_control(ds_pin_map[ds_index], 1);
			return;
		}

	button_control(ds_pin_map[ds_index], 0);
}


void loop()
{
	read_gc_controller();

	gc_report[GC_A_BUTTON] = Gamecube.report.a;
	gc_report[GC_B_BUTTON] = Gamecube.report.b;
	gc_report[GC_X_BUTTON] = Gamecube.report.x;
	gc_report[GC_Y_BUTTON] = Gamecube.report.y;
	gc_report[GC_L_TRIGGER] = Gamecube.report.left > 127;
	gc_report[GC_L_BUTTON] = Gamecube.report.l;
	gc_report[GC_R_TRIGGER] = Gamecube.report.right > 127;
	gc_report[GC_R_BUTTON] = Gamecube.report.r;
	gc_report[GC_Z_BUTTON] = Gamecube.report.z;
	gc_report[GC_START_BUTTON] = Gamecube.report.start;

	map_button(GC_A_BUTTON);
	map_button(GC_B_BUTTON);
	map_button(GC_X_BUTTON);
	map_button(GC_Y_BUTTON);
	map_button(GC_L_TRIGGER);
	map_button(GC_L_BUTTON);
	map_button(GC_R_TRIGGER);
	map_button(GC_R_BUTTON);
	map_button(GC_Z_BUTTON);
	map_button(GC_START_BUTTON);

	if(is_c_stick_smash_enabled)
	{
		while((Gamecube.report.cxAxis < 100 && Gamecube.report.cxAxis > 80) || 
			(Gamecube.report.cxAxis > 150 && Gamecube.report.cxAxis < 170) || 
			(Gamecube.report.cyAxis < 100 && Gamecube.report.cyAxis > 80) || 
			(Gamecube.report.cyAxis > 150 && Gamecube.report.cyAxis < 170))
		{
			read_gc_controller();
			analogWrite(CPAD_X_PIN, (int)(127));
			analogWrite(CPAD_Y_PIN, (int)(127));
			button_control(DS_A_PIN, 0);
		}

		while(Gamecube.report.cxAxis < 80 || Gamecube.report.cxAxis > 170 || Gamecube.report.cyAxis > 170 || Gamecube.report.cyAxis < 80)
		{
			read_gc_controller();

			//  left samsh
			if(Gamecube.report.cxAxis < 80)
			{
				analogWrite(CPAD_X_PIN, (int)(255));
				analogWrite(CPAD_Y_PIN, (int)(127));
			}

			//  right samsh
			else if(Gamecube.report.cxAxis > 170)
			{
				analogWrite(CPAD_X_PIN, (int)(0));
				analogWrite(CPAD_Y_PIN, (int)(127));
			}

			//  up samsh
			else if(Gamecube.report.cyAxis > 170)
			{
				analogWrite(CPAD_X_PIN, (int)(127));
				analogWrite(CPAD_Y_PIN, (int)(255));
			}

			//  down samsh
			else
			{
				analogWrite(CPAD_X_PIN, (int)(127));
				analogWrite(CPAD_Y_PIN, (int)(0));
			}
			
			button_control(DS_A_PIN, 1);
		}
	}
	else
	{
		// if GC main stick is mapped to 3DS's circle pad
		if(ds_button_map[GC_MAIN_STICK] == DS_CIRCLE_PAD)
		{
			analogWrite(CPAD_X_PIN, (int)(255 - Gamecube.report.xAxis));
			analogWrite(CPAD_Y_PIN, (int)(Gamecube.report.yAxis));
		}
		
		// if GC c-stick is mapped to 3DS's circle pad
		if(ds_button_map[GC_C_STICK] == DS_CIRCLE_PAD)
		{
			analogWrite(CPAD_X_PIN, (int)(255 - Gamecube.report.cxAxis));
			analogWrite(CPAD_Y_PIN, (int)(Gamecube.report.cyAxis));
		}
	}

	// if GC main stick is mapped to 3DS's circle pad
	if(ds_button_map[GC_MAIN_STICK] == DS_CIRCLE_PAD)
	{
		analogWrite(CPAD_X_PIN, (int)(255 - Gamecube.report.xAxis));
		analogWrite(CPAD_Y_PIN, (int)(Gamecube.report.yAxis));
	}

	// if GC main stick is mapped to 3DS d-pad
	if(ds_button_map[GC_MAIN_STICK] == DS_DPAD)
	{
		button_control(DS_DPAD_LEFT_PIN, Gamecube.report.xAxis < 80);
		button_control(DS_DPAD_RIGHT_PIN, Gamecube.report.xAxis > 180);
		button_control(DS_DPAD_UP_PIN, Gamecube.report.yAxis > 170);
		button_control(DS_DPAD_DOWN_PIN, Gamecube.report.yAxis < 70);
	}

	// if GC d-pad is mapped to 3DS's circle pad
	// why would anyone do this? Not implemented.
	if(ds_button_map[GC_DPAD] == DS_CIRCLE_PAD)
	{
		analogWrite(CPAD_X_PIN, 127);
		analogWrite(CPAD_Y_PIN, 127);
	}

	// if GC d-pad is mapped to 3DS d-pad
	if(ds_button_map[GC_DPAD] == DS_DPAD)
	{
		button_control(DS_DPAD_UP_PIN, Gamecube.report.dup);
		button_control(DS_DPAD_DOWN_PIN, Gamecube.report.ddown);
		button_control(DS_DPAD_LEFT_PIN, Gamecube.report.dleft);
		button_control(DS_DPAD_RIGHT_PIN, Gamecube.report.dright);
	}

	// if GC c-stick is mapped to 3DS d-pad
	if(ds_button_map[GC_C_STICK] == DS_DPAD)
	{
		button_control(DS_DPAD_LEFT_PIN, Gamecube.report.cxAxis < 80);
		button_control(DS_DPAD_RIGHT_PIN, Gamecube.report.cxAxis > 180);
		button_control(DS_DPAD_UP_PIN, Gamecube.report.cyAxis > 170);
		button_control(DS_DPAD_DOWN_PIN, Gamecube.report.cyAxis < 70);
	}

	// if GC c-stick is mapped to 3DS's circle pad
	if(ds_button_map[GC_C_STICK] == DS_CIRCLE_PAD)
	{
		analogWrite(CPAD_X_PIN, (int)(255 - Gamecube.report.cxAxis));
		analogWrite(CPAD_Y_PIN, (int)(Gamecube.report.cyAxis));
	}

}
