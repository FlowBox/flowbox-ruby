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
 * @file   rb_bi_flow_container_buffer.cc
 * @author Dominik Schatzmann <schadomi@gmail.com>
 * @author Daniel Aschwanden <nimdanitro@gmail.com>
 * @date   October, 2012
 * @brief  Ruby C++ Wrapper for BiFlowContainerBuffer.
 *
 * Ruby C++ Wrapper for BiFlowContainerBuffer.
 *
 */

#include "rb_bi_flow_container_buffer.h"

void rb_bi_flow_container_buffer_free(void *p) {
  delete (BiFlowContainerBuffer*) p;
  p = NULL;
}

VALUE rb_bi_flow_container_buffer_alloc(VALUE klass) {
  BiFlowContainerBuffer* p = NULL;
  VALUE obj;
  p = new BiFlowContainerBuffer();
  if (p == NULL)
    rb_raise(rb_path2class("FlowBox::Core::BiFlowContainerBuffer"),
             "Out of Memory");

  obj = Data_Wrap_Struct(klass, 0, rb_bi_flow_container_buffer_free, p);
  return (obj);
}

VALUE rb_bi_flow_container_buffer_pop(VALUE self) {
  RB_BI_FLOW_BUFFER_UNWRAP
  BiFlowContainer* biflow_container;
  biflow_container = biflow_container_buffer->pop();
  if (biflow_container != NULL) {
    VALUE klass_fc = rb_path2class("FlowBox::Core::BiFlowContainerBuffer");
    return (Data_Wrap_Struct(klass_fc, 0, 0, biflow_container));
  } else {
    return Qnil;
  }
}

VALUE rb_bi_flow_container_buffer_push(VALUE self, VALUE biflow) {
  if (biflow == Qnil)
    return (self);

  RB_BI_FLOW_BUFFER_UNWRAP
  BiFlowContainer* fc;
  Data_Get_Struct(biflow, BiFlowContainer, fc);
  biflow_container_buffer->push(fc);
  return (self);
}

