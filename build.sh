#!/bin/bash
rm -rf build/ install/ log/
colcon build
source install/setup.bash
ros2 launch village_wang village_wang_launch.py