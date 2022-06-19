#include <ros/ros.h>
#include <sensor_msgs/Joy.h>
#include <geometry_msgs/Twist.h>

/**
 * 動かし方
 * roslaunch ypspur_ros_bridge yamabiko.launch
 * roslaunch yamasemi_sim gmapping_yamasemi_2022.launch
 * rosrun joy joy_node
 * pspボタンを押す
 * rosrun joy_sample joy_sub_node
 *
 */

class JoyController
{
private:
    ros::Subscriber sub_joy_;
    ros::Publisher pub_twist_;
    ros::Publisher pub_twist_sim;
    sensor_msgs::Joy joy_msg;

    void joy_callback(const sensor_msgs::Joy &msg)
    {

        // rostopic echo /joyから受け取るmsgの内容を出力
        // ROS_INFO("axes[0]:%f", msg.axes[0]);   //スティック0の傾き状態を表示-1~1
        // ROS_INFO("button[0]:%d", msg.buttons[0]); //ボタンの状態を表示0 or 1

        joy_msg = msg;
        // ROS_INFO("msg:%ld", msg.axes.size());
        // ROS_INFO("rec:%ld", joy_msg.buttons.size());
    }

public:
    JoyController() //コンストラクタ
    {
        ros::NodeHandle nh;
        pub_twist_ = nh.advertise<geometry_msgs::Twist>("cmd_vel", 5);
        pub_twist_sim = nh.advertise<geometry_msgs::Twist>("/beego/diff_drive_controller/cmd_vel", 10);
        sub_joy_ = nh.subscribe("joy", 10, &JoyController::joy_callback, this);
    }
    void mainloop()
    {

        ros::Rate loop_rate(10);
        joy_msg.buttons.resize(17);
        joy_msg.axes.resize(6);

        while (ros::ok())
        {
            ros::spinOnce();
            geometry_msgs::Twist pub_msg;

            if (joy_msg.buttons[6] == 1) //コントローラのL2ボタンを押しながらのときだけ実機を操作できる
            {
                pub_msg.linear.x = 0.1*joy_msg.axes[1];
                pub_msg.angular.z = 0.1*joy_msg.axes[3];
                pub_twist_.publish(pub_msg);
            }

            if (joy_msg.buttons[7] == 1) //コントローラのR2ボタンを押しながらのときに
            {
                pub_msg.linear.x = 0.1*joy_msg.axes[1];
                pub_msg.angular.z = 0.1*joy_msg.axes[3];
                pub_twist_sim.publish(pub_msg);
            }

            loop_rate.sleep();
        }
    }
};

int main(int argc, char **argv)
{
    ros::init(argc, argv, "joy_sub_node");
    JoyController joy_test;
    joy_test.mainloop();

    return 0;
}