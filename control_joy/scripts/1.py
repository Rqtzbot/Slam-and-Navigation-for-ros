#!/usr/bin/env python3 
import rospy
import time
from std_msgs.msg import *
from sensor_msgs.msg import Joy
from nav_msgs.msg import Odometry

if __name__ == "__main__":
    with open("./pose.txt", "w", encoding="utf-8") as f:
            f.write(str(1) + "\n")
            f.write(str(1) + "\n")