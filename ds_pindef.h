#define DS_A_PIN 13
#define DS_B_PIN A0
#define DS_X_PIN A1
#define DS_Y_PIN A2
#define DS_L_PIN A3
#define DS_R_PIN A4
#define DS_DPAD_UP_PIN A5
#define DS_DPAD_DOWN_PIN 2
#define DS_DPAD_LEFT_PIN 1
#define DS_DPAD_RIGHT_PIN 4
#define DS_START_PIN 3
#define DS_SELECT_PIN 0

char ds_pin_map[8] = {DS_A_PIN, DS_B_PIN, DS_X_PIN, DS_Y_PIN, DS_L_PIN, DS_R_PIN, DS_START_PIN, DS_SELECT_PIN};

void ds_pin_init()
{
	pinMode(DS_A_PIN, INPUT);
	pinMode(DS_B_PIN, INPUT);
	pinMode(DS_X_PIN, INPUT);
	pinMode(DS_Y_PIN, INPUT);
	pinMode(DS_L_PIN, INPUT);
	pinMode(DS_R_PIN, INPUT);
	pinMode(DS_DPAD_UP_PIN, INPUT);
	pinMode(DS_DPAD_DOWN_PIN, INPUT);
	pinMode(DS_DPAD_LEFT_PIN, INPUT);
	pinMode(DS_DPAD_RIGHT_PIN, INPUT);
	pinMode(DS_START_PIN, INPUT);
	pinMode(DS_SELECT_PIN, INPUT);
}

// pull a pin to ground to press a button
void button_control(char pin, char state)
{
	if(pin < 0)
		return;

	if(state == 0)
		pinMode(pin, INPUT);
	else
	{
		pinMode(pin, OUTPUT);
		digitalWrite(pin, LOW);
	}
}
