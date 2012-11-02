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
 * @file   rb_flow_container_pool.cc
 * @author Dominik Schatzmann <schadomi@gmail.com>
 * @date   October, 2012
 * @brief  Ruby C++ Wrapper for FlowContainerPool.
 *
 * Ruby C++ Wrapper for FlowContainerPool.
 *
 */

#include "rb_flow_container_pool.h"

void rb_flow_container_pool_free(void *p) {
  // delete (FlowContainerPool*) p;
  // we don't delete this data, it's a singleton
  p = NULL;
}

VALUE rb_flow_container_pool_alloc(VALUE klass) {
  FlowContainerPool* p = NULL;
  VALUE obj;
  p = FlowContainerPool::instance();  // use a instance, instead a new
  if (p == NULL)
    rb_raise(rb_path2class("FlowBox::Core::FlowContainerPoolError"),
             "Out of Memory");

  obj = Data_Wrap_Struct(klass, 0, rb_flow_container_pool_free, p);
  return (obj);
}

VALUE rb_flow_container_pool_pop(VALUE self) {
  RB_FLOW_CONTAINER_POOL_UNWRAP
  FlowContainer* fc;
  fc = fcp->pop();
  if (fc != NULL) {
    VALUE klass_fc = rb_path2class("FlowBox::Core::FlowContainer");
    return (Data_Wrap_Struct(klass_fc, 0, 0, fc));
  } else {
    return Qnil;
  }
}

VALUE rb_flow_container_pool_push(VALUE self, VALUE flow_container) {
  if (flow_container == Qnil)
    return (self);

  RB_FLOW_CONTAINER_POOL_UNWRAP
  FlowContainer* fc;
  Data_Get_Struct(flow_container, FlowContainer, fc);
  fcp->push(fc);
  return (self);
}

VALUE rb_flow_container_pool_set_capacity(VALUE self, VALUE capacity) {
  RB_FLOW_CONTAINER_POOL_UNWRAP
  fcp->set_capacity(NUM2INT(capacity));
  return (self);
}

VALUE rb_flow_container_pool_get_stat(VALUE self) {
  RB_FLOW_CONTAINER_POOL_UNWRAP
  VALUE array = rb_ary_new();
  rb_ary_push(array, INT2NUM(fcp->stat_pop()));
  rb_ary_push(array, INT2NUM(fcp->stat_push()));
  rb_ary_push(array, INT2NUM(fcp->stat_min()));
  rb_ary_push(array, INT2NUM(fcp->stat_max()));
  rb_ary_push(array, INT2NUM(fcp->stat_used()));
  rb_ary_push(array, INT2NUM(fcp->stat_capacity()));
  fcp->stat_reset();
  return (array);
}
