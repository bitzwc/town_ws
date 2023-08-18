import rclpy

from rclpy.node import Node
from std_msgs.msg import String
from std_msgs.msg import UInt32


class WriterNode(Node):
    def __init__(self, name):
        super().__init__(name)
        self.get_logger().info("大家好，我是%s" % name)
        self.pub_novel = self.create_publisher(String, "sexy_girl", 10)
        time_period = 5
        self.i = 0  # i 是个计数器，用来算章节编号的
        #计时器，按这个时间来发布
        self.create_timer(timer_period_sec=time_period, callback=self.time_callback)

        self.account = 0
        self.create_subscription(UInt32, 'sexy_girl_money', self.recv_money_callback, 10)

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



def main(args=None):
    rclpy.init(args=args)

    node = WriterNode("li4")
    # node.get_logger().info("大家好，我是作家李四")
    rclpy.spin(node)
    rclpy.shutdown()
