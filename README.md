# rotary-turbo-PCA9685
Rotary encoder turbo and PCA9685


Logarithmic applied scale to rotary encoder turbo. Turbo mode start if turning rapidly rotary weel and stop automatically after 3 seconds. Tested with blue-pill and PCA9685.
For used scale I added gnumeric spreadsheet for double type value control match with integers in arduino schetch.

Test environment:

![alt text](https://github.com/hvcoabtap/rotary-turbo-PCA9685/blob/master/Testing.JPG?raw=true)

If use Serial2, send string x\*y for pwm setting (e.g 0\*4095 means led0 always on, fixed on start to 0), for rate (frequency) use Rx (e.g. R24 for rate 24 Hz).


Arduino serial encoder debugging:

![alt text](https://github.com/hvcoabtap/rotary-turbo-PCA9685/blob/master/Screenshot%20at%202020-06-26%2016-52-42.png?raw=true)
