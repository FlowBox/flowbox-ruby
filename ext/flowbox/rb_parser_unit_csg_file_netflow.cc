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
 * @file   rb_parser_unit_csg_file_netflow.cc
 * @author Dominik Schatzmann <schadomi@gmail.com>
 * @date   October, 2012
 * @brief  Ruby C++ Wrapper for ParserUnitCSGFileNetflow.
 *
 * Ruby C++ Wrapper for ParserUnitCSGFileNetflow.
 *
 */

#include "rb_parser_unit_csg_file_netflow.h"

void rb_parser_unit_csg_file_netflow_free(void *p) {
  delete (ParserUnitCSGFileNetflow*) p;
  p = NULL;
}

VALUE rb_parser_unit_csg_file_netflow_alloc(VALUE klass) {
  ParserUnitCSGFileNetflow* p = NULL;
  VALUE obj;
  p = new ParserUnitCSGFileNetflow();
  if (p == NULL)
    rb_raise(rb_path2class("FlowBox::Core::ParserUnitCSGFileNetflowError"),
             "Out of Memory");

  obj = Data_Wrap_Struct(klass, 0, rb_parser_unit_csg_file_netflow_free, p);
  return (obj);
}

// configuration
VALUE rb_parser_unit_csg_file_netflow_conf_push(VALUE self,
                                                VALUE configuration_h) {
  RB_PARSER_UNIT_CSG_FILE_NETFLOW_UNWRAP
  ParserUnitCSGFileNetflow::Configuration conf_c;

  // parser general
  // :stat_interval_s => T_FIXNUM
  VALUE stat_interval_s = rb_hash_aref(configuration_h,
                                       ID2SYM(rb_intern("stat_interval_s")));
  Check_Type(stat_interval_s, T_FIXNUM);
  conf_c.set_stat_interval_s(NUM2INT(stat_interval_s));

  // :export_time_window_s => T_FIXNUM
  VALUE export_time_window_s = rb_hash_aref(
      configuration_h, ID2SYM(rb_intern("export_time_window_s")));
  Check_Type(export_time_window_s, T_FIXNUM);
  conf_c.set_export_time_window_s(NUM2INT(export_time_window_s));

  // csg-file specific
  // csgfile_input => STRING
  VALUE csgfile_input = rb_hash_aref(configuration_h,
                                     ID2SYM(rb_intern("csgfile_input")));
  Check_Type(csgfile_input, T_STRING);
  conf_c.set_path(RSTRING_PTR(csgfile_input));

  // :csgfile_scan_interval_s => T_FIXNUM
  VALUE csgfile_scan_interval_s = rb_hash_aref(
      configuration_h, ID2SYM(rb_intern("csgfile_scan_interval_s")));
  Check_Type(csgfile_scan_interval_s, T_FIXNUM);
  conf_c.set_scan_interval_s(NUM2INT(csgfile_scan_interval_s));

  // :csgfile_remove_files_after => Boolean
  VALUE csgfile_remove_files_after = rb_hash_aref(
      configuration_h, ID2SYM(rb_intern("csgfile_remove_files_after")));
  if (csgfile_remove_files_after == Qtrue)
    conf_c.set_remove_files_after(true);
  else if (csgfile_remove_files_after == Qfalse)
    conf_c.set_remove_files_after(false);

  // buffers
  // :output => Object
  VALUE output = rb_hash_aref(configuration_h, ID2SYM(rb_intern("output")));
  if (TYPE(output) == T_DATA) {
    FlowContainerBuffer* output_buf_p;
    Data_Get_Struct(output, FlowContainerBuffer, output_buf_p);
    conf_c.set_output(output_buf_p);
  }

  // :flow exporters => []
  VALUE flow_exporter_a = rb_hash_aref(configuration_h,
                                       ID2SYM(rb_intern("flow_exporters")));
  Check_Type(flow_exporter_a, T_ARRAY);

  while (RARRAY_LEN(flow_exporter_a) > 0) {

    VALUE export_device_h = rb_ary_shift(flow_exporter_a);

    // :id => T_FIXNUM
    VALUE export_devices_id = rb_hash_aref(export_device_h,
                                           ID2SYM(rb_intern("id")));
    Check_Type(export_devices_id, T_FIXNUM);

    // :address => T_STRING
    VALUE export_devices_address = rb_hash_aref(export_device_h,
                                                ID2SYM(rb_intern("address")));
    Check_Type(export_devices_address, T_STRING);
    struct sockaddr addr;
    addr.sa_family = AF_INET;
    inet_pton(AF_INET, RSTRING_PTR(export_devices_address),
              &((struct sockaddr_in*) (&addr))->sin_addr);

    // :observation_domain => T_FIXNUM
    VALUE export_devices_observation_domain = rb_hash_aref(
        export_device_h, ID2SYM(rb_intern("observation_domain")));
    Check_Type(export_devices_observation_domain, T_FIXNUM);

    // :version => T_FIXNUM
    VALUE export_devices_format = rb_hash_aref(export_device_h,
                                               ID2SYM(rb_intern("format")));
    Check_Type(export_devices_format, T_FIXNUM);

    // Build Export Device
    ParserUnitCSGFileNetflow::ExportDevice export_device_c(
        addr, NUM2INT(export_devices_observation_domain),
        NUM2INT(export_devices_format), NUM2INT(export_devices_id));
    conf_c.add_export_device(export_device_c);
  };

  // forward configuration
  puf->conf_push(conf_c);
  return (self);
}

// observation
VALUE rb_parser_unit_csg_file_netflow_obs_get_block(void* data) {
  ParserUnitCSGFileNetflow* puf = (ParserUnitCSGFileNetflow*) data;
  ParserUnitCSGFileNetflow::Observation* obs =
      new ParserUnitCSGFileNetflow::Observation();
  try {
    (*obs) = puf->obs_get();  // blocking
  } catch (...) {
    obs->set_valid(false);
  }

  // rb_bug("object allocation during garbage collection phase");
  // -- hot fix -- don't allocate ruby memory in non-blocking threads
  return ((VALUE) obs);
}

VALUE rb_parser_unit_csg_file_netflow_obs_get(VALUE self) {
  RB_PARSER_UNIT_CSG_FILE_NETFLOW_UNWRAP
  VALUE observation = rb_thread_blocking_region(
      rb_parser_unit_csg_file_netflow_obs_get_block, puf, RUBY_UBF_IO, 0);

  ParserUnitCSGFileNetflow::Observation* obs =
      (ParserUnitCSGFileNetflow::Observation*) observation;

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
VALUE rb_parser_unit_csg_file_netflow_stat_get_block(void* data) {
  ParserUnitCSGFileNetflow* puf = (ParserUnitCSGFileNetflow*) data;
  // copy 
  ParserUnitCSGFileNetflow::Statistics* stat =
      new ParserUnitCSGFileNetflow::Statistics();
  try {
    (*stat) = puf->stat_get();
  } catch (...) {
    stat->set_valid(false);
  }
  // rb_bug("object allocation during garbage collection phase");
  // -- hot fix -- don't allocate ruby memory in non-blocking threads
  return ((VALUE) stat);
}

VALUE rb_parser_unit_csg_file_netflow_stat_get(VALUE self) {

  RB_PARSER_UNIT_CSG_FILE_NETFLOW_UNWRAP
  VALUE statistic = rb_thread_blocking_region(
      rb_parser_unit_csg_file_netflow_stat_get_block, puf, RUBY_UBF_IO, 0);
  ParserUnitCSGFileNetflow::Statistics* stat =
      (ParserUnitCSGFileNetflow::Statistics*) statistic;

  VALUE answer = Qnil;
  if (stat->get_valid() == true) {
    answer = rb_hash_new();
    rb_hash_aset(answer, ID2SYM(rb_intern("time_s")),
                 ULL2NUM(stat->get_time_s()));
    rb_hash_aset(answer, ID2SYM(rb_intern("duration_s")),
                 ULL2NUM(stat->get_duration_s()));
    rb_hash_aset(answer, ID2SYM(rb_intern("real_time_s")),
                 ULL2NUM(stat->get_real_time_s()));
    rb_hash_aset(answer, ID2SYM(rb_intern("real_duration_s")),
                 ULL2NUM(stat->get_real_duration_s()));
    rb_hash_aset(answer, ID2SYM(rb_intern("packets")),
                 ULL2NUM(stat->get_packets()));
    rb_hash_aset(answer, ID2SYM(rb_intern("iv_min_size")),
                 ULL2NUM(stat->get_iv_min_size()));
    rb_hash_aset(answer, ID2SYM(rb_intern("iv_format")),
                 ULL2NUM(stat->get_iv_format()));
    rb_hash_aset(answer, ID2SYM(rb_intern("iv_exporter")),
                 ULL2NUM(stat->get_iv_exporter()));
    rb_hash_aset(answer, ID2SYM(rb_intern("iv_export_time_too_small")),
                 ULL2NUM(stat->get_iv_export_time_too_small()));
    rb_hash_aset(answer, ID2SYM(rb_intern("iv_export_time_too_big")),
                 ULL2NUM(stat->get_iv_export_time_too_big()));
  }

//    rb_hash_aset(answer, ID2SYM(rb_intern("packets_illegal_format"), ULL2NUM(stat->get_packets_illegal_format_));
//    rb_hash_aset(answer, ID2SYM(rb_intern("packets_missing_export_device"), ULL2NUM(stat->get_packets_missing_export_device_));
//    std::map<std::string, std::map<std::string, uint64_t>>::iterator current = stat->export_device_stats.begin();
//    std::map<std::string, std::map<std::string, uint64_t>>::iterator end = stat->export_device_stats.end();
//    while(current != end){
//      VALUE sub_hash = rb_hash_new();
//      std::map<std::string, uint64_t>::iterator sub_current = (current->second).begin();
//      std::map<std::string, uint64_t>::iterator sub_end = (current->second).end();
//      while(sub_current != sub_end){
//        rb_hash_aset(sub_hash, rb_str_new2((sub_current->first).c_str()), ULL2NUM(sub_current->second));
//        sub_current++;
//      }
//      rb_hash_aset(answer, rb_str_new2(current->first.c_str()), sub_hash);
//      current++;
//    }
//  }

  delete stat;
  return (answer);
}

// data
VALUE rb_parser_unit_csg_file_netflow_data_process_block(void* data) {
  ParserUnitCSGFileNetflow* puf = (ParserUnitCSGFileNetflow*) data;
  puf->data_process();
  return (Qnil);
}

VALUE rb_parser_unit_csg_file_netflow_data_process(VALUE self) {
  RB_PARSER_UNIT_CSG_FILE_NETFLOW_UNWRAP
  rb_thread_blocking_region(rb_parser_unit_csg_file_netflow_data_process_block,
                            puf, RUBY_UBF_IO, 0);
  return (self);
}

