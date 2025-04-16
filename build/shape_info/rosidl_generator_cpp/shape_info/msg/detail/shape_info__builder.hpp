// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from shape_info:msg/ShapeInfo.idl
// generated code does not contain a copyright notice

#ifndef SHAPE_INFO__MSG__DETAIL__SHAPE_INFO__BUILDER_HPP_
#define SHAPE_INFO__MSG__DETAIL__SHAPE_INFO__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "shape_info/msg/detail/shape_info__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace shape_info
{

namespace msg
{

namespace builder
{

class Init_ShapeInfo_y
{
public:
  explicit Init_ShapeInfo_y(::shape_info::msg::ShapeInfo & msg)
  : msg_(msg)
  {}
  ::shape_info::msg::ShapeInfo y(::shape_info::msg::ShapeInfo::_y_type arg)
  {
    msg_.y = std::move(arg);
    return std::move(msg_);
  }

private:
  ::shape_info::msg::ShapeInfo msg_;
};

class Init_ShapeInfo_x
{
public:
  explicit Init_ShapeInfo_x(::shape_info::msg::ShapeInfo & msg)
  : msg_(msg)
  {}
  Init_ShapeInfo_y x(::shape_info::msg::ShapeInfo::_x_type arg)
  {
    msg_.x = std::move(arg);
    return Init_ShapeInfo_y(msg_);
  }

private:
  ::shape_info::msg::ShapeInfo msg_;
};

class Init_ShapeInfo_shape_id
{
public:
  Init_ShapeInfo_shape_id()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_ShapeInfo_x shape_id(::shape_info::msg::ShapeInfo::_shape_id_type arg)
  {
    msg_.shape_id = std::move(arg);
    return Init_ShapeInfo_x(msg_);
  }

private:
  ::shape_info::msg::ShapeInfo msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::shape_info::msg::ShapeInfo>()
{
  return shape_info::msg::builder::Init_ShapeInfo_shape_id();
}

}  // namespace shape_info

#endif  // SHAPE_INFO__MSG__DETAIL__SHAPE_INFO__BUILDER_HPP_
