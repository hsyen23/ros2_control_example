#include <algorithm>
#include <array>
#include <string>
#include <limits>
#include <vector>

#include "my_hardware_interface/my_hardware_interface.hpp"

#include "hardware_interface/types/hardware_interface_return_values.hpp"
#include "hardware_interface/types/hardware_interface_type_values.hpp"
#include "rclcpp/rclcpp.hpp"

namespace my_hardwares
{
constexpr const char * kMyHardwareName = "MyHardware";

/*
In the first line, the parent on_init is called to process the parameters read from the .yaml configuration file. 
This is done by using hardware_interface::SystemInterface::on_init(info). 
This validation should return CallbackReturn::ERROR if an internal error occurred during parameter reading.
*/
CallbackReturn MyHardware::on_init(const hardware_interface::HardwareInfo & info) // info has all information passed in from .yaml
{
  RCLCPP_DEBUG(rclcpp::get_logger(kMyHardwareName), "configure");
  if (hardware_interface::SystemInterface::on_init(info) != CallbackReturn::SUCCESS)
  {
    return CallbackReturn::ERROR;
  }

  joints_.resize(info_.joints.size(), Joint()); // initiate the variable in the instance with correct size.
  joint_ids_.resize(info_.joints.size(), 0);    // initiate the variable in the instance with correct size.

  for (uint i = 0; i < info_.joints.size(); i++) {  // initialize variables in the instance with default value (NaN).
    joint_ids_[i] = std::stoi(info_.joints[i].parameters.at("id"));
    joints_[i].state.position = std::numeric_limits<double>::quiet_NaN();
    joints_[i].state.velocity = std::numeric_limits<double>::quiet_NaN();
    joints_[i].state.effort = std::numeric_limits<double>::quiet_NaN();
    joints_[i].command.position = std::numeric_limits<double>::quiet_NaN();
    joints_[i].command.velocity = std::numeric_limits<double>::quiet_NaN();
    joints_[i].command.effort = std::numeric_limits<double>::quiet_NaN();
    RCLCPP_INFO(rclcpp::get_logger(kMyHardwareName), "joint_id %d: %d", i, joint_ids_[i]);
  }

  // First check whether there is "use_dummpy" parameter in .yaml, if it exists then check whether it is true.
  if (
    info_.hardware_parameters.find("use_dummy") != info_.hardware_parameters.end() &&
    info_.hardware_parameters.at("use_dummy") == "true") {
    use_dummy_ = true;
    RCLCPP_INFO(rclcpp::get_logger(kMyHardwareName), "dummy mode");
    return CallbackReturn::SUCCESS;
  }
  
  // Not dummy mode, we need to setup the variables that used to communicate with the actuators.
  auto usb_port = info_.hardware_parameters.at("usb_port");
  auto baud_rate = std::stoi(info_.hardware_parameters.at("baud_rate"));
  const char * log = nullptr;

  RCLCPP_INFO(rclcpp::get_logger(kMyHardwareName), "usb_port: %s", usb_port.c_str());
  RCLCPP_INFO(rclcpp::get_logger(kMyHardwareName), "baud_rate: %d", baud_rate);
  
  // Call the method do connection with hardware
  if (!fake_hardware_.init(usb_port.c_str(), baud_rate, info_.joints.size(), &log)) {
    RCLCPP_FATAL(rclcpp::get_logger(kMyHardwareName), "%s", log);
    return CallbackReturn::ERROR;
  }
  // Afterward, ping each component by ID. If you fail to communicate with one of the servos, log the error message and exit.
  for (uint i = 0; i < info_.joints.size(); ++i) {
    if (!fake_hardware_.ping(joint_ids_[i], &log)) {
      RCLCPP_FATAL(rclcpp::get_logger(kMyHardwareName), "%s", log);
      return CallbackReturn::ERROR;
    }
  }
  
  return CallbackReturn::SUCCESS;
}

/*
The export_state_interfaces() method is supposed to define the state interfaces that your hardware offers.

The code below adds three different hardware interface types to the state_interfaces vector. 
This means that the position state is registered as an available state broadcast by the Hardware. 
Additionally, the code above registers the states HW_IF_VELOCITY and HW_IF_EFFORT as two additional state variables broadcast on the hardware side.
*/
std::vector<hardware_interface::StateInterface> MyHardware::export_state_interfaces()
{
  RCLCPP_DEBUG(rclcpp::get_logger(kMyHardwareName), "export_state_interfaces");
  std::vector<hardware_interface::StateInterface> state_interfaces;
  for (uint i = 0; i < info_.joints.size(); i++) {
    state_interfaces.emplace_back(hardware_interface::StateInterface(
      info_.joints[i].name, hardware_interface::HW_IF_POSITION, &joints_[i].state.position)); // hardware_interface::HW_IF_POSITION = "position"
                                                                                              // This means we can define new type of interface like "power", then the second argument just the string "power"
                                                                                              // First arguament is the name (it will be "arm_joint1" or "arm_joint2" in our case)
                                                                                              // Second argument is the name of interface type
                                                                                              // Thrid argument is the reference of the value to this class instance 
    state_interfaces.emplace_back(hardware_interface::StateInterface(
      info_.joints[i].name, hardware_interface::HW_IF_VELOCITY, &joints_[i].state.velocity)); // hardware_interface::HW_IF_VELOCITY = "velocity"
    state_interfaces.emplace_back(hardware_interface::StateInterface(
      info_.joints[i].name, hardware_interface::HW_IF_EFFORT, &joints_[i].state.effort));     // hardware_interface::HW_IF_EFFORT = "effort"
  }

  return state_interfaces;
}

/*
The export_command_interfaces() method is where you define the command interfaces that your hardware offers.

First, create a command_interfaces vector, then store the type of commands accepted by your hardware device into that vector. 
As per the SDK documentation, we suppose the firmware currently supports the position, velocity, and current control modes, among others. 
Therefore, in this example, you only register HW_IF_POSITION and HW_IF_VELOCITY as supported control modes.
*/
std::vector<hardware_interface::CommandInterface> MyHardware::export_command_interfaces()
{
  RCLCPP_DEBUG(rclcpp::get_logger(kMyHardwareName), "export_command_interfaces");
  std::vector<hardware_interface::CommandInterface> command_interfaces;
  for (uint i = 0; i < info_.joints.size(); i++) {
    command_interfaces.emplace_back(hardware_interface::CommandInterface(
      info_.joints[i].name, hardware_interface::HW_IF_POSITION, &joints_[i].command.position));
    command_interfaces.emplace_back(hardware_interface::CommandInterface(
      info_.joints[i].name, hardware_interface::HW_IF_VELOCITY, &joints_[i].command.velocity));
  }

  return command_interfaces;
}

/*
Use on_activate() to ensure all are set up and ready to receive control commands.
*/
CallbackReturn MyHardware::on_activate(const rclcpp_lifecycle::State & previous_state)
{
  RCLCPP_DEBUG(rclcpp::get_logger(kMyHardwareName), "start");
  for (uint i = 0; i < joints_.size(); i++) {
    if (use_dummy_ && std::isnan(joints_[i].state.position)) {
      joints_[i].state.position = 0.0;
      joints_[i].state.velocity = 0.0;
      joints_[i].state.effort = 0.0;
    }
  }

  // I don't know how to properly create rclcpp::Time, rclcpp::Duration for read() and write()
  rclcpp::Time end_period(0,0);
  rclcpp::Duration period(0,1);
  read(end_period, period);
  reset_command();
  write(end_period, period);

  return CallbackReturn::SUCCESS;
}

/*
Next, implement the on_deactivate() method to perform a clean device shutdown and disable any type of movement on the hardware.

To successfully shutdown the fake device, there are no actions required. 
Therefore, this method takes only one line of code which is a debug log for diagnostic purposes. 
*/
CallbackReturn MyHardware::on_deactivate(const rclcpp_lifecycle::State & previous_state)
{
  RCLCPP_DEBUG(rclcpp::get_logger(kMyHardwareName), "stop");
  return CallbackReturn::SUCCESS;
}

/*
Next, implement the read() function that is called in the control loop as input to the computation of the control command.
*/
return_type MyHardware::read(const rclcpp::Time & time, const rclcpp::Duration & period)
{
  if (use_dummy_) {
    return return_type::OK;
  }

  for (uint i = 0; i < joints_.size(); i++) {
    joints_[i].state.position = fake_hardware_.read_position_data(joint_ids_[i]);
    joints_[i].state.velocity = fake_hardware_.read_velocity_data(joint_ids_[i]);
    joints_[i].state.effort = fake_hardware_.read_velocity_data(joint_ids_[i]) / 2; // suppose velocity/2 is current
  }

  return return_type::OK;
}

/*
Next, implement the write() method required to issue commands for the hardware device.
*/
return_type MyHardware::write(const rclcpp::Time & time, const rclcpp::Duration & period)
{
  const char * log = nullptr;

  if (use_dummy_) {
    for (auto & joint : joints_) {
      joint.state.position = joint.command.position;
    }

    return return_type::OK;
  }

  // First, check velocity command, if velocity command is not zero, then write velocity data, then position.
  // We do not have effort command interface, therefore there is no effort control
  if(std::any_of(joints_.cbegin(), joints_.cend(), [](auto j) { return j.command.velocity != 0.0; }))
  {
      for (uint i = 0; i < joint_ids_.size(); i++) {
        if(!fake_hardware_.write_velocity_data(joint_ids_[i], joints_[i].command.velocity, &log)){
          RCLCPP_FATAL(rclcpp::get_logger(kMyHardwareName), "%s", log);
          return return_type::ERROR;
        }
      }
  }else{
      for (uint i = 0; i < joint_ids_.size(); i++) {
        if(!fake_hardware_.write_position_data(joint_ids_[i], joints_[i].command.position, &log)){
          RCLCPP_FATAL(rclcpp::get_logger(kMyHardwareName), "%s", log);
          return return_type::ERROR;
        }
      }
  }
  return return_type::OK;
}

/*
Sometimes you want to have a fresh start for the control algorithm and make the joint's current command to be exactly the current position. 
This is particularly useful at startup or when changing operation mode.
*/
return_type MyHardware::reset_command()
{
  for (uint i = 0; i < joints_.size(); i++) {
    joints_[i].command.position = joints_[i].state.position;
    joints_[i].command.velocity = 0.0;
    joints_[i].command.effort = 0.0;
  }

  return return_type::OK;
}

}  // namespace my_hardwares 

#include "pluginlib/class_list_macros.hpp"

PLUGINLIB_EXPORT_CLASS(my_hardwares::MyHardware, hardware_interface::SystemInterface)

