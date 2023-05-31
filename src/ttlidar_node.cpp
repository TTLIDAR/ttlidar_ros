
#include "ros/ros.h"
#include "sensor_msgs/LaserScan.h"
#include "ttlidar_driver.h"


#define DEG2RAD(x) ((x)*M_PI/180.)

using namespace std;
using namespace ttlidar;

bool start_scan(TTlidarDriver * drv)
{
	result_t     result;

	result = drv->startScan(SCAN_MODE,5000);
	if (IS_OK(result)) { 
		printf("TTldiar start scan ok!\n");
	}
	else {
		printf("Error, cannot make the lidar start scan: %x\n", result);
		return false;
	}
	return true;
}

bool stop_scan(TTlidarDriver * drv)
{
	result_t     result;

	result = drv->stopScan();
	if (IS_OK(result)) { 
		printf("TTldiar stop scan ok!\n");
	}
	else {
		printf("Error, cannot make the lidar stop scan: %x\n", result);
		return false;
	}
	return true;
}

void publish_scan(ros::Publisher *pub,
                  LIDAR_SCAN_INFO_T *scan_points,size_t points_count,
                  ros::Time start,double scan_time,                 
                  float angle_min, float angle_max,
                  std::string frame_id)
{
    sensor_msgs::LaserScan scan_msg;

    scan_msg.header.stamp = start;
    scan_msg.header.frame_id = frame_id;
    scan_msg.angle_min = angle_min;
    scan_msg.angle_max = angle_max;
    scan_msg.angle_increment = (scan_msg.angle_max - scan_msg.angle_min) /(double)(points_count - 1);

    scan_msg.scan_time = scan_time;
    scan_msg.time_increment = scan_time / (double)(points_count-1);
    scan_msg.range_min = 0.15;
    scan_msg.range_max = 16.0;

    scan_msg.ranges.resize(points_count);
    scan_msg.intensities.resize(points_count);

    //copy ttlidar_points to scan_msg
    for (size_t i = 0; i < points_count; i++)
    {
        float read_value = (float) scan_points[i].distance;
        if (read_value == 0.0)
            scan_msg.ranges[points_count- 1- i] = std::numeric_limits<float>::infinity();
        else
            scan_msg.ranges[points_count -1- i] = read_value;

	//	scan_msg.intensities[points_count -1- i] = 0;

	}

    pub->publish(scan_msg);
}

int main(int argc, char * argv[])
{
    // read ros param
    ros::init(argc, argv, "ttlidar_node");

	int    serial_baudrate = 115200;
	string serial_port;
    string frame_id;

    
    ros::NodeHandle nh;
    ros::NodeHandle nh_private("~");
    nh_private.param<std::string>("serial_port", serial_port, "/dev/ttyUSB0"); 
    nh_private.param<std::string>("frame_id", frame_id, "laser");
    ros::Publisher scan_pub = nh.advertise<sensor_msgs::LaserScan>("scan", 1000);

    // create the driver instance
    TTlidarDriver *drv =   new TTlidarDriver();

    if (IS_FAIL(drv->connect(serial_port.c_str(), serial_baudrate))) {
        ROS_ERROR("Error, Open serail port %s failed! \n",serial_port.c_str());        
      
        delete drv;
        return -1;
    }

    printf("TTLidar connected\n");
    
    //two service for start/stop lidar scan
    // ros::ServiceServer stop_scan_service = nh.advertiseService("stop_scan", stop_scan);
    // ros::ServiceServer start_scan_service = nh.advertiseService("start_scan", start_scan);

    //set lidar rotate speed
    //drv->setMotorRpm(6,1000);

    //start scan
    if (!drv->startScan(SCAN_MODE,1000))
	{
		//delete drv;
        //return -1;
	}
	
    printf("TTLidar strat scan\n");


    float angle_min; 
    float angle_max;
    ros::Time start_scan_time;
    ros::Time end_scan_time;
    double scan_duration;
    result_t  result;
    ros::Rate r(30);
    start_scan_time = ros::Time::now();
    
    while (ros::ok())
	{
        LIDAR_SCAN_INFO_T scan_points[360 *6];
        size_t points_count = 360*6;

		result = drv->grabScanData(scan_points, points_count);

		if (IS_OK(result)) 
		{
			printf("TTlidar points_count = %d\n", points_count);

            angle_min = DEG2RAD(0.0f);
            angle_max = DEG2RAD(359.0f);


            end_scan_time = ros::Time::now();
			scan_duration = (end_scan_time - start_scan_time).toSec() ;

            //if scan successful, publish lidar scan
            publish_scan(&scan_pub, 
                         &scan_points[0], points_count,
                         start_scan_time, scan_duration,
                         angle_min, angle_max,
                         frame_id);

            start_scan_time = end_scan_time;    
		}
		else
		{
			printf("TTlidar grabScanData error: %x\n", result);
		}
        usleep(50);
        //r.sleep();
        ros::spinOnce();
	}

    printf("TTLidar stop scan\n");

    drv->disconnect(); 
    delete drv;
    return 0;
}



