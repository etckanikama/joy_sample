#include <ros/ros.h>
#include <sensor_msgs/Joy.h>
#include <geometry_msgs/Twist.h>

class JoyController
{
private:
    ros::Subscriber sub_joy_;
    ros::Publisher pub_twist_;
    sensor_msgs::Joy joy_msg;

    void joy_callback(const sensor_msgs::Joy &msg)
    {

        // rostopic echo /joyから受け取るmsgの内容を出力
        // ROS_INFO("axes[0]:%f", msg.axes[0]);      //スティック0の傾き状態を表示-1~1
        // ROS_INFO("button[0]:%d", msg.buttons[0]); //ボタンの状態を表示0 or 1

        joy_msg = msg;
        //ROS_INFO("msg:%ld", msg.buttons.size());
        //ROS_INFO("rec:%ld", joy_msg.buttons.size());
    }

public:
    JoyController() //コンストラクタ
    {
        ros::NodeHandle nh;
        pub_twist_ = nh.advertise<geometry_msgs::Twist>("cmd_vel", 5);
        sub_joy_ = nh.subscribe("joy", 10, &JoyController::joy_callback, this);
    }
    void mainloop()
    {
        ros::Rate loop_rate(10);
        joy_msg.buttons.resize(17);
        //ROS_INFO("button[0]:%d", joy_msg.buttons[0]);
        //printf("hoge:%d", joy_msg.buttons[0]);
        while (ros::ok())
        {
             ros::spinOnce();
             //if(joy_msg.buttons[0] == 0)
             ROS_INFO("button[0]:%d", joy_msg.buttons[0]);
        //     geometry_msgs::Twist pub_msg;
        //     ROS_INFO("button[0]:%d", joy_msg.buttons[0]);
        //     // if (joy_msg.buttons[0] == 1)
        //     // {
        //     //     pub_msg.linear.x = 0.2;
        //     //     pub_msg.angular.z = 0.0;
        //     // }
        //     // else
        //     // {
        //     //     pub_msg.linear.x = 0.0;
        //     // }

        //     pub_twist_.publish(pub_msg);
        //     loop_rate.sleep();
        }
    }
};

int main(int argc, char **argv)
{
    ros::init(argc, argv, "joy_sub_node");
    JoyController joy_test;
    joy_test.mainloop();

    aaaaaa

    return 0;
}