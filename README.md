# rotary-turbo-PCA9685
Rotary encoder turbo and PCA9685


Logarithmic applied scale to rotary encoder turbo. Turbo mode start if turning rapidly rotary weel and stop automatically after 3 seconds. Tested with stm32f103c8t6 (blue-pill) and PCA9685.
For used scale I added gnumeric spreadsheet for double type value control match with integers in arduino schetch.

You can modify this code according yours own needs.
Pls consider this: <br />
&nbsp;&nbsp;a - using for led dimming: <br />
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;with logarithmic derived scale consider apparent light intensity modulation not of light source but for whole light reflecting 3d area; <br />
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;with scale range (here 500 steps) and timing for turbo mode, you can adapt this code with whole 3d area regarding light reflecting caracteristics and rotary using mode also reducing 360° rotary turns number not needing very large single step luminosity difference; <br />
&nbsp;&nbsp;b - with motors, ... be  careful using turbo. :-)


My test environment:

![alt text](https://github.com/hvcoabtap/rotary-turbo-PCA9685/blob/master/Testing.JPG?raw=true)

If use Serial2, send string x\*y for pwm setting (e.g 0\*4095 means led0 always on, fixed on start to 0), for rate (frequency) use Rx (e.g. R24 for rate 24 Hz).


Arduino serial encoder debugging:

![alt text](https://github.com/hvcoabtap/rotary-turbo-PCA9685/blob/master/Screenshot%20at%202020-06-26%2016-52-42.png?raw=true)
