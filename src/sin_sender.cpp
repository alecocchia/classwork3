//include ros header file
#include "ros/ros.h" 
//header file of the custom message
//  This message belongs to the package in which it is defined
#include "classwork3/sinusoid.h"


int main(int argc, char **argv) {

	//Initialize the ROS node with name: sinus_sender
	ros::init(argc, argv,"sinus_sender");
	
	//Declare the node handle: our interface with the ROS system
	ros::NodeHandle sin_handle;

	//Create a publisher object:
	//	Input:  - type of message: sinusoid
	//			- topic name: /sinusoid
	//			- message queue: 1 (0: infinite queue)
	ros::Publisher sinus_pub = sin_handle.advertise<classwork3::sinusoid>("/sinusoid", 5);


    //Define the custom datatype
    classwork3::sinusoid data;
    //Fill the fields
    data.amplitude = 100;
    data.period=0.2;
    float t;
    ros::Rate rate((10)*(1/data.period)); 

	// Typical loop: neverending loop: a controller works until actuators are activated
	//		while (ros::ok()): works until the ROS node is not terminated (by the user with ctrl+c or similar)
    t=0;
	while ( ros::ok() ) {
        //Fill the data value
		data.value = data.amplitude*sin((2*3.14/data.period)*t);

		//ROS_INFO: Like a printf, but with the timestamp
		ROS_INFO("%f",data.value); 

		//Publish the message over the ROS network
		sinus_pub.publish(data);
		
		//Rate to maintain the 10 Hz
        t+=1.0/50.0;
		rate.sleep();
	}
	
	return 0;
}