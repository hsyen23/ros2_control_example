// Copyright (c) 2021, Bence Magyar and Denis Stogl
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef MY_CONTROLLER_HPP_
#define MY_CONTROLLER_HPP_

#include <memory>
#include <string>
#include <vector>

#include "controller_interface/controller_interface.hpp"
#include "rclcpp_lifecycle/node_interfaces/lifecycle_node_interface.hpp"
#include "rclcpp_lifecycle/state.hpp"
#include "realtime_tools/realtime_buffer.h"
#include "realtime_tools/realtime_publisher.h"

#include "control_msgs/msg/joint_controller_state.hpp"
#include "control_msgs/msg/joint_jog.hpp"

namespace my_controller {
using CallbackReturn =
    rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn; // to shorter form CallbackReturn::SUCCESS and CallbackReturn::ERROR

class MyController : public controller_interface::ControllerInterface {
public:
  MyController();                                                           // construtor

  CallbackReturn on_init() override;                                        // declare all of the parameters that we will accept during the controller's lifetime

  controller_interface::InterfaceConfiguration
  command_interface_configuration() const override;                         // This method is where we define what command interfaces are required.

  controller_interface::InterfaceConfiguration
  state_interface_configuration() const override;                           // This method is where we define what state interfaces are required.

  CallbackReturn
  on_configure(const rclcpp_lifecycle::State &previous_state) override;     // read the parameter values and declare any required subscribers and publishers as required. This method is run previous to the update method call and ensures that once the control algorithm is put into action, everything is set and ready to go.

  CallbackReturn
  on_activate(const rclcpp_lifecycle::State &previous_state) override;      // declare a command message and set the default value for the command .

  CallbackReturn
  on_deactivate(const rclcpp_lifecycle::State &previous_state) override;    // The on_deactivate() method is called by the controller manager to shutdown the controller. Often it only contains a return statement that indicates success. If you need to execute some code instructions to be able to perform a graceful shutdown, then make sure you make it as real time as you can.

  controller_interface::return_type
  update(const rclcpp::Time &time, const rclcpp::Duration &period) override;// The update() function is called in the control loop in order to produce a control command for the hardware.

protected:
  std::vector<std::string> joint_names_;                                    // we will read parameter value from on_configure(), and assign the value to this variable.
  std::string interface_name_;                                              // we will read parameter value from on_configure(), and assign the value to this variable.

  // Command subscribers and Controller State publisher
  using ControllerCommandMsg = control_msgs::msg::JointJog;                 // the type (interface/message) of published command topic

  rclcpp::Subscription<ControllerCommandMsg>::SharedPtr command_subscriber_ =
      nullptr;                                                              // used for create command subscriber
  realtime_tools::RealtimeBuffer<std::shared_ptr<ControllerCommandMsg>>
      input_command_;                                                       // buffer to store command topic, it can do writeFromNonRT() and readFromRT().

  using ControllerStateMsg = control_msgs::msg::JointControllerState;       // the type (interface/message) of published state topic (used for observation/ddebug, has no influence)
  using ControllerStatePublisher =
      realtime_tools::RealtimePublisher<ControllerStateMsg>;                // used for create state real-time publisher

  rclcpp::Publisher<ControllerStateMsg>::SharedPtr s_publisher_;            // used for create state publisher
  std::unique_ptr<ControllerStatePublisher> state_publisher_;               // used for create state publisher

};

} // namespace rrbot_controller

#endif // MY_CONTROLLER_HPP_
