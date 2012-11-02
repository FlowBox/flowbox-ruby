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
 * @file   rb_flowblock.cc
 * @author Dominik Schatzmann <schadomi@gmail.com>
 * @date   October, 2012
 * @brief  Ruby C++ Wrapper for FlowBlock.
 *
 * Ruby C++ Wrapper for FlowBlock.
 *
 */

#include "rb_flowblock.h"

void rb_flowbock_free(void *p) {
  delete (Flowblock*) p;
  p = NULL;
}

VALUE rb_flowblock_alloc(VALUE klass) {
  Flowblock* p = NULL;
  VALUE obj;
  p = new Flowblock();
  if (p == NULL)
    rb_raise(rb_path2class("FlowBox::Core::FlowblockError"), "Out of Memory");

  p->input_ = NULL;
  p->output_ = NULL;
  p->container_pool_ = FlowContainerPool::instance();

  obj = Data_Wrap_Struct(klass, 0, rb_flowbock_free, p);
  return (obj);
}

VALUE rb_flowblock_set_input(VALUE self, VALUE input) {
  RB_FLOWBLOCK_UNWRAP
  if (TYPE(input) == T_DATA)
    Data_Get_Struct(input, FlowContainerBuffer, fb->input_);
  else
    fb->input_ = NULL;
  return (self);
}

VALUE rb_flowblock_set_output(VALUE self, VALUE output) {
  RB_FLOWBLOCK_UNWRAP
  if (TYPE(output) == T_DATA)
    Data_Get_Struct(output, FlowContainerBuffer, fb->output_);
  else
    fb->output_ = NULL;
  return (self);
}

/*
 *  call-seq:
 *     flowblock.each {|flow| block }   -> ary
 *
 *  Calls <i>block</i> once for each flow in +self+, passing that
 *  element as a parameter.
 *
 *  If no block is given, an enumerator is returned instead.
 *
 *     a.each {|flow| print flow.byets, " Bytes \n" }
 *
 *  produces:
 *
 *     32 Bytes
 *     64 Bytes
 *     ...
 */

VALUE rb_flowblock_each(VALUE self) {
  RB_FLOWBLOCK_UNWRAP
  FlowContainer* fc;
  while (true) {

    // ups no input buffer ...
    if (fb->input_ == NULL) {
      std::cout << "NoInputBufferfound" << std::endl;
      rb_raise(rb_path2class("FlowBox::Core::FlowblockError"),
               "NoInputBufferfound");
    }

    fc = fb->input_->pop();

    if (fc == NULL) {
      std::cout << "fc == NULL" << std::endl;
      std::cout.flush();
      sleep(1);

      if (fb->input_->is_state_fin()) {
        // forward the signal to the next buffer (if possible)
        if (fb->output_ != NULL) {
          fb->output_->signal_fin();
        }

        // break the while(true) ...
        // ... and say goodbye
        break;

        // unknown reason
      } else {
        std::cout << "NoFlowContainerError" << std::endl;
        rb_raise(rb_path2class("FlowBox::Core::FlowblockError"),
                 "NoFlowContainerError");
      }
    } else {
      // WORK:
      //std::cout << "."; // << std::endl;
      //std::cout.flush();

      FlowContainer::iterator flow = fc->begin();
      FlowContainer::iterator end = fc->end_used();

      while (flow != end) {
        rb_yield(
            Data_Wrap_Struct(rb_path2class("FlowBox::Core::Flow"), 0, 0,
                             &(*flow)));
        flow++;
      }

      // forward the buffer to the next element
      if (fb->output_ != NULL) {
        fb->output_->push(fc);
      } else {
        fb->container_pool_->push(fc);
      }
    }
  }  // while(true)
  return self;
}
