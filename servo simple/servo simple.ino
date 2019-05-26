#include <Servo.h>
Servo servo1;
int degree, serial_data = 0 , read_degree = 0;
void setup()
{
 Serial.begin(9600);
 servo1.attach(9); // تعیین این که سرو موتورمون به کدوم پایه وصل هستش
 }

void loop()
{
servo1.write(40);
delay(2000);
//servo1.write(-150);
//delay(2000);
servo1.write(0);
delay(2000);
//servo1.write(-50);

}