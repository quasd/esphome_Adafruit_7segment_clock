#include "esphome.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"

int hours = 0;
int minutes = 0;
int state = 0;
bool blinkColon = true;
char old_time[4];
Adafruit_7segment matrix = Adafruit_7segment();

class MyCustomComponent : public Component
{
public:
    void setup() override
    {
        matrix.begin(0x70);
        matrix.println("0000");
        matrix.writeDisplay();
    }
    void loopDisplay(){
        for (int i=0; i < 5; i++){
            if (state == 0){
                //matrix.writeDigitRaw(0, 8);
                matrix.writeDigitRaw(0, 1);
                matrix.writeDigitRaw(1, 2);
                matrix.writeDigitRaw(3, 4);
                matrix.writeDigitRaw(4, 8);
                state++;
            } else if (state == 1 ) {
                //matrix.writeDigitRaw(0, 16);
                matrix.writeDigitRaw(0, 2);
                matrix.writeDigitRaw(1, 4);
                matrix.writeDigitRaw(3, 8);
                matrix.writeDigitRaw(4, 16);
                state++;
           } else if (state == 2 ) {
                matrix.writeDigitRaw(0, 4);
                matrix.writeDigitRaw(1, 8);
                matrix.writeDigitRaw(3, 16);
                matrix.writeDigitRaw(4, 32);
                state++;
           } else if (state == 3 ) {
                matrix.writeDigitRaw(0, 8);
                matrix.writeDigitRaw(1, 16);
                matrix.writeDigitRaw(3, 32);
                matrix.writeDigitRaw(4, 1);
                state++;
           } else if (state == 4 ) {
                matrix.writeDigitRaw(0, 16);
                matrix.writeDigitRaw(1, 32);
                matrix.writeDigitRaw(3, 1);
                matrix.writeDigitRaw(4, 2);
                state++;
           } else if (state == 5 ) {
                matrix.writeDigitRaw(0, 32);
                matrix.writeDigitRaw(1, 1);
                matrix.writeDigitRaw(3, 2);
                matrix.writeDigitRaw(4, 4);
                state = 0;
           }
            matrix.writeDisplay();
            delay(30);
        }
    }
    void loop() override
    {
        auto date_now = id(homeassistant_time).now();
        char time_to_print[4];
        sprintf(time_to_print, "%02i%02i", date_now.hour, date_now.minute);

        if ( ! date_now.is_valid()){
            loopDisplay();
        } else {
            for (int i = 0;  i < 4; i++){
                if( time_to_print[i] != old_time[i] ) {
                    for (int j=0; j < 2; j++){
                        loopDisplay();
                    }
                    ESP_LOGD("custom","time should be updated to : %s", time_to_print);
                    break;
                }
            }
            matrix.println(time_to_print);
            if (date_now.second%2 == 0){
                matrix.drawColon(true);
            } else{
                matrix.drawColon(false);
            }


            //ESP_LOGD("custom","time should be updated to : %s", String(minutes)[0]);
            matrix.writeDisplay();
            strncpy(old_time, time_to_print, 4);
            delay(1000);
        }
    }
};
