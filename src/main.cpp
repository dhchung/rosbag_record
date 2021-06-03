#include <ros/ros.h>
#include <sensor_msgs/Image.h>
#include <std_msgs/Bool.h>
#include <std_msgs/String.h>
#include <string>
#include <sys/stat.h>
#include <rosbag/bag.h>
#include <sensor_msgs/PointCloud2.h>
#include <sensor_msgs/Imu.h>


#define PREFIX_PATH "/mnt/DataDisk/"
bool data_logging;
std::string data_prefix = "Stoplogging";
std::string stop_logging_msg = "Stoplogging";
std::string dir_name;
std::string dir_name_left;
std::string dir_name_right;

int data_no = 0;

FILE * ptr_time = nullptr;

rosbag::Bag write_bag;



void dataLoggingFlagCallback(const std_msgs::Bool::ConstPtr &msg){
    if(msg->data) {
        if(data_logging) {

        } else {
            data_logging = true;
            ROS_INFO("Data Logging Set True");
            data_no = 0;
        }
    } else {
        if(data_logging){
            data_logging = false;
            ROS_INFO("Data Logging Set False");
        }
    }
}

void dataPrefixCallBack(const std_msgs::String::ConstPtr & msg) {
    if(msg->data.compare(stop_logging_msg)!=0) {
        if(data_prefix.compare(stop_logging_msg) == 0) {
            std::cout<<"Prefix changed to logging"<<std::endl;
            data_prefix = msg->data;
            dir_name = PREFIX_PATH + data_prefix + ".bag";
            write_bag.open(dir_name, rosbag::bagmode::Write);
        }
    } else {
        if(data_prefix.compare(stop_logging_msg)!=0) {
            std::cout<<"Prefix changed to stop logging"<<std::endl;
            data_prefix = msg->data;
            write_bag.close();
        }
    }
}


void lidar_port_subscribe_callback(const sensor_msgs::PointCloud2Ptr & ptcld) {
    if(data_logging) {
        write_bag.write("/lidar_port/os_cloud_node/points", ptcld->header.stamp, *ptcld);
        // std::cout<<"I heard lidar_port"<<std::endl;
    }
}
void imu_port_subscribe_callback(const sensor_msgs::ImuPtr & imu) {
    if(data_logging) {
        write_bag.write("/lidar_port/os_cloud_node/imu", imu->header.stamp, *imu);
        // std::cout<<"I heard lidar_port imu"<<std::endl;
    }
}

void lidar_starboard_subscribe_callback(const sensor_msgs::PointCloud2Ptr & ptcld) {
    if(data_logging){
        write_bag.write("/lidar_starboard/os_cloud_node/points", ptcld->header.stamp, *ptcld);
        // std::cout<<"I heard lidar_starboard"<<std::endl;
    }
}
void imu_starboard_subscribe_callback(const sensor_msgs::ImuPtr & imu) {
    if(data_logging){
        write_bag.write("/lidar_starboard/os_cloud_node/imu", imu->header.stamp, *imu);
        // std::cout<<"I heard lidar_starboard imu"<<std::endl;
    }
}

int main(int argc, char ** argv){
    // Will be implemented later
    // if(argc<2) {
    //     std::cout<<"rosrun rosbag_record record_by_message <Topics>"<<std::endl;
    //     return 0;
    // }

    // if(argc>1) {
    //     std::cout<<"Topics to record"<<std::endl;
    //     for(int i = 1; i <argc ; ++i) {
    //         std::cout<<argv[i]<<std::endl;
    //     }
    // }


    ros::init(argc, argv, "record_by_message");
    ros::NodeHandle nh;
    ros::Subscriber sub_bool = nh.subscribe("/datalogging", 1, dataLoggingFlagCallback);
    ros::Subscriber sub_prefix = nh.subscribe("/save_prefix", 1, dataPrefixCallBack);

    ros::Subscriber lidar_port_sub = nh.subscribe("/lidar_port/os_cloud_node/points", 1, lidar_port_subscribe_callback);
    ROS_INFO("SUBSCRIBING /lidar_port/os_cloud_node/points");
    ros::Subscriber lidar_starboard_sub = nh.subscribe("/lidar_starboard/os_cloud_node/points", 1, lidar_starboard_subscribe_callback);
    ROS_INFO("SUBSCRIBING /lidar_starboard/os_cloud_node/points");

    ros::Subscriber imu_port_sub = nh.subscribe("/lidar_port/os_cloud_node/imu", 1, imu_port_subscribe_callback);
    ROS_INFO("SUBSCRIBING /lidar_port/os_cloud_node/imu");
    ros::Subscriber imu_starboard_sub = nh.subscribe("/lidar_starboard/os_cloud_node/imu", 1, imu_starboard_subscribe_callback);
    ROS_INFO("SUBSCRIBING /lidar_starboard/os_cloud_node/imu");


    ros::spin();
    return 0;
}