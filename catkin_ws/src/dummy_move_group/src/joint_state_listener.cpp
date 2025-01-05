#include <ros/ros.h>
#include <sensor_msgs/JointState.h>
#include <vector>

#define PI 3.1415926

std::vector<double> last_joint_position(6, 0.0);

// 回调函数，用于处理接收到的消息
void jointStateCallback(const sensor_msgs::JointState::ConstPtr& msg)
{
    // 确保 position 数组不为空，且接收的数据于上次接收的数据不同
    if (msg->position.size() > 0 
        && (msg->position[0] != last_joint_position[0] || msg->position[1] != last_joint_position[1]
        || msg->position[2] != last_joint_position[2] || msg->position[3] != last_joint_position[3]
        || msg->position[4] != last_joint_position[4] || msg->position[5] != last_joint_position[5])) 
    {
        // 打印关节数量
        ROS_INFO("Received joint states: %zu", msg->position.size());
        // 打印各关节的角度数值
        for (size_t i = 0; i < msg->position.size(); ++i) 
        {
            // 由于创建URDF时，第三轴的坐标系发生偏置，因此需要将第三关节加90度
            if(i == 2)
            {
                ROS_INFO("Joint %zu: %f", i, msg->position[i]/PI*180 + 90);
            }else
            {
                ROS_INFO("Joint %zu: %f", i, msg->position[i]/PI*180);
            }
        }
    }

    // 更新 last_joint_position 为当前的关节数据
    last_joint_position = msg->position;
}

int main(int argc, char* argv[])
{
    // 初始化 ROS 节点
    ros::init(argc, argv, "joint_state_listener");
    ros::NodeHandle nh;

    // 订阅 /joint_states 话题
    ros::Subscriber sub = nh.subscribe("/joint_states", 10, jointStateCallback);

    // 保持节点运行，直到按下 Ctrl+C
    ros::spin();

    return 0;
}
