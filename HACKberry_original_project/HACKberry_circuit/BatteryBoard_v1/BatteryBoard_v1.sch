EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:myParts
LIBS:BatteryBoard_v1-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "BatteryBoard_v1"
Date ""
Rev ""
Comp "exiii"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L DC_PLUG U1
U 1 1 556063DB
P 3900 4050
F 0 "U1" H 3900 4350 60  0000 C CNN
F 1 "DC_PLUG_MC2374L" H 3900 3750 60  0000 C CNN
F 2 "" H 3850 4050 60  0000 C CNN
F 3 "" H 3850 4050 60  0000 C CNN
	1    3900 4050
	1    0    0    -1  
$EndComp
Wire Wire Line
	4250 3950 5800 3950
Wire Wire Line
	4250 4150 5800 4150
Wire Notes Line
	3700 3850 3700 4250
Wire Notes Line
	3700 4250 4350 4250
Wire Notes Line
	4350 4250 4350 3850
Wire Notes Line
	4350 3850 3700 3850
Wire Notes Line
	5700 3850 5700 4250
Wire Notes Line
	5700 4250 6350 4250
Wire Notes Line
	6350 4250 6350 3850
Wire Notes Line
	6350 3850 5700 3850
$Comp
L SPRING_PROBE U2
U 1 1 55606759
P 6150 4050
F 0 "U2" H 6150 4350 60  0000 C CNN
F 1 "SPRING_PROBE_VN-3-1" H 6150 3750 60  0000 C CNN
F 2 "" H 6100 4050 60  0000 C CNN
F 3 "" H 6100 4050 60  0000 C CNN
	1    6150 4050
	-1   0    0    -1  
$EndComp
$EndSCHEMATC
