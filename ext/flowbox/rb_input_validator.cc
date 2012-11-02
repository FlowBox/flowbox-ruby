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
 * @file   rb_input_validator.cc
 * @author Dominik Schatzmann <schadomi@gmail.com>
 * @date   October, 2012
 * @brief  Ruby C++ Wrapper for InputValidator.
 *
 * Ruby C++ Wrapper for InputValidator.
 *
 */

#include "rb_input_validator.h"

void rb_input_validator_free(void *p) {
  delete (InputValidator*) p;
  p = NULL;
}

VALUE rb_input_validator_alloc(VALUE klass) {
  InputValidator* p = NULL;
  VALUE obj;
  p = new InputValidator();
  if (p == NULL)
    rb_raise(rb_path2class("FlowBox::Core::InputValidatorError"),
             "Out of Memory");

  obj = Data_Wrap_Struct(klass, 0, rb_input_validator_free, p);
  return (obj);
}

// configuration
VALUE rb_input_validator_conf_push(VALUE self, VALUE configuration_h) {
  RB_INPUT_VALIDATOR_UNWRAP
  InputValidator::Configuration conf_c;

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

  // sliding_window
  VALUE sliding_window = rb_hash_aref(configuration_h,
                                      ID2SYM(rb_intern("sliding_window")));
  Check_Type(sliding_window, T_FIXNUM);
  conf_c.set_sliding_window_s(NUM2INT(sliding_window));

  // push
  iv->conf_push(conf_c);

  return (self);
}

// observation
VALUE rb_input_validator_obs_get_block(void* data) {
  InputValidator* iv = (InputValidator*) data;
  InputValidator::Observation* obs = new InputValidator::Observation();
  try {
    (*obs) = iv->obs_get();  // blocking
  } catch (...) {
    obs->set_valid(false);
  }
  return ((VALUE) obs);
}
VALUE rb_input_validator_obs_get(VALUE self) {
  RB_INPUT_VALIDATOR_UNWRAP
  VALUE observation = rb_thread_blocking_region(
      rb_input_validator_obs_get_block, iv, RUBY_UBF_IO, 0);
  InputValidator::Observation* obs = (InputValidator::Observation*) observation;
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
VALUE rb_input_validator_stat_get_block(void* data) {
  InputValidator* iv = (InputValidator*) data;
  InputValidator::Statistics* stat = new InputValidator::Statistics();
  try {
    (*stat) = iv->stat_get();  // blocking
  } catch (...) {
    stat->set_valid(false);
  }
  return ((VALUE) stat);
}

VALUE rb_input_validator_stat_get(VALUE self) {
  RB_INPUT_VALIDATOR_UNWRAP
  VALUE statistic = rb_thread_blocking_region(rb_input_validator_stat_get_block,
                                              iv, RUBY_UBF_IO, 0);
  InputValidator::Statistics* stat = (InputValidator::Statistics*) statistic;
  VALUE answer = Qnil;
  if (stat->get_valid() == true) {
    answer = rb_hash_new();
    rb_hash_aset(answer, ID2SYM(rb_intern("time_s")),
                 ULL2NUM(stat->get_time_s()));
    rb_hash_aset(answer, ID2SYM(rb_intern("duration_s")),
                 ULL2NUM(stat->get_duration_s()));
    rb_hash_aset(answer, ID2SYM(rb_intern("flows")),
                 ULL2NUM(stat->get_flows()));
    rb_hash_aset(answer, ID2SYM(rb_intern("iv_time_freshness")),
                 ULL2NUM(stat->get_iv_time_freshness()));
    rb_hash_aset(answer, ID2SYM(rb_intern("iv_time_causality")),
                 ULL2NUM(stat->get_iv_time_causality()));
    rb_hash_aset(answer, ID2SYM(rb_intern("iv_bytes")),
                 ULL2NUM(stat->get_iv_bytes()));
    rb_hash_aset(answer, ID2SYM(rb_intern("iv_packets")),
                 ULL2NUM(stat->get_iv_packets()));
    rb_hash_aset(answer, ID2SYM(rb_intern("iv_mtu")),
                 ULL2NUM(stat->get_iv_mtu()));
  }
  delete stat;
  return (answer);
}

// data
VALUE rb_input_validator_data_process_block(void* data) {
  InputValidator* iv = (InputValidator*) data;
  iv->data_process();
  return (Qnil);
}

VALUE rb_input_validator_data_process(VALUE self) {
  RB_INPUT_VALIDATOR_UNWRAP
  rb_thread_blocking_region(rb_input_validator_data_process_block, iv,
                            RUBY_UBF_IO, 0);
  return (self);
}

