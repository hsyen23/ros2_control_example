#ifndef MY_HARDWARE_INTERFACE_HPP_
#define MY_HARDWARE_INTERFACE_HPP_

#include <vector>
#include <map>

#include <hardware_interface/handle.hpp>
#include <hardware_interface/hardware_info.hpp>
#include <hardware_interface/system_interface.hpp>
#include <rclcpp_lifecycle/state.hpp> 
#include "rclcpp/macros.hpp"

#include "my_fake_hardware_SDK/my_fake_hardware_SDK.hpp"

using hardware_interface::return_type; // to use short form return_type::OK instead of the more verbose version hardware_interface::return_type::OK.
using CallbackReturn = rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn;

namespace my_hardwares
{
/*
Create a JointValue structure with fields for a position, velocity, and effort value. 
This structure becomes very handy as the data field goes into the structure Joint, which you will use later to keep track of both the state and the command for each joint.

Here we say our robot actuator has these three state to read or write. We can also add more custom states depends on the actuator.
*/
struct JointValue
{
  double position{0.0};
  double velocity{0.0};
  double effort{0.0};
};

struct Joint
{
  JointValue state{};
  JointValue command{};
};

class MyHardware
: public hardware_interface::SystemInterface
{
public:
  RCLCPP_SHARED_PTR_DEFINITIONS(MyHardware) // This is to make a shared pointer from this class.

  CallbackReturn on_init(const hardware_interface::HardwareInfo & info) override;

  std::vector<hardware_interface::StateInterface> export_state_interfaces() override;

  std::vector<hardware_interface::CommandInterface> export_command_interfaces() override;

  CallbackReturn on_activate(const rclcpp_lifecycle::State & previous_state) override;

  CallbackReturn on_deactivate(const rclcpp_lifecycle::State & previous_state) override;

  return_type read(const rclcpp::Time & time, const rclcpp::Duration & period) override;

  return_type write(const rclcpp::Time & time, const rclcpp::Duration & period) override;

private:
  return_type reset_command();
  MyFakeHardware fake_hardware_;

  std::vector<Joint> joints_;
  std::vector<uint8_t> joint_ids_;

  bool use_dummy_{false};
};
}  // namespace my_hardwares

#endif  // MY_HARDWARE_INTERFACE_HPP_
