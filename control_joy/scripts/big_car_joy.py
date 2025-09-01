#!/usr/bin/env python3 
import rospy
import time
from std_msgs.msg import *
from sensor_msgs.msg import Joy
from geometry_msgs.msg import Twist
from nav_msgs.msg import Odometry
# aim_x = [1]
# aim_y = [1]
aim_x = []
aim_y = []
def odom_callback(msg):
    # global aim_x, aim_y
    while not rospy.is_shutdown():   
        while key_a :
            aim_x.append(msg.pose.pose.position.x)
            aim_y.append(msg.pose.pose.position.y)
    while not rospy.is_shutdown():     
        while key_b :
            aim_x.append(msg.pose.pose.position.x)
            aim_y.append(msg.pose.pose.position.y)
        
        # if key_b :
    #     aim_x[1] = msg.pose.pose.position.x
    #     aim_y[1] = msg.pose.pose.position.y
    # if key_x :
    #     aim_x[2] = msg.pose.pose.position.x
    #     aim_y[2] = msg.pose.pose.position.y


def callback_joy(msg):
    global Left_rocker_l_r, Left_rocker_f_a, Left_trigger_key, Right_rocker_l_r, Right_rocker_f_a, Right_trigger_key, Left_key_l_r, Left_key_f_a
    global key_a, key_b,key_x,key_y,key_left_shoulder_key,key_ringt_shoulder_key,key_back,key_start,key_home,key_left_rocker,key_right_rocker
    #rospy.loginfo(msg)
    #print(type(msg))

    axes = msg.axes
    buttons = msg.buttons
    try:   
        if not rospy.is_shutdown():
            #rospy.loginfo(axes)
            Left_rocker_l_r = axes[0]            #初始状态为0，最大为1，最小为-1。向左为正，向右为负 手柄左摇杆左右摆
            Left_rocker_f_a = axes[1]            #初始状态为0，最大为1，最小为-1。向前为正，向后为负 手柄左摇杆前后摆
            Left_trigger_key = axes[2]           #初始状态为1，按到底为-1                       手柄左摇杆的前方按键LT
            Right_rocker_l_r = axes[3]           #初始状态为0，最大为1，最小为-1。向左为正，向右为负 手柄右摇杆左右摆
            Right_rocker_f_a = axes[4]           #初始状态为0，最大为1，最小为-1。向前为正，向后为负 手柄右摇杆前后摆
            Right_trigger_key = axes[5]          #初始状态为1，按到底为-1                        手柄右摇杆的前方按键RT
            Left_key_l_r = axes[6]               #初始状态为0，最大为1，最小为-1。向左为正，向右为负 手柄左右按键
            Left_key_f_a = axes[7]               #初始状态为0，最大为1，最小为-1。向前为正，向后为负 手柄前后按键


            # 按键初始状态为0，按下后为1
            key_a = buttons[0]      #按键a
            key_b = buttons[1]      #按键b
            key_x = buttons[2]      #按键x
            key_y = buttons[3]      #按键y
            key_left_shoulder_key = buttons[4]
            key_ringt_shoulder_key = buttons[5]
            key_back = buttons[6]
            key_start = buttons[7]
            key_home = buttons[8]
            key_left_rocker = buttons[9]
            key_right_rocker = buttons[10]

            Left_trigger_key = (Left_trigger_key - 1) * (-1)
            Right_trigger_key = Right_trigger_key - 1
            #rospy.loginfo(Left_rocker_f_a)
            #print(type(Left_rocker_f_a))
        else:
            rospy.signal_shutdown("exit")
            exit()
    except KeyboardInterrupt as result:
        rospy.signal_shutdown("exit")
        print(result)

if __name__ == "__main__":

    rospy.init_node("sub_joy")
    rospy.Subscriber("/joy",Joy,callback_joy)
    pub = rospy.Publisher("/cmd_vel",Twist,queue_size=10)
    odom = rospy.Subscriber("/odom",Odometry,odom_callback)


    V_msg = Twist()
    global Left_rocker_l_r, Left_rocker_f_a, Left_trigger_key, Right_rocker_l_r, Right_rocker_f_a, Right_trigger_key, Left_key_l_r, Left_key_f_a
    global key_a, key_b,key_x,key_y,key_left_shoulder_key,key_ringt_shoulder_key,key_back,key_start,key_home,key_left_rocker,key_right_rocker
    
    Left_rocker_l_r = 0
    Left_rocker_f_a = 0
    Left_trigger_key = 0
    Right_rocker_l_r = 0
    Right_rocker_f_a = 0    
    Right_trigger_key = 0
    Left_key_l_r = 0
    Left_key_f_a = 0

    # 按键初始状态为0，按下后为1
    key_a = 0
    key_b = 0
    key_x = 0
    key_y = 0
    key_left_shoulder_key = 0
    key_ringt_shoulder_key = 0
    key_back = 0
    key_start = 0
    key_home = 0
    key_left_rocker = 0
    key_right_rocker = 0
    while not rospy.is_shutdown():
        try:
            #time.sleep(0.1)
            V_msg.linear.x = Left_rocker_f_a * 0.5
            V_msg.linear.y = Right_rocker_l_r * 0.5
            V_msg.angular.z=(Left_trigger_key + Right_trigger_key) / 2 
            rospy.loginfo(V_msg)
            pub.publish(V_msg)
    
            if key_a:
                with open("./pose1.txt","w",encoding="utf-8") as f:
                    f.write(str(aim_x[0]))
                    f.write(str(aim_y[0]))
            if key_b:
                with open("./pose2.txt","w",encoding="utf-8") as f:
                    f.write(str(aim_x[1]))
                    f.write(str(aim_y[1]))
            if key_x:
                with open("./pose3.txt","w",encoding="utf-8") as f:
                    f.write(str(aim_x[2]))
                    f.write(str(aim_y[2]))

            if rospy.is_shutdown():
                break
        except KeyboardInterrupt as result:
            rospy.signal_shutdown("exit")
            print(result)
   
    rospy.spin()
