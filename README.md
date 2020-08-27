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

# S/W

1. Tool: STM32CubeIDE
2. Language: C
3. Main libraries
	- BNO080
	- ICM20602
	- M8N UBX Parsing
	











