#include "rclcpp/rclcpp.hpp"

static const rclcpp::Logger LOGGER = rclcpp::get_logger("wang2");

class SingleDogNode : public rclcpp::Node{
public:
    SingleDogNode(std::string name):Node(name){
        std::string log_level = "2";
        RCLCPP_INFO(LOGGER, "log_level = " + log_level);
        // this->declare_parameter("rcl_log_level", "0");     /*声明参数，默认值0*/
        this->get_parameter("rcl_log_level", log_level); /*获取参数，存入log_level*/
        RCLCPP_INFO(LOGGER, "log_level = " + log_level);
        
        // rclcpp::Logger->level = log_level;
        RCLCPP_INFO(LOGGER, "大家好，我是" + name);
        // RCLCPP_INFO(this->get_logger(), "大家好，我是" + name);
        // RCLCPP_DEBUG(this->get_logger(), "我是DEBUG级别的日志，我被打印出来了!");
        // RCLCPP_INFO(this->get_logger(), "我是INFO级别的日志，我被打印出来了!");
        // RCLCPP_WARN(this->get_logger(), "我是WARN级别的日志，我被打印出来了!");
        // RCLCPP_ERROR(this->get_logger(), "我是ERROR级别的日志，我被打印出来了!");
        // RCLCPP_FATAL(this->get_logger(), "我是FATAL级别的日志，我被打印出来了!");
    };
    //todo:订阅“小说”

    //todo:发布“小说付费”

private:

};

int main(int argc, char **argv){
    rclcpp::init(argc, argv);
    auto node = std::make_shared<SingleDogNode>("wang2");
    // RCLCPP_INFO(node->get_logger(), "大家好，我是王二");
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}