# Arduino code for running context experiments in Pardes

This directory contains the Arduino code for running context experiments in Pardes. The code is designed to be used with the `py_ar_serial.py` script in the `scripts` directory.

The code is organized as follows:

- Pretrial interval: The Arduino waits for a specified time before starting the trial.
- Trial: composed of a preodor, an odor, and a postodor period. During the odor period, the Arduino sets a digital output pin to HIGH to activate the odor delivery system.
- Six fixed-lenght intertrial intervals: The Arduino waits for a specified time before starting the next trial.
- Posttrial interval: The Arduino waits for a specified time before concluding the experiment.

The lenght of all these periods can be adjusted by changing the values of the corresponding variables in the code.

The number of trial repetitions can be adjusted by changing the value of the integer in the _for_ loop within the `void loop()`:

``` c++
void loop() {
  preTrialInterval(preTrialDuration);
  // Main protocol execution
  for (int i = 0; i < 6; i++)
  ```

## Usage

1. Connect the Arduino to the computer via USB.
2. Upload the `ODU_GT.ino` file to the Arduino.
3. Run the `py_ar_serial.py` script as per the instructions in the `scripts` directory.
4. Reset the arduino to start the experiment.

## Note

The excel worksheet located in the directory can be used to calculate the time intervals for the experiment.
