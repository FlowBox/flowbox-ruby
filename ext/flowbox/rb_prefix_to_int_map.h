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
 * @file   rb_prefix_to_int_map.h
 * @author Dominik Schatzmann <schadomi@gmail.com>
 * @date   October, 2012
 * @brief  Ruby C++ Wrapper for PrefixToIntMap.
 *
 * Ruby C++ Wrapper for PrefixToIntMap.
 *
 */

#ifndef FLOWBOXRUBY_EXT_RB_PREFIX_TO_INT_MAP_H_
#define FLOWBOXRUBY_EXT_RB_PREFIX_TO_INT_MAP_H_

#include "ruby.h"
#include <flowbox/prefix_to_int_map.h>

#define RB_PREFIX_TO_INT_MAP_UNWRAP PrefixToIntMap* ptim; Data_Get_Struct(self, PrefixToIntMap, ptim);

void rb_prefix_to_int_map_free(void *p);
VALUE rb_prefix_to_int_map_alloc(VALUE klass);
VALUE rb_prefix_to_int_map_insert(VALUE self, VALUE prefix, VALUE value);
VALUE rb_prefix_to_int_map_lookup(VALUE self, VALUE prefix);
VALUE rb_prefix_to_int_map_set_value_not_found(VALUE self, VALUE not_found);
VALUE rb_prefix_to_int_map_get_value_not_found(VALUE self);
VALUE rb_prefix_to_int_map_clear(VALUE self);

#endif  // FLOWBOXRUBY_EXT_RB_PREFIX_TO_INT_MAP_H_
