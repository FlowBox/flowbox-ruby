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
 * @file   rb_prefix.h
 * @author Dominik Schatzmann <schadomi@gmail.com>
 * @date   October, 2012
 * @brief  Ruby C++ Wrapper for Prefix class.
 *
 * Ruby C++ Wrapper for Prefix class.
 *
 */

#ifndef FLOWBOXRUBY_EXT_RB_PREFIX_H_
#define FLOWBOXRUBY_EXT_RB_PREFIX_H_

#include "ruby.h"
#include <flowbox/prefix.h>

#define RB_PREFIX_UNWRAP Prefix* prefix; Data_Get_Struct(self, Prefix, prefix);

void rb_init_prefix(void);

void rb_prefix_free(void *p);
VALUE rb_prefix_alloc(VALUE klass);

VALUE rb_prefix_to_s(VALUE self);

// ACCESSOR
VALUE rb_prefix_getter_valid(VALUE self);
VALUE rb_prefix_getter_family(VALUE self);
VALUE rb_prefix_getter_length(VALUE self);
VALUE rb_prefix_getter_max_length(VALUE self);
VALUE rb_prefix_getter_bit_at(VALUE self, VALUE position);

// MODIFIER
VALUE rb_prefix_from(VALUE self, VALUE prefix_txt);
VALUE rb_prefix_clear(VALUE self);
VALUE rb_prefix_cut(VALUE self, VALUE position);

// COMPARE
VALUE rb_prefix_common_bits(VALUE self, VALUE other);
VALUE rb_prefix_includes(VALUE self, VALUE other);
VALUE rb_prefix_equal(VALUE self, VALUE other);

#endif  // FLOWBOXRUBY_EXT_RB_PREFIX_H_
