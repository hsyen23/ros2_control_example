// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from custom_interfaces:msg/Controlnote.idl
// generated code does not contain a copyright notice

#ifndef CUSTOM_INTERFACES__MSG__DETAIL__CONTROLNOTE__BUILDER_HPP_
#define CUSTOM_INTERFACES__MSG__DETAIL__CONTROLNOTE__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "custom_interfaces/msg/detail/controlnote__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace custom_interfaces
{

namespace msg
{

namespace builder
{

class Init_Controlnote_values
{
public:
  explicit Init_Controlnote_values(::custom_interfaces::msg::Controlnote & msg)
  : msg_(msg)
  {}
  ::custom_interfaces::msg::Controlnote values(::custom_interfaces::msg::Controlnote::_values_type arg)
  {
    msg_.values = std::move(arg);
    return std::move(msg_);
  }

private:
  ::custom_interfaces::msg::Controlnote msg_;
};

class Init_Controlnote_the_joint_names
{
public:
  explicit Init_Controlnote_the_joint_names(::custom_interfaces::msg::Controlnote & msg)
  : msg_(msg)
  {}
  Init_Controlnote_values the_joint_names(::custom_interfaces::msg::Controlnote::_the_joint_names_type arg)
  {
    msg_.the_joint_names = std::move(arg);
    return Init_Controlnote_values(msg_);
  }

private:
  ::custom_interfaces::msg::Controlnote msg_;
};

class Init_Controlnote_header
{
public:
  Init_Controlnote_header()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_Controlnote_the_joint_names header(::custom_interfaces::msg::Controlnote::_header_type arg)
  {
    msg_.header = std::move(arg);
    return Init_Controlnote_the_joint_names(msg_);
  }

private:
  ::custom_interfaces::msg::Controlnote msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::custom_interfaces::msg::Controlnote>()
{
  return custom_interfaces::msg::builder::Init_Controlnote_header();
}

}  // namespace custom_interfaces

#endif  // CUSTOM_INTERFACES__MSG__DETAIL__CONTROLNOTE__BUILDER_HPP_
