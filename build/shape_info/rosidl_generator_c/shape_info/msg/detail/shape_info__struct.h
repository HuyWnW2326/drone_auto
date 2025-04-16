// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from shape_info:msg/ShapeInfo.idl
// generated code does not contain a copyright notice

#ifndef SHAPE_INFO__MSG__DETAIL__SHAPE_INFO__STRUCT_H_
#define SHAPE_INFO__MSG__DETAIL__SHAPE_INFO__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

/// Struct defined in msg/ShapeInfo in the package shape_info.
typedef struct shape_info__msg__ShapeInfo
{
  int32_t shape_id;
  double x;
  double y;
} shape_info__msg__ShapeInfo;

// Struct for a sequence of shape_info__msg__ShapeInfo.
typedef struct shape_info__msg__ShapeInfo__Sequence
{
  shape_info__msg__ShapeInfo * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} shape_info__msg__ShapeInfo__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // SHAPE_INFO__MSG__DETAIL__SHAPE_INFO__STRUCT_H_
