How to get  ttlidar ros package
   git clone https://github.com/TTLIDAR/ttlidar_ros.git

How to build ttlidar ros package
********************************************************************************************************
    1) Clone this project to your catkin's workspace src folder
    2) Please select correct serial in “src\ttlidar_node.cpp”：/dev/ttyUSB0(default)
    3) cd catkin_ws
    4) catkin_make (to build ttlidar_node and subscribe_demo_node)
    5) source devel/setup.bash


How to run ttlidar ros package
********************************************************************************************************
1. Only run ttlidar ros package
roslaunch ttlidar_node ttlidar.launch (for TTLIDAR A1/T1)

------------------------------------------------------------
2. There're two ways to run ttlidar ros package, and subscribe scan_msg

2.1 Run ttlidar node and view in the rviz
roslaunch ttlidar_node view_ttlidar.launch (for TTLIDAR A1/T1)

2.2 Run ttlidar_node and simple subscribe_demo_node 
1)roslaunch ttlidar_node ttlidar.launch (for TTLIDAR A1/T1)
2)rosrun ttlidar_ros subcscribe_demo_node

You should see ttlidar's scan info in the console


