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
 * @file   rb_topology_filter.cc
 * @author Dominik Schatzmann <schadomi@gmail.com>
 * @date   October, 2012
 * @brief  Ruby C++ Wrapper for TopologyFilter.
 *
 * Ruby C++ Wrapper for TopologyFilter.
 *
 */

#include "rb_topology_filter.h"

void rb_topology_filter_free(void *p) {
  delete (TopologyFilter*) p;
  p = NULL;
}

VALUE rb_topology_filter_alloc(VALUE klass) {
  TopologyFilter* p = NULL;
  VALUE obj;
  p = new TopologyFilter();
  if (p == NULL)
    rb_raise(rb_path2class("FlowBox::Core::TopologyFilterError"),
             "Out of Memory");

  obj = Data_Wrap_Struct(klass, 0, rb_topology_filter_free, p);
  return (obj);
}

// configuration
VALUE rb_topology_filter_conf_push(VALUE self, VALUE configuration_h) {
  RB_TOPOLOGY_FILTER_UNWRAP
  TopologyFilter::Configuration conf_c;

  // input
  VALUE input = rb_hash_aref(configuration_h, ID2SYM(rb_intern("input")));
  if (TYPE(input) == T_DATA) {
    FlowContainerBuffer* input_buf_p;
    Data_Get_Struct(input, FlowContainerBuffer, input_buf_p);
    conf_c.set_input(input_buf_p);
  }

  // output
  VALUE output = rb_hash_aref(configuration_h, ID2SYM(rb_intern("output")));
  if (TYPE(output) == T_DATA) {
    FlowContainerBuffer* output_buf_p;
    Data_Get_Struct(output, FlowContainerBuffer, output_buf_p);
    conf_c.set_output(output_buf_p);
  }

  // stat_interval
  VALUE export_stat_interval = rb_hash_aref(configuration_h,
                                            ID2SYM(rb_intern("stat_interval")));
  Check_Type(export_stat_interval, T_FIXNUM);
  conf_c.set_stat_interval(NUM2INT(export_stat_interval));

  // interfaces
  VALUE interfaces = rb_hash_aref(configuration_h,
                                  ID2SYM(rb_intern("interfaces")));
  Check_Type(interfaces, T_ARRAY);
  int interfaces_len = RARRAY_LEN(interfaces);

  for (int offset = 0; offset < interfaces_len; offset++) {
    VALUE interface = rb_ary_entry(interfaces, offset);
    // a interface
    Check_Type(interface, T_HASH);

    VALUE export_device_id = rb_hash_aref(
        interface, ID2SYM(rb_intern("flow_exporter_id")));
    Check_Type(export_device_id, T_FIXNUM);

    VALUE snmp_if_id = rb_hash_aref(interface, ID2SYM(rb_intern("snmp_if_id")));
    Check_Type(snmp_if_id, T_FIXNUM);

    VALUE filter = rb_hash_aref(interface, ID2SYM(rb_intern("filter")));
    Check_Type(filter, T_FIXNUM);

    VALUE addr_expected = rb_hash_aref(interface,
                                       ID2SYM(rb_intern("addr_expected")));
    Check_Type(addr_expected, T_STRING);

    conf_c.add_interface(NUM2INT(export_device_id), NUM2INT(snmp_if_id),
                         NUM2INT(filter), RSTRING_PTR(addr_expected));
  }
  tf->conf_push(conf_c);
  return (self);
}

// observation
VALUE rb_topology_filter_obs_get_block(void* data) {
  TopologyFilter* tf = (TopologyFilter*) data;
  TopologyFilter::Observation* obs = new TopologyFilter::Observation();
  try {
    (*obs) = tf->obs_get();  // blocking
  } catch (...) {
    obs->set_valid(false);
  }
  return ((VALUE) obs);
}

VALUE rb_topology_filter_obs_get(VALUE self) {
  RB_TOPOLOGY_FILTER_UNWRAP
  VALUE observation = rb_thread_blocking_region(
      rb_topology_filter_obs_get_block, tf, RUBY_UBF_IO, 0);

  TopologyFilter::Observation* obs = (TopologyFilter::Observation*) observation;

  VALUE answer = Qnil;
  if (obs->get_valid() == true) {
    // convert c++ data to ruby data
    answer = rb_hash_new();
    rb_hash_aset(answer, ID2SYM(rb_intern("time_s")),
                 INT2NUM(obs->get_time_s()));
    rb_hash_aset(answer, ID2SYM(rb_intern("flow_exporter_id")),
                 INT2NUM(obs->get_export_device_id()));
    rb_hash_aset(answer, ID2SYM(rb_intern("snmp_if_id")),
                 INT2NUM(obs->get_snmp_if_id()));
    rb_hash_aset(answer, ID2SYM(rb_intern("state")), INT2NUM(obs->get_state()));
    rb_hash_aset(answer, ID2SYM(rb_intern("addr_observed")),
                 rb_str_new2((obs->get_observed_addrs().c_str())));
  }
  delete obs;
  return (answer);
}

// statistics
VALUE rb_topology_filter_stat_get_block(void* data) {
  TopologyFilter* tf = (TopologyFilter*) data;
  TopologyFilter::Statistics* stat = new TopologyFilter::Statistics();
  try {
    (*stat) = tf->stat_get();  // blocking
  } catch (...) {
    stat->set_valid(false);
  }
  return ((VALUE) stat);
}

VALUE rb_topology_filter_stat_get(VALUE self) {
  RB_TOPOLOGY_FILTER_UNWRAP
  VALUE statistic = rb_thread_blocking_region(rb_topology_filter_stat_get_block,
                                              tf, RUBY_UBF_IO, 0);
  TopologyFilter::Statistics* stat = (TopologyFilter::Statistics*) statistic;
  VALUE answer = Qnil;
  if (stat->get_valid() == true) {
    answer = rb_hash_new();
    rb_hash_aset(answer, ID2SYM(rb_intern("time_s")),
                 ULL2NUM(stat->get_time_s()));
    rb_hash_aset(answer, ID2SYM(rb_intern("duration_s")),
                 ULL2NUM(stat->get_duration_s()));
    rb_hash_aset(answer, ID2SYM(rb_intern("flows")),
                 ULL2NUM(stat->get_flows()));
    rb_hash_aset(answer, ID2SYM(rb_intern("flows_filtered")),
                 ULL2NUM(stat->get_flows_filtered()));
  }
  delete stat;
  return (answer);
}

// data
VALUE rb_topology_filter_data_process_block(void* data) {
  TopologyFilter* tf = (TopologyFilter*) data;
  tf->data_process();
  return (Qnil);
}

VALUE rb_topology_filter_data_process(VALUE self) {
  RB_TOPOLOGY_FILTER_UNWRAP
  VALUE statistic = rb_thread_blocking_region(
      rb_topology_filter_data_process_block, tf, RUBY_UBF_IO, 0);
  return (self);
}

