# Tutorial for put std_msgs/msg/Header into custom message
First, check the official website to understand the structure of Header.

https://docs.ros2.org/latest/api/std_msgs/msg/Header.html

## First of all, `Header` has dependency too!
From the official website, it's clear that std_msgs/msg/Header contains another message builtin_interfaces/msg/Time.

Therefore, to use Header, we require two packages (std_msgs and builtin_interfaces) as dependencies.

## Modify CMakeLists.txt
```
find_package(std_msgs REQUIRED)
find_package(rosidl_default_generators REQUIRED)

rosidl_generate_interfaces(${PROJECT_NAME}
  "msg/Controlnote.msg"
  DEPENDENCIES std_msgs builtin_interfaces
```

## Modify package.xml
```
<depend>std_msgs</depend>
<depend>builtin_interfaces</depend>

<build_depend>rosidl_default_generators</build_depend>
<exec_depend>rosidl_default_runtime</exec_depend>
<member_of_group>rosidl_interface_packages</member_of_group>
```

# Miscellaneous

## Interface, Message clarification
Interface is a idea, and Message is the object to fullfill this idea.

Therefore, ".msg", ".srv", and ".action" all are interfaces.

## Rule of name
The interface with name std_msgs/msg/Header represents:

std_msgs: The package name that generates this message. That is why we can use find_package(std_msgs REQUIRED).

msg: Shows that this is message type interface.

Header: The file name is Header.msg. Therefore, we can find the message file in std_msgs/msg/Header.msg.

## Don't need to sepcify Header structure again in custom message
Like use standard data type, just write in `.msg`.
```
std_msgs/Header header
string[] joint_names
float64[] values
```
Not
```
std_msgs/Header header
	builtin_interfaces/Time stamp
		int32 sec
		uint32 nanosec
	string frame_id
	
string[] joint_names
float64[] values
```
