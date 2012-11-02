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
 * @file   rb_bi_flow_container.cc
 * @author Dominik Schatzmann <schadomi@gmail.com>
 * @author Daniel Aschwanden <nimdanitro@gmail.com>
 * @date   October, 2012
 * @brief  Ruby C++ Wrapper for BiFlowContainer.
 *
 * Ruby C++ Wrapper for BiFlowContainer.
 *
 */

#include "rb_bi_flow_container.h"

/**
 * Clear the container
 * @param p
 */
void rb_bi_flow_container_free(void *p) {
  BiFlowContainerPool* bfc_pool = BiFlowContainerPool::instance();
  bfc_pool->push((BiFlowContainer*) p);
  p = NULL;
}


/**
 * Makes a biflow container for Ruby
 * @param klass
 * @return a wrap of a Connections pointer
 */
VALUE rb_bi_flow_container_alloc(VALUE klass) {
  BiFlowContainer* p = NULL;
  VALUE obj;
  BiFlowContainerPool* bfc_pool = BiFlowContainerPool::instance();
  p = bfc_pool->pop();
  if (p == NULL)
    rb_raise(rb_path2class("FlowBox::Core::BiFlowContainerError"),
             "Out of Memory");

  obj = Data_Wrap_Struct(klass, 0, rb_bi_flow_container_free, p);
  return (obj);
}


/**
 * Calls yield with every valid biflow in the container. The yield value is of
 * type Biflow
 * @param self
 * @return
 */
VALUE rb_bi_flow_container_each_valid_biflow(VALUE self) {
  RB_BI_FLOW_CONTAINER_UNWRAP
  VALUE klass_biflow = rb_path2class("FlowBox::Core::BiFlow");
  BiFlow* iter = bfc->biflows();

  // Todo: use each from std
  while (iter != NULL) {
    if (iter->valid_ == true)
      rb_yield(Data_Wrap_Struct(klass_biflow, 0, 0, &(*iter)));
    iter = iter->next_;
  }
  return (self);
}

/**
 * Calls yield with every biflow in the container (Even if invalid). The yield value is of
 * type BiFlow
 * @param self
 * @return
 */
VALUE rb_bi_flow_container_each_biflow(VALUE self) {
  RB_BI_FLOW_CONTAINER_UNWRAP
  VALUE klass_biflow = rb_path2class("FlowBox::Core::BiFlow");
  BiFlow* iter = bfc->biflows();

  // Todo: use each from std
  while (iter != NULL) {
    rb_yield(Data_Wrap_Struct(klass_biflow, 0, 0, &(*iter)));
    iter = iter->next_;
  }
  return (self);
}

VALUE rb_bi_flow_container_time(VALUE self) {
  RB_BI_FLOW_CONTAINER_UNWRAP
  return (ULL2NUM(bfc->time()));
}
