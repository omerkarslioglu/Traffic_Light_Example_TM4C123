![tm4c123examples ](https://user-images.githubusercontent.com/67158049/125168162-b27b9480-e1ac-11eb-943c-11a753f56c37.png)


# Traffic Light Example | TM4C123
General Purpose of This Laboratory : Making a traffic light scenario using the final state machine and with SysTick that is the peripheral unit of the microcontroller .

![image](https://user-images.githubusercontent.com/67158049/125168064-41d47800-e1ac-11eb-9345-1c064e75d1f6.png)


# Pin/Port Selection :

OUTPUTS :

PE1 , PE2 and PE3 are connected to south leds .
PE1 is connected to red .
PE2 is connected to yellow .
PE3 is connected to green .
(SOUTH PE1-3)
PD1 , PD2 and PD3 are connected to west leds .
PD1 is connected to red .
PD2 is connected to yellow .
PD3 is connected to green .
(WEST PD1-3)
PE4 and PE5 are connected to walking leds .
PE4 is connected to red .
PE5 is connected to green .
Pin/Port Selection :

INPUTS :

PA2 , PA3 and PA4 are connected to switches .
Circular reading value is 1 . I used pull up ressistor .
If switches are pressed , the data is read as 0 .
All of them are active low .

![image](https://user-images.githubusercontent.com/67158049/125167897-6c720100-e1ab-11eb-8a1c-cad461cecba2.png)

# Designing Final State Machine

Considering all the first diagnostic situations, I determined the outputs as follows.
Note that the format of the leds is :
Walking Leds (GREEN RED)-South Leds (GREEN YELLOW RED) - West Leds (GREEN YELLOW RED)

![image](https://user-images.githubusercontent.com/67158049/125167936-a8a56180-e1ab-11eb-95e5-b2f2e33d76f3.png)

![image](https://user-images.githubusercontent.com/67158049/125167943-b35ff680-e1ab-11eb-9630-be0eedf8e526.png)

![image](https://user-images.githubusercontent.com/67158049/125167947-c07ce580-e1ab-11eb-8386-b0c81a3c8d6a.png)

![Screenshot from 2023-07-18 10-20-32](https://github.com/omerkarslioglu/Traffic_Light_Example_TM4C123/assets/67158049/8336575a-c5c0-4638-ae95-5819ac1066e4)

Thanks for reading ...

Also you can look to the report for details .

Ömer Karslıoğlu



