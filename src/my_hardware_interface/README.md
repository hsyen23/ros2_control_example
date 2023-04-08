# How to run
Launch for robot_state_publisher and controller_manager
```
ros2 launch robot_model test_ros2_control.launch.py
```
Spawn my_controller and joint_state_broadcaster
```
ros2 run controller_manager spawner my_controller
ros2 run controller_manager spawner joint_state_broadcaster
```
View on rviz2
```
rviz2
```
Send command to my_controller
```
ros2 topic pub --once /my_controller/commands control_msgs/msg/JointJog "{joint_names: ["arm_joint1", "arm_joint2"], displacements: [10.0, 0.0]}"
```
We can use this to check value (the value should be half of original one because that is how the fake SDK work)
```
ros2 topic echo /joint_states
```
Because we also publish `/joint_states` to `robot_state_publisher`, rviz2 will also display robot correctly.


# How to create custom hardware interface
### SDK Installation
The SDK has be packed into a ros2 package.

Therefore, we can use the library as long as we indicate it as dependency.
### Create a Package for Your Hardware Interface
```
ros2 pkg create --build-type=ament_cmake my_hardware_interface --dependencies rclcpp pluginlib hardware_interface my_fake_hardware_SDK
```
### Add a header file (.hpp)
Check the file written in `include/my_hardware_interface/my_hardware_interface.hpp`

### Add a .cpp source File
Check the file written in `src/my_hardware_interface.cpp`

### Plugin registration
Add the following code in `.cpp` to registrate the file as plugin.
```
#include "pluginlib/class_list_macros.hpp"
PLUGINLIB_EXPORT_CLASS(my_hardwares::MyHardware, hardware_interface::SystemInterface)
```

### Writing Export Definition for pluginlib
Create plugin XML file named `my_hardwares.xml`
```
<library path="my_hardware_interface">
  <class name="my_hardwares/MyHardware"
         type="my_hardwares::MyHardware"
         base_class_type="hardware_interface::SystemInterface">
    <description>
      My hardware interface plugin for ros2_control
    </description>
  </class>
</library>
```

### Prepare your CMakeLists.txt and package.xml Files
`CMakeLists.txt`
```
add_library(
  ${PROJECT_NAME}
  SHARED
  src/my_hardware_interface.cpp
)

target_include_directories(
  ${PROJECT_NAME}
  PRIVATE
  include
)

ament_target_dependencies(
  ${PROJECT_NAME}
  rclcpp
  hardware_interface
  pluginlib
  my_fake_hardware_SDK
  )

pluginlib_export_plugin_description_file(hardware_interface my_hardwares.xml)

# INSTALL
install(
  TARGETS ${PROJECT_NAME}
  DESTINATION lib
)
install(
  DIRECTORY include/
  DESTINATION include
)

## EXPORTS
ament_export_include_directories(
  include
)
ament_export_libraries(
  ${PROJECT_NAME}
)
ament_export_dependencies(
  rclcpp
  hardware_interface
  pluginlib
  my_fake_hardware_SDK
)
```
`package.xml`
```
  <depend>rclcpp</depend>
  <depend>pluginlib</depend>
  <depend>hardware_interface</depend>
  <depend>my_fake_hardware_SDK</depend>
```

### Add ros2_control tag to .urdf
I use another urdf `test_ros2_control.urdf.xacro` to test the custom hardware interface.

The ros2_control tag is in `ros2_control_custom_hardware_interface.xacro`.

Remember setup joints, parameters, and plugin correctly in `ros2_control_custom_hardware_interface.xacro`.
### Pass .yaml to controller_manager in launch file
Check `test_ros2_control.launch.py`

# How to read hardware interface parameter by `hardware_interface::HardwareInfo & info`
![Alt text](picture/control_one_joint.gif "how_to_read_hardware_interface_parameter")