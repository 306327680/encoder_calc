//
// Created by echo on 23-3-30.
//

#include "../include/main.h"

#include <serial/serial.h>
#include <unistd.h>
#include "std_msgs/Int32.h"

main_fuction  m;
void odomCallback(const nav_msgs::Odometry::ConstPtr& msg)
{
    nav_msgs::Odometry result;
    result.header = msg->header;

    if(m.first_call){
        m.first_call = false;
    }else{
        double dt = (msg->header.stamp - m.last_time).toSec();
        double vx = (msg->pose.pose.position.x - m.distance_x_prev)/1000*M_PI*2*m.wheel_diameter/dt;
        double vy = (msg->pose.pose.position.y - m.distance_y_prev)/1000*M_PI*2*m.wheel_diameter/dt;
        vy = -vy;
        double Vave = (vx+vy)/2;
        m.yaw = m.yaw + asin((vx-vy)/m.wheel_distance) *dt ;
        m.curr_x = m.curr_x + Vave*dt*sin(m.yaw);
        m.curr_y = m.curr_y + Vave*dt*cos(m.yaw);
        std::cerr<<"vx: "<<vx<<" vy: "<<vy<<" (vx-vy)/m.wheel_distance): "<<(vx-vy)/m.wheel_distance
        <<" v ave: "<<Vave<<" yaw: "<<m.yaw<<" m.curr_x: "<<m.curr_x<<" m.curr_y: "<<m.curr_y<<std::endl;
    }
    result.pose.pose.position.y = m.curr_x;
    result.pose.pose.position.x = m.curr_y;
    result.pose.pose.orientation.z = sin(m.yaw/2 -M_PI_4) - cos(m.yaw/2-M_PI_4);
    result.pose.pose.orientation.w = cos(m.yaw/2-M_PI_4) + sin(m.yaw/2-M_PI_4);

    m.encoder_pub.publish(result);
    m.distance_x_prev = msg->pose.pose.position.x;
    m.distance_y_prev = msg->pose.pose.position.y;
    m.last_time = msg->header.stamp;
}
int main(int argc, char** argv)
{

    ros::init(argc, argv, "encoder_result");
    ros::NodeHandle nh("~");
    m.encoder_pub = nh.advertise<nav_msgs::Odometry>("encoder_result", 1000);

    ros::Subscriber sub = nh.subscribe("/encoder_com/encoder", 1000, odomCallback);
    ros::Rate r(100);
    while(ros::ok()){

        ros::spinOnce();
        r.sleep();
    }
    return 0;
}