// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from custom_interfaces:msg/Controlnote.idl
// generated code does not contain a copyright notice

#ifndef CUSTOM_INTERFACES__MSG__DETAIL__CONTROLNOTE__STRUCT_H_
#define CUSTOM_INTERFACES__MSG__DETAIL__CONTROLNOTE__STRUCT_H_

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
// Member 'the_joint_names'
#include "rosidl_runtime_c/string.h"
// Member 'values'
#include "rosidl_runtime_c/primitives_sequence.h"

/// Struct defined in msg/Controlnote in the package custom_interfaces.
typedef struct custom_interfaces__msg__Controlnote
{
  std_msgs__msg__Header header;
  rosidl_runtime_c__String__Sequence the_joint_names;
  rosidl_runtime_c__double__Sequence values;
} custom_interfaces__msg__Controlnote;

// Struct for a sequence of custom_interfaces__msg__Controlnote.
typedef struct custom_interfaces__msg__Controlnote__Sequence
{
  custom_interfaces__msg__Controlnote * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} custom_interfaces__msg__Controlnote__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // CUSTOM_INTERFACES__MSG__DETAIL__CONTROLNOTE__STRUCT_H_
