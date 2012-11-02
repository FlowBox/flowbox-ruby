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
 * @file   rb_parser_unit_csg_file_netflow.h
 * @author Dominik Schatzmann <schadomi@gmail.com>
 * @date   October, 2012
 * @brief  Ruby C++ Wrapper for ParserUnitCSGFileNetflow.
 *
 * Ruby C++ Wrapper for ParserUnitCSGFileNetflow.
 *
 */

#ifndef FLOWBOXRUBY_EXT_RB_PARSER_UNIT_CSG_FILE_NETFLOW_H_
#define FLOWBOXRUBY_EXT_RB_PARSER_UNIT_CSG_FILE_NETFLOW_H_

#include "ruby.h"
#include <flowbox/parser_unit_csg_file_netflow.h>

#define RB_PARSER_UNIT_CSG_FILE_NETFLOW_UNWRAP ParserUnitCSGFileNetflow* puf; Data_Get_Struct(self, ParserUnitCSGFileNetflow, puf);

void rb_parser_unit_csg_file_netflow_free(void *p);
VALUE rb_parser_unit_csg_file_netflow_alloc(VALUE klass);

// configuration
VALUE rb_parser_unit_csg_file_netflow_conf_push(VALUE self,
                                                VALUE configuration_h);

// observation
VALUE rb_parser_unit_csg_file_netflow_obs_get_block(void* data);
VALUE rb_parser_unit_csg_file_netflow_obs_get(VALUE self);

// statistics
VALUE rb_parser_unit_csg_file_netflow_stat_get_block(void* data);
VALUE rb_parser_unit_csg_file_netflow_stat_get(VALUE self);

// data
VALUE rb_parser_unit_csg_file_netflow_data_process_block(void* data);
VALUE rb_parser_unit_csg_file_netflow_data_process(VALUE self);

#endif  // FLOWBOXRUBY_EXT_RB_PARSER_UNIT_CSG_FILE_NETFLOW_H_
