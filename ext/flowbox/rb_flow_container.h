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
 * @file   rb_flow_container.h
 * @author Dominik Schatzmann <schadomi@gmail.com>
 * @date   October, 2012
 * @brief  Ruby C++ Wrapper for FlowContainer.
 *
 * Ruby C++ Wrapper for FlowContainer.
 *
 */

#ifndef FLOWBOXRUBY_EXT_RB_FLOW_CONTAINER_H_
#define FLOWBOXRUBY_EXT_RB_FLOW_CONTAINER_H_

#include "ruby.h"
#include <flowbox/flow_container.h>

#define RB_FLOW_CONTAINER_UNWRAP FlowContainer* flows; Data_Get_Struct(self, FlowContainer, flows);

void rb_flow_container_free(void *p);
VALUE rb_flow_container_alloc(VALUE klass);
VALUE rb_flow_container_resize(VALUE self, VALUE size);
VALUE rb_flow_container_capacity(VALUE self);
VALUE rb_flow_container_used(VALUE self);
VALUE rb_flow_container_reset(VALUE self);
VALUE rb_flow_container_each_used(VALUE self);
VALUE rb_flow_container_each_all(VALUE self);

VALUE rb_flow_container_update_used_by(VALUE self, VALUE flows_to_add);

#endif  // FLOWBOXRUBY_EXT_RB_FLOW_CONTAINER_H_
