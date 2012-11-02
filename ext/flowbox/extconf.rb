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
# FlowBox: Make Makefile
################################################################################
# use this file to autogenerate the Makefile to build the c++ extensions

# $> rvm use current ruby
# $> ruby extconf.rb --with-flowbox-dir= /path/to/<git_flowbox_csg>/flow_box_cc/
# $> ruby extconf.rb --with-flowbox-dir=/home/schadomi/eth/git_flowbox_csg/flow_box_cc/

## TODO: Analysie if http://rbplusplus.rubyforge.org/  or http://github.com/ffi/ffi
## can be used to replace the manual wrapper code

require 'mkmf'

# add standart C++ libs
$LIBS += " -lstdc++ "
# darf erst verwendet werden, wenn FlowBoxCC auch mit gnu++0x compliert wird.
#$CFLAGS += " -std=gnu++0x "
$CFLAGS += " -Wall -Wextra -g3 "
def error_exit message
  $stderr.puts message
  exit false
end
def error_lib_missing lib, package
  msg  = "ERROR: #{lib} was not found on this system"
  msg += " possible soluation: $> sudo apt-get install #{package}"
  puts msg
end

# require some standart libs
[
  ['stdc++', 'libc6-dev'],
  ['pthread', 'glibc' ],
  ['bz2', 'libbz2-dev' ]
].each do |lib, packet|
  unless have_library(lib)
    error_exit error_lib_missing(lib, packet)
  end
end

# Alternative method to search for the flowbox lib
FlowBoxInclude = []
FlowBoxLib = []

flowbox_cc = ENV['FLOWBOX_CC']
if flowbox_cc != nil
  FlowBoxInclude << "#{flowbox_cc}/include"
  FlowBoxLib << "#{flowbox_cc}/lib"
end
dir_config('flowbox', FlowBoxInclude, FlowBoxLib)

unless have_library('flowbox')
  puts "cc based part of FlowBox was not found on this system"
  puts "Solution 1: Call extconf.rb with the path to the lib"
  puts "  ruby extconf.rb --with-flowbox-dir= /path/to/libflowbox/"
  puts " -- "
  puts "Solution 2: Adjust add the path to <libflowbox>/ to your bash variables"
  puts "   $> FLOWBOX_CC=/path/to/libflowbox/include"
  puts "   $> export FLOWBOX_CC"

  puts " -- "
  exit(-1)
end
CONFIG['warnflags'].gsub!('-Wdeclaration-after-statement', '')
CONFIG['warnflags'].gsub!('-Wimplicit-function-declaration', '')

create_makefile('flowbox/flowbox')

=begin
# Alternative method to search for the flowbox lib
dir_config('flowbox')
# --with-flowbox-include= /path/to/<git_flowbox_csg>/flow_box_cc/include
# --with-flowbox-lib= /path/to/<git_flowbox_csg>/flow_box_cc/lib
# ruby extconf.rb --with-flowbox-include=/home/schadomi/git_flowbox_csg/flow_box_cc/include --with-flowbox-lib=/home/schadomi/git_flowbox_csg/flow_box_cc/lib
unless have_library('flowbox')
  puts "flowbox not found -- fix it first"
  exit(-1)
end
#
=end
