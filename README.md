# Arduino-A-SMPS-full_bridge-driver
Use a timer of Atmega to generate a pair of waveforms to drive switch mode power supplies in full bridge, half bridge, forward or backward topology at a constant frequency of 31kHz.

It senses voltage, current, temperature and fault feedback and is intended to be used with fancy A316J Optocoupler IGBT drivers. It utilizes fast PWM and fast Digital library found on GIthub. Many other pieces are also fouud online. It now works but it still is under heavy construction. You can probably figure out a way to use it on other Atmega lineup chips. 


Todo:
Add temperature control and fan control
Add display and on-board controls
publish PCB design
make transformer
make circuit
figure out how to do current sensing and if at all.
add a fancy decrease frequency to increase power mode at the upper duty cycle limit
