#include "ros/ros.h"
#include "math.h"
#include <move_base_msgs/MoveBaseActionResult.h>
#include <visualization_msgs/Marker.h>
#include <visualization_msgs/MarkerArray.h>
#include "geometry_msgs/PointStamped.h"
#include "geometry_msgs/PoseStamped.h"
#include "actionlib/client/simple_action_client.h"
#include "actionlib/server/simple_action_server.h"
#include "iostream"
#include "fstream"
int count=0,index_text=0,flag=0;
int try_again=1;
bool file1_flag=true,file2_flag=true,file3_flag=true;
visualization_msgs::MarkerArray MarkerArry;//创建makerarry数组存储marker对象
ros::Subscriber point_sub;
ros::Subscriber res_sub;
ros::Publisher goal_pub;
ros::Publisher mult_pub;
ros::Publisher point_pub;
void point_pub_funtion()
{
    std::ifstream file1("src/mult_nav/dis/distance1_data.txt");
    if(!file1)
    {
        ROS_INFO("无法打开文件1!");
        file1_flag=false;
    }
    double value1,aim1[3];
    int i=0;
    while(file1>>value1)
    {
        aim1[i]=value1;
        i++;
    }
   
    geometry_msgs::PoseStamped get_aim1;
    get_aim1.header.frame_id="map";
    get_aim1.header.stamp=ros::Time::now();
    get_aim1.pose.position.x=aim1[0];
    get_aim1.pose.position.y=aim1[1];
    get_aim1.pose.position.z=aim1[2];
    if(file1_flag)
    {
        point_pub.publish(get_aim1);
    }
    ros::Duration delay(1);
    delay.sleep();
    std::ifstream file2("src/mult_nav/dis/distance2_data.txt");
    if(!file2)
    {
        ROS_INFO("无法打开文件2!");
         file2_flag=false;
    }
    double value2,aim2[3];
    int j=0;
    while(file2>>value2)
    {
        aim2[j]=value2;
        j++;
    }
    geometry_msgs::PoseStamped get_aim2;
    get_aim2.header.frame_id="map";
    get_aim2.header.stamp=ros::Time::now();
    get_aim2.pose.position.x=aim2[0];
    get_aim2.pose.position.y=aim2[1];
    get_aim2.pose.position.z=aim2[2];
    if(file2_flag)point_pub.publish(get_aim2);
    delay.sleep();
    std::ifstream file3("src/mult_nav/dis/distance3_data.txt");
    if(!file3)
    {
        ROS_INFO("无法打开文件3!"); 
        file3_flag=false;
    }
    double value3,aim3[3];
    int t=0;
    while(file3>>value3)
    {
        aim3[t]=value3;
        t++;
    }
    geometry_msgs::PoseStamped get_aim3;
    get_aim3.header.frame_id="map";
    get_aim3.header.stamp=ros::Time::now();
    get_aim3.pose.position.x=aim3[0];
    get_aim3.pose.position.y=aim3[1];
    get_aim3.pose.position.z=aim3[2];
    if(file3_flag)point_pub.publish(get_aim3);
}
void do_aimpoint(const geometry_msgs::PointStamped::ConstPtr &msg)
{
    ROS_INFO("添加一个新的目标点%d",count);
    visualization_msgs::Marker marker;//创建marker对象
    marker.header.frame_id="map";
    marker.type=marker.TEXT_VIEW_FACING;//一直面向屏幕的字符格式

    marker.action=marker.ADD;
    marker.scale.x=1;
    marker.scale.y=1;
    marker.scale.z=1;
    //颜色设置
    marker.color.a=1;
    marker.color.r=1;
    marker.color.g=0;
    marker.color.b=0;
    marker.pose.position.x=msg->point.x;
    marker.pose.position.y=msg->point.y;
    marker.pose.position.z=msg->point.z;
    printf("%lf",marker.pose.position.x);
    printf("%lf",marker.pose.position.y);
    printf("%lf",marker.pose.position.z);
    marker.text=(count);
    
    MarkerArry.markers.push_back(marker);//将marker对象加入markerarry数组
    int m=0;
    for(int id=0;id<MarkerArry.markers.size();id++)
    {
        MarkerArry.markers[id].id=m;
        m+=1;
    }
    goal_pub.publish(MarkerArry);
     
    if(count==0)
    {
        geometry_msgs::PoseStamped pose;
        pose.header.frame_id="map";
        pose.header.stamp=ros::Time::now();
        pose.pose.position.x=msg->point.x;
        pose.pose.position.y=msg->point.y;
        pose.pose.orientation.z=0;
        pose.pose.orientation.w=1;

        mult_pub.publish(pose);
        index_text+=1;
    }
    count+=1;
}
void ifget_aimpoint(const move_base_msgs::MoveBaseActionResult::ConstPtr &msg)//判断是否达到目标点的回调函数
{

    if(msg->status.status==3&&count>0)//这里是为了与2d nav goal区分开，因为点击2dnavgoal后count=0，所以不会进入到该判断中，只有点击——publish_point才会进入该判断
    {

        try_again=1;
        // ROS_INFO("成功到达目标点");
        if(index_text==count)
        {
            ROS_INFO("成功到达目标点%d",index_text-1);
            if(flag==1)
            {
                index_text=-1;
                geometry_msgs::PoseStamped ps1;//前往下一个目标点
                ps1.header.frame_id="map";
                ps1.header.stamp=ros::Time::now();
                ps1.pose.position.x=MarkerArry.markers[index_text].pose.position.x;
                ps1.pose.position.y=MarkerArry.markers[index_text].pose.position.y;
                ps1.pose.orientation.z=0;
                ps1.pose.orientation.w=1;
                
                mult_pub.publish(ps1);
                index_text+=1;
            }
            
        }
        if(index_text<count)//说明该点还不是终点
        {
            printf("%d ",count);
            ROS_INFO("成功到达目标点%d",index_text-1);
            geometry_msgs::PoseStamped ps2;
            ps2.header.frame_id="map";
            ps2.header.stamp=ros::Time::now();
            ps2.pose.position.x=MarkerArry.markers[index_text].pose.position.x;
            ps2.pose.position.y=MarkerArry.markers[index_text].pose.position.y;
            ps2.pose.orientation.z=0;
            ps2.pose.orientation.w=1;

            mult_pub.publish(ps2);
            index_text+=1;
        }

    }
    else if(count>0)
    {
        ROS_INFO("不能到达目标点%d",index_text-1);
        if(try_again==1)
        {
            ROS_INFO("在次尝试到达目标点%d",index_text-1);
            geometry_msgs::PoseStamped ps3;
            ps3.header.frame_id="map";
            ps3.header.stamp=ros::Time::now();
            ps3.pose.position.x=MarkerArry.markers[index_text-1].pose.position.x;
            ps3.pose.position.y=MarkerArry.markers[index_text-1].pose.position.y;
            ps3.pose.orientation.z=0;
            ps3.pose.orientation.w=1;
            mult_pub.publish(ps3);
            try_again=0;
        }
        else if(index_text<MarkerArry.markers.size())
        {
            ROS_INFO("尝试到达目标点%d失败",index_text-1);
            if(index_text==count)
            index_text=0;
            geometry_msgs::PoseStamped ps4;
            ps4.header.frame_id="map";
            ps4.header.stamp=ros::Time::now();
            ps4.pose.position.x=MarkerArry.markers[index_text].pose.position.x;
            ps4.pose.position.y=MarkerArry.markers[index_text].pose.position.y;
            ps4.pose.orientation.z=0;
            ps4.pose.orientation.w=1;
            mult_pub.publish(ps4);
            index_text+=1;
        }
    }
}
void processing()
{
    flag=1;
}
int main(int argc, char *argv[])
{
    
    setlocale(LC_ALL,"");
    ros::init(argc,argv,"mult_nav");
    ros::NodeHandle nh;
    goal_pub=nh.advertise<visualization_msgs::MarkerArray>("/path_point",100);
    mult_pub=nh.advertise<geometry_msgs::PoseStamped>("/move_base_simple/goal",1);
    point_sub=nh.subscribe("/point",10,do_aimpoint);
    point_pub=nh.advertise<geometry_msgs::PointStamped>("/point",10);
    res_sub=nh.subscribe("/move_base/result",10,ifget_aimpoint);
    point_pub_funtion();
    ros::Rate loop_rate(100);
    while(ros::ok())
    {
        processing();
        ros::spinOnce();
        loop_rate.sleep();
    }
    return 0;
}
