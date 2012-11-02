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
 * @file   rb_prefix_to_int_map.cc
 * @author Dominik Schatzmann <schadomi@gmail.com>
 * @date   October, 2012
 * @brief  Ruby C++ Wrapper for PrefixToIntMap.
 *
 * Ruby C++ Wrapper for PrefixToIntMap.
 *
 */

#include "rb_prefix_to_int_map.h"

void rb_prefix_to_int_map_free(void *p) {
  delete (PrefixToIntMap*) p;
  p = NULL;
}

VALUE rb_prefix_to_int_map_alloc(VALUE klass) {
  PrefixToIntMap* p = NULL;
  VALUE obj;
  p = new PrefixToIntMap();
  if (p == NULL)
    rb_raise(rb_path2class("FlowBox::Core:PrefixToIntMapError"),
             "Out of Memory");

  obj = Data_Wrap_Struct(klass, 0, rb_prefix_to_int_map_free, p);
  return (obj);
}

// void insert(const Prefix& prefix, int value);
VALUE rb_prefix_to_int_map_insert(VALUE self, VALUE prefix, VALUE value) {
  RB_PREFIX_TO_INT_MAP_UNWRAP
  Prefix* prefix_c;
  Data_Get_Struct(prefix, Prefix, prefix_c);
  ptim->insert(*prefix_c, NUM2INT(value));
  return (self);
}

// int lookup(const Prefix& prefix);
VALUE rb_prefix_to_int_map_lookup(VALUE self, VALUE prefix) {
  RB_PREFIX_TO_INT_MAP_UNWRAP
  Prefix* prefix_c;
  Data_Get_Struct(prefix, Prefix, prefix_c);
  int value = ptim->lookup(*prefix_c);
  return (INT2NUM(value));
}

// void set_value_not_found(int not_found);
VALUE rb_prefix_to_int_map_set_value_not_found(VALUE self, VALUE not_found) {
  RB_PREFIX_TO_INT_MAP_UNWRAP
  ptim->set_value_not_found(NUM2INT(not_found));
  return (self);
}

// int get_value_not_found(void);
VALUE rb_prefix_to_int_map_get_value_not_found(VALUE self) {
  RB_PREFIX_TO_INT_MAP_UNWRAP
  int value = ptim->get_value_not_found();
  return (INT2NUM(value));
}

// void clear(void);
VALUE rb_prefix_to_int_map_clear(VALUE self) {
  RB_PREFIX_TO_INT_MAP_UNWRAP
  ptim->clear();
  return (self);
}

