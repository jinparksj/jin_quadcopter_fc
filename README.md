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
    











