import rclpy
import time
from rclpy.node import Node
from std_msgs.msg import String
from village_interfaces.srv import BorrowMoney


# 继承Node
class BaiPiaoNode(Node):
    def __init__(self, name) -> None:
        # 初始化生成一个节点，给节点命名name
        super().__init__(name) #这里必须执行父类的初始化方法
        self.get_logger().info("创建节点成功 %s" % name)
        # 订阅小说
        self.create_subscription(String, "sexy_girl", self.subscript_callback, 10)
        self.borrow_money_client_ = self.create_client(BorrowMoney, "borrow_money")
        # 累计借的钱
        self.total_money = 0

    # 订阅topic的回调
    def subscript_callback(self, str: String):
        self.get_logger().info("我是 %s, 收到消息 %s" % (self.get_name(), str.data)) # String是rcl封装的类型，不是python的str，需要使用.data转化成python的str类型

    # 借钱回调
    def borrow_response_callback(self, response):
        success = response.result().success
        money = response.result().money
        if success:
            self.get_logger().info("借钱%d元成功！" % money)
            self.total_money += money
            self.get_logger().info("累计借钱%d元！" % self.total_money)
        else:
            self.get_logger().info("借钱失败！")

    # 借钱吃饭
    def borrow_money_eat(self):
        self.get_logger().info("我是%s,借钱吃饭了" % self.get_name())
        # 等待服务上线1秒钟，返回服务是否已经在线的bool结果
        while not self.borrow_money_client_.wait_for_service(1.0):
            self.get_logger().info("服务端还未在线")

        request = BorrowMoney.Request()
        request.name = self.get_name()
        request.money = 20
        
        # 这里创建了异步请求，回调是不会有返回值执行的，因为主程序已经完成了，不会等待回调，而回调则会等待服务端根据request返回response
        # 这里需要及时退出函数
        self.borrow_money_client_.call_async(request).add_done_callback(self.borrow_response_callback)
        # time.sleep(5)


def main():
    rclpy.init() #rcl：ros客户端库
    node = BaiPiaoNode('li3')
    node.borrow_money_eat() #借钱吃饭
    rclpy.spin(node) # 保持节点运行，检测是否收到退出指令（Ctrl+C）
    rclpy.shutdown() #rcl关闭