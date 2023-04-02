// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from custom_interfaces:msg/MultiLinkPos.idl
// generated code does not contain a copyright notice

#ifndef CUSTOM_INTERFACES__MSG__DETAIL__MULTI_LINK_POS__BUILDER_HPP_
#define CUSTOM_INTERFACES__MSG__DETAIL__MULTI_LINK_POS__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "custom_interfaces/msg/detail/multi_link_pos__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace custom_interfaces
{

namespace msg
{

namespace builder
{

class Init_MultiLinkPos_values
{
public:
  explicit Init_MultiLinkPos_values(::custom_interfaces::msg::MultiLinkPos & msg)
  : msg_(msg)
  {}
  ::custom_interfaces::msg::MultiLinkPos values(::custom_interfaces::msg::MultiLinkPos::_values_type arg)
  {
    msg_.values = std::move(arg);
    return std::move(msg_);
  }

private:
  ::custom_interfaces::msg::MultiLinkPos msg_;
};

class Init_MultiLinkPos_controllable
{
public:
  explicit Init_MultiLinkPos_controllable(::custom_interfaces::msg::MultiLinkPos & msg)
  : msg_(msg)
  {}
  Init_MultiLinkPos_values controllable(::custom_interfaces::msg::MultiLinkPos::_controllable_type arg)
  {
    msg_.controllable = std::move(arg);
    return Init_MultiLinkPos_values(msg_);
  }

private:
  ::custom_interfaces::msg::MultiLinkPos msg_;
};

class Init_MultiLinkPos_joint_names
{
public:
  explicit Init_MultiLinkPos_joint_names(::custom_interfaces::msg::MultiLinkPos & msg)
  : msg_(msg)
  {}
  Init_MultiLinkPos_controllable joint_names(::custom_interfaces::msg::MultiLinkPos::_joint_names_type arg)
  {
    msg_.joint_names = std::move(arg);
    return Init_MultiLinkPos_controllable(msg_);
  }

private:
  ::custom_interfaces::msg::MultiLinkPos msg_;
};

class Init_MultiLinkPos_header
{
public:
  Init_MultiLinkPos_header()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_MultiLinkPos_joint_names header(::custom_interfaces::msg::MultiLinkPos::_header_type arg)
  {
    msg_.header = std::move(arg);
    return Init_MultiLinkPos_joint_names(msg_);
  }

private:
  ::custom_interfaces::msg::MultiLinkPos msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::custom_interfaces::msg::MultiLinkPos>()
{
  return custom_interfaces::msg::builder::Init_MultiLinkPos_header();
}

}  // namespace custom_interfaces

#endif  // CUSTOM_INTERFACES__MSG__DETAIL__MULTI_LINK_POS__BUILDER_HPP_
