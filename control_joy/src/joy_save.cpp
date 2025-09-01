#include <ros/ros.h>
#include "sensor_msgs/Joy.h"
#include "tf2_ros/buffer.h"
#include "geometry_msgs/Twist.h"
#include "std_msgs/String.h"
#include "nav_msgs/Odometry.h"
#include "tf2_ros/transform_listener.h"
#include <iostream>
#include <fstream>
using namespace std;
double left_rocker_f_b,Right_rocker_l_r,Left_trigger_key,Right_trigger_key,key_a,key_b,key_x;
double aim_x[3],aim_y[3],aim_z[3];
void joy_callback(const sensor_msgs::Joy::ConstPtr &msg)
{
    try
    {
         if(ros::ok())
        {
            left_rocker_f_b=msg->axes[1];
            Right_rocker_l_r=msg->axes[3];
            Left_trigger_key=msg->axes[2];
            Right_trigger_key=msg->axes[5];

            key_a=msg->buttons[0];
            key_b=msg->buttons[1];
            key_x=msg->buttons[2];

            Left_trigger_key = (Left_trigger_key - 1) * (-1);
            Right_trigger_key = Right_trigger_key - 1;
       }
        else 
           ros::shutdown();
    }
    catch(const std::exception &e)
    {
        ROS_INFO("错误");
    }
        
   
}
int main(int argc, char *argv[])
{
    setlocale(LC_ALL,"");
    ros::init(argc,argv,"control_joy");
    ros::NodeHandle nh;
    ros::Subscriber joy_sub=nh.subscribe("/joy",10,joy_callback);
    ros::Publisher cmd_pub=nh.advertise<geometry_msgs::Twist>("/cmd_vel",100);
    tf2_ros::Buffer buffer;
    tf2_ros::TransformListener sub(buffer);
    ros::Rate loop_rate(6);
    
    while(ros::ok())
    {
        try
        {
             if(key_a==1)
            {
                geometry_msgs::TransformStamped base_link_to_map1=buffer.lookupTransform("map","base_link",ros::Time(0));
                aim_x[0]=base_link_to_map1.transform.translation.x;
                aim_y[0]=base_link_to_map1.transform.translation.y;
                aim_z[0]=base_link_to_map1.transform.translation.z;
                std::ofstream file("distance1_data.txt");
                file<<aim_x[0]<<endl<<aim_y[0]<<endl<<aim_z[0];
                system("play /home/robotprg/bigcar1_ws/src/bigcar_nav/voice/getpoint.mp3");
            }
            if(key_b==1)
            {
                geometry_msgs::TransformStamped base_link_to_map2=buffer.lookupTransform("map","base_link",ros::Time(0));
                aim_x[1]=base_link_to_map2.transform.translation.x;
                aim_y[1]=base_link_to_map2.transform.translation.y;
                aim_z[1]=base_link_to_map2.transform.translation.z;
                std::ofstream file("distance2_data.txt");
                file<<aim_x[1]<<endl<<aim_y[1]<<endl<<aim_z[1];
                system("play /home/robotprg/bigcar1_ws/src/bigcar_nav/voice/getpoint.mp3");
            }
            if(key_x==1)
            {
                geometry_msgs::TransformStamped base_link_to_map3=buffer.lookupTransform("map","base_link",ros::Time(0));
                aim_x[2]=base_link_to_map3.transform.translation.x;
                aim_y[2]=base_link_to_map3.transform.translation.y;
                aim_z[2]=base_link_to_map3.transform.translation.z;
                std::ofstream file("distance3_data.txt");
                file<<aim_x[2]<<endl<<aim_y[2]<<endl<<aim_z[2];
                system("play /home/robotprg/bigcar1_ws/src/bigcar_nav/voice/getpoint.mp3");
            }
            geometry_msgs::Twist vel;
            vel.linear.x=left_rocker_f_b*0.5;
            vel.linear.y=Right_rocker_l_r*0.5;
            vel.angular.z=(Left_trigger_key + Right_trigger_key) / 2 ;
            cmd_pub.publish(vel);
            if (ros::isShuttingDown())
            {
                break;
            }

        }
        catch(const std::exception &e)
        {
            ROS_INFO("错误");
        }
        ros::spinOnce();
        loop_rate.sleep();
    }
       
    return 0;
}
