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
 * @file   rb_bi_flow_cache.cc
 * @author Dominik Schatzmann <schadomi@gmail.com>
 * @author Daniel Aschwanden <nimdanitro@gmail.com>
 * @date   October, 2012
 * @brief  Ruby C++ Wrapper for BiFlowCache.
 *
 * Ruby C++ Wrapper for BiFlowCache.
 *
 */

#include "rb_bi_flow_cache.h"

void rb_bi_flow_cache_free(void *p) {
  delete (BiFlowCache*) p;
  p = NULL;
}

VALUE rb_bi_flow_cache_alloc(VALUE klass) {
  BiFlowCache* p = NULL;
  VALUE obj;
  p = new BiFlowCache();
  if (p == NULL)
    rb_raise(rb_path2class("FlowBox::Core::BiFlowCacheError"), "Out of Memory");

  obj = Data_Wrap_Struct(klass, 0, rb_bi_flow_cache_free, p);
  return (obj);
}

// configuration
VALUE rb_bi_flow_cache_conf_push(VALUE self, VALUE configuration_h) {
  RB_BI_FLOW_CACHE_UNWRAP
  BiFlowCache::Configuration conf_c;

  // input
  VALUE input = rb_hash_aref(configuration_h, ID2SYM(rb_intern("input")));
  if (TYPE(input) == T_DATA) {
    FlowContainerBuffer* input_buf_p;
    Data_Get_Struct(input, FlowContainerBuffer, input_buf_p);
    conf_c.set_input(input_buf_p);
  };

  // output
  VALUE output = rb_hash_aref(configuration_h, ID2SYM(rb_intern("output")));
  if (TYPE(output) == T_DATA) {
    FlowContainerBuffer* output_buf_p;
    Data_Get_Struct(output, FlowContainerBuffer, output_buf_p);
    conf_c.set_output(output_buf_p);
  }

  VALUE output_biflow = rb_hash_aref(configuration_h,
                                     ID2SYM(rb_intern("output_biflow")));
  if (TYPE(output_biflow) == T_DATA) {
    BiFlowContainerBuffer* output_buf_p;
    Data_Get_Struct(output_biflow, BiFlowContainerBuffer, output_buf_p);
    conf_c.set_biflow_output(output_buf_p);
  }

  // stat_interval
  VALUE export_stat_interval = rb_hash_aref(configuration_h,
                                            ID2SYM(rb_intern("stat_interval")));
  Check_Type(export_stat_interval, T_FIXNUM);
  conf_c.set_stat_interval(NUM2INT(export_stat_interval));

  // prune interval
  VALUE export_prune_interval = rb_hash_aref(
      configuration_h, ID2SYM(rb_intern("prune_interval")));
  Check_Type(export_prune_interval, T_FIXNUM);
  conf_c.set_prune_interval(NUM2INT(export_prune_interval));

  // export timeout
  VALUE export_to = rb_hash_aref(configuration_h,
                                 ID2SYM(rb_intern("export_timeout")));
  Check_Type(export_to, T_FIXNUM);
  conf_c.set_export_timeout(NUM2INT(export_to));

  // policy
  VALUE import_policy = rb_hash_aref(configuration_h,
                                     ID2SYM(rb_intern("import_policy")));
  Check_Type(import_policy, T_FIXNUM);
  if (NUM2INT(import_policy) == BiFlowCache::kImportPolicyAddAll)
    conf_c.set_import_policy(BiFlowCache::kImportPolicyAddAll);
  else if (NUM2INT(import_policy) == BiFlowCache::kImportPolicyAddBorderOnly)
    conf_c.set_import_policy(BiFlowCache::kImportPolicyAddBorderOnly);
  else if (NUM2INT(import_policy)
      == BiFlowCache::kImportPolicyAddOutBorderInAll)
    conf_c.set_import_policy(BiFlowCache::kImportPolicyAddOutBorderInAll);
  else
    rb_raise(rb_path2class("FlowBox::Core::BiFlowCacheError"),
             "Unknown Policy Type");

  // push
  biflowcache->conf_push(conf_c);

  return (self);
}

// observation
VALUE rb_bi_flow_cache_obs_get_block(void* data) {
  BiFlowCache* biflowcache = (BiFlowCache*) data;
  BiFlowCache::Observation* obs = new BiFlowCache::Observation();
  try {
    (*obs) = biflowcache->obs_get();  // blocking
  } catch (...) {
    obs->set_valid(false);
  }
  return ((VALUE) obs);
}

VALUE rb_bi_flow_cache_obs_get(VALUE self) {
  RB_BI_FLOW_CACHE_UNWRAP
  VALUE observation = rb_thread_blocking_region(rb_bi_flow_cache_obs_get_block,
                                                biflowcache, RUBY_UBF_IO, 0);
  BiFlowCache::Observation* obs = (BiFlowCache::Observation*) observation;
  VALUE answer = Qnil;
  if (obs->get_valid() == true) {
    answer = rb_hash_new();
    rb_hash_aset(answer, ID2SYM(rb_intern("time_s")),
                 ULL2NUM(obs->get_time_s()));
    rb_hash_aset(answer, ID2SYM(rb_intern("msg")),
                 rb_str_new2((obs->get_message()).c_str()));
  }
  delete obs;
  return (answer);
}

// statistics
VALUE rb_bi_flow_cache_stat_get_block(void* data) {
  BiFlowCache* biflowcache = (BiFlowCache*) data;
  BiFlowCache::Statistics* stat = new BiFlowCache::Statistics();
  try {
    (*stat) = biflowcache->stat_get();  // blocking
  } catch (...) {
    stat->set_valid(false);
  }
  return ((VALUE) stat);
}

VALUE rb_bi_flow_cache_stat_get(VALUE self) {
  RB_BI_FLOW_CACHE_UNWRAP
  VALUE statistic = rb_thread_blocking_region(rb_bi_flow_cache_stat_get_block,
                                              biflowcache, RUBY_UBF_IO, 0);
  BiFlowCache::Statistics* stat = (BiFlowCache::Statistics*) statistic;
  BiFlowPool::Statistics* pool_stats_ = new BiFlowPool::Statistics;
  VALUE answer = Qnil;
  if (stat->valid() == true) {
    answer = rb_hash_new();

    rb_hash_aset(answer, ID2SYM(rb_intern("time_s")), ULL2NUM(stat->time_s()));
    rb_hash_aset(answer, ID2SYM(rb_intern("duration_add")),
                 ULL2NUM(stat->duration_add()));
    rb_hash_aset(answer, ID2SYM(rb_intern("duration_prune")),
                 ULL2NUM(stat->duration_prune()));

    rb_hash_aset(answer, ID2SYM(rb_intern("flows_in")),
                 ULL2NUM(stat->flows_in()));
    rb_hash_aset(answer, ID2SYM(rb_intern("flows_valid")),
                 ULL2NUM(stat->flows_valid()));

    rb_hash_aset(answer, ID2SYM(rb_intern("biflows_new")),
                 ULL2NUM(stat->biflows_new()));
    rb_hash_aset(answer, ID2SYM(rb_intern("biflows_cached")),
                 ULL2NUM(stat->biflows_cached()));
    rb_hash_aset(answer, ID2SYM(rb_intern("prune_too_young")),
                 ULL2NUM(stat->prune_too_young()));
    rb_hash_aset(answer, ID2SYM(rb_intern("prune_exported")),
                 ULL2NUM(stat->prune_exported()));

    rb_hash_aset(answer, ID2SYM(rb_intern("elements")),
                 ULL2NUM(stat->elements()));
    rb_hash_aset(answer, ID2SYM(rb_intern("buckets")),
                 ULL2NUM(stat->buckets()));

    rb_hash_aset(answer, ID2SYM(rb_intern("pool_total")),
                 ULL2NUM(pool_stats_->biflows_total()));
    rb_hash_aset(answer, ID2SYM(rb_intern("pool_available")),
                 ULL2NUM(pool_stats_->biflows_available()));
    rb_hash_aset(answer, ID2SYM(rb_intern("pool_new")),
                 ULL2NUM(pool_stats_->biflows_new()));
    rb_hash_aset(answer, ID2SYM(rb_intern("pool_deleted")),
                 ULL2NUM(pool_stats_->biflows_deleted()));
  }
  delete stat;
  delete pool_stats_;
  return (answer);
}

// data
VALUE rb_bi_flow_cache_data_process_block(void* data) {
  BiFlowCache* biflowcache = (BiFlowCache*) data;
  biflowcache->data_process();
  return (Qnil);
}

VALUE rb_bi_flow_cache_data_process(VALUE self) {
  RB_BI_FLOW_CACHE_UNWRAP
  rb_thread_blocking_region(rb_bi_flow_cache_data_process_block, biflowcache,
                            RUBY_UBF_IO, 0);
  return (self);
}

