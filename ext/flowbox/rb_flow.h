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
 * @file   rb_flow.h
 * @author Dominik Schatzmann <schadomi@gmail.com>
 * @date   October, 2012
 * @brief  Ruby C++ Wrapper for Flow class.
 *
 * Ruby C++ Wrapper for Flow class.
 *
 */

#ifndef FLOWBOXRUBY_EXT_RB_FLOW_H
#define FLOWBOXRUBY_EXT_RB_FLOW_H

#include "ruby.h"
#include <flowbox/flow.h>

#define RB_FLOW_UNWRAP Flow* flow; Data_Get_Struct(self, Flow, flow);

void rb_flow_free(void *p);
VALUE rb_flow_alloc(VALUE klass);

// ACCESSOR: GETTER
VALUE rb_flow_getter_addr_src_s(VALUE self);
VALUE rb_flow_getter_addr_dst_s(VALUE self);
VALUE rb_flow_getter_addr_next_s(VALUE self);
VALUE rb_flow_getter_start_s(VALUE self);
VALUE rb_flow_getter_stop_s(VALUE self);
VALUE rb_flow_getter_sys_start_r(VALUE self);
VALUE rb_flow_getter_sys_stop_r(VALUE self);
VALUE rb_flow_getter_packets(VALUE self);
VALUE rb_flow_getter_bytes(VALUE self);
VALUE rb_flow_getter_valid(VALUE self);
VALUE rb_flow_getter_port_src(VALUE self);
VALUE rb_flow_getter_port_dst(VALUE self);
VALUE rb_flow_getter_protocol(VALUE self);
VALUE rb_flow_getter_export_device_id(VALUE self);
VALUE rb_flow_getter_if_in(VALUE self);
VALUE rb_flow_getter_if_out(VALUE self);
VALUE rb_flow_getter_direction(VALUE self);
VALUE rb_flow_getter_border(VALUE self);
VALUE rb_flow_getter_addr_length(VALUE self);

// ACCESSOR: SETTER
VALUE rb_flow_setter_addr_src_s(VALUE self, VALUE src_s);
VALUE rb_flow_setter_addr_dst_s(VALUE self, VALUE dst_s);
VALUE rb_flow_setter_addr_next_s(VALUE self, VALUE next_s);
VALUE rb_flow_setter_start_s(VALUE self, VALUE start_s);
VALUE rb_flow_setter_stop_s(VALUE self, VALUE stop_s);
VALUE rb_flow_setter_sys_start_r(VALUE self, VALUE sys_start_r);
VALUE rb_flow_setter_sys_stop_r(VALUE self, VALUE sys_stop_r);
VALUE rb_flow_setter_packets(VALUE self, VALUE packets);
VALUE rb_flow_setter_bytes(VALUE self, VALUE bytes);
VALUE rb_flow_setter_valid(VALUE self, VALUE valid);
VALUE rb_flow_setter_port_src(VALUE self, VALUE port_src);
VALUE rb_flow_setter_port_dst(VALUE self, VALUE port_dst);
VALUE rb_flow_setter_protocol(VALUE self, VALUE protocol);
VALUE rb_flow_setter_export_device_id(VALUE self, VALUE export_device_id);
VALUE rb_flow_setter_if_in(VALUE self, VALUE if_in);
VALUE rb_flow_setter_if_out(VALUE self, VALUE if_out);
VALUE rb_flow_setter_direction(VALUE self, VALUE direction);
VALUE rb_flow_setter_border(VALUE self, VALUE border);
VALUE rb_flow_setter_addr_length(VALUE self, VALUE addr_length);

// MODIFIER:
VALUE rb_flow_clear(VALUE self);
VALUE rb_flow_zero(VALUE self);

// DEBUG:
VALUE rb_flow_getter_memory_footprint(VALUE self);

#endif  // FLOWBOXRUBY_EXT_RB_FLOW_H
