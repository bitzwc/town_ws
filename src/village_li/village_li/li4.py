import rclpy

from rclpy.node import Node
from std_msgs.msg import String
from std_msgs.msg import UInt32
from sensor_msgs.msg import Image
from village_interfaces.srv import BorrowMoney
from village_interfaces.msg import Novel

class WriterNode(Node):
    def __init__(self, name):
        super().__init__(name)
        self.get_logger().info("大家好，我是%s" % name)
        self.pub_novel = self.create_publisher(Novel, "sexy_girl", 10)
        time_period = 5
        self.i = 0  # i 是个计数器，用来算章节编号的
        #计时器，按这个时间来发布
        self.timer = self.create_timer(timer_period_sec=time_period, callback=self.time_callback)
        self.account = 100
        self.create_subscription(UInt32, 'sexy_girl_money', self.recv_money_callback, 10)

        #创建服务，在节点初始化时完成
        #参数：服务接口类型
             #服务名称：全局唯一
             #回调函数：服务端处理逻辑
        self.borrow_server = self.create_service(BorrowMoney, "borrow_money", self.borrow_money_callback)

        #声明一个空的图像
        self.image = Image()

        #订阅图片topic
        self.create_subscription(Image, "image", self.image_callback, 10)

        # 声明参数,参数名字，默认值
        self.declare_parameter("write_timer_period",5)

    # 发布“小说”
    def time_callback(self):
        msg = Novel()
        msg.content = '第%d回: 潋滟湖 %d 次偶遇胡艳娘' % (self.i, self.i)
        msg.image = self.image
        self.pub_novel.publish(msg)
        self.get_logger().info('李四:我发布了艳娘传奇："%s"' % msg.content)  # 打印一下发布的数据，供我们看
        self.get_logger().info('李四:并且为艳娘传奇配上了插图，长："%d"，宽：%d' % (msg.image.height,msg.image.width))    #打印一下发布的插图尺寸，供我们看
        self.i += 1

        # 获取参数的值
        timer_period = self.get_parameter('write_timer_period').get_parameter_value().integer_value

        #将参数的值（秒）转化成纳秒 *10^9，赋给计时器
        self.timer.timer_period_ns = timer_period * (1000*1000*1000)

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

    # 图片回调
    def image_callback(self, image):
        self.image = image



def main(args=None):
    rclpy.init(args=args)

    node = WriterNode("li4")
    # node.get_logger().info("大家好，我是作家李四")
    rclpy.spin(node)
    rclpy.shutdown()
