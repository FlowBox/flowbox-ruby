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
 * @file   rb_bi_flow.cc
 * @author Dominik Schatzmann <schadomi@gmail.com>
 * @author Daniel Aschwanden <nimdanitro@gmail.com>
 * @date   October, 2012
 * @brief   Ruby C++ Wrapper for BiFlow class.
 *
 * Ruby C++ Wrapper for BiFlow class.
 *
 */

#include "rb_bi_flow.h"

/**
 * Free a Flow
 * @param p Pointer to Flow
 */
void rb_bi_flow_free(void *p) {
  delete (BiFlow*) p;
  p = NULL;
}

/**
 * Allocation function
 */
VALUE rb_bi_flow_alloc(VALUE klass) {
  BiFlow* p = NULL;
  VALUE obj;
  p = new BiFlow();
  if (p == NULL)
    rb_raise(rb_path2class("FlowBox::Core::BiFlowError"), "Out of Memory");

  obj = Data_Wrap_Struct(klass, 0, rb_bi_flow_free, p);
  return (obj);
}

// ACCESSOR: GETTER ------------------------------------------------------------
/**
 * Returns the source address string
 * @param self
 * @return
 */
VALUE rb_bi_flow_getter_in_addr_s(VALUE self) {
  RB_BI_FLOW_UNWRAP
  std::string buf;
  Flow::addr_to_s(buf, biflow->in_addr_, biflow->addr_length_);
  return (rb_str_new2(buf.c_str()));
}

/**
 * Returns the destination address
 * @param self
 * @return
 */
VALUE rb_bi_flow_getter_out_addr_s(VALUE self) {
  RB_BI_FLOW_UNWRAP
  std::string buf;
  Flow::addr_to_s(buf, biflow->out_addr_, biflow->addr_length_);
  return (rb_str_new2(buf.c_str()));
}

/**
 * Returns the next internal hop router address
 * @param self
 * @return
 */
VALUE rb_bi_flow_getter_next_in_addr_s(VALUE self) {
  RB_BI_FLOW_UNWRAP
  std::string buf;
  Flow::addr_to_s(buf, biflow->next_in_addr_, biflow->addr_length_);
  return (rb_str_new2(buf.c_str()));
}

/**
 * Returns the next external hop router address
 * @param self
 * @return
 */
VALUE rb_bi_flow_getter_next_out_addr_s(VALUE self) {
  RB_BI_FLOW_UNWRAP
  std::string buf;
  Flow::addr_to_s(buf, biflow->next_out_addr_, biflow->addr_length_);
  return (rb_str_new2(buf.c_str()));
}

/**
 * Returns in_out_start_s
 * @param self
 * @return
 */
VALUE rb_bi_flow_getter_in_out_start_s(VALUE self) {
  RB_BI_FLOW_UNWRAP
  return (ULL2NUM(biflow->in_out_start_s_));
}

/**
 * Returns in_out_stop_s
 * @param self
 * @return
 */
VALUE rb_bi_flow_getter_in_out_stop_s(VALUE self) {
  RB_BI_FLOW_UNWRAP
  return (ULL2NUM(biflow->in_out_stop_s_));
}

/**
 * Returns out_in_start_s
 * @param self
 * @return
 */
VALUE rb_bi_flow_getter_out_in_start_s(VALUE self) {
  RB_BI_FLOW_UNWRAP
  return (ULL2NUM(biflow->out_in_start_s_));
}

/**
 * Returns out_in_stop_s
 * @param self
 * @return
 */
VALUE rb_bi_flow_getter_out_in_stop_s(VALUE self) {
  RB_BI_FLOW_UNWRAP
  return (ULL2NUM(biflow->out_in_stop_s_));
}

/**
 * Returns in_out_bytes
 * @param self
 * @return
 */
VALUE rb_bi_flow_getter_in_out_bytes(VALUE self) {
  RB_BI_FLOW_UNWRAP
  return (ULL2NUM(biflow->in_out_bytes_));
}

/**
 * Returns in_out_packets
 * @param self
 * @return
 */
VALUE rb_bi_flow_getter_in_out_packets(VALUE self) {
  RB_BI_FLOW_UNWRAP
  return (ULL2NUM(biflow->in_out_packets_));
}

/**
 * Returns out_in_bytes
 * @param self
 * @return
 */
VALUE rb_bi_flow_getter_out_in_bytes(VALUE self) {
  RB_BI_FLOW_UNWRAP
  return (ULL2NUM(biflow->out_in_bytes_));
}

/**
 * Returns out_in_packets
 * @param self
 * @return
 */
VALUE rb_bi_flow_getter_out_in_packets(VALUE self) {
  RB_BI_FLOW_UNWRAP
  return (ULL2NUM(biflow->out_in_packets_));
}

/**
 * Returns if a biflow is valid
 * @param self
 * @return
 */
VALUE rb_bi_flow_getter_valid(VALUE self) {
  RB_BI_FLOW_UNWRAP
  if (biflow->valid_ == true)
    return (Qtrue);
  else
    return (Qfalse);
}

/**
 * Returns the internal port
 * @param self
 * @return
 */
VALUE rb_bi_flow_getter_in_port(VALUE self) {
  RB_BI_FLOW_UNWRAP
  return (INT2NUM(biflow->in_port_));
}

/**
 * Returns the external port
 * @param self
 * @return
 */
VALUE rb_bi_flow_getter_out_port(VALUE self) {
  RB_BI_FLOW_UNWRAP
  return (INT2NUM(biflow->out_port_));
}

/**
 * Returns the protocol number
 * @param self
 * @return
 */
VALUE rb_bi_flow_getter_protocol(VALUE self) {
  RB_BI_FLOW_UNWRAP
  return (INT2NUM(biflow->protocol_));
}

/**
 * Returns export_device_id_ of in->out flow
 * @param self
 * @return
 */
VALUE rb_bi_flow_getter_in_out_export_device_id(VALUE self) {
  RB_BI_FLOW_UNWRAP
  return (INT2NUM(biflow->in_out_export_device_id_));
}

/**
 * Returns export_device_id_ of out->in flow
 * @param self
 * @return
 */
VALUE rb_bi_flow_getter_out_in_export_device_id(VALUE self) {
  RB_BI_FLOW_UNWRAP
  return (INT2NUM(biflow->out_in_export_device_id_));
}

/**
 * Returns interface of in->out flow
 * @param self
 * @return
 */
VALUE rb_bi_flow_getter_in_out_if(VALUE self) {
  RB_BI_FLOW_UNWRAP
  return (INT2NUM(biflow->in_out_if_));
}

/**
 * Returns interface of in->out flow
 * @param self
 * @return
 */
VALUE rb_bi_flow_getter_out_in_if(VALUE self) {
  RB_BI_FLOW_UNWRAP
  return (INT2NUM(biflow->out_in_if_));
}

/**
 * Returns the address length
 * @param self
 * @return
 */
VALUE rb_bi_flow_getter_addr_length(VALUE self) {
  RB_BI_FLOW_UNWRAP
  return (INT2NUM(biflow->addr_length_));
}

/**
 * Returns the 5TP hkey
 * @param self
 * @return
 */
VALUE rb_bi_flow_getter_hkey5(VALUE self) {
  RB_BI_FLOW_UNWRAP
  return (INT2NUM(biflow->hkey5_));
}

/**
 * Returns the 3TP hkey IN
 * @param self
 * @return
 */
VALUE rb_bi_flow_getter_hkey3_in(VALUE self) {
  RB_BI_FLOW_UNWRAP
  return (INT2NUM(biflow->hkey3_in_));
}

/**
 * Returns the 3TP hkey OUT
 * @param self
 * @return
 */
VALUE rb_bi_flow_getter_hkey3_out(VALUE self) {
  RB_BI_FLOW_UNWRAP
  return (INT2NUM(biflow->hkey3_out_));
}

/**
 * Returns the 1TP hkey IN
 * @param self
 * @return
 */
VALUE rb_bi_flow_getter_hkey1_in(VALUE self) {
  RB_BI_FLOW_UNWRAP
  return (INT2NUM(biflow->hkey1_in_));
}

/**
 * Returns the 1TP hkey OUT
 * @param self
 * @return
 */
VALUE rb_bi_flow_getter_hkey1_out(VALUE self) {
  RB_BI_FLOW_UNWRAP
  return (INT2NUM(biflow->hkey1_out_));
}

// ACCESSOR: SETTER ------------------------------------------------------------
VALUE rb_bi_flow_setter_in_addr_s(VALUE self, VALUE src_s) {
  RB_BI_FLOW_UNWRAP
  int e;
  if ((e = Flow::addr_import_s(RSTRING_PTR(src_s), biflow->in_addr_))
      != Flow::kOK) {
    std::cout << e << std::endl;
    rb_raise(rb_path2class("FlowBox::Core::FlowError"), "Address Error");
  }
  return (self);
}

VALUE rb_bi_flow_setter_out_addr_s(VALUE self, VALUE dst_s) {
  RB_BI_FLOW_UNWRAP
  int e;
  if ((e = Flow::addr_import_s(RSTRING_PTR(dst_s), biflow->out_addr_))
      != Flow::kOK) {
    rb_raise(rb_path2class("FlowBox::Core::FlowError"), "Address Error");
  }
  return (self);
}

VALUE rb_bi_flow_setter_next_in_addr_s(VALUE self, VALUE next_s) {
  RB_BI_FLOW_UNWRAP
  int e;
  if ((e = Flow::addr_import_s(RSTRING_PTR(next_s), biflow->next_in_addr_))
      != Flow::kOK) {
    rb_raise(rb_path2class("FlowBox::Core::FlowError"), "Address Error");
  }
  return (self);
}

VALUE rb_bi_flow_setter_next_out_addr_s(VALUE self, VALUE next_s) {
  RB_BI_FLOW_UNWRAP
  int e;
  if ((e = Flow::addr_import_s(RSTRING_PTR(next_s), biflow->next_out_addr_))
      != Flow::kOK) {
    rb_raise(rb_path2class("FlowBox::Core::FlowError"), "Address Error");
  }
  return (self);
}

VALUE rb_bi_flow_setter_in_out_start_s(VALUE self, VALUE start_s) {
  RB_BI_FLOW_UNWRAP
  biflow->in_out_start_s_ = NUM2ULL(start_s);
  return (self);
}

VALUE rb_bi_flow_setter_in_out_stop_s(VALUE self, VALUE stop_s) {
  RB_BI_FLOW_UNWRAP
  biflow->in_out_stop_s_ = NUM2ULL(stop_s);
  return (self);
}

VALUE rb_bi_flow_setter_out_in_start_s(VALUE self, VALUE start_s) {
  RB_BI_FLOW_UNWRAP
  biflow->out_in_start_s_ = NUM2ULL(start_s);
  return (self);
}

VALUE rb_bi_flow_setter_out_in_stop_s(VALUE self, VALUE stop_s) {
  RB_BI_FLOW_UNWRAP
  biflow->out_in_stop_s_ = NUM2ULL(stop_s);
  return (self);
}

VALUE rb_bi_flow_setter_in_out_packets(VALUE self, VALUE packets) {
  RB_BI_FLOW_UNWRAP
  biflow->in_out_packets_ = NUM2ULONG(packets);
  return (self);
}

VALUE rb_bi_flow_setter_in_out_bytes(VALUE self, VALUE bytes) {
  RB_BI_FLOW_UNWRAP
  biflow->in_out_bytes_ = NUM2ULONG(bytes);
  return (self);
}

VALUE rb_bi_flow_setter_out_in_packets(VALUE self, VALUE packets) {
  RB_BI_FLOW_UNWRAP
  biflow->out_in_packets_ = NUM2ULONG(packets);
  return (self);
}

VALUE rb_bi_flow_setter_out_in_bytes(VALUE self, VALUE bytes) {
  RB_BI_FLOW_UNWRAP
  biflow->out_in_bytes_ = NUM2ULONG(bytes);
  return (self);
}

VALUE rb_bi_flow_setter_valid(VALUE self, VALUE valid) {
  RB_BI_FLOW_UNWRAP
  if (valid == Qtrue)
    biflow->valid_ = true;
  else
    biflow->valid_ = false;
  return (self);
}

VALUE rb_bi_flow_setter_in_port(VALUE self, VALUE port) {
  RB_BI_FLOW_UNWRAP
  biflow->in_port_ = NUM2UINT(port);
  return (self);
}

VALUE rb_bi_flow_setter_out_port(VALUE self, VALUE port) {
  RB_BI_FLOW_UNWRAP
  biflow->out_port_ = NUM2UINT(port);
  return (self);
}

VALUE rb_bi_flow_setter_protocol(VALUE self, VALUE protocol) {
  RB_BI_FLOW_UNWRAP
  biflow->protocol_ = NUM2UINT(protocol);
  return (self);
}

VALUE rb_bi_flow_setter_in_out_export_device_id(VALUE self,
                                                VALUE export_device_id) {
  RB_BI_FLOW_UNWRAP
  biflow->in_out_export_device_id_ = NUM2INT(export_device_id);
  return (self);
}

VALUE rb_bi_flow_setter_out_in_export_device_id(VALUE self,
                                                VALUE export_device_id) {
  RB_BI_FLOW_UNWRAP
  biflow->out_in_export_device_id_ = NUM2INT(export_device_id);
  return (self);
}

VALUE rb_bi_flow_setter_in_out_if(VALUE self, VALUE iface) {
  RB_BI_FLOW_UNWRAP
  biflow->in_out_if_ = NUM2INT(iface);
  return (self);
}

VALUE rb_bi_flow_setter_out_in_if(VALUE self, VALUE iface) {
  RB_BI_FLOW_UNWRAP
  biflow->out_in_if_ = NUM2INT(iface);
  return (self);
}

VALUE rb_bi_flow_setter_addr_length(VALUE self, VALUE addr_length) {
  RB_BI_FLOW_UNWRAP
  int addr_length_i = NUM2INT(addr_length);
  if (addr_length_i != Flow::kAddressLengthIPv4
      && addr_length_i != Flow::kAddressLengthIPv6) {
    rb_raise(rb_path2class("FlowBox::Core::FlowError"),
             "Invalid IP address type");
  } else {
    biflow->addr_length_ = addr_length_i;
  }
  return (self);
}

// MODIFIER: -------------------------------------------------------------------
VALUE rb_bi_flow_reset(VALUE self) {
  RB_BI_FLOW_UNWRAP
  biflow->reset();
  return (self);
}

VALUE rb_bi_flow_prepare_hashes(VALUE self) {
  RB_BI_FLOW_UNWRAP
  biflow->prepare_hashes();
  return (self);
}

