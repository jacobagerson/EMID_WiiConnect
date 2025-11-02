#include "ESP32Wiimote.h"
#include "Arduino.h"

ESP32Wiimote wiimote;

static bool logging = true;
static long last_ms = 0;
static int num_run = 0, num_updates = 0;

void setup()
{
    Serial.begin(115200);
    Serial2.begin(9600, SERIAL_8N1, 16, 17); // RX=16, TX=17

    

    pinMode(14, OUTPUT);
    pinMode(12, OUTPUT);
    pinMode(33, OUTPUT);
    pinMode(25, OUTPUT);
    pinMode(26, OUTPUT);
    pinMode(27, OUTPUT);

    Serial.println("ESP32Wiimote");

    
    
    wiimote.init();
    if (! logging)
        wiimote.addFilter(ACTION_IGNORE, FILTER_ACCEL); // optional
    
    Serial.println("Started");
    last_ms = millis();
}

void loop()
{
    wiimote.task();
    num_run++;

    if (wiimote.available() > 0) 
    {
        ButtonState  button  = wiimote.getButtonState();
        AccelState   accel   = wiimote.getAccelState();
        //NunchukState nunchuk = wiimote.getNunchukState();


        num_updates++;
        if (logging)
        {

            char ca     = (button & BUTTON_A)     ? 'A' : '.';
            if (ca == 'A') digitalWrite(14, HIGH); else digitalWrite(14, LOW);
            char cb     = (button & BUTTON_B)     ? 'B' : '.';
            if (cb == 'B') digitalWrite(12, HIGH); else digitalWrite(12, LOW);
            char cc     = (button & BUTTON_C)     ? 'C' : '.';
            char cz     = (button & BUTTON_Z)     ? 'Z' : '.';
            char c1     = (button & BUTTON_ONE)   ? '1' : '.';
            char c2     = (button & BUTTON_TWO)   ? '2' : '.';
            char cminus = (button & BUTTON_MINUS) ? '-' : '.';
            char cplus  = (button & BUTTON_PLUS)  ? '+' : '.';
            char chome  = (button & BUTTON_HOME)  ? 'H' : '.';
            char cleft  = (button & BUTTON_LEFT)  ? '<' : '.';
            if (cleft == '<') digitalWrite(33, HIGH); else digitalWrite(33, LOW);
            char cright = (button & BUTTON_RIGHT) ? '>' : '.';
            if (cright == '>') digitalWrite(26, HIGH); else digitalWrite(26, LOW);
            char cup    = (button & BUTTON_UP)    ? '^' : '.';
            if (cup == '^') digitalWrite(25, HIGH); else digitalWrite(25, LOW);
            char cdown  = (button & BUTTON_DOWN)  ? 'v' : '.';
            if (cdown == 'v') digitalWrite(27, HIGH); else digitalWrite(27, LOW);
            
    // move cursor to start of line and clear it so each update overwrites the previous line
    // Serial.print('\r');
    // for (int i = 0; i < 80; ++i) Serial.print(' ');
    // Serial.print('\r');
            Serial.printf("button: %05x = ", (int)button);
            Serial.print(ca);
            Serial.print(cb);
            Serial.print(cc);
            Serial.print(cz);
            Serial.print(c1);
            Serial.print(c2);
            Serial.print(cminus);
            Serial.print(chome);
            Serial.print(cplus);
            Serial.print(cleft);
            Serial.print(cright);
            Serial.print(cup);
            Serial.print(cdown);
            Serial.printf(", wiimote.axis: %3d/%3d/%3d", accel.xAxis, accel.yAxis, accel.zAxis);
            Serial.print("\n");
            // Serial.printf(", nunchuk.axis: %3d/%3d/%3d", nunchuk.xAxis, nunchuk.yAxis, nunchuk.zAxis);
            // Serial.printf(", nunchuk.stick: %3d/%3d\n", nunchuk.xStick, nunchuk.yStick);
        }
    }

    if (! logging)
    {
        long ms = millis();
        if (ms - last_ms >= 1000)
        {
            Serial.printf("Run %d times per second with %d updates\n", num_run, num_updates);
            num_run = num_updates = 0;
            last_ms += 1000;
        }
    }
    delay(10);
    
}