/**
 * \file
 * \brief Main file for the WE-SensorFeatherWing.
 *
 * \copyright (c) 2020 Würth Elektronik eiSos GmbH & Co. KG
 *
 * \page License
 *
 * THE SOFTWARE INCLUDING THE SOURCE CODE IS PROVIDED “AS IS”. YOU ACKNOWLEDGE
 * THAT WÜRTH ELEKTRONIK EISOS MAKES NO REPRESENTATIONS AND WARRANTIES OF ANY
 * KIND RELATED TO, BUT NOT LIMITED TO THE NON-INFRINGEMENT OF THIRD PARTIES’
 * INTELLECTUAL PROPERTY RIGHTS OR THE MERCHANTABILITY OR FITNESS FOR YOUR
 * INTENDED PURPOSE OR USAGE. WÜRTH ELEKTRONIK EISOS DOES NOT WARRANT OR
 * REPRESENT THAT ANY LICENSE, EITHER EXPRESS OR IMPLIED, IS GRANTED UNDER ANY
 * PATENT RIGHT, COPYRIGHT, MASK WORK RIGHT, OR OTHER INTELLECTUAL PROPERTY
 * RIGHT RELATING TO ANY COMBINATION, MACHINE, OR PROCESS IN WHICH THE PRODUCT
 * IS USED. INFORMATION PUBLISHED BY WÜRTH ELEKTRONIK EISOS REGARDING
 * THIRD-PARTY PRODUCTS OR SERVICES DOES NOT CONSTITUTE A LICENSE FROM WÜRTH
 * ELEKTRONIK EISOS TO USE SUCH PRODUCTS OR SERVICES OR A WARRANTY OR
 * ENDORSEMENT THEREOF
 *
 * THIS SOURCE CODE IS PROTECTED BY A LICENSE.
 * FOR MORE INFORMATION PLEASE CAREFULLY READ THE LICENSE AGREEMENT FILE LOCATED
 * IN THE ROOT DIRECTORY OF THIS PACKAGE
 */
#include "sensorBoard.h"

// USB-Serial debug Interface
TypeSerial *SerialDebug;

// Sensors
// WE absolute pressure sensor object
PADS *sensorPADS;
// WE Temperature sensor object
TIDS *sensorTIDS;
// WE humidity sensor object
HIDS *sensorHIDS;

void setup() {
    delay(2000);
    // Using the USB serial port for debug messages
    SerialDebug = SSerial_create(&Serial);
    SSerial_begin(SerialDebug, 115200);

    // Create sensor objects
    sensorPADS = PADSCreate(SerialDebug);
    sensorTIDS = TIDSCreate(SerialDebug);
    sensorHIDS = HIDSCreate(SerialDebug);

    // Initialize the sensors in default mode
    if (!PADS_simpleInit(sensorPADS)) {
        SSerial_printf(SerialDebug, "PADS init failed \r\n");
    }
    if (!TIDS_simpleInit(sensorTIDS)) {
        SSerial_printf(SerialDebug, "TIDS init failed \r\n");
    }
    if (!HIDS_simpleInit(sensorHIDS)) {
        SSerial_printf(SerialDebug, "HIDS init failed \r\n");
    }
}

// int SSerial_available(TypeSerial *m);
// int SSerial_read(TypeSerial *m);

void loop() {
    //  Check for data request, then read all sensors
    if (SSerial_available(SerialDebug) > 0) {
        if (PADS_readSensorData(sensorPADS)) {
            if (TIDS_readSensorData(sensorTIDS)) {
                if (HIDS_readSensorData(sensorHIDS)) {
                    // if all okay, send data over USB in CSV format
                    SSerial_printf(SerialDebug, "%f,%f,%f,\n",
                                   sensorTIDS->data[tidsTemperature],
                                   sensorHIDS->data[hidsRelHumidity],
                                   sensorPADS->data[padsPressure]);
                }
            }
        }
        // flush incoming buffer
        while (SSerial_available(SerialDebug) > 0) {
            SSerial_read(SerialDebug);
        }
    }
    delay(200);
}
