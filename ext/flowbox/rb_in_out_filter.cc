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
 * @file   rb_in_out_filter.cc
 * @author Dominik Schatzmann <schadomi@gmail.com>
 * @date   October, 2012
 * @brief  Ruby C++ Wrapper for InOutFilter.
 *
 * Ruby C++ Wrapper for InOutFilter.
 *
 */

#include "rb_in_out_filter.h"

void rb_in_out_filter_free(void *p) {
  delete (InOutFilter*) p;
  p = NULL;
}

VALUE rb_in_out_filter_alloc(VALUE klass) {
  InOutFilter* p = NULL;
  VALUE obj;
  p = new InOutFilter();
  if (p == NULL)
    rb_raise(rb_path2class("FlowBox::Core::InOutFilterError"), "Out of Memory");

  obj = Data_Wrap_Struct(klass, 0, rb_in_out_filter_free, p);
  return (obj);
}

// configuration
VALUE rb_in_out_filter_conf_push(VALUE self, VALUE configuration_h) {
  RB_IN_OUT_FILTER_UNWRAP
  InOutFilter::Configuration conf_c;

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
  VALUE policy = rb_hash_aref(configuration_h, ID2SYM(rb_intern("policy")));
  Check_Type(policy, T_FIXNUM);
  if (NUM2INT(policy) == InOutFilter::kTag)
    conf_c.set_policy(InOutFilter::kTag);
  else if (NUM2INT(policy) == InOutFilter::kFilter)
    conf_c.set_policy(InOutFilter::kFilter);
  else
    rb_raise(rb_path2class("FlowBox::Core::InOutFilterError"),
             "Unknown Policy Type");

  iof->conf_push(conf_c);
  return (self);
}

// observation
VALUE rb_in_out_filter_obs_get_block(void* data) {

  InOutFilter* iof = (InOutFilter*) data;
  InOutFilter::Observation* obs = new InOutFilter::Observation();
  try {
    (*obs) = iof->obs_get();  // blocking
  } catch (...) {
    obs->set_valid(false);
  }
  return ((VALUE) obs);
}
VALUE rb_in_out_filter_obs_get(VALUE self) {
  RB_IN_OUT_FILTER_UNWRAP
  VALUE observation = rb_thread_blocking_region(rb_in_out_filter_obs_get_block,
                                                iof, RUBY_UBF_IO, 0);
  InOutFilter::Observation* obs = (InOutFilter::Observation*) observation;
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
VALUE rb_in_out_filter_stat_get_block(void* data) {
  InOutFilter* iof = (InOutFilter*) data;
  InOutFilter::Statistics* stat = new InOutFilter::Statistics();
  try {
    (*stat) = iof->stat_get();  // blocking
  } catch (...) {
    stat->set_valid(false);
  }
  return ((VALUE) stat);
}

VALUE rb_in_out_filter_stat_get(VALUE self) {
  RB_IN_OUT_FILTER_UNWRAP
  VALUE statistic = rb_thread_blocking_region(rb_in_out_filter_stat_get_block,
                                              iof, RUBY_UBF_IO, 0);
  InOutFilter::Statistics* stat = (InOutFilter::Statistics*) statistic;
  VALUE answer = Qnil;
  if (stat->get_valid() == true) {
    answer = rb_hash_new();
    rb_hash_aset(answer, ID2SYM(rb_intern("time_s")),
                 ULL2NUM(stat->get_time_s()));
    rb_hash_aset(answer, ID2SYM(rb_intern("duration_s")),
                 ULL2NUM(stat->get_duration_s()));
    rb_hash_aset(answer, ID2SYM(rb_intern("flows")),
                 ULL2NUM(stat->get_flows()));
    rb_hash_aset(answer, ID2SYM(rb_intern("in_in")),
                 ULL2NUM(stat->get_in_in()));
    rb_hash_aset(answer, ID2SYM(rb_intern("in_out")),
                 ULL2NUM(stat->get_in_out()));
    rb_hash_aset(answer, ID2SYM(rb_intern("out_in")),
                 ULL2NUM(stat->get_out_in()));
    rb_hash_aset(answer, ID2SYM(rb_intern("out_out")),
                 ULL2NUM(stat->get_out_out()));
  }
  delete stat;
  return (answer);
}

// data
VALUE rb_in_out_filter_data_process_block(void* data) {
  InOutFilter* iof = (InOutFilter*) data;
  iof->data_process();
  return (Qnil);
}

VALUE rb_in_out_filter_data_process(VALUE self) {
  RB_IN_OUT_FILTER_UNWRAP
  rb_thread_blocking_region(rb_in_out_filter_data_process_block, iof,
                            RUBY_UBF_IO, 0);
  return (self);
}

