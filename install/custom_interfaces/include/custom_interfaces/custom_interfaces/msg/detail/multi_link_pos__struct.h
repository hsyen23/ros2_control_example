// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from custom_interfaces:msg/MultiLinkPos.idl
// generated code does not contain a copyright notice

#ifndef CUSTOM_INTERFACES__MSG__DETAIL__MULTI_LINK_POS__STRUCT_H_
#define CUSTOM_INTERFACES__MSG__DETAIL__MULTI_LINK_POS__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

// Include directives for member types
// Member 'header'
#include "std_msgs/msg/detail/header__struct.h"
// Member 'joint_names'
#include "rosidl_runtime_c/string.h"
// Member 'controllable'
// Member 'values'
#include "rosidl_runtime_c/primitives_sequence.h"

/// Struct defined in msg/MultiLinkPos in the package custom_interfaces.
typedef struct custom_interfaces__msg__MultiLinkPos
{
  std_msgs__msg__Header header;
  rosidl_runtime_c__String__Sequence joint_names;
  rosidl_runtime_c__boolean__Sequence controllable;
  rosidl_runtime_c__double__Sequence values;
} custom_interfaces__msg__MultiLinkPos;

// Struct for a sequence of custom_interfaces__msg__MultiLinkPos.
typedef struct custom_interfaces__msg__MultiLinkPos__Sequence
{
  custom_interfaces__msg__MultiLinkPos * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} custom_interfaces__msg__MultiLinkPos__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // CUSTOM_INTERFACES__MSG__DETAIL__MULTI_LINK_POS__STRUCT_H_
