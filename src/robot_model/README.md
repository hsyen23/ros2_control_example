# robot_model
Contains robot model URDF and launch files.

# How to display URDF model in rviz2
![Alt text](https://github.com/hsyen23/ros2_control_example/blob/main/picture/urdf_publishing_explanation.jpg "urdf_publishing_explanation")

`robot_model/robot_state_publisher_launch.py` is a launch file to load urdf.xml to robot_state_publisher.

```
ros2 launch robot_model robot_state_publisher_launch.py
```

Use `joint_state_publisher_gui` for publishing `joint_state` to node `robot_state_publisher` to generate correct `TF` for joints.

```
ros2 run joint_state_publisher_gui joint_state_publisher_gui
```

Now we can see robot model in rviz2, and manupulate it with `joint_state_publisher_gui`.
```
rviz2
```

# How to display robot model in Gazebo
##### A. Use ros2_control plugin to publish `joint_state`
![Alt text](https://github.com/hsyen23/ros2_control_example/blob/main/picture/ros2_control_Gazebo_pipeline.jpg "ros2_control_Gazebo_pipeline")

`robot_model/gazeobo_sim.launch.py` is a launch file for simulation in Gazebo.

```
ros2 launch robot_model gazeobo_sim.launch.py
```
Note that we are using ros2_control Gazebo plugin in `robot.urdf.xacro` now. Therefore, we need `joint_state_broadcaster` from `controller_manager` to publish `joint_state`.
```
ros2 run controller_manager spawner joint_state_broadcaster
```
##### B. If we don't want to use ros2_control to publish `joint_state`
![Alt text](https://github.com/hsyen23/ros2_control_example/blob/main/picture/interaction_with_Gazebo.png "interaction_with_Gazebo")

We need to remove `include gazebo_ros2_control.xacro` in `robot.urdf.xacro`, and add `gazebo_ros_joint_state_publisher` plugin in `gazebo.xacro`.
  
