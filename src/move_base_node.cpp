#include <ros/ros.h>
#include <sensor_msgs/Joy.h>
#include "move_base_msgs/MoveBaseActionGoal.h"
#include "actionlib_msgs/GoalStatusArray.h"
// tf listener
#include "tf2_ros/transform_listener.h"
#include "tf2_geometry_msgs/tf2_geometry_msgs.h"
// quaternion tranform
#include "geometry_msgs/Quaternion.h" //geometry_msgs::Quaternion
#include "tf/transform_listener.h"    //tf::Quaternion

/**
 * 動かし方
 * roslaunch yamasemi_sim amcl_yamasemi_2022.launch
 * rosrun
 */

// beego/mapにおけるbeego/base_linkを見ることで我々が見ている座標系を取得できる

class MoveGoal
{
private:
    ros::Publisher pub_goal;
    ros::Subscriber sub_status;

    // subscriber
    void status_callback(const actionlib_msgs::GoalStatusArray::ConstPtr &msg)
    {
        int status_id = 0;
        // actionlib_msgs/GaolStatus[] status_list
        // uint8 PENDING         = 0
        // uint8 ACTIVE          = 1
        // uint8 PREEMPTED       = 2
        // uint8 SUCCEEDED       = 3
        // uint8 ABORTED         = 4
        // uint8 REJECTED        = 5
        // uint8 PREEMPTING      = 6
        // uint8 RECALLING       = 7
        // uint8 RECALLED        = 8
        // uint8 LOST            = 9
        if (!msg->status_list.empty())
        {
            actionlib_msgs::GoalStatus goal_status = msg->status_list[0];
            status_id = goal_status.status;
        }

        // ROS_INFO("statusid = %d\n", status_id);
        // if (status_id == 3)
        // {
        //     ROS_INFO("STOP!!!!!!!!!!!!!!!!!!!!!!!!!!");
        //     return;
        // }
    }

public:
    MoveGoal() //コンストラクタ
    {
        ros::NodeHandle nh;
        // move_baseにpublishする
        pub_goal = nh.advertise<move_base_msgs::MoveBaseActionGoal>("move_base/goal", 1);
        sub_status = nh.subscribe("move_base/status", 10, &MoveGoal::status_callback, this);
    }

    //
    void mainloop()
    {

        ros::Rate loop_rate(10);

        move_base_msgs::MoveBaseActionGoal nav_goal;
        nav_goal.goal.target_pose.header.frame_id = "beego/map";
        nav_goal.goal.target_pose.pose.position.x = -1.0;
        nav_goal.goal.target_pose.pose.position.y = 1.0;
        nav_goal.goal.target_pose.pose.orientation.w = 1.0;

        tf2_ros::Buffer tfBuffer;
        tf2_ros::TransformListener tfListener(tfBuffer);
        //ros::Duration(1.0).sleep(); //tfのlookupTransformの計算で時間がかかるのでwhileの外でsleep

        int cnt = 0;
        while (ros::ok())
        {
            ros::spinOnce();


            cnt++;
            ROS_INFO("cnt: %d", cnt);
            geometry_msgs::TransformStamped tfstamp;
            try
            {
                tfstamp = tfBuffer.lookupTransform("beego/map", "beego/base_link", ros::Time(0));
            }
            catch (tf2::TransformException& ex)
            {
                ROS_INFO("hoge");
                ROS_WARN("%s",ex.what());
                //return;
            }
            
            // 到達判定　この範囲に入ったら座標を更新→これはアルゴリズムの話だね
            
            ROS_INFO("x: %f\n", tfstamp.transform.translation.x);
            pub_goal.publish(nav_goal);
            loop_rate.sleep();
        }
    }
};

int main(int argc, char **argv)
{
    ros::init(argc, argv, "move_goal");
    MoveGoal move_goal;
    move_goal.mainloop();

    return 0;
}