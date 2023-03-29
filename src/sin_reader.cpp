#include "ros/ros.h"
#include "std_msgs/Float32.h"
#include "classwork3/sinusoid.h"

using namespace std;

//Use a class to store the topic data 
//	Just a choice: the best way to share data between a main loop and the ROS callbacks
class ROS_SUB {
	public:
		ROS_SUB();
		void topic_cb(classwork3::sinusoid);
	
	private:
		ros::NodeHandle _filter_nh;
		//Subscriber object
		ros::Subscriber _topic_sub;
        ros::Publisher _filter_pub;

		int _count=0;
		vector<float> _values;
		vector<float> _filter_values; 
		std_msgs::Float32 _y;
};

ROS_SUB::ROS_SUB() {
	//Initialize a subscriber:
	//	Input: 	topic name: /sinusoid
	//			queue:	1
	//			Callback function
	//			Object context: the value of data members
	_filter_pub= _filter_nh.advertise<std_msgs::Float32>("/filter", 5);
	_topic_sub = _filter_nh.subscribe("/sinusoid", 1, &ROS_SUB::topic_cb, this);


}

//Callback function: the input of the function is the data to read
void ROS_SUB::topic_cb(classwork3::sinusoid sen) {

	//if first iteration, y(0)=0, u(0) = sinsusoid value in t=0
    if(_count == 0){
		_filter_values.push_back(sen.value);
		_y.data=0;
	} else {	//else y(k)=0.9512*y(k-1) + 0.4877*u(k-1) (choise of F(s)=1/(1+s*Tau), with tau = 0.2 (->pole in -5 = w_taglio), discretization with Ts = 0.01)
		_y.data=(0.9512*_y.data+0.4877*_values.back());
	}
	_filter_pub.publish(_y);
	ROS_INFO("%f",_y.data); 
	_values.push_back(sen.value);
	_count++;
	

}

int main( int argc, char** argv ) {

	//Init the ros node with ros_subscriber name
	ros::init(argc, argv, "ros_subscriber");

	//Create the ROS_SUB class object
	ROS_SUB rs;
	
	//ros::spin() blocks the main thread from exiting until ROS invokes a shutdown - via a Ctrl + C for example
	// It is written as the last line of code of the main thread of the program.
	//Also the spin invokes the callbacks to flush their queue and process incoming data
	ros::spin(); 

	//----This function will be never overcome

	return 0;
}


