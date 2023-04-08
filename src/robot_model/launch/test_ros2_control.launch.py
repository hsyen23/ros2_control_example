import os
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription, TimerAction
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import Command

from launch_ros.actions import Node
import xacro


def generate_launch_description():

    # Specify the name of the package and path to xacro file within the package
    pkg_name = 'robot_model'
    file_subpath = 'description/test_ros2_control.urdf.xacro'


    # Use xacro to process the file
    xacro_file = os.path.join(get_package_share_directory(pkg_name),file_subpath)
    robot_description_raw = xacro.process_file(xacro_file).toxml()


    # Configure the node
    node_robot_state_publisher = Node(
        package='robot_state_publisher',
        executable='robot_state_publisher',
        output='screen',
        parameters=[{'robot_description': robot_description_raw,
        'use_sim_time': False}] # add other parameters here if required
    )

    # robot_description = Command(['ros2 param get --hide-type /robot_state_publisher robot_description'])

    controller_params_file = os.path.join(get_package_share_directory('robot_model'),'config','my_controllers_for_fake_hardware_interface.yaml')

    controller_manager = Node(
        package = "controller_manager",
        executable = "ros2_control_node",
        parameters = [{'robot_description' : robot_description_raw},
                      controller_params_file]
    )

    delayed_controller_manager = TimerAction(period = 3.0, actions = [controller_manager])






    # Run the node
    return LaunchDescription([
        node_robot_state_publisher,
        delayed_controller_manager
    ])
