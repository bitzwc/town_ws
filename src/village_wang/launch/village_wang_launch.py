import launch
from launch.substitutions import EnvironmentVariable
from launch.substitutions import LaunchConfiguration
from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import DeclareLaunchArgument
import os
from ament_index_python.packages import get_package_share_directory
import yaml
import ament_index_python.packages
import sys

def generate_launch_description():

    # print(sys.argv[0])
    # print(__file__)
    print("****************")

    #share目录
    print(get_package_share_directory('village_wang'))
    #项目目录
    print(os.getcwd())
    
    # param参数配置文件路径
    parameters_configuration = os.path.join(
        os.getcwd(), 
        'src/village_wang/config', 
        'village_wang_parameters_configuration.yaml'
    )
    print(parameters_configuration)
    
    return LaunchDescription([
        DeclareLaunchArgument(
            'node_prefix',
            default_value=[EnvironmentVariable("USER"), '_'],
            description='Prefix for node names'
        ),
        #启动mpc控制节点，指定包名、可执行文件名、节点名、参数配置
        Node(
            package='village_wang',
            executable='wang2_node',
            name='wang2',
            parameters=[parameters_configuration],
            # remappings=None,
            # arguments=None,
            output='screen',
        ),
    ])
