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
 * @file   rb_border_filter.cc
 * @author Dominik Schatzmann <schadomi@gmail.com>
 * @date   October, 2012
 * @brief   Ruby C++ Wrapper for BorderFilter.
 *
 * Ruby C++ Wrapper for BorderFilter.
 *
 */

#include "rb_border_filter.h"
void rb_border_filter_free(void *p) {
  delete (BorderFilter*) p;
  p = NULL;
}

VALUE rb_border_filter_alloc(VALUE klass) {
  BorderFilter* p = NULL;
  VALUE obj;
  p = new BorderFilter();
  if (p == NULL)
    rb_raise(rb_path2class("FlowBox::Core::BorderFilterError"),
             "Out of Memory");

  obj = Data_Wrap_Struct(klass, 0, rb_border_filter_free, p);
  return (obj);
}

// configuration
VALUE rb_border_filter_conf_push(VALUE self, VALUE configuration_h) {
  RB_BORDER_FILTER_UNWRAP
  BorderFilter::Configuration conf_c;

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
  VALUE interfaces = rb_hash_aref(configuration_h,
                                  ID2SYM(rb_intern("interfaces")));
  Check_Type(interfaces, T_ARRAY);
  int interfaces_len = RARRAY_LEN(interfaces);
  for (int offset = 0; offset < interfaces_len; offset++) {

    // a interface
    VALUE interface = rb_ary_entry(interfaces, offset);
    Check_Type(interface, T_HASH);
    BorderFilter::Interface interface_c;

    VALUE export_device_id = rb_hash_aref(
        interface, ID2SYM(rb_intern("export_device_id")));
    Check_Type(export_device_id, T_FIXNUM);
    interface_c.export_device_id_ = NUM2INT(export_device_id);

    VALUE interface_id = rb_hash_aref(interface,
                                      ID2SYM(rb_intern("interface_id")));
    Check_Type(interface_id, T_FIXNUM);
    interface_c.interface_id_ = NUM2INT(interface_id);

    VALUE purpose = rb_hash_aref(interface, ID2SYM(rb_intern("purpose")));
    Check_Type(purpose, T_FIXNUM);
    if (NUM2INT(purpose) == BorderFilter::kOther)
      interface_c.purpose_ = BorderFilter::kOther;
    else if (NUM2INT(purpose) == BorderFilter::kBorder)
      interface_c.purpose_ = BorderFilter::kBorder;
    else
      rb_raise(rb_path2class("FlowBox::Core::BorderFilterError"),
               "Unknown Purpose Type");

    conf_c.add_interface(interface_c);
  }

  // policy
  VALUE policy = rb_hash_aref(configuration_h, ID2SYM(rb_intern("policy")));
  Check_Type(policy, T_FIXNUM);
  if (NUM2INT(policy) == BorderFilter::kTag)
    conf_c.set_policy(BorderFilter::kTag);
  else if (NUM2INT(policy) == BorderFilter::kFilter)
    conf_c.set_policy(BorderFilter::kFilter);
  else
    rb_raise(rb_path2class("FlowBox::Core::BorderFilterError"),
             "Unknown Policy Type");

  bf->conf_push(conf_c);
  return (self);
}

// observation
VALUE rb_border_filter_obs_get_block(void* data) {

  BorderFilter* bf = (BorderFilter*) data;
  BorderFilter::Observation* obs = new BorderFilter::Observation();
  try {
    (*obs) = bf->obs_get();  // blocking
  } catch (...) {
    obs->set_valid(false);
  }
  return ((VALUE) obs);
}
VALUE rb_border_filter_obs_get(VALUE self) {
  RB_BORDER_FILTER_UNWRAP
  VALUE observation = rb_thread_blocking_region(rb_border_filter_obs_get_block,
                                                bf, RUBY_UBF_IO, 0);
  BorderFilter::Observation* obs = (BorderFilter::Observation*) observation;
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
VALUE rb_border_filter_stat_get_block(void* data) {
  BorderFilter* bf = (BorderFilter*) data;
  BorderFilter::Statistics* stat = new BorderFilter::Statistics();
  try {
    (*stat) = bf->stat_get();  // blocking
  } catch (...) {
    stat->set_valid(false);
  }
  return ((VALUE) stat);
}

VALUE rb_border_filter_stat_get(VALUE self) {
  RB_BORDER_FILTER_UNWRAP
  VALUE statistic = rb_thread_blocking_region(rb_border_filter_stat_get_block,
                                              bf, RUBY_UBF_IO, 0);
  BorderFilter::Statistics* stat = (BorderFilter::Statistics*) statistic;
  VALUE answer = Qnil;
  if (stat->get_valid() == true) {
    answer = rb_hash_new();
    rb_hash_aset(answer, ID2SYM(rb_intern("time_s")),
                 ULL2NUM(stat->get_time_s()));
    rb_hash_aset(answer, ID2SYM(rb_intern("duration_s")),
                 ULL2NUM(stat->get_duration_s()));
    rb_hash_aset(answer, ID2SYM(rb_intern("flows")),
                 ULL2NUM(stat->get_flows()));
    rb_hash_aset(answer, ID2SYM(rb_intern("other")),
                 ULL2NUM(stat->get_other()));
    rb_hash_aset(answer, ID2SYM(rb_intern("border_in")),
                 ULL2NUM(stat->get_border_in()));
    rb_hash_aset(answer, ID2SYM(rb_intern("border_out")),
                 ULL2NUM(stat->get_border_out()));
  }
  delete stat;
  return (answer);
}

// data
VALUE rb_border_filter_data_process_block(void* data) {
  BorderFilter* bf = (BorderFilter*) data;
  bf->data_process();
  return (Qnil);
}

VALUE rb_border_filter_data_process(VALUE self) {
  RB_BORDER_FILTER_UNWRAP
  rb_thread_blocking_region(rb_border_filter_data_process_block, bf,
                            RUBY_UBF_IO, 0);
  return (self);
}

