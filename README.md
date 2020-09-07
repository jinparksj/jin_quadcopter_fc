# Jin Quadcopter Flight Control
Personal development for quadcopter flight control firmware

# FC H/W

1. MCU: STM32F405R
2. Main 9 DOF Sensor IC: BNO080/085 - control 3 axis angle position
3. Sub 6 DOF Sensor IC: ICM-20602 - control 3 angular acceleration
4. Barometer: LPS22HH - hovering later
5. EEPROM: AT24C08 - memory for PID gain
6. LED and Buzzer for debugging
7. GPS/GNSS: UBlox M8N (Communication protocol - UBX (Not NMEA)
8. Trasmitter and Receiver: FS-i6, FS-iA6B based on i-BUS protocol
9. Electronic Speed Controller (ESC): Favourite LITTLEBEE 30A - S

# S/W

1. Tool: STM32CubeIDE
2. Language: C
3. Main libraries
	- BNO080
	- ICM20602
	- M8N UBX Parsing
4. ESC Firmware
    - Protocol: OneShot125 - PWM signals, Duty Cycle: 125us ~ 250us (8kHz ~ 4kHz)
    - Firmware: BLHeli_S
    - 10500 Steps in Duty Cycle
    - ESC Control Factors in STM32F4
        - Use TIM5 in STM32F4, CH1, CH2, CH3, and CH4 Generating by TIM5
        - TCLK: 84MHz
        - ARR(Auto Reload Register): 42000, Set as 41999 in STM32CubeIDE
        - PSC(Prescaler): 1, Set as 0 in STM32CubeIDE
        - CK_CNT = 84MHz / 1 = 84MHz
        - CCR(Capture/Compare Register): 10500 ~ 21000 -> Total Steps : 10500
        - Count Moder: Up Count
    
# CALIBRATION

1. BNO080 - 9 DOF Sensor
    - BNO080's MEMS sensor should be calibrated for best performance
    - Hilcrest Lab's "Dynamic Calibration Algorithms": It can calibrate the sensor simply by moving the device in specifice motions
    - The calibration results are saved in flash on the BNO080 in a file known as the Dynamic Calibration Data (DCD) file
    - Ideally the calibration process would be performed several feet away from high magnetic intefers like tables with magnetic components,
    desktop PC towes, monitors, etc. 
    - Configuration
        - Enable dynamic calibration for accelerometer, gyroscrope, and magnetometer
        - Enable Game Rotation Vector output
        - Enable Magnetic Field output
    - Calibration Procedure: Accelerometer -> Gyroscrope -> Magnetometer
        - Position the device in a relatively clean magnetic environment
        - Begin observing the Status bit of the Magnetic Field output
        - Perform the accelerometer calibration motions. The accelerometer will be calibrated after the device is moved into
        4-6 unique orientations and held in each orientation for ~1 second. One way to think about this is the "cube" method.
            - Orient the device so that it is sitting on each face of the cube sequentially
            - Hold the device in each position for 1 second
                - If one of the faces is difficult to position the device, you do not need to do all 6 faces.
                - The position do not need to be perfectly aligned with the cube faces
                - It does not matter which order the cube face positions are moved into
        - Perform the gyroscope calibration
            - Set the device down on a stationary surface for ~2-3 seconds to calibrate the gyroscope
        - Perform the magnetometer calibration
            - Rotate the device ~180degree and back to the beginning position in each axis(roll, pitch, yaw)
            - The speed of the rotation should be ~2 seconds per axis
            - Observe the Status bit of the magnetic field output
            - Continue rotations until the Magnetic Field Status bit reads 2 or 3 (medium or high) 
        - When finished with the calibration methods, run the Save DCD Now command, which will save this calibration data
        into flash
                   
2. ICM20602 - 6 DOF Sensor
    - Estimates the gyroscope offset values of X, Y, and Z
    - Multiply the offset values by -2
    - Write the values to each registers like below
        - ICM20602_Writebyte(0x13, (gyro_x_offset * -2) >> 8);
          ICM20602_Writebyte(0x14, (gyro_x_offset * -2));
          
          ICM20602_Writebyte(0x15, (gyro_y_offset * -2) >> 8);
          ICM20602_Writebyte(0x16, (gyro_y_offset * -2));
          
          ICM20602_Writebyte(0x17, (gyro_z_offset * -2) >> 8);
          ICM20602_Writebyte(0x18, (gyro_z_offset * -2));
        - Big Endian
        - Should write every boot-up process





