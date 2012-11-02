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
 * @file   rb_bi_flow.h
 * @author Dominik Schatzmann <schadomi@gmail.com>
 * @author Daniel Aschwanden <nimdanitro@gmail.com>
 * @date   October, 2012
 * @brief   Ruby C++ Wrapper for BiFlow class.
 *
 * Ruby C++ Wrapper for BiFlow class.
 *
 */

#ifndef FLOWBOXRUBY_EXT_RB_BI_FLOW_H_
#define FLOWBOXRUBY_EXT_RB_BI_FLOW_H_

#include "ruby.h"
#include <flowbox/bi_flow.h>

#define RB_BI_FLOW_UNWRAP BiFlow* biflow; Data_Get_Struct(self, BiFlow, biflow);

void rb_bi_flow_free(void *p);
VALUE rb_bi_flow_alloc(VALUE klass);

// ACCESSOR: GETTER
VALUE rb_bi_flow_getter_in_addr_s(VALUE self);
VALUE rb_bi_flow_getter_out_addr_s(VALUE self);
VALUE rb_bi_flow_getter_next_in_addr_s(VALUE self);
VALUE rb_bi_flow_getter_next_out_addr_s(VALUE self);

VALUE rb_bi_flow_getter_in_out_start_s(VALUE self);
VALUE rb_bi_flow_getter_in_out_stop_s(VALUE self);
VALUE rb_bi_flow_getter_out_in_start_s(VALUE self);
VALUE rb_bi_flow_getter_out_in_stop_s(VALUE self);

VALUE rb_bi_flow_getter_in_out_packets(VALUE self);
VALUE rb_bi_flow_getter_in_out_bytes(VALUE self);
VALUE rb_bi_flow_getter_out_in_packets(VALUE self);
VALUE rb_bi_flow_getter_out_in_bytes(VALUE self);

VALUE rb_bi_flow_getter_valid(VALUE self);

VALUE rb_bi_flow_getter_in_port(VALUE self);
VALUE rb_bi_flow_getter_out_port(VALUE self);
VALUE rb_bi_flow_getter_protocol(VALUE self);
VALUE rb_bi_flow_getter_in_out_export_device_id(VALUE self);
VALUE rb_bi_flow_getter_out_in_export_device_id(VALUE self);
VALUE rb_bi_flow_getter_in_out_if(VALUE self);
VALUE rb_bi_flow_getter_out_in_if(VALUE self);
VALUE rb_bi_flow_getter_addr_length(VALUE self);

VALUE rb_bi_flow_getter_hkey5(VALUE self);
VALUE rb_bi_flow_getter_hkey3_in(VALUE self);
VALUE rb_bi_flow_getter_hkey3_out(VALUE self);
VALUE rb_bi_flow_getter_hkey1_in(VALUE self);
VALUE rb_bi_flow_getter_hkey1_out(VALUE self);

// ACCESSOR: SETTER
VALUE rb_bi_flow_setter_in_addr_s(VALUE self, VALUE src_s);
VALUE rb_bi_flow_setter_out_addr_s(VALUE self, VALUE dst_s);
VALUE rb_bi_flow_setter_next_in_addr_s(VALUE self, VALUE next_s);
VALUE rb_bi_flow_setter_next_out_addr_s(VALUE self, VALUE next_s);
VALUE rb_bi_flow_setter_in_out_start_s(VALUE self, VALUE start_s);
VALUE rb_bi_flow_setter_in_out_stop_s(VALUE self, VALUE stop_s);
VALUE rb_bi_flow_setter_out_in_start_s(VALUE self, VALUE start_s);
VALUE rb_bi_flow_setter_out_in_stop_s(VALUE self, VALUE start_s);

VALUE rb_bi_flow_setter_in_out_packets(VALUE self, VALUE packets);
VALUE rb_bi_flow_setter_in_out_bytes(VALUE self, VALUE bytes);
VALUE rb_bi_flow_setter_out_in_packets(VALUE self, VALUE packtes);
VALUE rb_bi_flow_setter_out_in_bytes(VALUE self, VALUE bytes);

VALUE rb_bi_flow_setter_valid(VALUE self, VALUE valid);

VALUE rb_bi_flow_setter_in_port(VALUE self, VALUE port);
VALUE rb_bi_flow_setter_out_port(VALUE self, VALUE port);
VALUE rb_bi_flow_setter_protocol(VALUE self, VALUE protocol);
VALUE rb_bi_flow_setter_in_out_export_device_id(VALUE self,
                                                VALUE export_device_id);
VALUE rb_bi_flow_setter_out_in_export_device_id(VALUE self,
                                                VALUE export_device_id);
VALUE rb_bi_flow_setter_in_out_if(VALUE self, VALUE iface);
VALUE rb_bi_flow_setter_out_in_if(VALUE self, VALUE iface);
VALUE rb_bi_flow_setter_addr_length(VALUE self, VALUE addr_length);

// MODIFIER:
VALUE rb_bi_flow_reset(VALUE self);
VALUE rb_bi_flow_prepare_hashes(VALUE self);

#endif  // FLOWBOXRUBY_EXT_RB_BI_FLOW_H_
