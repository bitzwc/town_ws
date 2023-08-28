#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/u_int32.hpp"
#include "std_msgs/msg/string.hpp"
#include "village_interfaces/srv/sell_novel.hpp"
#include <queue>


//占位符，占位回调函数的参数个数
using std::placeholders::_1; //?
using std::placeholders::_2;


class SingleDogNode : public rclcpp::Node{
public:
    // 构造函数,有一个参数为节点名称，实现Node初始化，将name传入
    SingleDogNode(std::string name):Node(name){
        RCLCPP_INFO(this->get_logger(), "大家好，我是" + name);

        //创建一个订阅者来订阅李四写的小说，通过名字sexy_girl
        //std::bind和std::function结合实现回调技术，因为topic_callback是成员函数，需要有对象来调用，这里直接用
        //_1是占位符，表示topic_callback函数的参数只有一个
        sub_novel = this->create_subscription<std_msgs::msg::String>("sexy_girl", 10, std::bind(&SingleDogNode::topic_callback, this, _1));

        //创建发布者
        pub_money = this->create_publisher<std_msgs::msg::UInt32>("sexy_girl_money", 10);

        //创建回调函数组对象
        callback_group_service_ = this->create_callback_group(rclcpp::CallbackGroupType::MutuallyExclusive);

        //创建服务端，用于提供买书服务
        server_ = this->create_service<village_interface::srv::SellNovel>(
            "sell_novel",                             
            std::bind(&SingleDogNode::sell_book_callback,this,_1,_2),
            
        );
    };

private:
    //声明一个订阅者（成员变量）,用于订阅小说
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr sub_novel;

    //声明一个发布者（成员变量）,用于给李四钱
    rclcpp::Publisher<std_msgs::msg::UInt32>::SharedPtr pub_money;

    //声明一个小说章节队列
    std::queue<std::string>  novels_queue;

    //声明一个服务回调组，解决卖书回调和接收小说回调互相等待出现死锁
    rclcpp::CallbackGroup::SharedPtr callback_group_service_;

    //声明一个服务端
    rclcpp::Service<village_interfaces::srv::SellNovel>::SharedPtr server_;

    //声明话题数据的回调函数，看到小说后，才给李四打钱，打钱由小说topic的消息来触发
    void topic_callback(const std_msgs::msg::String::SharedPtr msg){
        //新建一张人民币
        std_msgs::msg::UInt32 money;
        money.data = 10;

        //发送人民币给李四
        pub_money->publish(money);
        RCLCPP_INFO(this->get_logger(), "朕已阅：'%s'，打赏李四：%d 元的稿费", msg->data.c_str(), money.data);
    };

    //声明一个回调函数，当收到买书请求时调用该函数，用于处理数据
    void sell_book_callback(const village_interfaces::srv::SellNovel::Request::SharedPtr request,
        const village_interfaces::srv::SellNovel::Response::SharedPtr response)
    {
        //对请求数据进行处理
    }

};

int main(int argc, char **argv){
    rclcpp::init(argc, argv);
    auto node = std::make_shared<SingleDogNode>("wang2");
    // RCLCPP_INFO(node->get_logger(), "大家好，我是王二");
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}