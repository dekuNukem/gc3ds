#define DATA_PIN 7
#define CPAD_X_PIN 5
#define CPAD_Y_PIN 6
#include <EEPROM.h>
#include "Nintendo.h"
#include "nokia_lcd.h"
#include "ds_pindef.h"
#include "setting_menu.h"

void setup()
{
	ds_pin_init();
	map_init();
	pinMode(CPAD_X_PIN, OUTPUT);
	pinMode(CPAD_Y_PIN, OUTPUT);	
	analogWrite(CPAD_X_PIN, 127);
	analogWrite(CPAD_Y_PIN, 127);
	lcd.clear();
	lcd.gotoXY(0, 0);
	lcd.print("waiting for");
	lcd.gotoXY(0, 2);
	lcd.print("GameCube");
	lcd.gotoXY(0, 4);
	lcd.print("controller");

	while(Gamecube.begin(DATA_PIN) == false);

	if(Gamecube.read(false) && Gamecube.report.start == 1)
		do_menu();

	lcd.clear();
	lcd.gotoXY(0, 0);
	lcd.print("controller");
	lcd.gotoXY(0, 1);
	lcd.print("found");
	TCCR0B = TCCR0B & 0b11111000 | 0x01;
}

char get_3ds_pin(char index)
{
	if(ds_button_map[index] >= 0 and ds_button_map[index] <= 7)
		return ds_pin_map[ds_button_map[index]];
	else
		return -1;
}

void loop()
{
	read_gc_controller();

	button_control(get_3ds_pin(GC_A_BUTTON), Gamecube.report.a);
	button_control(get_3ds_pin(GC_B_BUTTON), Gamecube.report.b);
	button_control(get_3ds_pin(GC_X_BUTTON), Gamecube.report.x);
	button_control(get_3ds_pin(GC_Y_BUTTON), Gamecube.report.y);
	button_control(get_3ds_pin(GC_L_TRIGGER), Gamecube.report.left > 127);
	button_control(get_3ds_pin(GC_R_TRIGGER), Gamecube.report.right > 127);
	button_control(get_3ds_pin(GC_L_BUTTON), Gamecube.report.l);
	button_control(get_3ds_pin(GC_R_BUTTON), Gamecube.report.r);
	button_control(get_3ds_pin(GC_Z_BUTTON), Gamecube.report.z);
	button_control(get_3ds_pin(GC_START_BUTTON), Gamecube.report.start);

	// for 3ds circle pad
	if(ds_button_map[GC_MAIN_STICK] == DS_CIRCLE_PAD)
	{
		analogWrite(CPAD_X_PIN, (int)(255 - Gamecube.report.xAxis));
		analogWrite(CPAD_Y_PIN, (int)(Gamecube.report.yAxis));
	}
	if(ds_button_map[GC_C_STICK] == DS_CIRCLE_PAD)
	{
		analogWrite(CPAD_X_PIN, (int)(255 - Gamecube.report.cxAxis));
		analogWrite(CPAD_Y_PIN, (int)(Gamecube.report.cyAxis));
	}

	// for 3ds d-pad
	if(ds_button_map[GC_DPAD] == DS_DPAD)
	{
		button_control(DS_DPAD_UP_PIN, Gamecube.report.dup);
		button_control(DS_DPAD_DOWN_PIN, Gamecube.report.ddown);
		button_control(DS_DPAD_LEFT_PIN, Gamecube.report.dleft);
		button_control(DS_DPAD_RIGHT_PIN, Gamecube.report.dright);
	}
	if(ds_button_map[GC_C_STICK] == DS_DPAD)
	{
		button_control(DS_DPAD_LEFT_PIN, Gamecube.report.cxAxis < 80);
		button_control(DS_DPAD_RIGHT_PIN, Gamecube.report.cxAxis > 180);
		button_control(DS_DPAD_UP_PIN, Gamecube.report.cyAxis > 170);
		button_control(DS_DPAD_DOWN_PIN, Gamecube.report.cyAxis < 70);
	}
	if(ds_button_map[GC_MAIN_STICK] == DS_DPAD)
	{
		button_control(DS_DPAD_LEFT_PIN, Gamecube.report.xAxis < 80);
		button_control(DS_DPAD_RIGHT_PIN, Gamecube.report.xAxis > 180);
		button_control(DS_DPAD_UP_PIN, Gamecube.report.yAxis > 170);
		button_control(DS_DPAD_DOWN_PIN, Gamecube.report.yAxis < 70);
	}
	
}
