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
 * @file   rb_noise_filter.cc
 * @author Dominik Schatzmann <schadomi@gmail.com>
 * @author Daniel Aschwanden <nimdanitro@gmail.com>
 * @date   October, 2012
 * @brief  Ruby C++ Wrapper for NoiseFilter.
 *
 * Ruby C++ Wrapper for NoiseFilter.
 *
 */

#include "rb_noise_filter.h"

void rb_noise_filter_free(void *p) {
  delete (NoiseFilter*) p;
  p = NULL;
}

VALUE rb_noise_filter_alloc(VALUE klass) {
  NoiseFilter* p = NULL;
  VALUE obj;
  p = new NoiseFilter();
  if (p == NULL)
    rb_raise(rb_path2class("FlowBox::Core::NoiseFilterError"), "Out of Memory");

  obj = Data_Wrap_Struct(klass, 0, rb_noise_filter_free, p);
  return (obj);
}

// configuration
VALUE rb_noise_filter_conf_push(VALUE self, VALUE configuration_h) {
  RB_NOISE_FILTER_UNWRAP
  NoiseFilter::Configuration conf_c;

  // input_flow
  VALUE in_flow = rb_hash_aref(configuration_h,
                               ID2SYM(rb_intern("input_flow")));
  if (TYPE(in_flow) == T_DATA) {
    FlowContainerBuffer* input_buf_p;
    Data_Get_Struct(in_flow, FlowContainerBuffer, input_buf_p);
    conf_c.set_flow_input(input_buf_p);
  };

  // out_flow
  VALUE out_flow = rb_hash_aref(configuration_h,
                                ID2SYM(rb_intern("output_flow")));
  if (TYPE(out_flow) == T_DATA) {
    FlowContainerBuffer* output_buf_p;
    Data_Get_Struct(out_flow, FlowContainerBuffer, output_buf_p);
    conf_c.set_flow_output(output_buf_p);
  };

  // input_biflow
  VALUE in_biflow = rb_hash_aref(configuration_h,
                                 ID2SYM(rb_intern("input_biflow")));
  if (TYPE(in_biflow) == T_DATA) {
    BiFlowContainerBuffer* input_buf_p;
    Data_Get_Struct(in_biflow, BiFlowContainerBuffer, input_buf_p);
    conf_c.set_biflow_input(input_buf_p);
  };

  // output_biflow_filtered
  VALUE output_filtered_biflow = rb_hash_aref(
      configuration_h, ID2SYM(rb_intern("output_biflow_filtered")));
  if (TYPE(output_filtered_biflow) == T_DATA) {
    BiFlowContainerBuffer* output_buf_p;
    Data_Get_Struct(out_flow, BiFlowContainerBuffer, output_buf_p);
    conf_c.set_biflow_output_filtered(output_buf_p);
  };

  // output biflow
  VALUE output_biflow = rb_hash_aref(configuration_h,
                                     ID2SYM(rb_intern("output_biflow")));
  if (TYPE(output_biflow) == T_DATA) {
    BiFlowContainerBuffer* output_buf;
    Data_Get_Struct(output_biflow, BiFlowContainerBuffer, output_buf);
    conf_c.set_biflow_output(output_buf);
  }

  // udp_packets_min
  VALUE udp_packets_min = rb_hash_aref(configuration_h,
                                       ID2SYM(rb_intern("udp_packets_min")));
  Check_Type(udp_packets_min, T_FIXNUM);
  conf_c.set_udp_packets_min(NUM2INT(udp_packets_min));

  // tcp_packets_min
  VALUE tcp_packets_min = rb_hash_aref(configuration_h,
                                       ID2SYM(rb_intern("tcp_packets_min")));
  Check_Type(tcp_packets_min, T_FIXNUM);
  conf_c.set_tcp_packets_min(NUM2INT(tcp_packets_min));

  // stat_interval
  VALUE export_stat_interval = rb_hash_aref(configuration_h,
                                            ID2SYM(rb_intern("stat_interval")));
  Check_Type(export_stat_interval, T_FIXNUM);
  conf_c.set_stat_interval(NUM2INT(export_stat_interval));

  // push
  noisefilter->conf_push(conf_c);

  return (self);
}

// observation
VALUE rb_noise_filter_obs_get_block(void* data) {
  NoiseFilter* noisefilter = (NoiseFilter*) data;
  NoiseFilter::Observation* obs = new NoiseFilter::Observation();
  try {
    (*obs) = noisefilter->obs_get();  // blocking
  } catch (...) {
    obs->set_valid(false);
  }
  return ((VALUE) obs);
}

VALUE rb_noise_filter_obs_get(VALUE self) {
  RB_NOISE_FILTER_UNWRAP
  VALUE observation = rb_thread_blocking_region(rb_noise_filter_obs_get_block,
                                                noisefilter, RUBY_UBF_IO, 0);
  NoiseFilter::Observation* obs = (NoiseFilter::Observation*) observation;
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
VALUE rb_noise_filter_stat_get_block(void* data) {
  NoiseFilter* noisefilter = (NoiseFilter*) data;
  NoiseFilter::Statistics* stat = new NoiseFilter::Statistics();
  try {
    (*stat) = noisefilter->stat_get();  // blocking
  } catch (...) {
    stat->set_valid(false);
  }
  return ((VALUE) stat);
}

VALUE rb_noise_filter_stat_get(VALUE self) {
  RB_NOISE_FILTER_UNWRAP
  VALUE statistic = rb_thread_blocking_region(rb_noise_filter_stat_get_block,
                                              noisefilter, RUBY_UBF_IO, 0);
  NoiseFilter::Statistics* stat = (NoiseFilter::Statistics*) statistic;

  VALUE answer = Qnil;
  if (stat->valid() == true) {
    answer = rb_hash_new();
    rb_hash_aset(answer, ID2SYM(rb_intern("time_s")), ULL2NUM(stat->time_s()));
    rb_hash_aset(answer, ID2SYM(rb_intern("duration_s")),
                 ULL2NUM(stat->duration_s()));
    rb_hash_aset(answer, ID2SYM(rb_intern("flows_in")),
                 ULL2NUM(stat->flows_in()));
    rb_hash_aset(answer, ID2SYM(rb_intern("flows_valid")),
                 ULL2NUM(stat->flows_valid()));
    rb_hash_aset(answer, ID2SYM(rb_intern("flows_kept")),
                 ULL2NUM(stat->flows_kept()));
    rb_hash_aset(answer, ID2SYM(rb_intern("flows_filtered")),
                 ULL2NUM(stat->flows_filtered()));
  }
  delete stat;
  return (answer);
}

// data
VALUE rb_noise_filter_data_process_block(void* data) {
  NoiseFilter* noisefilter = (NoiseFilter*) data;
  noisefilter->data_process();
  return (Qnil);
}

VALUE rb_noise_filter_data_process(VALUE self) {
  RB_NOISE_FILTER_UNWRAP
  rb_thread_blocking_region(rb_noise_filter_data_process_block, noisefilter,
                            RUBY_UBF_IO, 0);
  return (self);
}

