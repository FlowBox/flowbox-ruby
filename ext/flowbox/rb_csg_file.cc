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
 * @file   rb_csg_file.cc
 * @author Dominik Schatzmann <schadomi@gmail.com>
 * @date   October, 2012
 * @brief   Ruby C++ Wrapper for CSGFile class.
 *
 * Ruby C++ Wrapper for CSGFile class.
 *
 */

#include "rb_csg_file.h"
void rb_csg_file_free(void *p) {
  delete (CSGFile*) p;
  p = NULL;
}

VALUE rb_csg_file_alloc(VALUE klass) {
  CSGFile* p = NULL;
  VALUE obj;
  p = new CSGFile();
  if (p == NULL) {
    rb_raise(rb_path2class("FlowBox::Core::CSGFileError"), "Out of Memory");
  }
  obj = Data_Wrap_Struct(klass, 0, rb_csg_file_free, p);
  return (obj);
}

// API
VALUE rb_csg_file_process_file(VALUE self, VALUE data_p, VALUE stat_p) {
  RB_CSG_FILE_UNWRAP

  Check_Type(data_p, T_STRING);
  Check_Type(stat_p, T_STRING);
  csg_file->process_file(RSTRING_PTR(data_p), RSTRING_PTR(stat_p));

  return (self);
}

VALUE rb_csg_file_get_packet(VALUE self, VALUE packet) {
  RB_CSG_FILE_UNWRAP
  Packet* csg_packet_p;
  Packet* ruby_packet_p;
  Data_Get_Struct(packet, Packet, ruby_packet_p);
  csg_packet_p = csg_file->get_packet();
  memcpy(ruby_packet_p, csg_packet_p, sizeof(Packet));
  return (self);
}

VALUE rb_csg_file_get_time_r(VALUE self) {
  RB_CSG_FILE_UNWRAP
  return (ULL2NUM(csg_file->get_time_r()));
}

VALUE rb_csg_file_get_stream_state(VALUE self) {
  RB_CSG_FILE_UNWRAP
  return (ULL2NUM(csg_file->get_stream_state()));
}

VALUE rb_csg_file_next(VALUE self) {
  RB_CSG_FILE_UNWRAP
  csg_file->next();
  return (self);
}

