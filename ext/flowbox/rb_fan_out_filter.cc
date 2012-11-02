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
 * @file   rb_fan_out_filter.cc
 * @author Dominik Schatzmann <schadomi@gmail.com>
 * @author Daniel Aschwanden <nimdanitro@gmail.com>
 * @date   October, 2012
 * @brief   Ruby C++ Wrapper for FanOutFilter.
 *
 * Ruby C++ Wrapper for FanOutFilter.
 *
 */

#include "rb_fan_out_filter.h"

void rb_fan_out_filter_free(void *p) {
  delete (FanOutFilter*) p;
  p = NULL;
}

VALUE rb_fan_out_filter_alloc(VALUE klass) {
  FanOutFilter* p = NULL;
  VALUE obj;
  p = new FanOutFilter();
  if (p == NULL)
    rb_raise(rb_path2class("FlowBox::Core::FanOutFilterError"),
             "Out of Memory");

  obj = Data_Wrap_Struct(klass, 0, rb_fan_out_filter_free, p);
  return (obj);
}

// configuration
VALUE rb_fan_out_filter_conf_push(VALUE self, VALUE configuration_h) {
  RB_FAN_OUT_FILTER_UNWRAP
  FanOutFilter::Configuration conf_c;

  // input
  VALUE input = rb_hash_aref(configuration_h, ID2SYM(rb_intern("input")));
  if (TYPE(input) == T_DATA) {
    BiFlowContainerBuffer* input_buf_p;
    Data_Get_Struct(input, BiFlowContainerBuffer, input_buf_p);
    conf_c.set_input(input_buf_p);
  };

  // output
  VALUE output_good = rb_hash_aref(configuration_h,
                                   ID2SYM(rb_intern("output_good")));
  if (TYPE(output_good) == T_DATA) {
    BiFlowContainerBuffer* output_buf_good;
    Data_Get_Struct(output_good, BiFlowContainerBuffer, output_buf_good);
    conf_c.set_output_good(output_buf_good);
  }

  // output
  VALUE output_bad = rb_hash_aref(configuration_h,
                                  ID2SYM(rb_intern("output_bad")));
  if (TYPE(output_bad) == T_DATA) {
    BiFlowContainerBuffer* output_buf_bad;
    Data_Get_Struct(output_bad, BiFlowContainerBuffer, output_buf_bad);
    conf_c.set_output_bad(output_buf_bad);
  }

  // stat_interval
  VALUE export_stat_interval = rb_hash_aref(configuration_h,
                                            ID2SYM(rb_intern("stat_interval")));
  Check_Type(export_stat_interval, T_FIXNUM);
  conf_c.set_stat_interval(NUM2INT(export_stat_interval));

  // in_bad_min
  VALUE in_bad_min = rb_hash_aref(configuration_h,
                                  ID2SYM(rb_intern("in_bad_min")));
  Check_Type(in_bad_min, T_FIXNUM);
  conf_c.set_in_bad_min(NUM2INT(in_bad_min));

  // in_ratio_min
  VALUE in_ratio_min = rb_hash_aref(configuration_h,
                                    ID2SYM(rb_intern("in_ratio_min")));
  Check_Type(in_ratio_min, T_FIXNUM);
  conf_c.set_in_ratio_min(NUM2INT(in_ratio_min));

  // out_bad_min
  VALUE out_bad_min = rb_hash_aref(configuration_h,
                                   ID2SYM(rb_intern("out_bad_min")));
  Check_Type(out_bad_min, T_FIXNUM);
  conf_c.set_out_bad_min(NUM2INT(out_bad_min));

  // out_ratio_min
  VALUE out_ratio_min = rb_hash_aref(configuration_h,
                                     ID2SYM(rb_intern("out_ratio_min")));
  Check_Type(out_ratio_min, T_FIXNUM);
  conf_c.set_in_ratio_min(NUM2INT(out_ratio_min));

  // push
  fanoutfilter->conf_push(conf_c);

  return (self);
}

// observation
VALUE rb_fan_out_filter_obs_get_block(void* data) {
  FanOutFilter* fanoutfilter = (FanOutFilter*) data;
  FanOutFilter::Observation* obs = new FanOutFilter::Observation();
  try {
    (*obs) = fanoutfilter->obs_get();  // blocking
  } catch (...) {
    obs->set_valid(false);
  }
  return ((VALUE) obs);
}
VALUE rb_fan_out_filter_obs_get(VALUE self) {
  RB_FAN_OUT_FILTER_UNWRAP
  VALUE observation = rb_thread_blocking_region(rb_fan_out_filter_obs_get_block,
                                                fanoutfilter, RUBY_UBF_IO, 0);
  FanOutFilter::Observation* obs = (FanOutFilter::Observation*) observation;
  VALUE answer = Qnil;
  if (obs->valid() == true) {
    answer = rb_hash_new();
    rb_hash_aset(answer, ID2SYM(rb_intern("time_s")), ULL2NUM(obs->time_s()));
    rb_hash_aset(answer, ID2SYM(rb_intern("msg")),
                 rb_str_new2((obs->message()).c_str()));
  }
  delete obs;
  return (answer);
}

// statistics
VALUE rb_fan_out_filter_stat_get_block(void* data) {
  FanOutFilter* fanoutfilter = (FanOutFilter*) data;
  FanOutFilter::Statistics* stat = new FanOutFilter::Statistics();
  try {
    (*stat) = fanoutfilter->stat_get();  // blocking
  } catch (...) {
    stat->set_valid(false);
  }
  return ((VALUE) stat);
}

VALUE rb_fan_out_filter_stat_get(VALUE self) {
  RB_FAN_OUT_FILTER_UNWRAP
  VALUE statistic = rb_thread_blocking_region(rb_fan_out_filter_stat_get_block,
                                              fanoutfilter, RUBY_UBF_IO, 0);
  FanOutFilter::Statistics* stat = (FanOutFilter::Statistics*) statistic;

  VALUE answer = Qnil;
  if (stat->valid() == true) {
    answer = rb_hash_new();

    rb_hash_aset(answer, ID2SYM(rb_intern("time_s")), ULL2NUM(stat->time_s()));
    rb_hash_aset(answer, ID2SYM(rb_intern("duration_s")),
                 ULL2NUM(stat->duration_s()));
    rb_hash_aset(answer, ID2SYM(rb_intern("duration_all_build")),
                 ULL2NUM(stat->duration_all_build()));
    rb_hash_aset(answer, ID2SYM(rb_intern("duration_selection_build")),
                 ULL2NUM(stat->duration_selection_build()));
    rb_hash_aset(answer, ID2SYM(rb_intern("duration_split")),
                 ULL2NUM(stat->duration_split()));

    rb_hash_aset(answer, ID2SYM(rb_intern("biflows_in")),
                 ULL2NUM(stat->biflows_in()));
    rb_hash_aset(answer, ID2SYM(rb_intern("biflows_kept")),
                 ULL2NUM(stat->biflows_kept()));
    rb_hash_aset(answer, ID2SYM(rb_intern("biflows_filtered")),
                 ULL2NUM(stat->biflows_filtered()));

    rb_hash_aset(answer, ID2SYM(rb_intern("all_in")), ULL2NUM(stat->all_in()));
    rb_hash_aset(answer, ID2SYM(rb_intern("all_out")),
                 ULL2NUM(stat->all_out()));
    rb_hash_aset(answer, ID2SYM(rb_intern("selection_in")),
                 ULL2NUM(stat->selection_in()));

    rb_hash_aset(answer, ID2SYM(rb_intern("selection_out")),
                 ULL2NUM(stat->selection_out()));
    rb_hash_aset(answer, ID2SYM(rb_intern("hosts_in")),
                 ULL2NUM(stat->hosts_in()));
    rb_hash_aset(answer, ID2SYM(rb_intern("hosts_out")),
                 ULL2NUM(stat->hosts_out()));

    rb_hash_aset(answer, ID2SYM(rb_intern("buckets_all_in")),
                 ULL2NUM(stat->buckets_all_in()));
    rb_hash_aset(answer, ID2SYM(rb_intern("buckets_all_out")),
                 ULL2NUM(stat->buckets_all_out()));
    rb_hash_aset(answer, ID2SYM(rb_intern("buckets_selection_in")),
                 ULL2NUM(stat->buckets_selection_in()));
    rb_hash_aset(answer, ID2SYM(rb_intern("buckets_selection_out")),
                 ULL2NUM(stat->buckets_selection_out()));
  }
  delete stat;
  return (answer);
}

// data
VALUE rb_fan_out_filter_data_process_block(void* data) {
  FanOutFilter* fanoutfilter = (FanOutFilter*) data;
  fanoutfilter->data_process();
  return (Qnil);
}

VALUE rb_fan_out_filter_data_process(VALUE self) {
  RB_FAN_OUT_FILTER_UNWRAP
  rb_thread_blocking_region(rb_fan_out_filter_data_process_block, fanoutfilter,
                            RUBY_UBF_IO, 0);
  return (self);
}

