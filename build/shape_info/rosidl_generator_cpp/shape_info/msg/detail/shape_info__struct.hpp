// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from shape_info:msg/ShapeInfo.idl
// generated code does not contain a copyright notice

#ifndef SHAPE_INFO__MSG__DETAIL__SHAPE_INFO__STRUCT_HPP_
#define SHAPE_INFO__MSG__DETAIL__SHAPE_INFO__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


#ifndef _WIN32
# define DEPRECATED__shape_info__msg__ShapeInfo __attribute__((deprecated))
#else
# define DEPRECATED__shape_info__msg__ShapeInfo __declspec(deprecated)
#endif

namespace shape_info
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct ShapeInfo_
{
  using Type = ShapeInfo_<ContainerAllocator>;

  explicit ShapeInfo_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->shape_id = 0l;
      this->x = 0.0;
      this->y = 0.0;
    }
  }

  explicit ShapeInfo_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_alloc;
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->shape_id = 0l;
      this->x = 0.0;
      this->y = 0.0;
    }
  }

  // field types and members
  using _shape_id_type =
    int32_t;
  _shape_id_type shape_id;
  using _x_type =
    double;
  _x_type x;
  using _y_type =
    double;
  _y_type y;

  // setters for named parameter idiom
  Type & set__shape_id(
    const int32_t & _arg)
  {
    this->shape_id = _arg;
    return *this;
  }
  Type & set__x(
    const double & _arg)
  {
    this->x = _arg;
    return *this;
  }
  Type & set__y(
    const double & _arg)
  {
    this->y = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    shape_info::msg::ShapeInfo_<ContainerAllocator> *;
  using ConstRawPtr =
    const shape_info::msg::ShapeInfo_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<shape_info::msg::ShapeInfo_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<shape_info::msg::ShapeInfo_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      shape_info::msg::ShapeInfo_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<shape_info::msg::ShapeInfo_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      shape_info::msg::ShapeInfo_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<shape_info::msg::ShapeInfo_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<shape_info::msg::ShapeInfo_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<shape_info::msg::ShapeInfo_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__shape_info__msg__ShapeInfo
    std::shared_ptr<shape_info::msg::ShapeInfo_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__shape_info__msg__ShapeInfo
    std::shared_ptr<shape_info::msg::ShapeInfo_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const ShapeInfo_ & other) const
  {
    if (this->shape_id != other.shape_id) {
      return false;
    }
    if (this->x != other.x) {
      return false;
    }
    if (this->y != other.y) {
      return false;
    }
    return true;
  }
  bool operator!=(const ShapeInfo_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct ShapeInfo_

// alias to use template instance with default allocator
using ShapeInfo =
  shape_info::msg::ShapeInfo_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace shape_info

#endif  // SHAPE_INFO__MSG__DETAIL__SHAPE_INFO__STRUCT_HPP_
