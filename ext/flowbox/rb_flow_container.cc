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
 * @file   rb_flow_container.cc
 * @author Dominik Schatzmann <schadomi@gmail.com>
 * @date   October, 2012
 * @brief  Ruby C++ Wrapper for FlowContainer.
 *
 * Ruby C++ Wrapper for FlowContainer.
 *
 */

#include "rb_flow_container.h"

/**
 * Clear the container
 * @param p
 */
void rb_flow_container_free(void *p) {
  delete (FlowContainer*) p;
  p = NULL;
}

/**
 * Makes a container for Ruby
 * @param klass
 * @return a wrap of a Connections pointer
 */
VALUE rb_flow_container_alloc(VALUE klass) {
  FlowContainer* p = NULL;
  VALUE obj;
  p = new FlowContainer();
  if (p == NULL)
    rb_raise(rb_path2class("FlowBox::Core::FlowContainerError"),
             "Out of Memory");

  obj = Data_Wrap_Struct(klass, 0, rb_flow_container_free, p);
  return (obj);
}

/**
 * Resize the connections container size
 * @param self
 * @param size the size to use
 * @return
 */
VALUE rb_flow_container_resize(VALUE self, VALUE size) {
  RB_FLOW_CONTAINER_UNWRAP
  int size_ = NUM2INT(size);
  flows->resize(size_);
  return (self);
}

/**
 * Returns the capacity of the connections container
 * @param self
 * @return the capacity of the connections container
 */
VALUE rb_flow_container_capacity(VALUE self) {
  RB_FLOW_CONTAINER_UNWRAP
  return (INT2NUM(flows->capacity()));
}

/**update_used_by(int flows_to_add)
 * Returns the number of used connections in this container
 * @param self
 * @return the number of used connections in this container
 */
VALUE rb_flow_container_used(VALUE self) {
  RB_FLOW_CONTAINER_UNWRAP
  return (INT2NUM(flows->used()));
}

/**
 * Resets the connection container
 * @param self
 * @return
 */
VALUE rb_flow_container_reset(VALUE self) {
  RB_FLOW_CONTAINER_UNWRAP
  flows->reset();
  return (self);
}

/**
 * Calls yield with every used connection in the container. The yield value is of
 * type Connection
 * @param self
 * @return
 */
VALUE rb_flow_container_each_used(VALUE self) {
  RB_FLOW_CONTAINER_UNWRAP
  VALUE klass_flow = rb_path2class("FlowBox::Core::Flow");
  FlowContainer::iterator iter(flows->begin());
  FlowContainer::iterator end(flows->end_used());
  // Todo: use each from std 
  while (iter != end) {
    rb_yield(Data_Wrap_Struct(klass_flow, 0, 0, &(*iter)));
    iter++;
  };
  return (self);
}

/**
 * Calls yield with every connection in the container (Even if unused). The yield value is of
 * type Connection
 * @param self
 * @return
 */
VALUE rb_flow_container_each_all(VALUE self) {
  RB_FLOW_CONTAINER_UNWRAP
  VALUE klass_flow = rb_path2class("FlowBox::Core::Flow");
  FlowContainer::iterator iter(flows->begin());
  FlowContainer::iterator end(flows->end());
  // Todo: use each from std 
  while (iter != end) {
    rb_yield(Data_Wrap_Struct(klass_flow, 0, 0, &(*iter)));
    iter++;
  };
  return (self);
}

VALUE rb_flow_container_update_used_by(VALUE self, VALUE flows_to_add) {
  RB_FLOW_CONTAINER_UNWRAP
  flows->update_used_by(NUM2INT(flows_to_add));
  return (self);
}

