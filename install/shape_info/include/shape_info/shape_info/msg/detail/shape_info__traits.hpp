// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from shape_info:msg/ShapeInfo.idl
// generated code does not contain a copyright notice

#ifndef SHAPE_INFO__MSG__DETAIL__SHAPE_INFO__TRAITS_HPP_
#define SHAPE_INFO__MSG__DETAIL__SHAPE_INFO__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "shape_info/msg/detail/shape_info__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

namespace shape_info
{

namespace msg
{

inline void to_flow_style_yaml(
  const ShapeInfo & msg,
  std::ostream & out)
{
  out << "{";
  // member: shape_id
  {
    out << "shape_id: ";
    rosidl_generator_traits::value_to_yaml(msg.shape_id, out);
    out << ", ";
  }

  // member: x
  {
    out << "x: ";
    rosidl_generator_traits::value_to_yaml(msg.x, out);
    out << ", ";
  }

  // member: y
  {
    out << "y: ";
    rosidl_generator_traits::value_to_yaml(msg.y, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const ShapeInfo & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: shape_id
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "shape_id: ";
    rosidl_generator_traits::value_to_yaml(msg.shape_id, out);
    out << "\n";
  }

  // member: x
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "x: ";
    rosidl_generator_traits::value_to_yaml(msg.x, out);
    out << "\n";
  }

  // member: y
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "y: ";
    rosidl_generator_traits::value_to_yaml(msg.y, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const ShapeInfo & msg, bool use_flow_style = false)
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

}  // namespace shape_info

namespace rosidl_generator_traits
{

[[deprecated("use shape_info::msg::to_block_style_yaml() instead")]]
inline void to_yaml(
  const shape_info::msg::ShapeInfo & msg,
  std::ostream & out, size_t indentation = 0)
{
  shape_info::msg::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use shape_info::msg::to_yaml() instead")]]
inline std::string to_yaml(const shape_info::msg::ShapeInfo & msg)
{
  return shape_info::msg::to_yaml(msg);
}

template<>
inline const char * data_type<shape_info::msg::ShapeInfo>()
{
  return "shape_info::msg::ShapeInfo";
}

template<>
inline const char * name<shape_info::msg::ShapeInfo>()
{
  return "shape_info/msg/ShapeInfo";
}

template<>
struct has_fixed_size<shape_info::msg::ShapeInfo>
  : std::integral_constant<bool, true> {};

template<>
struct has_bounded_size<shape_info::msg::ShapeInfo>
  : std::integral_constant<bool, true> {};

template<>
struct is_message<shape_info::msg::ShapeInfo>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // SHAPE_INFO__MSG__DETAIL__SHAPE_INFO__TRAITS_HPP_
