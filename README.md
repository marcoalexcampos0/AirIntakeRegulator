# AirIntakeRegulator

The purpose of this project is to have a servo controlled by a remotely placed potenciometer.

This  is intended for an electric generator, that during start-up needs the air intake to be remotely controlled (from the boat cabin, while the generator is on the back/stern).

It has a calibration routine that allows to set the max and min values of the servo arm angle (0-180), and saves it to EEPROM. 

Calibration routine:

- Enter calibration routing: Do a long press on the button until you ear a second beep
- Set the max value: Press several times the button to get the servo into the max position
- Change to min value: Do a long press
- Set the min value: Press several times until it is on the min position
- Exit & save: Do a long press.
