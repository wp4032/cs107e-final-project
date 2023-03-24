## Project title
DREAMS DON'T DIE -- MOONWALKER
An EMG-based Brain Machine Interface to Control The World

## Team members
Willian Pan

## Project description
Our nerves communicate with all of our body through send electrical and chemical signals through neurons to the 
desired muscle. It is possible to read these signals from putting electrodes on the skin to detect when certain muscles
fire and relax. Here at DREAMS DON'T DIE Inc., we have designed a wristband that detects these electrical muscle signals
to determine when we have activated a muscle in our hand. As well, using an accelerometer, we can determine the position
of our hand in order to interact with the digital world through this wristband. The project is to build this device and have
it integrate with drawing on the screen like a simple Paint program.

## Instructions
To start the program, run `make` in terminal and then copy the `moonwalker.bin` file to the Raspberry Pi memory card and rename the file
to `kernel.img` while keeping the `bootcode.bin` and `start.elf`. 

Connect a USB A connection the Pi to power the device and also an HDMI cable to output the screen on any desired screen


## Member contribution
William built and designed the physical mechanical parts, circuitry, and software of the device

## References
This project was influenced by CTRL Labs, a company acquired by Meta, that is a wristband with EMG sensors used to interface 
in virtual reality. The device can differentiate when someone uses their fingers in VR, allowing better interaction with virtual 
reality.

The following project also cited several algorithms found online (they are also sprinkled throughout the source code in comments):
- Device Patent: https://patentimages.storage.googleapis.com/3c/5a/7b/39b871805f7eaa/US11045137.pdf
- EMG Circuit: https://www.instructables.com/Muscle-EMG-Sensor-for-a-Microcontroller/
- Tracking arm using accelerometer: https://synrg.csl.illinois.edu/papers/ArmTrak_Mobisys.pdf
- Accelerometer and gyroscope sensor fusion: https://github.com/seanboe/SimpleFusion/blob/master/src/simpleFusion.cpp
- Accelerometer, I2C, LSM6DS33: from CS 107E
- Square root function: https://www.codeproject.com/Articles/570700/SquareplusRootplusalgorithmplusforplusC
- Xiaolin Wu's line drawing algorithm: https://en.wikipedia.org/wiki/Xiaolin_Wu%27s_line_algorithm
- Bresenham circle drawing algorithm: https://www.gatevidyalay.com/bresenham-circle-drawing-algorithm/
- Circle drawing octant: https://www.cs.helsinki.fi/group/goa/mallinnus/ympyrat/ymp1.html

## Self-evaluation
I say that I am pretty proud of building something from the mechanical parts to the electrical parts to the software. It really broadens
my horizons of what full stack means and why hardware is hard. I was able to execute on half of my plan as I was able to get a commercial EMG circuit
board from AdvancerLabs working on Thursday March 23 afternoon, but broke it while soldering. My homemade circuit seemed to work on the oscilloscope, but 
when interfacing with the MCP3008 ADC, it seemed to not produce anything really substatantial. Even though there was this hiccup, I was still proud 
that I was able to get over that barrier and still complete something I am proud of. From this project, I realized why I am a builder and want to be a 
builder for the rest of my life.

## Photos
See doc directory
