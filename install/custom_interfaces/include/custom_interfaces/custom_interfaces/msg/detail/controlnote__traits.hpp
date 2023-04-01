// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from custom_interfaces:msg/Controlnote.idl
// generated code does not contain a copyright notice

#ifndef CUSTOM_INTERFACES__MSG__DETAIL__CONTROLNOTE__TRAITS_HPP_
#define CUSTOM_INTERFACES__MSG__DETAIL__CONTROLNOTE__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "custom_interfaces/msg/detail/controlnote__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

// Include directives for member types
// Member 'header'
#include "std_msgs/msg/detail/header__traits.hpp"

namespace custom_interfaces
{

namespace msg
{

inline void to_flow_style_yaml(
  const Controlnote & msg,
  std::ostream & out)
{
  out << "{";
  // member: header
  {
    out << "header: ";
    to_flow_style_yaml(msg.header, out);
    out << ", ";
  }

  // member: the_joint_names
  {
    if (msg.the_joint_names.size() == 0) {
      out << "the_joint_names: []";
    } else {
      out << "the_joint_names: [";
      size_t pending_items = msg.the_joint_names.size();
      for (auto item : msg.the_joint_names) {
        rosidl_generator_traits::value_to_yaml(item, out);
        if (--pending_items > 0) {
          out << ", ";
        }
      }
      out << "]";
    }
    out << ", ";
  }

  // member: values
  {
    if (msg.values.size() == 0) {
      out << "values: []";
    } else {
      out << "values: [";
      size_t pending_items = msg.values.size();
      for (auto item : msg.values) {
        rosidl_generator_traits::value_to_yaml(item, out);
        if (--pending_items > 0) {
          out << ", ";
        }
      }
      out << "]";
    }
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const Controlnote & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: header
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "header:\n";
    to_block_style_yaml(msg.header, out, indentation + 2);
  }

  // member: the_joint_names
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.the_joint_names.size() == 0) {
      out << "the_joint_names: []\n";
    } else {
      out << "the_joint_names:\n";
      for (auto item : msg.the_joint_names) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "- ";
        rosidl_generator_traits::value_to_yaml(item, out);
        out << "\n";
      }
    }
  }

  // member: values
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.values.size() == 0) {
      out << "values: []\n";
    } else {
      out << "values:\n";
      for (auto item : msg.values) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "- ";
        rosidl_generator_traits::value_to_yaml(item, out);
        out << "\n";
      }
    }
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const Controlnote & msg, bool use_flow_style = false)
{
  std::ostringstream out;
  if (use_flow_style) {
    to_flow_style_yaml(msg, out);
  } else {
    to_block_style_yaml(msg, out);
  }
  return out.str();
}

}  // namespace msg

}  // namespace custom_interfaces

namespace rosidl_generator_traits
{

[[deprecated("use custom_interfaces::msg::to_block_style_yaml() instead")]]
inline void to_yaml(
  const custom_interfaces::msg::Controlnote & msg,
  std::ostream & out, size_t indentation = 0)
{
  custom_interfaces::msg::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use custom_interfaces::msg::to_yaml() instead")]]
inline std::string to_yaml(const custom_interfaces::msg::Controlnote & msg)
{
  return custom_interfaces::msg::to_yaml(msg);
}

template<>
inline const char * data_type<custom_interfaces::msg::Controlnote>()
{
  return "custom_interfaces::msg::Controlnote";
}

template<>
inline const char * name<custom_interfaces::msg::Controlnote>()
{
  return "custom_interfaces/msg/Controlnote";
}

template<>
struct has_fixed_size<custom_interfaces::msg::Controlnote>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<custom_interfaces::msg::Controlnote>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<custom_interfaces::msg::Controlnote>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // CUSTOM_INTERFACES__MSG__DETAIL__CONTROLNOTE__TRAITS_HPP_
