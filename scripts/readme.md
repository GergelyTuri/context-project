# Recording serial output messages from the Arduino

`py_ar_serial.py` is a python script that reads serial output messages from an Arduino and saves them to a file. The script is designed to be used with the Arduino code in the `Arduino_code` directory.

## Usage

1. Connect the Arduino to the computer via USB.
2. upload `ODU_GT.ino` file to the Arduino.
3. Open an Anaconda prompt
4. Activate the conda environment with the required packages (e.g. `conda activate context-project`)
5. Navigate to the `scripts` directory
6. Run the script with the command `python py_ar_serial.py -ids mouse_id -p COMX`, where:
    - `mouse_id` is the ID of the mouse
    - `COMX` is the port where the Arduino is connected (e.g. COM3)

The collected data will be saved in a file named `mouse_id_YYYY-MM-DD_HH-MM-SS.json` in the `scripts/data` directory.

## Notes

Although not tested, the script can potentially read up to two serial ports simultaneously. To do this, run the script with the command `python py_ar_serial.py -ids mouse_id1 mouse_id2 -p COMX COMY`, where:

- `mouse_id1` and `mouse_id2` are the IDs of the mice
- `COMX` and `COMY` are the ports where the Arduinos are connected (e.g. COM3 and COM4)

Let the maintainers know if you want to try this feature for further recommendations.

## Known issues

- The script uses `delay()` in the Arduino code to control the timing of the events. This is not optimal for long-term experiments, as it may cause the Arduino to miss messages and make time intervals not-so-accurate. A better approach would be to use a timer interrupt to send messages at regular intervals.
- Due to this approach, the script may miss messages and/or the serial messages do not show up in the Anaconda window. It is also likely that the user needs to use `CTRL-C` to stop the script when the experiment is done.
