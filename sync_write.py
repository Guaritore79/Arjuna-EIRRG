#!/usr/bin/env python
# -*- coding: utf-8 -*-

################################################################################
# Copyright 2017 ROBOTIS CO., LTD.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
################################################################################

# Author: Ryu Woon Jung (Leon)

#
# *********     Sync Write Example      *********
#
#
# Available Dynamixel model on this example : All models using Protocol 1.0
# This example is tested with two Dynamixel MX-28, and an USB2DYNAMIXEL
# Be sure that Dynamixel MX properties are already set as %% ID : 1 / Baudnum : 34 (Baudrate : 57600)
#

import os
import numpy as np

if os.name == 'nt':
    import msvcrt
    def getch():
        return msvcrt.getch().decode()
else:
    import sys, tty, termios
    fd = sys.stdin.fileno()
    old_settings = termios.tcgetattr(fd)
    def getch():
        try:
            tty.setraw(sys.stdin.fileno())
            ch = sys.stdin.read(1)
        finally:
            termios.tcsetattr(fd, termios.TCSADRAIN, old_settings)
        return ch

from dynamixel_sdk import *                    # Uses Dynamixel SDK library

def mapping(x, in_min, in_max, out_min,  out_max):
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min


# Konversi derajat ke posisi AX-12
def degree_to_position(degree):
    return (int)(mapping(degree, 0, 359, 0, 4095))

j2l = 80
j3l = 62


def inv_kin(x_input, y_input, z_input):
    v1 = np.arctan2(x_input, y_input) + np.deg2rad(180)
    h = np.sqrt((y_input * y_input) + (x_input * x_input))
    l = np.sqrt((h * h) + (z_input * z_input))

    #inverse cal
    # l = np.sqrt(y_input**2 + z_input**2)
    
    v3 = np.arccos((j2l**2 + j3l**2 - l**2) / (2 * j2l * j3l))
    v3 = v3 + np.deg2rad(90)
    
    vb = np.arccos((j2l**2 + l**2 - j3l**2) / (2 * j2l * l))
    va = np.arctan2(z_input, h)
    v2 = va + vb + np.deg2rad(180)

    v1deg = np.rad2deg(v1)
    v2deg = np.rad2deg(v2)
    v3deg = np.rad2deg(v3)

    return v1deg, v2deg, v3deg

#convert position to dynamixel
def legPosition (x_goal, y_goal, z_goal):
        y_rest = 80
        z_rest = -62

        y_goal_total = y_rest - y_goal
        z_goal_total = z_rest - z_goal

        v1deg, v2deg, v3deg = inv_kin(x_goal, y_goal_total, z_goal_total )

        pos1 = degree_to_position(v1deg)
        pos2 = degree_to_position(v2deg)
        pos3 = degree_to_position(v3deg)

        return pos1, pos2, pos3, v1deg, v2deg, v3deg

# Control table address
ADDR_MX_TORQUE_ENABLE      = 24               # Control table address is different in Dynamixel model
ADDR_MX_GOAL_POSITION      = 30
ADDR_MX_PRESENT_POSITION   = 36

# Data Byte Length
LEN_MX_GOAL_POSITION       = 4
LEN_MX_PRESENT_POSITION    = 4

# Protocol version
PROTOCOL_VERSION            = 1.0               # See which protocol version is used in the Dynamixel

# Default setting
# DXL1_ID                     = 1
# DXL2_ID                     = 2
# DXL3_ID                     = 3

BAUDRATE                    = 1000000             
DEVICENAME                  = '/dev/ttyUSB0'    # Check which port is being used on your controller
                                                

TORQUE_ENABLE               = 1                 # Value for enabling the torque
TORQUE_DISABLE              = 0                 # Value for disabling the torque
DXL_MINIMUM_POSITION_VALUE  = 0           # Dynamixel will rotate between this value
DXL_MAXIMUM_POSITION_VALUE  = 1023           # and this value (note that the Dynamixel would not move when the position value is out of movable range. Check e-manual about the range of the Dynamixel you use.)
DXL_MOVING_STATUS_THRESHOLD = 20                # Dynamixel moving status threshold

index = 0
dxl_goal_position = [DXL_MINIMUM_POSITION_VALUE, DXL_MAXIMUM_POSITION_VALUE]         # Goal position


# Initialize PortHandler instance
# Set the port path
# Get methods and members of PortHandlerLinux or PortHandlerWindows
portHandler = PortHandler(DEVICENAME)

# Initialize PacketHandler instance
# Set the protocol version
# Get methods and members of Protocol1PacketHandler or Protocol2PacketHandler
packetHandler = PacketHandler(PROTOCOL_VERSION)

# Initialize GroupSyncWrite instance
groupSyncWrite = GroupSyncWrite(portHandler, packetHandler, ADDR_MX_GOAL_POSITION, LEN_MX_GOAL_POSITION)

# Open port
if portHandler.openPort():
    print("Succeeded to open the port")
else:
    print("Failed to open the port")
    print("Press any key to terminate...")
    getch()
    quit()


# Set port baudrate
if portHandler.setBaudRate(BAUDRATE):
    print("Succeeded to change the baudrate")
else:
    print("Failed to change the baudrate")
    print("Press any key to terminate...")
    getch()
    quit()


# # Enable Dynamixel#1 Torque
# dxl_comm_result, dxl_error = packetHandler.write1ByteTxRx(portHandler, DXL1_ID, ADDR_MX_TORQUE_ENABLE, TORQUE_ENABLE)
# if dxl_comm_result != COMM_SUCCESS:
#     print("%s" % packetHandler.getTxRxResult(dxl_comm_result))
# elif dxl_error != 0:
#     print("%s" % packetHandler.getRxPacketError(dxl_error))
# else:
#     print("Dynamixel#%d has been successfully connected" % DXL1_ID)

# # Enable Dynamixel#2 Torque
# dxl_comm_result, dxl_error = packetHandler.write1ByteTxRx(portHandler, DXL2_ID, ADDR_MX_TORQUE_ENABLE, TORQUE_ENABLE)
# if dxl_comm_result != COMM_SUCCESS:
#     print("%s" % packetHandler.getTxRxResult(dxl_comm_result))
# elif dxl_error != 0:
#     print("%s" % packetHandler.getRxPacketError(dxl_error))
# else:
#     print("Dynamixel#%d has been successfully connected" % DXL2_ID)

#     print(f"✅ Servo {DXL2_ID} torque enabled")

# Enable All Dynamixel Torque
for i in range (1, 19):
    dxl_comm_result, dxl_error = packetHandler.write1ByteTxRx(portHandler, i, ADDR_MX_TORQUE_ENABLE, TORQUE_ENABLE)
    if dxl_comm_result != COMM_SUCCESS:
        print("%s" % packetHandler.getTxRxResult(dxl_comm_result))
    elif dxl_error != 0:
        print("%s" % packetHandler.getRxPacketError(dxl_error))
    else:
        print("Dynamixel#%d has been successfully connected" % i)

# -----------------------------
# Loop utama: Input derajat
# -----------------------------
try:
    while True:
        # Input derajat untuk kedua servo sekaligus
        # user_input = input(f"\nMasukkan derajat Servo {DXL1_ID} dan Servo {DXL2_ID} (format: derajat1 derajat2, contoh: 90 180) : ")
        
        # try:
        #     deg1, deg2 = map(int, user_input.split())
        # except:
        #     print("⚠️ Format salah! Contoh input: 90 180")
        #     continue

        # Konversi ke posisi Dynamixel

        userInput = input(f"\n Masukkan posisi x, y & z : ")
        try:
            val1, val2, val3 = map(int, userInput.split())
        except:
            print("⚠️ Format salah! Contoh input: 10 20 10")
            continue


        pos1, pos2, pos3, v1deg, v2deg, v3deg= legPosition(val1, val2, val3)

        # Buat param untuk servo 1
        param_goal_position1 = [
            DXL_LOBYTE(DXL_LOWORD(pos1)),
            DXL_HIBYTE(DXL_LOWORD(pos1)),
            DXL_LOBYTE(DXL_HIWORD(pos1)),
            DXL_HIBYTE(DXL_HIWORD(pos1))
        ]

        # Buat param untuk servo 2
        param_goal_position2 = [
            DXL_LOBYTE(DXL_LOWORD(pos2)),
            DXL_HIBYTE(DXL_LOWORD(pos2)),
            DXL_LOBYTE(DXL_HIWORD(pos2)),
            DXL_HIBYTE(DXL_HIWORD(pos2))
        ]

        # Buat param untuk servo 3
        param_goal_position3 = [
            DXL_LOBYTE(DXL_LOWORD(pos3)),
            DXL_HIBYTE(DXL_LOWORD(pos3)),
            DXL_LOBYTE(DXL_HIWORD(pos3)),
            DXL_HIBYTE(DXL_HIWORD(pos3))
        ]

        # Tambahkan ke sync write
        # groupSyncWrite.addParam(DXL1_ID, param_goal_position1)
        # groupSyncWrite.addParam(DXL2_ID, param_goal_position2)
        # groupSyncWrite.addParam(DXL3_ID, param_goal_position3)

        #Add to sync write with loop

        #Top
        for i in range(1, 18, 3):
            groupSyncWrite.addParam(i, param_goal_position1)

        #Middle
        for j in range (2, 18, 3):
            groupSyncWrite.addParam(j, param_goal_position2)
        
        #Bottom
        for k in range(3, 19, 3):
            groupSyncWrite.addParam(k, param_goal_position3)




        # Kirim perintah
        dxl_comm_result = groupSyncWrite.txPacket()
        if dxl_comm_result != COMM_SUCCESS:
            print(packetHandler.getTxRxResult(dxl_comm_result))

        # Bersihkan parameter
        groupSyncWrite.clearParam()
        print("Upper servo")
        for i in range(1, 18, 3):
            print(f"➡️ Servo {i} ke {v1deg}° (pos:{pos1})")
        print("Center servo") 
        for j in range (2, 18, 3):
            print(f"➡️ Servo {j} ke {v2deg}° (pos:{pos2})")
        print("Lower servo")
        for k in range(3, 19, 3):
            print(f"➡️ Servo {k} ke {v3deg}° (pos:{pos3})")
            

        # print(f"➡️ Servo {DXL1_ID} ke {v1deg}° (pos:{pos1}), Servo {DXL2_ID} ke {v2deg}° (pos:{pos2}), Servo {DXL3_ID} ke {v3deg}° (pos:{pos3})")

except KeyboardInterrupt:
    print("\n🛑 Program dihentikan oleh user")

# Disable Dynamixel#1 Torque
# dxl_comm_result, dxl_error = packetHandler.write1ByteTxRx(portHandler, DXL1_ID, ADDR_MX_TORQUE_ENABLE, TORQUE_DISABLE)
# if dxl_comm_result != COMM_SUCCESS:
#     print("%s" % packetHandler.getTxRxResult(dxl_comm_result))
# elif dxl_error != 0:
#     print("%s" % packetHandler.getRxPacketError(dxl_error))

# # Disable Dynamixel#2 Torque
# dxl_comm_result, dxl_error = packetHandler.write1ByteTxRx(portHandler, DXL2_ID, ADDR_MX_TORQUE_ENABLE, TORQUE_DISABLE)
# if dxl_comm_result != COMM_SUCCESS:
#     print("%s" % packetHandler.getTxRxResult(dxl_comm_result))
# elif dxl_error != 0:
#     print("%s" % packetHandler.getRxPacketError(dxl_error))

# packetHandler.write1ByteTxRx(portHandler, DXL1_ID, ADDR_MX_TORQUE_ENABLE, TORQUE_DISABLE)
# packetHandler.write1ByteTxRx(portHandler, DXL2_ID, ADDR_MX_TORQUE_ENABLE, TORQUE_DISABLE)

for i in range (1, 19):
    # Disable All Dynamixel Torque
    dxl_comm_result, dxl_error = packetHandler.write1ByteTxRx(portHandler, i, ADDR_MX_TORQUE_ENABLE, TORQUE_DISABLE)
    if dxl_comm_result != COMM_SUCCESS:
        print("%s" % packetHandler.getTxRxResult(dxl_comm_result))
    elif dxl_error != 0:
        print("%s" % packetHandler.getRxPacketError(dxl_error))
    
    packetHandler.write1ByteTxRx(portHandler, i, ADDR_MX_TORQUE_ENABLE, TORQUE_DISABLE)



# Close port
portHandler.closePort()
print("✅ Torque dimatikan & port ditutup")
