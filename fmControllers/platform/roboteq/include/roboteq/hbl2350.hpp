/****************************************************************************
 # FroboMind
 # Copyright (c) 2011-2013, author Leon Bonde Larsen <leon@bondelarsen.dk>
 # All rights reserved.
 #
 # Redistribution and use in source and binary forms, with or without
 # modification, are permitted provided that the following conditions are met:
 #	* Redistributions of source code must retain the above copyright
 #  	notice, this list of conditions and the following disclaimer.
 #	* Redistributions in binary form must reproduce the above copyright
 #  	notice, this list of conditions and the following disclaimer in the
 #  	documentation and/or other materials provided with the distribution.
 #	* Neither the name FroboMind nor the
 #  	names of its contributors may be used to endorse or promote products
 #  	derived from this software without specific prior written permission.
 #
 # THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 # ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 # WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 # DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
 # DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 # (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 # LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 # ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 # (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 # SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 ****************************************************************************
 # 2013-06-02 Leon: Implemented regulator
 #
 #
 ****************************************************************************/
#ifndef HBL2350_HPP_
#define HBL2350_HPP_

#define TIME_BETWEEN_COMMANDS 0.2
#define LEFT 1
#define RIGHT 2

#include <ros/ros.h>
#include <geometry_msgs/TwistStamped.h>
#include <std_msgs/Bool.h>
#include <msgs/StringStamped.h>
#include <msgs/IntStamped.h>
#include <msgs/serial.h>
#include "roboteq/roboteq.hpp"
#include "roboteq/channel.hpp"
#include <cstdarg>

class hbl2350 : public RoboTeQ
{
/*
 * Class inheriting from the RoboTeQ base class and implementing the two channel
 * RoboTeQ HBL2350 motor controller model.
 * */
private:
	Channel ch1,ch2;
	ros::NodeHandle local_node_handler,global_node_handler;

public:
	bool closed_loop_operation;
	double mps_to_rpm;

	ros::Subscriber serial_sub, command_relay_sub, deadman_sub;

	ros::Duration max_time_diff;

	hbl2350();
	void spin(void);
	void initController(std::string);
	void updateStatus(void);

	// Methods overriding virtual methods in base class
	void hall_feedback(ros::Time time, int fb1, int fb2){ch1.onHallFeedback(time,fb1);ch2.onHallFeedback(time,fb2);}
	void power_feedback(ros::Time time, int fb1, int fb2){ch1.onPowerFeedback(time,fb1);ch2.onPowerFeedback(time,fb2);}

	// Callback methods
	void onDeadman(const std_msgs::Bool::ConstPtr& msg){ch1.onDeadman(msg); ch2.onDeadman(msg);}
	void onTimer(const ros::TimerEvent& event){updateStatus(); ch1.onTimer(event,status);ch2.onTimer(event,status);}
	void onSerial(const msgs::serial::ConstPtr& msg){serialCallback(msg);}

	// Mutator methods for setting up publishers
	void setSerialPub(ros::Publisher pub){serial_publisher = pub;}
	void setStatusPub(ros::Publisher pub){status_publisher = pub;}
	void setTemperaturePub(ros::Publisher pub){temperature_publisher = pub;}
	void setPowerCh1Pub(ros::Publisher pub){ch1.publisher.power = pub;}
	void setPowerCh2Pub(ros::Publisher pub){ch2.publisher.power = pub;}
	void setEncoderCh1Pub(ros::Publisher pub){ch1.publisher.hall = pub;}
	void setEncoderCh2Pub(ros::Publisher pub){ch2.publisher.hall = pub;}

	// Accessor method to block on during startup
	int	subscribers(){return serial_publisher.getNumSubscribers();}
};

#endif /* HBL2350_HPP_ */
