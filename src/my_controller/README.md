This is a forward controller, it reads the topic and then assign the value to the command_interface.

The controller can only control all joints with the same command_interface.

That's being said, joint1 and joint2 cannot be controlled by different command_interface. e.g. position for joint1, velocity for joint2.

Same for reading state_interface, it can only read the interface that is being control. e.g. my command_interface is effort, then I can only read value from effort interface.

Some notes for gazebo_hardware plugin. Gazebo_hardware provides three interface: position, velocity, and effort.

Therefore, we can directly controll these three value. e.g. my controller send command_interface position to 1.0. The robot in Gazebo will instantly set the postion to 1.0.

Effort interface is torque for rotational joint, force for prismatic joint.
