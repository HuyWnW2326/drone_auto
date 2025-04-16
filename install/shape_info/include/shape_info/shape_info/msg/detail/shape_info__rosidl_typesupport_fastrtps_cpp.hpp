// generated from rosidl_typesupport_fastrtps_cpp/resource/idl__rosidl_typesupport_fastrtps_cpp.hpp.em
// with input from shape_info:msg/ShapeInfo.idl
// generated code does not contain a copyright notice

#ifndef SHAPE_INFO__MSG__DETAIL__SHAPE_INFO__ROSIDL_TYPESUPPORT_FASTRTPS_CPP_HPP_
#define SHAPE_INFO__MSG__DETAIL__SHAPE_INFO__ROSIDL_TYPESUPPORT_FASTRTPS_CPP_HPP_

#include "rosidl_runtime_c/message_type_support_struct.h"
#include "rosidl_typesupport_interface/macros.h"
#include "shape_info/msg/rosidl_typesupport_fastrtps_cpp__visibility_control.h"
#include "shape_info/msg/detail/shape_info__struct.hpp"

#ifndef _WIN32
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wunused-parameter"
# ifdef __clang__
#  pragma clang diagnostic ignored "-Wdeprecated-register"
#  pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
# endif
#endif
#ifndef _WIN32
# pragma GCC diagnostic pop
#endif

#include "fastcdr/Cdr.h"

namespace shape_info
{

namespace msg
{

namespace typesupport_fastrtps_cpp
{

bool
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_shape_info
cdr_serialize(
  const shape_info::msg::ShapeInfo & ros_message,
  eprosima::fastcdr::Cdr & cdr);

bool
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_shape_info
cdr_deserialize(
  eprosima::fastcdr::Cdr & cdr,
  shape_info::msg::ShapeInfo & ros_message);

size_t
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_shape_info
get_serialized_size(
  const shape_info::msg::ShapeInfo & ros_message,
  size_t current_alignment);

size_t
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_shape_info
max_serialized_size_ShapeInfo(
  bool & full_bounded,
  bool & is_plain,
  size_t current_alignment);

}  // namespace typesupport_fastrtps_cpp

}  // namespace msg

}  // namespace shape_info

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_shape_info
const rosidl_message_type_support_t *
  ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_cpp, shape_info, msg, ShapeInfo)();

#ifdef __cplusplus
}
#endif

#endif  // SHAPE_INFO__MSG__DETAIL__SHAPE_INFO__ROSIDL_TYPESUPPORT_FASTRTPS_CPP_HPP_
