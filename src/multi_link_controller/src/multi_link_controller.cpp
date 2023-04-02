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

#include "multi_link_controller/multi_link_controller.hpp"

#include <limits>
#include <memory>
#include <string>
#include <vector>

float round_in_2pi(float r)
{
    r = fmod(r, 2*M_PI);
    float m = fmod (r,M_PI);
    int d = r / M_PI;
    return m - d * M_PI;
}

float clip(float n, float lower, float upper) {
  return std::max(lower, std::min(n, upper));
}

namespace multi_link_controller
{
MultiLinkController::MultiLinkController() : controller_interface::ControllerInterface() {} // empty constructor, ros pluginlib require all plugin have empty constructor!
    
// All the methods added must be inside this namespace

    /*
    Declare all of the parameters that we will accept during the controller's lifetime
    */
    CallbackReturn MultiLinkController::on_init() {
        try {
            // when you pass the yaml file, parameters declare automatically, so we actually don't need these two lines?
            auto_declare("joints", std::vector<std::string>()); // try to declare parameter in ROS named joints, if not declared by the yaml then initialized with std::vector<std::string>().
            auto_declare("command_interface_name", std::string());  // try to declare parameter in ROS named command_interface_name, if not declared by the yaml then initialized with std::string().
        } catch (const std::exception &e) {
            fprintf(stderr, "Exception thrown during init stage with message: %s \n",
                    e.what());
            return CallbackReturn::ERROR;
        }

        return CallbackReturn::SUCCESS;
    }


    /*
    read the parameter values and declare any required subscribers and publishers as required.
    This method is run previous to the update method call and ensures that once the control algorithm is put into action, everything is set and ready to go.

    Fetch the Ros parameter and assign them to the controller instance (those protected variables).

    In this controller, all paramters (joints, interface_name) values are strings.
    */
    CallbackReturn MultiLinkController::on_configure(
        const rclcpp_lifecycle::State & /*previous_state*/) {

        // base lamda function to check whether required ROS parameter value(s) is/are empty
        auto error_if_empty = [&](const auto &parameter, const char *parameter_name) {
            if (parameter.empty()) {
            RCLCPP_ERROR(get_node()->get_logger(), "'%s' parameter was empty",
                        parameter_name);
            return true;
            }
            return false;
        };

        // checking for the case if we have multiple values inside desired parameter (values stored as vector<string>)
        auto get_string_array_param_and_error_if_empty =
            [&](std::vector<std::string> &parameter, const char *parameter_name) {
                parameter = get_node()->get_parameter(parameter_name).as_string_array();
                return error_if_empty(parameter, parameter_name);
            };

        // checking for the case if we only have one value inside desired parameter (value stored as string)
        auto get_string_param_and_error_if_empty =
        [&](std::string &parameter, const char *parameter_name) {
            parameter = get_node()->get_parameter(parameter_name).as_string();
            return error_if_empty(parameter, parameter_name);
        };

        // return error if one of parameters has no value (being empty).
        if (
            get_string_array_param_and_error_if_empty(joint_names_, "joints") ||
            get_string_param_and_error_if_empty(command_interface_name_, "interface_name")) {
            return CallbackReturn::ERROR;
        }

        // Command Subscriber and callbacks
        // The callback function will examinate whether the received ControllerCommandMsg provides sufficient data for all joints (However, it only checks the amount of joint_names in the msg)
        auto callback_command =
            [&](const std::shared_ptr<ControllerCommandMsg> msg) -> void {
            if (msg->joint_names.size() == joint_names_.size()) {
            input_command_.writeFromNonRT(msg); // I not sure about this part, my guess is that writeFromNonRT() will put the msg into RT flow (to get correct timestamp automatically), then we call readFromRT() to acquired the msg with correct timestamp from the RT flow.
            } else {
            RCLCPP_ERROR(get_node()->get_logger(),
                        "Received %zu , but expected %zu joints in command. "
                        "Ignoring message.",
                        msg->joint_names.size(), joint_names_.size());
            }
        };
        command_subscriber_ = get_node()->create_subscription<ControllerCommandMsg>(
            "~/commands", rclcpp::SystemDefaultsQoS(), callback_command); // the topic: "~/commands" will be automatically replaced by "/multi_link_controller/commands", I guess it's due to the namespace.

        // State publisher (this publisher is for observation/debug purpose since we already have joint_state_broadcaster to publish joint_state for robot_state_publisher)
        // We declare a publisher for broadcasting the joint states and make it an unique pointer.
        s_publisher_ =
        get_node()->create_publisher<ControllerStateMsg>(
            "~/state", rclcpp::SystemDefaultsQoS());
        state_publisher_ = std::make_unique<ControllerStatePublisher>(s_publisher_);

        // The last tree lines above are used to prevent multiple threads from accessing the published message at the same time while setting the header frame_id value.
        state_publisher_->lock();
        // state_publisher_->msg_.header.frame_id = joint_names_[0];
        state_publisher_->msg_.data = "Now is on_configure() process";
        state_publisher_->unlock();

        RCLCPP_INFO_STREAM(get_node()->get_logger(), "configure successful");
        return CallbackReturn::SUCCESS;
    }

    /*
    This method is where we define what command interfaces are required.

    Here, we first create a new InterfaceConfiguration object and set the type as INDIVIDUAL. 
    There are three options of the interface configuration ALL, INDIVIDUAL, and NONE. 
    ALL and NONE will ask for access to all available interfaces or none of them. 
    The INDIVIDUAL configuration needs a detailed list of required interface names. 
    Those are usually provided as parameters.

    */
    controller_interface::InterfaceConfiguration MultiLinkController::command_interface_configuration() const
    {
        controller_interface::InterfaceConfiguration command_interfaces_config;
        command_interfaces_config.type = controller_interface::interface_configuration_type::INDIVIDUAL;

        command_interfaces_config.names.reserve(joint_names_.size()); // Reserve memory space for the vector of interface names.
        for (const auto & joint : joint_names_) {
            command_interfaces_config.names.push_back(joint + "/" + command_interface_name_); // command_interfaces_[i] order is determined by this line. (command_interfaces_[0] access interface with name = command_interfaces_config.names[0])
        }
        // Then each joint is provided with it's own interface name, which is kept inside command_interfaces_config.names. 
        // A full interface name has to have the structure <joint_name>/<interface_type>.

        // "command_interfaces_config" allow us to set value to "hardware_interface" through "command_interfaces_".
        return command_interfaces_config;
    }

    /*
    This method is where we define what state interfaces are required.

    This method performs a similar function to the previous method, 
    with the difference that this method is used to define what hardware sensor interfaces are required by the controller.
    */
    controller_interface::InterfaceConfiguration MultiLinkController::state_interface_configuration() const
    {
        controller_interface::InterfaceConfiguration state_interfaces_config;
        state_interfaces_config.type = controller_interface::interface_configuration_type::INDIVIDUAL;

        state_interfaces_config.names.reserve(joint_names_.size());
        for (const auto & joint : joint_names_) {
            state_interfaces_config.names.push_back(joint + "/" + "position"); // We save the sensor interface names to the InterfaceConfiguration object created before.
        }                                                                      // For this controller, we only reqire position on state_interface

        return state_interfaces_config;
    }

    /*
    This templated function takes in an unordered vector of interfaces as argument and converts it into an ordered vector. 
    It is required to fill ordered_interfaces with references to the matching interfaces in the same order as in joint_names.

    I am not sure when the function is called.
    */
    template <typename T>
    bool get_ordered_interfaces(
    std::vector<T> & unordered_interfaces, const std::vector<std::string> & joint_names,
    const std::string & interface_type, std::vector<std::reference_wrapper<T>> & ordered_interfaces)
    {
        for (const auto & joint_name : joint_names) {
            for (auto & command_interface : unordered_interfaces) {
            if (
                (command_interface.get_name() == joint_name) &&
                (command_interface.get_interface_name() == interface_type)) {
                ordered_interfaces.push_back(std::ref(command_interface));
            }
            }
        }

        return joint_names.size() == ordered_interfaces.size();
    }

    /*
    Declare a command message and set the default value for the command .

    on_activate ----quickly switch to----> update
    */
    CallbackReturn MultiLinkController::on_activate(const rclcpp_lifecycle::State & /*previous_state*/)
    {
    // Set default value in command
    std::shared_ptr<ControllerCommandMsg> msg = std::make_shared<ControllerCommandMsg>();
    msg->joint_names = joint_names_;
    msg->controllable.resize(joint_names_.size(), false);
    msg->values.resize(joint_names_.size(), std::numeric_limits<double>::quiet_NaN());
    input_command_.writeFromNonRT(msg); // update note for writeFromNonRT(msg). 
                                        // This will push the msg to RT flow, and exist in RT flow until next msg come in. 
                                        // In other word, it is like some nodes keep publish the same message on this "RT topic". 
                                        // That is why readFromRT() always can get value.
    // Preparation for PID
    previous_error_.resize(joint_names_.size(), 0);
    error_integral_.resize(joint_names_.size(), 0);
    return CallbackReturn::SUCCESS;
    }

    /*
    The on_deactivate() method is called by the controller manager to shutdown the controller. 
    Often it only contains a return statement that indicates success. 
    If you need to execute some code instructions to be able to perform a graceful shutdown, then make sure you make it as real time as you can.
    */
    CallbackReturn MultiLinkController::on_deactivate(const rclcpp_lifecycle::State & /*previous_state*/)
    {
    return CallbackReturn::SUCCESS;
    }

    /*
    The update() function is called in the control loop in order to produce a control command for the hardware.

    When controller enter active mode, it will keep looping the update() function untill being unactive.
    */
    controller_interface::return_type
    MultiLinkController::update(const rclcpp::Time &time,
                            const rclcpp::Duration &duration /*period*/) {
        auto current_command = input_command_.readFromRT(); // That is why we use readFromRT() to always ensure we get value as command.
                                                            // If we use a subscriber here, the update() will block here untill one publisher send a message, and it will back to blocked for next message.
      
        for (size_t i = 0; i < command_interfaces_.size(); ++i) {
            if (!std::isnan((*current_command)->values[i])) {
            // Here is PID control part 
            
            // compute error = target - current_state
            float error = round_in_2pi((*current_command)->values[i] - state_interfaces_[i].get_value());
            // select shortest arc
            float diff_error = (error - previous_error_[i])/ duration.seconds();
            previous_error_[i] = error;
            error_integral_[i] += I_[i] * error;
            float torque_output = (P_[i] * error + error_integral_[i] + D_[i] * diff_error) * (*current_command)->controllable[i];
            torque_output = clip(torque_output, -200.0, 200.0);
            command_interfaces_[i].set_value(torque_output);
            //RCLCPP_INFO_STREAM(get_node()->get_logger(),"Log tester");
            //RCLCPP_INFO_STREAM(get_node()->get_logger(),"see time interval: " << duration.seconds()); // display time interval between updates.
            }
        }




        // keep the state output part
        if (state_publisher_ && state_publisher_->trylock()) {
            //state_publisher_->msg_.header.stamp = time;
            //state_publisher_->msg_.set_point = state_interfaces_[0].get_value(); // we can use state_interfaces_[0].get_value() to know the state, but I need to discover the case for multiple data in state_interfaces
            state_publisher_->msg_.data = " Now is update() process";
            state_publisher_->unlockAndPublish();
        }

        return controller_interface::return_type::OK;
    }


}  // namespace multi_link_controller

#include "pluginlib/class_list_macros.hpp"

PLUGINLIB_EXPORT_CLASS(multi_link_controller::MultiLinkController, controller_interface::ControllerInterface)
