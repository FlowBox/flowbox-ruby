// Copyright (C) 2011-12 Dominik Schatzmann <schadomi@gmail.com>
// This file is part of FlowBox. FlowBox is free software: you can redistribute
// it and/or modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation, either version 3
// of the License, or (at your option) any later version.
//
// FlowBox is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with FlowBox. If not, see <http://www.gnu.org/licenses/>.

/**
 * @file   rb_flow_container_buffer.cc
 * @author Dominik Schatzmann <schadomi@gmail.com>
 * @date   October, 2012
 * @brief   Ruby C++ Wrapper for FlowContainerBuffer.
 *
 * Ruby C++ Wrapper for FlowContainerBuffer.
 *
 */

#include "rb_flow_container_buffer.h"

void rb_flow_container_buffer_free(void *p) {
  delete (FlowContainerBuffer*) p;
  p = NULL;
}

VALUE rb_flow_container_buffer_alloc(VALUE klass) {
  FlowContainerBuffer* p = NULL;
  VALUE obj;
  p = new FlowContainerBuffer();
  if (p == NULL)
    rb_raise(rb_path2class("FlowBox::Core::FlowContainerBufferError"),
             "Out of Memory");

  obj = Data_Wrap_Struct(klass, 0, rb_flow_container_buffer_free, p);
  return (obj);
}

VALUE rb_flow_container_buffer_pop(VALUE self) {
  RB_FLOW_CONTAINER_BUFFER_UNWRAP
  FlowContainer* fc;
  fc = fcb->pop();
  if (fc != NULL) {
    VALUE klass_fc = rb_path2class("FlowBox::Core::FlowContainer");
    return (Data_Wrap_Struct(klass_fc, 0, 0, fc));
  } else {
    return Qnil;
  }
}

VALUE rb_flow_container_buffer_push(VALUE self, VALUE flow_container) {
  if (flow_container == Qnil)
    return (self);

  RB_FLOW_CONTAINER_BUFFER_UNWRAP
  FlowContainer* fc;
  Data_Get_Struct(flow_container, FlowContainer, fc);
  fcb->push(fc);
  return (self);
}

