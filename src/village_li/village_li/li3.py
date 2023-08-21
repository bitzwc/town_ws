import rclpy
from rclpy.node import Node
from std_msgs.msg import String

# 继承Node
class BaiPiaoNode(Node):
    def __init__(self, name) -> None:
        # 初始化生成一个节点，给节点命名name
        super().__init__(name) #这里必须执行父类的初始化方法
        self.get_logger().info("创建节点成功 %s" % name)
        # 订阅小说
        self.create_subscription(String, "sexy_girl", self.subscript_callback, 10)

    # 订阅topic的回调
    def subscript_callback(self, str: String):
        self.get_logger().info("我是 %s, 收到消息 %s" % (self.get_name(), str.data)) # String是rcl封装的类型，不是python的str，需要使用.data转化成python的str类型


def main():
    rclpy.init() #rcl：ros客户端库
    node = BaiPiaoNode('li3')
    rclpy.spin(node) # 保持节点运行，检测是否收到退出指令（Ctrl+C）
    rclpy.shutdown() #rcl关闭