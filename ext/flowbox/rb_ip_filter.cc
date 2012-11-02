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
 * @file   rb_ip_filter.cc
 * @author Dominik Schatzmann <schadomi@gmail.com>
 * @date   October, 2012
 * @brief  Ruby C++ Wrapper for IPFilter.
 *
 * Ruby C++ Wrapper for IPFilter.
 *
 */

#include "rb_ip_filter.h"

void rb_ip_filter_free(void *p) {
  delete (IPFilter*) p;
  p = NULL;
}

VALUE rb_ip_filter_alloc(VALUE klass) {
  IPFilter* p = NULL;
  VALUE obj;
  p = new IPFilter();
  if (p == NULL)
    rb_raise(rb_path2class("FlowBox::Core::IPFilterError"), "Out of Memory");

  obj = Data_Wrap_Struct(klass, 0, rb_ip_filter_free, p);
  return (obj);
}

// configuration
VALUE rb_ip_filter_conf_push(VALUE self, VALUE configuration_h) {
  RB_IP_FILTER_UNWRAP
  IPFilter::Configuration conf_c;

  // input
  VALUE input = rb_hash_aref(configuration_h, ID2SYM(rb_intern("input")));
  if (TYPE(input) == T_DATA) {
    FlowContainerBuffer* input_buf_p;
    Data_Get_Struct(input, FlowContainerBuffer, input_buf_p);
    conf_c.set_input(input_buf_p);
  };

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

  // prefixes
  VALUE prefixes = rb_hash_aref(configuration_h, ID2SYM(rb_intern("prefixes")));
  Check_Type(prefixes, T_ARRAY);
  int prefixes_len = RARRAY_LEN(prefixes);
  for (int offset = 0; offset < prefixes_len; offset++) {
    // a prefix
    VALUE prefix = rb_ary_entry(prefixes, offset);
    Check_Type(prefix, T_STRING);
    conf_c.add_prefix(RSTRING_PTR(prefix));
  }

  // policy
  VALUE default_policy = rb_hash_aref(configuration_h,
                                      ID2SYM(rb_intern("default_policy")));
  Check_Type(default_policy, T_FIXNUM);
  if (NUM2INT(default_policy) == IPFilter::kKeep)
    conf_c.set_default_policy(IPFilter::kKeep);
  else if (NUM2INT(default_policy) == IPFilter::kFilter)
    conf_c.set_default_policy(IPFilter::kFilter);
  else
    rb_raise(rb_path2class("FlowBox::Core::IPFilterError"),
             "Unknown Policy Type");

  bf->conf_push(conf_c);
  return (self);
}

// observation
VALUE rb_ip_filter_obs_get_block(void* data) {

  IPFilter* bf = (IPFilter*) data;
  IPFilter::Observation* obs = new IPFilter::Observation();
  try {
    (*obs) = bf->obs_get();  // blocking
  } catch (...) {
    obs->set_valid(false);
  }
  return ((VALUE) obs);
}

VALUE rb_ip_filter_obs_get(VALUE self) {
  RB_IP_FILTER_UNWRAP
  VALUE observation = rb_thread_blocking_region(rb_ip_filter_obs_get_block, bf,
                                                RUBY_UBF_IO, 0);
  IPFilter::Observation* obs = (IPFilter::Observation*) observation;
  VALUE answer = Qnil;
  if (obs->get_valid() == true) {
    answer = rb_hash_new();
    rb_hash_aset(answer, ID2SYM(rb_intern("time_s")),
                 ULL2NUM(obs->get_time_s()));
    rb_hash_aset(answer, ID2SYM(rb_intern("msg")),
                 rb_str_new2((obs->get_message()).c_str()));
  }
  delete obs;
  return (answer);
}

// statistics
VALUE rb_ip_filter_stat_get_block(void* data) {
  IPFilter* bf = (IPFilter*) data;
  IPFilter::Statistics* stat = new IPFilter::Statistics();
  try {
    (*stat) = bf->stat_get();  // blocking
  } catch (...) {
    stat->set_valid(false);
  }
  return ((VALUE) stat);
}

VALUE rb_ip_filter_stat_get(VALUE self) {
  RB_IP_FILTER_UNWRAP
  VALUE statistic = rb_thread_blocking_region(rb_ip_filter_stat_get_block, bf,
                                              RUBY_UBF_IO, 0);
  IPFilter::Statistics* stat = (IPFilter::Statistics*) statistic;
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
VALUE rb_ip_filter_data_process_block(void* data) {
  IPFilter* bf = (IPFilter*) data;
  bf->data_process();
  return (Qnil);
}

VALUE rb_ip_filter_data_process(VALUE self) {
  RB_IP_FILTER_UNWRAP
  rb_thread_blocking_region(rb_ip_filter_data_process_block, bf, RUBY_UBF_IO,
                            0);
  return (self);
}

