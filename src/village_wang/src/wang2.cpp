#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/u_int32.hpp"
#include "std_msgs/msg/string.hpp"
#include "village_interfaces/srv/sell_novel.hpp"
#include <queue>


//占位符，占位回调函数的参数个数
using std::placeholders::_1; //?
using std::placeholders::_2;


static const rclcpp::Logger LOGGER = rclcpp::get_logger("wang2");

class SingleDogNode : public rclcpp::Node{
public:
    // 构造函数,有一个参数为节点名称，实现Node初始化，将name传入
    SingleDogNode(std::string name):Node(name){
<<<<<<< HEAD
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
=======
        RCLCPP_INFO(this->get_logger(), "大家好，我是" + name);
>>>>>>> 525b18b9634066c324fb6a9c651524fa9e13056d

        //创建一个订阅者来订阅李四写的小说，通过名字sexy_girl
        //std::bind和std::function结合实现回调技术，因为topic_callback是成员函数，需要有对象来调用，这里直接用
        //_1是占位符，表示topic_callback函数的参数只有一个
        sub_novel = this->create_subscription<std_msgs::msg::String>("sexy_girl", 10, std::bind(&SingleDogNode::topic_callback, this, _1));

        //创建发布者
        pub_money = this->create_publisher<std_msgs::msg::UInt32>("sexy_girl_money", 10);

        //创建回调函数组对象
        callback_group_service_ = this->create_callback_group(rclcpp::CallbackGroupType::MutuallyExclusive);

        //创建服务端，用于提供买书服务，服务的输入和输出用SellNovel结构，服务调用sell_book_callback
        server_ = this->create_service<village_interfaces::srv::SellNovel>(
            "sell_novel",                             
            std::bind(&SingleDogNode::sell_book_callback,this,_1,_2), //两个输入参数，占位
            rmw_qos_profile_services_default, //通信质量，这里使用服务默认的通信质量
            callback_group_service_ //回调组，我们前面创建回调组就是在这里使用的，告诉ROS2，当你要调用回调函数处理请求时，请把它放到单独线程的回调组中
        );

        //声明一下书的单价
        unsigned int novel_price = 1;

        //设置参数
        this->declare_parameter<std::uint32_t>("novel_price", novel_price); //这里为什么非要加上泛型？
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

        //攒书，写入队列
        novels_queue.push(msg->data);
    };

    //声明一个回调函数，当收到买书请求时调用该函数，用于处理数据
    void sell_book_callback(const village_interfaces::srv::SellNovel::Request::SharedPtr request,
        const village_interfaces::srv::SellNovel::Response::SharedPtr response)
    {
        //对请求数据进行处理
        //收到买书请求时，给了钱 request.money，需要给多少章书，看看书库里是否够novels_queue，不够就攒，在topic_callback中写入队列
        RCLCPP_INFO(this->get_logger(), "收到钱%d", request->money);

        // 设置rate周期为1s，代表1s检查一次
        rclcpp::Rate loop_rate(1);

        //买书的章节数
        //更新参数
        std::uint32_t novel_price = this->get_parameter("novel_price").as_int();
        unsigned int requestNovelNum = int(request->money / novel_price);
        while(novels_queue.size() < requestNovelNum){
            //判断系统是否还在运行
            if(!rclcpp::ok())
            {
                RCLCPP_ERROR(this->get_logger(), "程序被终止了");
                return ;
            }

            //打印一下当前的章节数量和缺少的数量
            RCLCPP_INFO(this->get_logger(), "书库的书有%d章，不够卖，继续攒书", novels_queue.size());

            //rate.sleep()让整个循环1s运行一次
            loop_rate.sleep();
        }
        //队列满足买书需求，跳出循环
        for(unsigned int i = 0; i < requestNovelNum; i++){
            response->novels.push_back(novels_queue.front());//
            novels_queue.pop();//
        }
    }

};

int main(int argc, char **argv){
    rclcpp::init(argc, argv);
    auto node = std::make_shared<SingleDogNode>("wang2");
    // RCLCPP_INFO(node->get_logger(), "大家好，我是王二");
    rclcpp::executors::MultiThreadedExecutor executor; //多线程执行器
    executor.add_node(node);
    executor.spin();
    rclcpp::shutdown();
    return 0;
}