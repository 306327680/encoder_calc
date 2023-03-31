//
// Created by echo on 23-3-30.
//

#ifndef ENCODER_CALC_MAIN_H
#define ENCODER_CALC_MAIN_H

#include "nav_msgs/Odometry.h"
#include "ros/ros.h"
class main_fuction {
public:
    main_fuction(){};
    ros::Publisher encoder_pub ;
    double wheel_distance = 0.7;
    double wheel_diameter = 0.22;
    double wheel_diameter_r = 0.22;
    double wheel_diameter_l = 0.22;
    double distance_x_prev = 0;
    double distance_y_prev = 0;
    ros::Time last_time;
    double yaw = 0;
    double curr_x =0;
    double curr_y =0;
    bool first_call = true;

};


#endif //ENCODER_CALC_MAIN_H
