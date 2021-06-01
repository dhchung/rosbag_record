#include <ros/ros.h>
#include <sensor_msgs/Image.h>
#include <std_msgs/Bool.h>
#include <std_msgs/String.h>
#include <string>
#include <sys/stat.h>
#include <rosbag/bag.h>
#include <geometry_msgs/Point.h>
#include <sensor_msgs/PointCloud2.h>





#define PREFIX_PATH "/media/dongha/HDD/"
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

int main(int argc, char ** argv){
    if(argc>1) {
        std::cout<<"Topics to record"<<std::endl;
        for(int i = 1; i <argc ; ++i) {
            std::cout<<argv[i]<<std::endl;
        }
    }





    return 0;
}