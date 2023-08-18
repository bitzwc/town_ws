#include "rclcpp/rclcpp.hpp"

class SingleDogNode : public rclcpp::Node{
public:
    SingleDogNode(std::string name):Node(name){
        RCLCPP_INFO(this->get_logger(), "大家好，我是" + name);
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