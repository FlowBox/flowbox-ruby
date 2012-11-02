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
 * @file   rb_flowblock.h
 * @author Dominik Schatzmann <schadomi@gmail.com>
 * @date   October, 2012
 * @brief  Ruby C++ Wrapper for FlowBlock.
 *
 * Ruby C++ Wrapper for FlowBlock.
 *
 */

#ifndef FLOWBOXRUBY_EXT_RB_FLOWBLOCK_H_
#define FLOWBOXRUBY_EXT_RB_FLOWBLOCK_H_

#include <flowbox/flow_container_pool.h>
#include <flowbox/flow_container_buffer.h>

#include "ruby.h"

struct Flowblock {
  FlowContainerPool* container_pool_;
  FlowContainerBuffer* input_;
  FlowContainerBuffer* output_;
};

#define RB_FLOWBLOCK_UNWRAP Flowblock* fb; Data_Get_Struct(self, Flowblock, fb);
void rb_flowblock_free(void *p);
VALUE rb_flowblock_alloc(VALUE klass);
VALUE rb_flowblock_set_input(VALUE self, VALUE input);
VALUE rb_flowblock_set_output(VALUE self, VALUE output);
VALUE rb_flowblock_each(VALUE self);

#endif  // FLOWBOXRUBY_EXT_RB_FLOWBLOCK_H_
