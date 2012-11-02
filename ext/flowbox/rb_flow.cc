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
 * @file   rb_flow.cc
 * @author Dominik Schatzmann <schadomi@gmail.com>
 * @date   October, 2012
 * @brief  Ruby C++ Wrapper for Flow class.
 *
 * Ruby C++ Wrapper for Flow class.
 *
 */

#include "rb_flow.h"

/**
 * Free a Flow
 * @param p Pointer to Flow
 */
void rb_flow_free(void *p) {
  delete (Flow*) p;
  p = NULL;
}

/**
 * Allocation function
 */
VALUE rb_flow_alloc(VALUE klass) {
  Flow* p = NULL;
  VALUE obj;
  p = new Flow();
  if (p == NULL)
    rb_raise(rb_path2class("FlowBox::Core::FlowError"), "Out of Memory");

  obj = Data_Wrap_Struct(klass, 0, rb_flow_free, p);
  return (obj);
}

// ACCESSOR: GETTER ------------------------------------------------------------
/**
 * Returns the source address string
 * @param self
 * @return
 */
VALUE rb_flow_getter_addr_src_s(VALUE self) {
  RB_FLOW_UNWRAP
  std::string buf;
  Flow::addr_to_s(buf, flow->addr_src_, flow->addr_length_);
  return (rb_str_new2(buf.c_str()));
}

/**
 * Returns the destination address
 * @param self
 * @return
 */
VALUE rb_flow_getter_addr_dst_s(VALUE self) {
  RB_FLOW_UNWRAP
  std::string buf;
  Flow::addr_to_s(buf, flow->addr_dst_, flow->addr_length_);
  return (rb_str_new2(buf.c_str()));
}

/**
 * Returns the next hop router address
 * @param self
 * @return
 */
VALUE rb_flow_getter_addr_next_s(VALUE self) {
  RB_FLOW_UNWRAP
  std::string buf;
  Flow::addr_to_s(buf, flow->addr_next_, flow->addr_length_);
  return (rb_str_new2(buf.c_str()));
}

/**
 * Returns start_s
 * @param self
 * @return
 */
VALUE rb_flow_getter_start_s(VALUE self) {
  RB_FLOW_UNWRAP
  return (ULL2NUM(flow->start_s_));
}

/**
 * Returns stop_s
 * @param self
 * @return
 */
VALUE rb_flow_getter_stop_s(VALUE self) {
  RB_FLOW_UNWRAP
  return (ULL2NUM(flow->stop_s_));
}

/**
 * Returns stop_s
 * @param self
 * @return
 */
VALUE rb_flow_getter_sys_start_r(VALUE self) {
  RB_FLOW_UNWRAP
  return (ULL2NUM(flow->sys_start_r_));
}

/**
 * Returns stop_s
 * @param self
 * @return
 */
VALUE rb_flow_getter_sys_stop_r(VALUE self) {
  RB_FLOW_UNWRAP
  return (ULL2NUM(flow->sys_stop_r_));
}

/**
 * Returns bytes
 * @param self
 * @return
 */
VALUE rb_flow_getter_bytes(VALUE self) {
  RB_FLOW_UNWRAP
  return (ULL2NUM(flow->bytes_));
}

/**
 * Returns packets
 * @param self
 * @return
 */
VALUE rb_flow_getter_packets(VALUE self) {
  RB_FLOW_UNWRAP
  return (ULL2NUM(flow->packets_));
}

/**
 * Returns if a flownection is valid
 * @param self
 * @return
 */
VALUE rb_flow_getter_valid(VALUE self) {
  RB_FLOW_UNWRAP
  if (flow->valid_ == true)
    return (Qtrue);
  else
    return (Qfalse);
}

/**
 * Returns the source port
 * @param self
 * @return
 */
VALUE rb_flow_getter_port_src(VALUE self) {
  RB_FLOW_UNWRAP
  return (INT2NUM(flow->port_src_));
}

/**
 * Returns the destination port
 * @param self
 * @return
 */
VALUE rb_flow_getter_port_dst(VALUE self) {
  RB_FLOW_UNWRAP
  return (INT2NUM(flow->port_dst_));
}

/**
 * Returns the protocol number
 * @param self
 * @return
 */
VALUE rb_flow_getter_protocol(VALUE self) {
  RB_FLOW_UNWRAP
  return (INT2NUM(flow->protocol_));
}

/**
 * Returns export_device_id_
 * @param self
 * @return
 */
VALUE rb_flow_getter_export_device_id(VALUE self) {
  RB_FLOW_UNWRAP
  return (INT2NUM(flow->export_device_id_));
}

/**
 * Returns in-interface
 * @param self
 * @return
 */
VALUE rb_flow_getter_if_in(VALUE self) {
  RB_FLOW_UNWRAP
  return (INT2NUM(flow->if_in_));
}

/**
 * Returns out-interface
 * @param self
 * @return
 */
VALUE rb_flow_getter_if_out(VALUE self) {
  RB_FLOW_UNWRAP
  return (INT2NUM(flow->if_out_));
}

/**
 * Returns the direction of the flownection
 * @param self
 * @return
 */
VALUE rb_flow_getter_direction(VALUE self) {
  RB_FLOW_UNWRAP
  return (INT2NUM(flow->direction_));
}

/**
 * Returns the border of the flownection
 * @param self
 * @return
 */
VALUE rb_flow_getter_border(VALUE self) {
  RB_FLOW_UNWRAP
  return (INT2NUM(flow->border_));
}

/**
 * Returns the address length
 * @param self
 * @return
 */
VALUE rb_flow_getter_addr_length(VALUE self) {
  RB_FLOW_UNWRAP
  return (INT2NUM(flow->addr_length_));
}

/**
 * Returns the address length
 * @param self
 * @return
 */
VALUE rb_flow_getter_memory_footprint(VALUE self) {
  RB_FLOW_UNWRAP
  return (INT2NUM(Flow::memory_footprint()));
}

// ACCESSOR: SETTER ------------------------------------------------------------
VALUE rb_flow_setter_addr_src_s(VALUE self, VALUE src_s) {
  RB_FLOW_UNWRAP
  int e;
  if ((e = Flow::addr_import_s(RSTRING_PTR(src_s), flow->addr_src_))
      != Flow::kOK) {
    std::cout << e << std::endl;
    rb_raise(rb_path2class("FlowBox::Core::FlowError"), "Address Error");
  }
  return (self);
}

VALUE rb_flow_setter_addr_dst_s(VALUE self, VALUE dst_s) {
  RB_FLOW_UNWRAP
  int e;
  if ((e = Flow::addr_import_s(RSTRING_PTR(dst_s), flow->addr_dst_))
      != Flow::kOK) {
    rb_raise(rb_path2class("FlowBox::Core::FlowError"), "Address Error");
  }
  return (self);
}

VALUE rb_flow_setter_addr_next_s(VALUE self, VALUE next_s) {
  RB_FLOW_UNWRAP
  int e;
  if ((e = Flow::addr_import_s(RSTRING_PTR(next_s), flow->addr_next_))
      != Flow::kOK) {
    rb_raise(rb_path2class("FlowBox::Core::FlowError"), "Address Error");
  }
  return (self);
}

VALUE rb_flow_setter_start_s(VALUE self, VALUE start_s) {
  RB_FLOW_UNWRAP
  flow->start_s_ = NUM2ULL(start_s);
  return (self);
}

VALUE rb_flow_setter_stop_s(VALUE self, VALUE stop_s) {
  RB_FLOW_UNWRAP
  flow->stop_s_ = NUM2ULL(stop_s);
  return (self);
}

VALUE rb_flow_setter_sys_start_r(VALUE self, VALUE sys_start_r) {
  RB_FLOW_UNWRAP
  flow->sys_start_r_ = NUM2ULL(sys_start_r);
  return (self);
}

VALUE rb_flow_setter_sys_stop_r(VALUE self, VALUE sys_stop_r) {
  RB_FLOW_UNWRAP
  flow->sys_stop_r_ = NUM2ULL(sys_stop_r);
  return (self);
}

VALUE rb_flow_setter_packets(VALUE self, VALUE packets) {
  RB_FLOW_UNWRAP
  flow->packets_ = NUM2ULONG(packets);
  return (self);
}

VALUE rb_flow_setter_bytes(VALUE self, VALUE bytes) {
  RB_FLOW_UNWRAP
  flow->bytes_ = NUM2ULONG(bytes);
  return (self);
}

VALUE rb_flow_setter_valid(VALUE self, VALUE valid) {
  RB_FLOW_UNWRAP
  if (valid == Qtrue)
    flow->valid_ = true;
  else
    flow->valid_ = false;
  return (self);
}

VALUE rb_flow_setter_port_src(VALUE self, VALUE port_src) {
  RB_FLOW_UNWRAP
  flow->port_src_ = NUM2UINT(port_src);
  return (self);
}

VALUE rb_flow_setter_port_dst(VALUE self, VALUE port_dst) {
  RB_FLOW_UNWRAP
  flow->port_dst_ = NUM2UINT(port_dst);
  return (self);
}

VALUE rb_flow_setter_protocol(VALUE self, VALUE protocol) {
  RB_FLOW_UNWRAP
  flow->protocol_ = NUM2UINT(protocol);
  return (self);
}

VALUE rb_flow_setter_export_device_id(VALUE self, VALUE export_device_id) {
  RB_FLOW_UNWRAP
  flow->export_device_id_ = NUM2INT(export_device_id);
  return (self);
}

VALUE rb_flow_setter_if_in(VALUE self, VALUE if_in) {
  RB_FLOW_UNWRAP
  flow->if_in_ = NUM2INT(if_in);
  return (self);
}

VALUE rb_flow_setter_if_out(VALUE self, VALUE if_out) {
  RB_FLOW_UNWRAP
  flow->if_out_ = NUM2INT(if_out);
  return (self);
}

VALUE rb_flow_setter_direction(VALUE self, VALUE direction) {
  RB_FLOW_UNWRAP
  flow->direction_ = NUM2INT(direction);
  return (self);
}

VALUE rb_flow_setter_border(VALUE self, VALUE border) {
  RB_FLOW_UNWRAP
  flow->border_ = NUM2INT(border);
  return (self);
}

VALUE rb_flow_setter_addr_length(VALUE self, VALUE addr_length) {
  RB_FLOW_UNWRAP
  int addr_length_i = NUM2INT(addr_length);
  if (addr_length_i != Flow::kAddressLengthIPv4
      && addr_length_i != Flow::kAddressLengthIPv6) {
    rb_raise(rb_path2class("FlowBox::Core::FlowError"),
             "Invalid IP address type");
  } else {
    flow->addr_length_ = addr_length_i;
  }
  return (self);
}

// MODIFIER: -------------------------------------------------------------------
VALUE rb_flow_clear(VALUE self) {
  RB_FLOW_UNWRAP
  flow->clear();
  return (self);
}

VALUE rb_flow_zero(VALUE self) {
  RB_FLOW_UNWRAP
  flow->zero();
  return (self);
}

