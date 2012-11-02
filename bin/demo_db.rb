#!/usr/bin/env ruby
# Copyright 2011,2012 Dominik Schatzmann (schadomi@gmail.com)
#
# This file is part of FlowBox. FlowBox is free software: you can redistribute 
# it and/or modify it under the terms of the GNU Lesser General Public 
# License as published by the Free Software Foundation, either version 3 
# of the License, or (at your option) any later version.
#
# FlowBox is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with FlowBox.  If not, see <http://www.gnu.org/licenses/>.

################################################################################
# BACK END USING DB DRIVER                                                     #
################################################################################
# Use this script to postprocess the statistics saved in the DB
#
# $> cd ../flow_box_rails/frontend
# $> rails runner this_file.rb
#
# (sometimes required)
# $> rvm use 1.9.2-p290
# $> RAILS_ENV=production bundle exec rails runner this_file.rb


require 'flowbox'
require 'flowbox-rails'

builder = FlowBox::Driver::DB::Builder.new()
builder.build_chain()
builder.start_chain()
builder.wait_on_finish()
