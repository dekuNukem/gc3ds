#define UNDEFINED -1
#define GC_A_BUTTON 0
#define GC_B_BUTTON 1
#define GC_X_BUTTON 2
#define GC_Y_BUTTON 3
#define GC_L_TRIGGER 4
#define GC_L_BUTTON 5
#define GC_R_TRIGGER 6
#define GC_R_BUTTON 7
#define GC_Z_BUTTON 8
#define GC_START_BUTTON 9
#define GC_MAIN_STICK 10
#define GC_DPAD 11
#define GC_C_STICK 12
#define DS_A_BUTTON 0
#define DS_B_BUTTON 1
#define DS_X_BUTTON 2
#define DS_Y_BUTTON 3
#define DS_L_BUTTON 4
#define DS_R_BUTTON 5
#define DS_START_BUTTON 6
#define DS_SELECT_BUTTON 7
#define DS_CIRCLE_PAD 8
#define DS_DPAD 9
#define BUF_SIZE 13

nokia_lcd lcd(12, 11, 10, 9, 8, 175);
char ds_button_map[13];
char print_buf[BUF_SIZE];

void do_remap();
void sel_3ds_button(char remap_menu_pos);


/*
	prepare to witness some truly messy code
	I wrote this while staying up late and being a little drunk :)
	but I think it works
*/

void get_ds_name(char index, char* buf)
{
	for(int i = 0; i < BUF_SIZE; i++)
		buf[i] = 0;

	switch(index)
	{
		case DS_A_BUTTON:
		sprintf(buf, "A button  ");
		break;

		case DS_B_BUTTON:
		sprintf(buf, "B button  ");
		break;

		case DS_X_BUTTON:
		sprintf(buf, "X button  ");
		break;

		case DS_Y_BUTTON:
		sprintf(buf, "Y button  ");
		break;

		case DS_L_BUTTON:
		sprintf(buf, "L button  ");
		break;

		case DS_R_BUTTON:
		sprintf(buf, "R button  ");
		break;

		case DS_START_BUTTON:
		sprintf(buf, "start btn ");
		break;

		case DS_SELECT_BUTTON:
		sprintf(buf, "selct btn ");
		break;

		case DS_CIRCLE_PAD:
		sprintf(buf, "circlePad ");
		break;

		case DS_DPAD:
		sprintf(buf, "d-pad     ");
		break;

		case UNDEFINED:
		case 127:
		sprintf(buf, "unmapped  ");
		break;

		default:
		sprintf(buf, "ds err %d", index);
		break;
	}
}

void get_gc_name(char index, char* buf)
{

	for(int i = 0; i < BUF_SIZE; i++)
		buf[i] = 0;

	switch(index)
	{
		case GC_A_BUTTON:
		sprintf(buf, "A button  ");
		break;

		case GC_B_BUTTON:
		sprintf(buf, "B button  ");
		break;

		case GC_X_BUTTON:
		sprintf(buf, "X button  ");
		break;

		case GC_Y_BUTTON:
		sprintf(buf, "Y button  ");
		break;

		case GC_L_TRIGGER:
		sprintf(buf, "L trigger ");
		break;

		case GC_L_BUTTON:
		sprintf(buf, "L button  ");
		break;

		case GC_R_TRIGGER:
		sprintf(buf, "R trigger ");
		break;

		case GC_R_BUTTON:
		sprintf(buf, "R button  ");
		break;

		case GC_Z_BUTTON:
		sprintf(buf, "Z button ");
		break;

		case GC_START_BUTTON:
		sprintf(buf, "start btn ");
		break;

		case GC_MAIN_STICK:
		sprintf(buf, "main stick");
		break;

		case GC_DPAD:
		sprintf(buf, "d-pad     ");
		break;

		case GC_C_STICK:
		sprintf(buf, "c stick   ");
		break;

		default:
		sprintf(buf, "gc err %d", index);
		break;
	}
}

void map_init()
{
	for(int i = 0; i < 13; i++)
		ds_button_map[i] = EEPROM.read(i);
}

void mapping_reset()
{
	EEPROM.write(GC_A_BUTTON, DS_A_BUTTON);
	EEPROM.write(GC_B_BUTTON, DS_B_BUTTON);
	EEPROM.write(GC_X_BUTTON, DS_X_BUTTON);
	EEPROM.write(GC_Y_BUTTON, DS_Y_BUTTON);
	EEPROM.write(GC_L_TRIGGER, DS_L_BUTTON);
	EEPROM.write(GC_L_BUTTON, UNDEFINED);
	EEPROM.write(GC_R_TRIGGER, DS_R_BUTTON);
	EEPROM.write(GC_R_BUTTON, UNDEFINED);
	EEPROM.write(GC_Z_BUTTON, UNDEFINED);
	EEPROM.write(GC_START_BUTTON, DS_START_BUTTON);
	EEPROM.write(GC_MAIN_STICK, DS_CIRCLE_PAD);
	EEPROM.write(GC_DPAD, DS_DPAD);
	EEPROM.write(GC_C_STICK, UNDEFINED);
}

void read_gc_controller()
{
	if(Gamecube.read(false))
		return;
	else
	{
		lcd.clear();
		lcd.gotoXY(0, 0);
		lcd.print("connect");
		lcd.gotoXY(0, 2);
		lcd.print("GameCube");
		lcd.gotoXY(0, 4);
		lcd.print("controller");
		while(Gamecube.begin(DATA_PIN) == false);
		lcd.clear();
		lcd.gotoXY(0, 0);
		lcd.print("controller");
		lcd.gotoXY(0, 1);
		lcd.print("found");
	}
}

bool unique_press_a()
{
	Gamecube.read(false);
	if(Gamecube.report.a == 1)
	{
		while(Gamecube.report.a == 1)
			Gamecube.read(false);
		return true;
	}
	else
		return false;
}

bool unique_press_b()
{
	Gamecube.read(false);
	if(Gamecube.report.b == 1)
	{
		while(Gamecube.report.b == 1)
			Gamecube.read(false);
		return true;
	}
	else
		return false;
}

bool unique_press_left()
{
	Gamecube.read(false);
	if(Gamecube.report.dleft == 1)
	{
		while(Gamecube.report.dleft == 1)
			Gamecube.read(false);
		return true;
	}
	else
		return false;
}

bool unique_press_right()
{
	Gamecube.read(false);
	if(Gamecube.report.dright == 1)
	{
		while(Gamecube.report.dright == 1)
			Gamecube.read(false);
		return true;
	}
	else
		return false;
}

// 12 char per row
void do_menu()
{
	char main_menu_pos = 0;
	char main_menu_item = 3;
	lcd.clear();
	while(1)
	{

		if(unique_press_left())
			main_menu_pos - 1 < 0 ? main_menu_pos = 0 : main_menu_pos--;
		if(unique_press_right())
			main_menu_pos + 1 >= main_menu_item ? main_menu_pos = main_menu_item : main_menu_pos++;

		if(main_menu_pos == 0)
		{
			lcd.gotoXY(0, 0);
			lcd.print("change      ");
			lcd.gotoXY(0, 1);
			lcd.print("button map");
			if(unique_press_a())
				do_remap();
		}
		else if(main_menu_pos == 1)
		{
			lcd.gotoXY(0, 0);
			lcd.print("reset      ");
			lcd.gotoXY(0, 1);
			lcd.print("mapping    ");
			if(unique_press_a())
			{
				mapping_reset();
				map_init();
				lcd.clear();
				lcd.gotoXY(0, 0);
				lcd.print("done");
				delay(2000);
			}
		}
		else
		{
			lcd.gotoXY(0, 0);
			lcd.print("unknown   ");
			lcd.gotoXY(0, 1);
			lcd.print("page    ");
		}

		lcd.gotoXY(0, 4);
		lcd.print("<>: browse");
		lcd.gotoXY(0, 5);
		lcd.print("A: select");

		if(unique_press_b())
		{
			lcd.clear();
			return;
		}
	}
}

void do_remap()
{
	char remap_menu_pos = 0;
	char remap_menu_item = 12;
	lcd.clear();
	while(1)
	{

		if(unique_press_left())
			remap_menu_pos - 1 < 0 ? remap_menu_pos = 0 : remap_menu_pos--;
		if(unique_press_right())
			remap_menu_pos + 1 >= remap_menu_item ? remap_menu_pos = remap_menu_item : remap_menu_pos++;

		lcd.gotoXY(0, 0);
		lcd.print("map gc:");

		lcd.gotoXY(0, 1);
		lcd.print(">");
		lcd.gotoXY(0, 3);
		lcd.print(" ");
		
		lcd.gotoXY(0, 2);
		lcd.print("to 3ds:");

		lcd.gotoXY(2, 1);
		get_gc_name(remap_menu_pos, print_buf);
		lcd.print(print_buf);

		lcd.gotoXY(2, 3);
		get_ds_name(ds_button_map[remap_menu_pos], print_buf);
		lcd.print(print_buf);

		lcd.gotoXY(0, 4);
		lcd.print("<>: browse");
		lcd.gotoXY(0, 5);
		lcd.print("A/B:sel/back");
		if(unique_press_a())
			sel_3ds_button(remap_menu_pos);
		if(unique_press_b())
		{
			lcd.clear();
			return;
		}
	}
}

void sel_3ds_button(char remap_menu_pos)
{
	lcd.gotoXY(0, 1);
	lcd.print(" ");
	lcd.gotoXY(0, 3);
	lcd.print(">");

	lcd.gotoXY(0, 4);
	lcd.print("<>: browse");
	lcd.gotoXY(0, 5);
	lcd.print("A/B:sel/back");

	bool is_button = remap_menu_pos < 10;
	char option = ds_button_map[remap_menu_pos];

	while(1)
	{
		if(unique_press_left())
			if(is_button)
				option - 1 < -1 ? option = -1 : option--;
			else
				option - 1 < 7 ? option = 7 : option--;
		if(unique_press_right())
			if(is_button)
				option + 1 > 7 ? option = 7 : option++;
			else
				option + 1 > 9 ? option = 9 : option++;

		char fff = option;
		if(!is_button)
			if(option <= 7)
			{
				option = 7;
				fff = UNDEFINED;
			}

		lcd.gotoXY(2, 3);
		get_ds_name(fff, print_buf);
		lcd.print(print_buf);

		if(unique_press_a())
		{
			EEPROM.write(remap_menu_pos, fff);
			map_init();
			return;
		}

		if(unique_press_b())
			return;
	}
}