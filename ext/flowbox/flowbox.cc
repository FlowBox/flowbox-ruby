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
 * @file   flowbox.cc
 * @author Dominik Schatzmann <schadomi@gmail.com>
 * @author Daniel Aschwanden <nimdanitro@gmail.com>
 * @date   October, 2012
 * @brief  The Ruby FlowBox Library Wrapper for libflowbox
 *
 * The Ruby FlowBox Library Wrapper for libflowbox
 * 
 */

#include <ruby.h>
static VALUE rb_flowbox_o;
static VALUE rb_flowbox;

#include "rb_flow.h"
static VALUE rb_eFlow;
static VALUE rb_cFlow;

#include "rb_flow_container.h"
static VALUE rb_eFlowContainer;
static VALUE rb_cFlowContainer;

#include "rb_flow_container_pool.h"
static VALUE rb_eFlowContainerPool;
static VALUE rb_cFlowContainerPool;

#include "rb_flow_container_buffer.h"
static VALUE rb_eFlowContainerBuffer;
static VALUE rb_cFlowContainerBuffer;

#include "rb_prefix.h"
static VALUE rb_ePrefix;
static VALUE rb_cPrefix;

#include "rb_prefix_to_int_map.h"
static VALUE rb_ePrefixToIntMap;
static VALUE rb_cPrefixToIntMap;

#include "rb_packet.h"
static VALUE rb_ePacket;
static VALUE rb_cPacket;

#include "rb_csg_file.h"
static VALUE rb_eCSGFile;
static VALUE rb_cCSGFile;

#include "rb_parser_unit_csg_file_netflow.h"
static VALUE rb_eParserUnitCSGFileNetflow;
static VALUE rb_cParserUnitCSGFileNetflow;

#include "rb_input_validator.h"
static VALUE rb_eInputValidator;
static VALUE rb_cInputValidator;

#include "rb_border_filter.h"
static VALUE rb_eBorderFilter;
static VALUE rb_cBorderFilter;

#include "rb_topology_filter.h"
static VALUE rb_eTopologyFilter;
static VALUE rb_cTopologyFilter;

#include "rb_in_out_filter.h"
static VALUE rb_eInOutFilter;
static VALUE rb_cInOutFilter;

#include "rb_ip_filter.h"
static VALUE rb_eIPFilter;
static VALUE rb_cIPFilter;

#include "rb_bi_flow.h"
static VALUE rb_eBiFlow;
static VALUE rb_cBiFlow;

#include "rb_bi_flow_cache.h"
static VALUE rb_eBiFlowCache;
static VALUE rb_cBiFlowCache;

#include "rb_fan_out_filter.h"
static VALUE rb_eFanOutFilter;
static VALUE rb_cFanOutFilter;

#include "rb_noise_filter.h"
static VALUE rb_eNoiseFilter;
static VALUE rb_cNoiseFilter;

#include "rb_bi_flow_container.h"
static VALUE rb_eBiFlowContainer;
static VALUE rb_cBiFlowContainer;

#include "rb_bi_flow_container_buffer.h"
static VALUE rb_eBiFlowContainerBuffer;
static VALUE rb_cBiFlowContainerBuffer;

#include "rb_flowblock.h"
static VALUE rb_eFlowblock;
static VALUE rb_cFlowblock;

extern "C" void Init_flowbox() {

  rb_flowbox_o = rb_define_module("FlowBox");
  rb_flowbox = rb_define_module_under(rb_flowbox_o, "Core");

  //------------------------------------------------------------------------------
  // Flow
  //------------------------------------------------------------------------------

  rb_eFlow = rb_define_class_under(rb_flowbox, "FlowError", rb_eStandardError);
  rb_cFlow = rb_define_class_under(rb_flowbox, "Flow", rb_cObject);
  rb_define_alloc_func(rb_cFlow, rb_flow_alloc);

  rb_define_const(rb_cFlow, "DirectionUnknown", INT2FIX(Flow::kDirectionUnknown));
  rb_define_const(rb_cFlow, "DirectionInIn", INT2FIX(Flow::kDirectionInIn));
  rb_define_const(rb_cFlow, "DirectionInOut", INT2FIX(Flow::kDirectionInOut));
  rb_define_const(rb_cFlow, "DirectionOutIn", INT2FIX(Flow::kDirectionOutIn));
  rb_define_const(rb_cFlow, "DirectionOutOut", INT2FIX(Flow::kDirectionOutOut));

  rb_define_const(rb_cFlow, "BorderUnknown", INT2FIX(Flow::kBorderUnknown));
  rb_define_const(rb_cFlow, "BorderIfIn", INT2FIX(Flow::kBorderIfIn));
  rb_define_const(rb_cFlow, "BorderIfOut", INT2FIX(Flow::kBorderIfOut));

  rb_define_const(rb_cFlow, "AddressLengthIPv4", INT2FIX(Flow::kAddressLengthIPv4));
  rb_define_const(rb_cFlow, "AddressLengthIPv6", INT2FIX(Flow::kAddressLengthIPv6));
  rb_define_const(rb_cFlow, "AddressUnknown", rb_str_new2(Flow::kAddressUnknown.c_str()));

  // ACCESSOR: GETTER
  // string
  rb_define_method(rb_cFlow, "addr_src_s", RUBY_METHOD_FUNC(rb_flow_getter_addr_src_s), 0);
  rb_define_method(rb_cFlow, "addr_dst_s", RUBY_METHOD_FUNC(rb_flow_getter_addr_dst_s), 0);
  rb_define_method(rb_cFlow, "addr_next_s", RUBY_METHOD_FUNC(rb_flow_getter_addr_next_s), 0);
  // ULL
  rb_define_method(rb_cFlow, "start_s", RUBY_METHOD_FUNC(rb_flow_getter_start_s), 0);
  rb_define_method(rb_cFlow, "stop_s", RUBY_METHOD_FUNC(rb_flow_getter_stop_s), 0);
  rb_define_method(rb_cFlow, "sys_start_r", RUBY_METHOD_FUNC(rb_flow_getter_sys_start_r), 0);
  rb_define_method(rb_cFlow, "sys_stop_r", RUBY_METHOD_FUNC(rb_flow_getter_sys_stop_r), 0);
  rb_define_method(rb_cFlow, "packets", RUBY_METHOD_FUNC(rb_flow_getter_packets), 0);
  rb_define_method(rb_cFlow, "bytes", RUBY_METHOD_FUNC(rb_flow_getter_bytes), 0);
  // True/False
  rb_define_method(rb_cFlow, "valid", RUBY_METHOD_FUNC(rb_flow_getter_valid), 0);
  // Int
  rb_define_method(rb_cFlow, "port_src", RUBY_METHOD_FUNC(rb_flow_getter_port_src), 0);
  rb_define_method(rb_cFlow, "port_dst", RUBY_METHOD_FUNC(rb_flow_getter_port_dst), 0);
  rb_define_method(rb_cFlow, "protocol", RUBY_METHOD_FUNC(rb_flow_getter_protocol), 0);
  rb_define_method(rb_cFlow, "export_device_id", RUBY_METHOD_FUNC(rb_flow_getter_export_device_id), 0);
  rb_define_method(rb_cFlow, "if_in", RUBY_METHOD_FUNC(rb_flow_getter_if_in), 0);
  rb_define_method(rb_cFlow, "if_out", RUBY_METHOD_FUNC(rb_flow_getter_if_out), 0);
  rb_define_method(rb_cFlow, "direction", RUBY_METHOD_FUNC(rb_flow_getter_direction), 0);
  rb_define_method(rb_cFlow, "border", RUBY_METHOD_FUNC(rb_flow_getter_border), 0);
  rb_define_method(rb_cFlow, "addr_length", RUBY_METHOD_FUNC(rb_flow_getter_addr_length), 0);

  // ACCESSOR: SETTER
  // string
  rb_define_method(rb_cFlow, "set_addr_src_s", RUBY_METHOD_FUNC(rb_flow_setter_addr_src_s), 1);
  rb_define_method(rb_cFlow, "set_addr_dst_s", RUBY_METHOD_FUNC(rb_flow_setter_addr_dst_s), 1);
  rb_define_method(rb_cFlow, "set_addr_next_s", RUBY_METHOD_FUNC(rb_flow_setter_addr_next_s), 1);
  // ULL
  rb_define_method(rb_cFlow, "set_start_s", RUBY_METHOD_FUNC(rb_flow_setter_start_s), 1);
  rb_define_method(rb_cFlow, "set_stop_s", RUBY_METHOD_FUNC(rb_flow_setter_stop_s), 1);
  rb_define_method(rb_cFlow, "set_sys_start_r", RUBY_METHOD_FUNC(rb_flow_setter_sys_start_r), 1);
  rb_define_method(rb_cFlow, "set_sys_stop_r", RUBY_METHOD_FUNC(rb_flow_setter_sys_stop_r), 1);
  rb_define_method(rb_cFlow, "set_packets", RUBY_METHOD_FUNC(rb_flow_setter_packets), 1);
  rb_define_method(rb_cFlow, "set_bytes", RUBY_METHOD_FUNC(rb_flow_setter_bytes), 1);
  // True/False
  rb_define_method(rb_cFlow, "set_valid", RUBY_METHOD_FUNC(rb_flow_setter_valid), 1);
  // Int
  rb_define_method(rb_cFlow, "set_port_src", RUBY_METHOD_FUNC(rb_flow_setter_port_src), 1);
  rb_define_method(rb_cFlow, "set_port_dst", RUBY_METHOD_FUNC(rb_flow_setter_port_dst), 1);
  rb_define_method(rb_cFlow, "set_protocol", RUBY_METHOD_FUNC(rb_flow_setter_protocol), 1);
  rb_define_method(rb_cFlow, "set_export_device_id", RUBY_METHOD_FUNC(rb_flow_setter_export_device_id), 1);
  rb_define_method(rb_cFlow, "set_if_in", RUBY_METHOD_FUNC(rb_flow_setter_if_in), 1);
  rb_define_method(rb_cFlow, "set_if_out", RUBY_METHOD_FUNC(rb_flow_setter_if_out), 1);
  rb_define_method(rb_cFlow, "set_direction", RUBY_METHOD_FUNC(rb_flow_setter_direction), 1);
  rb_define_method(rb_cFlow, "set_border", RUBY_METHOD_FUNC(rb_flow_setter_border), 1);
  rb_define_method(rb_cFlow, "set_addr_length", RUBY_METHOD_FUNC(rb_flow_setter_addr_length), 1);

  // MODIFIER:
  rb_define_method(rb_cFlow, "clear", RUBY_METHOD_FUNC(rb_flow_clear), 0);
  rb_define_method(rb_cFlow, "zero", RUBY_METHOD_FUNC(rb_flow_zero), 0);

  // DEBUG
  rb_define_method(rb_cFlow, "memory_footprint", RUBY_METHOD_FUNC(rb_flow_getter_memory_footprint), 0);

  //------------------------------------------------------------------------------
  // FlowContainer
  //------------------------------------------------------------------------------
  rb_eFlowContainer = rb_define_class_under(rb_flowbox, "FlowContainerError", rb_eStandardError);
  rb_cFlowContainer = rb_define_class_under(rb_flowbox, "FlowContainer", rb_cObject);

  rb_define_alloc_func(rb_cFlowContainer, rb_flow_container_alloc);

  rb_define_method(rb_cFlowContainer, "resize", RUBY_METHOD_FUNC(rb_flow_container_resize), 1);
  rb_define_method(rb_cFlowContainer, "capacity", RUBY_METHOD_FUNC(rb_flow_container_capacity), 0);
  rb_define_method(rb_cFlowContainer, "used", RUBY_METHOD_FUNC(rb_flow_container_used), 0);
  rb_define_method(rb_cFlowContainer, "reset", RUBY_METHOD_FUNC(rb_flow_container_reset), 0);
  rb_define_method(rb_cFlowContainer, "each_used", RUBY_METHOD_FUNC(rb_flow_container_each_used), 0);
  rb_define_method(rb_cFlowContainer, "each_all", RUBY_METHOD_FUNC(rb_flow_container_each_all), 0);
  rb_define_method(rb_cFlowContainer, "update_used_by", RUBY_METHOD_FUNC(rb_flow_container_update_used_by), 1);


  //------------------------------------------------------------------------------
  // FlowContainerPool (singleton realized via c++)
  //------------------------------------------------------------------------------
  rb_eFlowContainerPool = rb_define_class_under(rb_flowbox, "FlowContainerPoolError", rb_eStandardError);
  rb_cFlowContainerPool = rb_define_class_under(rb_flowbox, "FlowContainerPool", rb_cObject);
  rb_define_alloc_func(rb_cFlowContainerPool, rb_flow_container_pool_alloc);

  rb_define_method(rb_cFlowContainerPool, "push", RUBY_METHOD_FUNC(rb_flow_container_pool_push), 1);
  rb_define_method(rb_cFlowContainerPool, "pop", RUBY_METHOD_FUNC(rb_flow_container_pool_pop), 0);

  rb_define_const(rb_cFlowContainerPool, "CapacityDefault", INT2FIX(FlowContainerPool::kCapacityDefault));
  rb_define_method(rb_cFlowContainerPool, "set_capacity", RUBY_METHOD_FUNC(rb_flow_container_pool_set_capacity), 1);

  // statistics
  rb_define_method(rb_cFlowContainerPool, "get_stat", RUBY_METHOD_FUNC(rb_flow_container_pool_get_stat), 0);
  rb_define_const(rb_cFlowContainerPool, "StatPop", INT2FIX(FlowContainerPool::kStatPop));
  rb_define_const(rb_cFlowContainerPool, "StatPush", INT2FIX(FlowContainerPool::kStatPush));
  rb_define_const(rb_cFlowContainerPool, "StatMin", INT2FIX(FlowContainerPool::kStatMin));
  rb_define_const(rb_cFlowContainerPool, "StatMax", INT2FIX(FlowContainerPool::kStatMax));
  rb_define_const(rb_cFlowContainerPool, "StatUsed", INT2FIX(FlowContainerPool::kStatUsed));
  rb_define_const(rb_cFlowContainerPool, "StatCapacity", INT2FIX(FlowContainerPool::kStatCapacity));

  //------------------------------------------------------------------------------
  // FlowContainerBuffer
  //------------------------------------------------------------------------------
  rb_eFlowContainerBuffer = rb_define_class_under(rb_flowbox, "FlowContainerBufferError", rb_eStandardError);
  rb_cFlowContainerBuffer = rb_define_class_under(rb_flowbox, "FlowContainerBuffer", rb_cObject);
  rb_define_alloc_func(rb_cFlowContainerBuffer, rb_flow_container_buffer_alloc);

  rb_define_method(rb_cFlowContainerBuffer, "push", RUBY_METHOD_FUNC(rb_flow_container_buffer_push), 1);
  rb_define_method(rb_cFlowContainerBuffer, "pop", RUBY_METHOD_FUNC(rb_flow_container_buffer_pop), 0);

  //------------------------------------------------------------------------------
  // Prefix
  //------------------------------------------------------------------------------
  rb_ePrefix = rb_define_class_under(rb_flowbox, "PrefixError", rb_eStandardError);
  rb_cPrefix = rb_define_class_under(rb_flowbox, "Prefix", rb_cObject);
  rb_define_alloc_func(rb_cPrefix, rb_prefix_alloc);

  rb_define_const(rb_cPrefix, "FamilyUnkown", INT2FIX(Prefix::kFamilyUnkown));
  rb_define_const(rb_cPrefix, "FamilyIPv4", INT2FIX(Prefix::kFamilyIPv4));
  rb_define_const(rb_cPrefix, "FamilyIPv6", INT2FIX(Prefix::kFamilyIPv6));

  //GETTER
  rb_define_method(rb_cPrefix, "valid", RUBY_METHOD_FUNC(rb_prefix_getter_valid), 0);
  rb_define_method(rb_cPrefix, "family", RUBY_METHOD_FUNC(rb_prefix_getter_family), 0);
  rb_define_method(rb_cPrefix, "length", RUBY_METHOD_FUNC(rb_prefix_getter_length), 0);
  rb_define_method(rb_cPrefix, "max_length", RUBY_METHOD_FUNC(rb_prefix_getter_max_length), 0);
  rb_define_method(rb_cPrefix, "bit_at", RUBY_METHOD_FUNC(rb_prefix_getter_bit_at), 1);
  rb_define_method(rb_cPrefix, "to_s", RUBY_METHOD_FUNC(rb_prefix_to_s), 0);

  // MODIFIER
  rb_define_method(rb_cPrefix, "from", RUBY_METHOD_FUNC(rb_prefix_from), 1);
  rb_define_method(rb_cPrefix, "clear", RUBY_METHOD_FUNC(rb_prefix_clear), 0);
  rb_define_method(rb_cPrefix, "cut", RUBY_METHOD_FUNC(rb_prefix_cut), 1);

  // COMPARE
  rb_define_method(rb_cPrefix, "common_bits", RUBY_METHOD_FUNC(rb_prefix_common_bits), 1);
  rb_define_method(rb_cPrefix, "includes?", RUBY_METHOD_FUNC(rb_prefix_includes), 1);
  rb_define_method(rb_cPrefix, "equal?", RUBY_METHOD_FUNC(rb_prefix_equal), 1);

  //------------------------------------------------------------------------------
  // PrefixToIntMap
  //------------------------------------------------------------------------------
  rb_ePrefixToIntMap = rb_define_class_under(rb_flowbox, "PrefixToIntMapError", rb_eStandardError);
  rb_cPrefixToIntMap = rb_define_class_under(rb_flowbox, "PrefixToIntMap", rb_cObject);
  rb_define_alloc_func(rb_cPrefixToIntMap, rb_prefix_to_int_map_alloc);

  rb_define_method(rb_cPrefixToIntMap, "insert", RUBY_METHOD_FUNC(rb_prefix_to_int_map_insert), 2);
  rb_define_method(rb_cPrefixToIntMap, "lookup", RUBY_METHOD_FUNC(rb_prefix_to_int_map_lookup), 1);
  rb_define_method(rb_cPrefixToIntMap, "set_value_not_found", RUBY_METHOD_FUNC(rb_prefix_to_int_map_set_value_not_found), 1);
  rb_define_method(rb_cPrefixToIntMap, "get_value_not_found", RUBY_METHOD_FUNC(rb_prefix_to_int_map_get_value_not_found), 0);

  //------------------------------------------------------------------------------
  // Packet
  //------------------------------------------------------------------------------
  rb_ePacket = rb_define_class_under(rb_flowbox, "PacketError", rb_eStandardError);
  rb_cPacket = rb_define_class_under(rb_flowbox, "Packet", rb_cObject);

  rb_define_alloc_func(rb_cPacket, rb_packet_alloc);

  rb_define_method(rb_cPacket, "get_length", RUBY_METHOD_FUNC(rb_packet_get_length),0);
  rb_define_method(rb_cPacket, "get_addr", RUBY_METHOD_FUNC(rb_packet_get_addr),0);
  rb_define_method(rb_cPacket, "get_payload", RUBY_METHOD_FUNC(rb_packet_get_payload),0);

  //------------------------------------------------------------------------------
  // CSGFile
  //------------------------------------------------------------------------------
  rb_eCSGFile = rb_define_class_under(rb_flowbox, "CSGFileError", rb_eStandardError);
  rb_cCSGFile = rb_define_class_under(rb_flowbox, "CSGFile", rb_cObject);

  rb_define_alloc_func(rb_cCSGFile, rb_csg_file_alloc);

  rb_define_method(rb_cCSGFile, "process_file", RUBY_METHOD_FUNC(rb_csg_file_process_file), 2);
  rb_define_method(rb_cCSGFile, "get_packet", RUBY_METHOD_FUNC(rb_csg_file_get_packet),1);
  rb_define_method(rb_cCSGFile, "get_stream_state", RUBY_METHOD_FUNC(rb_csg_file_get_stream_state),0);
  rb_define_method(rb_cCSGFile, "get_time_r", RUBY_METHOD_FUNC(rb_csg_file_get_time_r),0);
  rb_define_method(rb_cCSGFile, "next", RUBY_METHOD_FUNC(rb_csg_file_next),0);

  //------------------------------------------------------------------------------
  // ParserUnitCSGFileNetflow
  //------------------------------------------------------------------------------
  rb_eParserUnitCSGFileNetflow = rb_define_class_under(rb_flowbox, "ParserUnitCSGFileNetflowError", rb_eStandardError);
  rb_cParserUnitCSGFileNetflow = rb_define_class_under(rb_flowbox, "ParserUnitCSGFileNetflow", rb_cObject);

  rb_define_alloc_func(rb_cParserUnitCSGFileNetflow, rb_parser_unit_csg_file_netflow_alloc);

  // configuration
  rb_define_method(rb_cParserUnitCSGFileNetflow, "conf_push", RUBY_METHOD_FUNC(rb_parser_unit_csg_file_netflow_conf_push), 1);

  // observation
  rb_define_method(rb_cParserUnitCSGFileNetflow, "obs_get", RUBY_METHOD_FUNC(rb_parser_unit_csg_file_netflow_obs_get), 0);

  // statistics
  rb_define_method(rb_cParserUnitCSGFileNetflow, "stat_get", RUBY_METHOD_FUNC(rb_parser_unit_csg_file_netflow_stat_get), 0);

  // data
  rb_define_method(rb_cParserUnitCSGFileNetflow, "data_process", RUBY_METHOD_FUNC(rb_parser_unit_csg_file_netflow_data_process), 0);

  //------------------------------------------------------------------------------
  // InputValidator
  //------------------------------------------------------------------------------

  rb_eInputValidator = rb_define_class_under(rb_flowbox, "InputValidatorError", rb_eStandardError);
  rb_cInputValidator = rb_define_class_under(rb_flowbox, "InputValidator", rb_cObject);

  rb_define_alloc_func(rb_cInputValidator, rb_input_validator_alloc);

  // configuration
  rb_define_method(rb_cInputValidator, "conf_push", RUBY_METHOD_FUNC(rb_input_validator_conf_push), 1);

  // observation
  rb_define_method(rb_cInputValidator, "obs_get", RUBY_METHOD_FUNC(rb_input_validator_obs_get), 0);

  // statistics
  rb_define_method(rb_cInputValidator, "stat_get", RUBY_METHOD_FUNC(rb_input_validator_stat_get), 0);

  // data
  rb_define_method(rb_cInputValidator, "data_process", RUBY_METHOD_FUNC(rb_input_validator_data_process), 0);

  //----------------------------------------------------------------------------
  // BorderFilter
  //----------------------------------------------------------------------------

  rb_eBorderFilter = rb_define_class_under(rb_flowbox, "BorderFilterError", rb_eStandardError);
  rb_cBorderFilter = rb_define_class_under(rb_flowbox, "BorderFilter", rb_cObject);

  rb_define_alloc_func(rb_cBorderFilter, rb_border_filter_alloc);

  rb_define_const(rb_cBorderFilter, "PurposeOther", INT2FIX(BorderFilter::kOther));
  rb_define_const(rb_cBorderFilter, "PurposeBorder", INT2FIX(BorderFilter::kBorder));
  rb_define_const(rb_cBorderFilter, "PolicyTag", INT2FIX(BorderFilter::kTag));
  rb_define_const(rb_cBorderFilter, "PolicyFilter", INT2FIX(BorderFilter::kFilter));

  // configuration
  rb_define_method(rb_cBorderFilter, "conf_push", RUBY_METHOD_FUNC(rb_border_filter_conf_push), 1);

  // observation
  rb_define_method(rb_cBorderFilter, "obs_get", RUBY_METHOD_FUNC(rb_border_filter_obs_get), 0);

  // statistics
  rb_define_method(rb_cBorderFilter, "stat_get", RUBY_METHOD_FUNC(rb_border_filter_stat_get), 0);

  // data
  rb_define_method(rb_cBorderFilter, "data_process", RUBY_METHOD_FUNC(rb_border_filter_data_process), 0);


  //------------------------------------------------------------------------------
  // TopologyFilter
  //------------------------------------------------------------------------------
  rb_eTopologyFilter = rb_define_class_under(rb_flowbox, "TopologyFilterError", rb_eStandardError);
  rb_cTopologyFilter = rb_define_class_under(rb_flowbox, "TopologyFilter", rb_cObject);

  rb_define_alloc_func(rb_cTopologyFilter, rb_topology_filter_alloc);

  rb_define_const(rb_cTopologyFilter, "FilterAll", INT2NUM(TopologyFilter::Interface::kFilterAll));
  rb_define_const(rb_cTopologyFilter, "FilterIn", INT2NUM(TopologyFilter::Interface::kFilterIn));
  rb_define_const(rb_cTopologyFilter, "FilterOut", INT2NUM(TopologyFilter::Interface::kFilterOut));
  rb_define_const(rb_cTopologyFilter, "FilterNon", INT2NUM(TopologyFilter::Interface::kFilterNon));

  rb_define_const(rb_cTopologyFilter, "StateNormal", INT2NUM(TopologyFilter::Interface::kStateNormal));
  rb_define_const(rb_cTopologyFilter, "StateAlarm", INT2NUM(TopologyFilter::Interface::kStateAlarm));

  // configuration
  rb_define_method(rb_cTopologyFilter, "conf_push", RUBY_METHOD_FUNC(rb_topology_filter_conf_push), 1);

  // observation
  rb_define_method(rb_cTopologyFilter, "obs_get", RUBY_METHOD_FUNC(rb_topology_filter_obs_get), 0);

  // statistics
  rb_define_method(rb_cTopologyFilter, "stat_get", RUBY_METHOD_FUNC(rb_topology_filter_stat_get), 0);

  // data
  rb_define_method(rb_cTopologyFilter, "data_process", RUBY_METHOD_FUNC(rb_topology_filter_data_process), 0);


  //------------------------------------------------------------------------------
  // InOutFilter
  //------------------------------------------------------------------------------

  rb_eInOutFilter = rb_define_class_under(rb_flowbox, "InOutFilterError", rb_eStandardError);
  rb_cInOutFilter = rb_define_class_under(rb_flowbox, "InOutFilter", rb_cObject);

  rb_define_alloc_func(rb_cInOutFilter, rb_in_out_filter_alloc);

  rb_define_const(rb_cInOutFilter, "PolicyTag", INT2FIX(InOutFilter::kTag));
  rb_define_const(rb_cInOutFilter, "PolicyFilter", INT2FIX(InOutFilter::kFilter));

  // configuration
  rb_define_method(rb_cInOutFilter, "conf_push", RUBY_METHOD_FUNC(rb_in_out_filter_conf_push), 1);

  // observation
  rb_define_method(rb_cInOutFilter, "obs_get", RUBY_METHOD_FUNC(rb_in_out_filter_obs_get), 0);

  // statistics
  rb_define_method(rb_cInOutFilter, "stat_get", RUBY_METHOD_FUNC(rb_in_out_filter_stat_get), 0);

  // data
  rb_define_method(rb_cInOutFilter, "data_process", RUBY_METHOD_FUNC(rb_in_out_filter_data_process), 0);

  //------------------------------------------------------------------------------
  // IPFilter
  //------------------------------------------------------------------------------

  rb_eIPFilter = rb_define_class_under(rb_flowbox, "IPFilterError", rb_eStandardError);
  rb_cIPFilter = rb_define_class_under(rb_flowbox, "IPFilter", rb_cObject);

  rb_define_alloc_func(rb_cIPFilter, rb_ip_filter_alloc);

  rb_define_const(rb_cIPFilter, "PolicyKeep", INT2FIX(IPFilter::kKeep));
  rb_define_const(rb_cIPFilter, "PolicyFilter", INT2FIX(IPFilter::kFilter));

  // configuration
  rb_define_method(rb_cIPFilter, "conf_push", RUBY_METHOD_FUNC(rb_ip_filter_conf_push), 1);

  // observation
  rb_define_method(rb_cIPFilter, "obs_get", RUBY_METHOD_FUNC(rb_ip_filter_obs_get), 0);

  // statistics
  rb_define_method(rb_cIPFilter, "stat_get", RUBY_METHOD_FUNC(rb_ip_filter_stat_get), 0);

  // data
  rb_define_method(rb_cIPFilter, "data_process", RUBY_METHOD_FUNC(rb_ip_filter_data_process), 0);


  //------------------------------------------------------------------------------
  // BiFlow
  //------------------------------------------------------------------------------

  rb_eBiFlow = rb_define_class_under(rb_flowbox, "BiFlowError", rb_eStandardError);
  rb_cBiFlow = rb_define_class_under(rb_flowbox, "BiFlow", rb_cObject);

  rb_define_alloc_func(rb_cBiFlow, rb_bi_flow_alloc);

  // ACCESSOR: GETTER
  // string
  rb_define_method(rb_cBiFlow, "in_addr", RUBY_METHOD_FUNC(rb_bi_flow_getter_in_addr_s), 0);
  rb_define_method(rb_cBiFlow, "out_addr", RUBY_METHOD_FUNC(rb_bi_flow_getter_out_addr_s), 0);
  rb_define_method(rb_cBiFlow, "next_in_addr", RUBY_METHOD_FUNC(rb_bi_flow_getter_next_in_addr_s), 0);
  rb_define_method(rb_cBiFlow, "next_out_addr", RUBY_METHOD_FUNC(rb_bi_flow_getter_next_out_addr_s), 0);

  // ULL
  rb_define_method(rb_cBiFlow, "in_out_start_s", RUBY_METHOD_FUNC(rb_bi_flow_getter_in_out_start_s), 0);
  rb_define_method(rb_cBiFlow, "in_out_stop_s", RUBY_METHOD_FUNC(rb_bi_flow_getter_in_out_stop_s), 0);
  rb_define_method(rb_cBiFlow, "out_in_start_s", RUBY_METHOD_FUNC(rb_bi_flow_getter_out_in_start_s), 0);
  rb_define_method(rb_cBiFlow, "out_in_stop_s", RUBY_METHOD_FUNC(rb_bi_flow_getter_out_in_stop_s), 0);

  rb_define_method(rb_cBiFlow, "in_out_packets", RUBY_METHOD_FUNC(rb_bi_flow_getter_in_out_packets), 0);
  rb_define_method(rb_cBiFlow, "in_out_bytes", RUBY_METHOD_FUNC(rb_bi_flow_getter_in_out_packets), 0);
  rb_define_method(rb_cBiFlow, "out_in_packets", RUBY_METHOD_FUNC(rb_bi_flow_getter_out_in_packets), 0);
  rb_define_method(rb_cBiFlow, "out_in_bytes", RUBY_METHOD_FUNC(rb_bi_flow_getter_out_in_packets), 0);

  // True/False
  rb_define_method(rb_cBiFlow, "valid", RUBY_METHOD_FUNC(rb_bi_flow_getter_valid), 0);
  // Int
  rb_define_method(rb_cBiFlow, "in_port", RUBY_METHOD_FUNC(rb_bi_flow_getter_in_port), 0);
  rb_define_method(rb_cBiFlow, "out_port", RUBY_METHOD_FUNC(rb_bi_flow_getter_out_port), 0);
  rb_define_method(rb_cBiFlow, "protocol", RUBY_METHOD_FUNC(rb_bi_flow_getter_protocol), 0);
  rb_define_method(rb_cBiFlow, "in_out_export_device_id", RUBY_METHOD_FUNC(rb_bi_flow_getter_in_out_export_device_id), 0);
  rb_define_method(rb_cBiFlow, "out_in_export_device_id", RUBY_METHOD_FUNC(rb_bi_flow_getter_out_in_export_device_id), 0);
  rb_define_method(rb_cBiFlow, "in_out_if", RUBY_METHOD_FUNC(rb_bi_flow_getter_in_out_if), 0);
  rb_define_method(rb_cBiFlow, "out_in_if", RUBY_METHOD_FUNC(rb_bi_flow_getter_out_in_if), 0);
  rb_define_method(rb_cBiFlow, "addr_length", RUBY_METHOD_FUNC(rb_bi_flow_getter_addr_length), 0);

  // Hashes
  rb_define_method(rb_cBiFlow, "hkey5", RUBY_METHOD_FUNC(rb_bi_flow_getter_hkey5), 0);
  rb_define_method(rb_cBiFlow, "hkey3_in", RUBY_METHOD_FUNC(rb_bi_flow_getter_hkey3_in), 0);
  rb_define_method(rb_cBiFlow, "hkey3_out", RUBY_METHOD_FUNC(rb_bi_flow_getter_hkey3_out), 0);
  rb_define_method(rb_cBiFlow, "hkey1_in", RUBY_METHOD_FUNC(rb_bi_flow_getter_hkey1_in), 0);
  rb_define_method(rb_cBiFlow, "hkey1_out", RUBY_METHOD_FUNC(rb_bi_flow_getter_hkey1_out), 0);


  // ACCESSOR: SETTER
  // string
  rb_define_method(rb_cBiFlow, "set_in_addr", RUBY_METHOD_FUNC(rb_bi_flow_setter_in_addr_s), 1);
  rb_define_method(rb_cBiFlow, "set_out_addr", RUBY_METHOD_FUNC(rb_bi_flow_setter_out_addr_s), 1);
  rb_define_method(rb_cBiFlow, "set_next_in_addr", RUBY_METHOD_FUNC(rb_bi_flow_setter_next_in_addr_s), 1);
  rb_define_method(rb_cBiFlow, "set_next_out_addr", RUBY_METHOD_FUNC(rb_bi_flow_setter_next_out_addr_s), 1);

  // ULL
  rb_define_method(rb_cBiFlow, "set_in_out_start_s", RUBY_METHOD_FUNC(rb_bi_flow_setter_in_out_start_s), 1);
  rb_define_method(rb_cBiFlow, "set_in_out_stop_s", RUBY_METHOD_FUNC(rb_bi_flow_setter_in_out_stop_s), 1);
  rb_define_method(rb_cBiFlow, "set_out_in_start_s", RUBY_METHOD_FUNC(rb_bi_flow_setter_out_in_start_s), 1);
  rb_define_method(rb_cBiFlow, "set_out_in_stop_s", RUBY_METHOD_FUNC(rb_bi_flow_setter_out_in_stop_s), 1);

  rb_define_method(rb_cBiFlow, "set_in_out_packets", RUBY_METHOD_FUNC(rb_bi_flow_setter_in_out_packets), 1);
  rb_define_method(rb_cBiFlow, "set_in_out_bytes", RUBY_METHOD_FUNC(rb_bi_flow_setter_in_out_packets), 1);
  rb_define_method(rb_cBiFlow, "set_out_in_packets", RUBY_METHOD_FUNC(rb_bi_flow_setter_out_in_packets), 1);
  rb_define_method(rb_cBiFlow, "set_out_in_bytes", RUBY_METHOD_FUNC(rb_bi_flow_setter_out_in_packets), 1);

  // True/False
  rb_define_method(rb_cBiFlow, "set_valid", RUBY_METHOD_FUNC(rb_bi_flow_setter_valid), 1);
  // Int
  rb_define_method(rb_cBiFlow, "set_in_port", RUBY_METHOD_FUNC(rb_bi_flow_setter_in_port), 1);
  rb_define_method(rb_cBiFlow, "set_out_port", RUBY_METHOD_FUNC(rb_bi_flow_setter_out_port), 1);
  rb_define_method(rb_cBiFlow, "set_protocol", RUBY_METHOD_FUNC(rb_bi_flow_setter_protocol), 1);
  rb_define_method(rb_cBiFlow, "set_in_out_export_device_id", RUBY_METHOD_FUNC(rb_bi_flow_setter_in_out_export_device_id), 1);
  rb_define_method(rb_cBiFlow, "set_out_in_export_device_id", RUBY_METHOD_FUNC(rb_bi_flow_setter_out_in_export_device_id), 1);
  rb_define_method(rb_cBiFlow, "set_in_out_if", RUBY_METHOD_FUNC(rb_bi_flow_setter_in_out_if), 1);
  rb_define_method(rb_cBiFlow, "set_out_in_if", RUBY_METHOD_FUNC(rb_bi_flow_setter_out_in_if), 1);
  rb_define_method(rb_cBiFlow, "set_addr_length", RUBY_METHOD_FUNC(rb_bi_flow_setter_addr_length), 1);

  // MODIFIER:
  rb_define_method(rb_cBiFlow, "reset", RUBY_METHOD_FUNC(rb_bi_flow_reset), 0);
  rb_define_method(rb_cBiFlow, "prepare_hashes", RUBY_METHOD_FUNC(rb_bi_flow_prepare_hashes), 0);


  //------------------------------------------------------------------------------
  // BiFlowCache
  //------------------------------------------------------------------------------

  rb_eBiFlowCache = rb_define_class_under(rb_flowbox, "BiFlowCacheError", rb_eStandardError);
  rb_cBiFlowCache = rb_define_class_under(rb_flowbox, "BiFlowCache", rb_cObject);

  rb_define_const(rb_cBiFlowCache, "ImportPolicyAddAll", INT2FIX(BiFlowCache::kImportPolicyAddAll));
  rb_define_const(rb_cBiFlowCache, "ImportPolicyAddBorderOnly", INT2FIX(BiFlowCache::kImportPolicyAddBorderOnly));
  rb_define_const(rb_cBiFlowCache, "ImportPolicyAddOutBorderInAll", INT2FIX(BiFlowCache::kImportPolicyAddOutBorderInAll));


  rb_define_alloc_func(rb_cBiFlowCache, rb_bi_flow_cache_alloc);

  // configuration
  rb_define_method(rb_cBiFlowCache, "conf_push", RUBY_METHOD_FUNC(rb_bi_flow_cache_conf_push), 1);

  // observation
  rb_define_method(rb_cBiFlowCache, "obs_get", RUBY_METHOD_FUNC(rb_bi_flow_cache_obs_get), 0);

  // statistics
  rb_define_method(rb_cBiFlowCache, "stat_get", RUBY_METHOD_FUNC(rb_bi_flow_cache_stat_get), 0);

  // data
  rb_define_method(rb_cBiFlowCache, "data_process", RUBY_METHOD_FUNC(rb_bi_flow_cache_data_process), 0);

  //------------------------------------------------------------------------------
  // BiFlowContainerBuffer
  //------------------------------------------------------------------------------
  rb_eBiFlowContainerBuffer = rb_define_class_under(rb_flowbox, "BiFlowContainerBufferError", rb_eStandardError);
  rb_cBiFlowContainerBuffer = rb_define_class_under(rb_flowbox, "BiFlowContainerBuffer", rb_cObject);

  rb_define_alloc_func(rb_cBiFlowContainerBuffer, rb_bi_flow_container_buffer_alloc);

  rb_define_method(rb_cBiFlowContainerBuffer, "push", RUBY_METHOD_FUNC(rb_bi_flow_container_buffer_push), 1);
  rb_define_method(rb_cBiFlowContainerBuffer, "pop", RUBY_METHOD_FUNC(rb_bi_flow_container_buffer_pop), 0);

  //------------------------------------------------------------------------------
  // BiFlowContainer
  //------------------------------------------------------------------------------
  rb_eBiFlowContainer = rb_define_class_under(rb_flowbox, "BiFlowContainerError", rb_eStandardError);
  rb_cBiFlowContainer = rb_define_class_under(rb_flowbox, "BiFlowContainer", rb_cObject);

  rb_define_alloc_func(rb_cBiFlowContainer, rb_bi_flow_container_alloc);

  rb_define_method(rb_cBiFlowContainer, "time", RUBY_METHOD_FUNC(rb_bi_flow_container_time), 1);
  rb_define_method(rb_cBiFlowContainer, "each_valid_biflow", RUBY_METHOD_FUNC(rb_bi_flow_container_each_valid_biflow), 0);
  rb_define_method(rb_cBiFlowContainer, "each_biflow", RUBY_METHOD_FUNC(rb_bi_flow_container_each_biflow), 0);

  //------------------------------------------------------------------------------
  // FanOutFilter
  //------------------------------------------------------------------------------

  rb_eFanOutFilter = rb_define_class_under(rb_flowbox, "FanOutFilterError", rb_eStandardError);
  rb_cFanOutFilter = rb_define_class_under(rb_flowbox, "FanOutFilter", rb_cObject);

  rb_define_alloc_func(rb_cFanOutFilter, rb_fan_out_filter_alloc);

  // configuration
  rb_define_method(rb_cFanOutFilter, "conf_push", RUBY_METHOD_FUNC(rb_fan_out_filter_conf_push), 1);

  // observation
  rb_define_method(rb_cFanOutFilter, "obs_get", RUBY_METHOD_FUNC(rb_fan_out_filter_obs_get), 0);

  // statistics
  rb_define_method(rb_cFanOutFilter, "stat_get", RUBY_METHOD_FUNC(rb_fan_out_filter_stat_get), 0);

  // data
  rb_define_method(rb_cFanOutFilter, "data_process", RUBY_METHOD_FUNC(rb_fan_out_filter_data_process), 0);

  //------------------------------------------------------------------------------
  // NoiseFilter
  //------------------------------------------------------------------------------

  rb_eNoiseFilter = rb_define_class_under(rb_flowbox, "NoiseFilteErrorr", rb_eStandardError);
  rb_cNoiseFilter = rb_define_class_under(rb_flowbox, "NoiseFilter", rb_cObject);

  rb_define_alloc_func(rb_cNoiseFilter, rb_noise_filter_alloc);

  // configuration
  rb_define_method(rb_cNoiseFilter, "conf_push", RUBY_METHOD_FUNC(rb_noise_filter_conf_push), 1);

  // observation
  rb_define_method(rb_cNoiseFilter, "obs_get", RUBY_METHOD_FUNC(rb_noise_filter_obs_get), 0);

  // statistics
  rb_define_method(rb_cNoiseFilter, "stat_get", RUBY_METHOD_FUNC(rb_noise_filter_stat_get), 0);

  // data
  rb_define_method(rb_cNoiseFilter, "data_process", RUBY_METHOD_FUNC(rb_noise_filter_data_process), 0);

  //------------------------------------------------------------------------------
  // RUBY ONLY UNITS -------------------------------------------------------------
  //------------------------------------------------------------------------------

  //------------------------------------------------------------------------------
  // Flowblock
  //------------------------------------------------------------------------------
  rb_eFlowblock = rb_define_class_under(rb_flowbox, "FlowblockError", rb_eStandardError);
  rb_cFlowblock = rb_define_class_under(rb_flowbox, "Flowblock", rb_cObject);
  rb_define_alloc_func(rb_cFlowblock, rb_flowblock_alloc);
  rb_define_method(rb_cFlowblock, "set_input", RUBY_METHOD_FUNC(rb_flowblock_set_input), 1);
  rb_define_method(rb_cFlowblock, "set_output", RUBY_METHOD_FUNC(rb_flowblock_set_output), 1);
  rb_define_method(rb_cFlowblock, "each", RUBY_METHOD_FUNC(rb_flowblock_each), 0);
  rb_define_method(rb_cFlowblock, "blabla", RUBY_METHOD_FUNC(rb_flowblock_each), 0);

}
