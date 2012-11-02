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
 * @file   rb_bi_flow_container_buffer.h
 * @author Dominik Schatzmann <schadomi@gmail.com>
 * @author Daniel Aschwanden <nimdanitro@gmail.com>
 * @date   October, 2012
 * @brief  Ruby C++ Wrapper for BiFlowContainerBuffer.
 *
 * Ruby C++ Wrapper for BiFlowContainerBuffer.
 *
 */

#ifndef FLOWBOXRUBY_EXT_RB_BI_FLOW_CONTAINER_BUFFER_H_
#define FLOWBOXRUBY_EXT_RB_BI_FLOW_CONTAINER_BUFFER_H_

#include "ruby.h"
#include <flowbox/bi_flow_container_buffer.h>

#define RB_BI_FLOW_BUFFER_UNWRAP BiFlowContainerBuffer* biflow_container_buffer; Data_Get_Struct(self, BiFlowContainerBuffer, biflow_container_buffer);
void rb_bi_flow_container_buffer_free(void *p);
VALUE rb_bi_flow_container_buffer_alloc(VALUE klass);
VALUE rb_bi_flow_container_buffer_pop(VALUE self);
VALUE rb_bi_flow_container_buffer_push(VALUE self, VALUE biflow);

#endif  // FLOWBOXRUBY_EXT_RB_BI_FLOW_CONTAINER_BUFFER_H_
