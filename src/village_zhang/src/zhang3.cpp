/*
    创建张三的节点
    创建一个客户端，向借钱服务端发出请求，获取到小说后阅读
*/

#include "rclcpp/rclcpp.hpp"
#include "village_interfaces/srv/sell_novel.hpp"

// 提前声明的占位符，留着创建客户端的时候用
using std::placeholders::_1;


//创建一个类节点，名字叫做PoorManNode,继承自Node.
class PoorManNode : public rclcpp::Node{
public:
    //构造函数，实现Node初始化方法
    PoorManNode(std::string name):Node(name){
        RCLCPP_INFO(this->get_logger(), "大家好，我是得了穷病的张三");

        //实例化客户端，创建SellNovel接口类型的客户端，请求sell_novel的服务
        client_ = this->create_client<village_interfaces::srv::SellNovel>("sell_novel");
    };

    /*买小说函数*/
    void buy_novel()
    {
        RCLCPP_INFO(this->get_logger(), "买小说去喽");
        //1、等待服务端上线，等待1s，返回服务是否在线
        while(!client_->wait_for_service(std::chrono::seconds(1))){

            //等待时检测rclcpp的状态
            if(!rclcpp::ok()){
                RCLCPP_ERROR(this->get_logger(), "等待服务的过程中被打断...");
                return;
            }
            RCLCPP_INFO(this->get_logger(), "等待服务端上线中");

        }
        //2、构造请求数据
        auto request = std::make_shared<village_interfaces::srv::SellNovel_Request>();
        
        //先来五块钱的看看好不好看
        request->money = 5; 

        //3.发送异步请求，然后等待返回，返回时调用回调函数，_1是占位符，回调函数的输入参数个数
        client_->async_send_request(request, std::bind(&PoorManNode::novels_callback, this, _1));
        
    }
    
    /*接收小说-回调函数*/
    void novels_callback(rclcpp::Client<village_interfaces::srv::SellNovel>::SharedFuture response)
    {
        //这里为什么还要get？response不是SellNovel定义的输出结构吗？
        auto result = response.get();
        RCLCPP_INFO(this->get_logger(), "收到%d章的小说，现在开始按章节开读", result->novels.size());

        //遍历结果小说，这里直接用:遍历，小说的类型是std::string
        for(std::string novel : result->novels){

            //打印小说
            RCLCPP_INFO(this->get_logger(), "%s", novel.c_str());//c_str是转化成C语言的str格式
        }
        RCLCPP_INFO(this->get_logger(), "小说读完了，好刺激，写的真不错，好期待下面的章节呀！");
    }

private:
    // 创建一个客户端
    rclcpp::Client<village_interfaces::srv::SellNovel>::SharedPtr client_;




};





/*主函数*/
int main(int argc, char **argv){
    //初始化
    rclcpp::init(argc, argv);

    /*产生一个Zhang3的节点*/
    auto node = std::make_shared<PoorManNode>("zhang3");

    node->buy_novel();
    
    /* 运行节点，并检测rclcpp状态*/
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}