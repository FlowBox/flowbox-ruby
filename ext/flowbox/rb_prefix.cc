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
 * @file   rb_prefix.cc
 * @author Dominik Schatzmann <schadomi@gmail.com>
 * @date   October, 2012
 * @brief  Ruby C++ Wrapper for Prefix class.
 *
 * Ruby C++ Wrapper for Prefix class.
 *
 */

#include "rb_prefix.h"

void rb_prefix_free(void *p) {
  delete (Prefix*) p;
  p = NULL;
}

VALUE rb_prefix_alloc(VALUE klass) {
  Prefix* p = NULL;
  VALUE obj;
  p = new Prefix();
  if (p == NULL)
    rb_raise(rb_path2class("StreaMon::Core::PrefixError"), "Out of Memory");

  obj = Data_Wrap_Struct(klass, 0, rb_prefix_free, p);
  return (obj);
}

VALUE rb_prefix_to_s(VALUE self) {
  RB_PREFIX_UNWRAP
  std::string buf;
  buf = prefix->to_s();
  return (rb_str_new2(buf.c_str()));
}

//------------------------------------------------------------------------------
// ACCESSOR
//------------------------------------------------------------------------------
VALUE rb_prefix_getter_valid(VALUE self) {
  RB_PREFIX_UNWRAP
  if (prefix->get_valid() == true)
    return (Qtrue);
  else
    return (Qfalse);
}

VALUE rb_prefix_getter_family(VALUE self) {
  RB_PREFIX_UNWRAP
  VALUE family = INT2FIX(prefix->get_family());
  return (family);
}

VALUE rb_prefix_getter_length(VALUE self) {
  RB_PREFIX_UNWRAP
  VALUE length = INT2FIX(prefix->get_length());
  return (length);
}

VALUE rb_prefix_getter_max_length(VALUE self) {
  RB_PREFIX_UNWRAP
  VALUE max_length = INT2FIX(prefix->get_max_length());
  return (max_length);
}

VALUE rb_prefix_getter_bit_at(VALUE self, VALUE position) {
  RB_PREFIX_UNWRAP
  Check_Type(position, T_FIXNUM);
  VALUE bit;
  try {
    bit = INT2FIX(prefix->get_bit_at(FIX2INT(position)));
  } catch (FlowBoxE e) {
    rb_raise(rb_path2class("FlowBox::Core::PrefixError"),
             "Requested bit is not within the prefix range");
  }
  return (bit);
}

//------------------------------------------------------------------------------
// MODIFIER
//------------------------------------------------------------------------------
VALUE rb_prefix_from(VALUE self, VALUE prefix_txt) {
  RB_PREFIX_UNWRAP
  Check_Type(prefix_txt, T_STRING);
  try {
    prefix->from(RSTRING_PTR(prefix_txt));
  } catch (FlowBoxE e) {
    rb_raise(rb_path2class("FlowBox::Core::PrefixError"), "Invalid Prefix");
  }
  return (self);
}

VALUE rb_prefix_clear(VALUE self) {
  RB_PREFIX_UNWRAP
  prefix->clear();
  return (self);
}

VALUE rb_prefix_cut(VALUE self, VALUE position) {
  RB_PREFIX_UNWRAP
  Check_Type(position, T_FIXNUM);
  try {
    prefix->cut(FIX2INT(position));
  } catch (FlowBoxE e) {
    rb_raise(rb_path2class("FlowBox::Core::PrefixError"),
             "Requested bit is not within the prefix range");
  }
  return (self);
}

//------------------------------------------------------------------------------
// COMPARE
//------------------------------------------------------------------------------
VALUE rb_prefix_common_bits(VALUE self, VALUE other) {
  RB_PREFIX_UNWRAP
  Check_Type(other, T_DATA);
  Prefix* other_c;
  Data_Get_Struct(other, Prefix, other_c);
  int common;
  try {
    common = prefix->common_bits(*other_c);
  } catch (FlowBoxE e) {
    rb_raise(
        rb_path2class("FlowBox::Core::PrefixError"),
        "Common bits can only be computed on prefixes from the same address family");
  }
  return (INT2FIX(common));
}

VALUE rb_prefix_includes(VALUE self, VALUE other) {
  RB_PREFIX_UNWRAP
  Check_Type(other, T_DATA);
  Prefix* other_c;
  Data_Get_Struct(other, Prefix, other_c);
  bool included;
  try {
    included = prefix->includes(*other_c);
  } catch (FlowBoxE e) {
    rb_raise(
        rb_path2class("FlowBox::Core::PrefixError"),
        "Include test can only be performed on prefixes from the same address family");
  }
  if (included)
    return (Qtrue);
  else
    return (Qfalse);
}

VALUE rb_prefix_equal(VALUE self, VALUE other) {
  RB_PREFIX_UNWRAP
  Check_Type(other, T_DATA);
  Prefix* other_c;
  Data_Get_Struct(other, Prefix, other_c);
  if (prefix->eql(*other_c))
    return (Qtrue);
  else
    return (Qfalse);
}

