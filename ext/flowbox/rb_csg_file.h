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
 * @file   rb_csg_file.h
 * @author Dominik Schatzmann <schadomi@gmail.com>
 * @date   October, 2012
 * @brief   Ruby C++ Wrapper for CSGFile class.
 *
 * Ruby C++ Wrapper for CSGFile class.
 *
 */

#ifndef FLOWBOXRUBY_EXT_RB_CSG_FILE_H_
#define FLOWBOXRUBY_EXT_RB_CSG_FILE_H_

#include "ruby.h"
#include <flowbox/csg_file.h>

#define RB_CSG_FILE_UNWRAP CSGFile* csg_file; Data_Get_Struct(self, CSGFile, csg_file);

void rb_csg_file_free(void *p);
VALUE rb_csg_file_alloc(VALUE klass);

// API
VALUE rb_csg_file_process_file(VALUE self, VALUE data_p, VALUE stat_p);
VALUE rb_csg_file_get_packet(VALUE self, VALUE packet);
VALUE rb_csg_file_get_time_r(VALUE self);
VALUE rb_csg_file_get_stream_state(VALUE self);
VALUE rb_csg_file_next(VALUE self);

#endif  // FLOWBOXRUBY_EXT_RB_CSG_FILE_H_
