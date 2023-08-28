import rclpy

from rclpy.node import Node
from std_msgs.msg import String
from std_msgs.msg import UInt32
from village_interfaces.srv import BorrowMoney

class WriterNode(Node):
    def __init__(self, name):
        super().__init__(name)
        self.get_logger().info("大家好，我是%s" % name)
        self.pub_novel = self.create_publisher(String, "sexy_girl", 10)
        time_period = 5
        self.i = 0  # i 是个计数器，用来算章节编号的
        #计时器，按这个时间来发布
        self.create_timer(timer_period_sec=time_period, callback=self.time_callback)
        self.account = 100
        self.create_subscription(UInt32, 'sexy_girl_money', self.recv_money_callback, 10)

        #创建服务，在节点初始化时完成
        #参数：服务接口类型
             #服务名称：全局唯一
             #回调函数：服务端处理逻辑
        self.borrow_server = self.create_service(BorrowMoney, "borrow_money", self.borrow_money_callback)

    # 发布“小说”
    def time_callback(self):
        msg = String()
        msg.data = '第%d回: 潋滟湖 %d 次偶遇胡艳娘' % (self.i, self.i)
        self.pub_novel.publish(msg)
        self.get_logger().info('李四:我发布了艳娘传奇："%s"' % msg.data)  # 打印一下发布的数据，供我们看
        self.i += 1

    # 订阅“小说付费”
    def recv_money_callback(self, money:UInt32):
        self.account += money.data
        self.get_logger().info('已经收到稿费 : %d' % self.account)

    # 对外借钱 
    # 实现create_service里的callback方法，固定参数顺序，必须是request和response输入，response输出，request和response的数据结构参考srv里定义的输入和输出
    def borrow_money_callback(self, request, response):
        # 借钱超过账户的10%，借钱失败，返回0
        if request.money <= int(self.account * 0.1):
            self.account -= request.money
            response.money = request.money
            response.success = True
            self.get_logger().info('借钱成功， 借出 %d，余额 %d' %(request.money, self.account))
        else:
            response.money = 0
            response.success = False
            self.get_logger().info('借钱失败， 余额 %d 不足，借钱 %d 失败' %(self.account, request.money))
        return response


def main(args=None):
    rclpy.init(args=args)

    node = WriterNode("li4")
    # node.get_logger().info("大家好，我是作家李四")
    rclpy.spin(node)
    rclpy.shutdown()
