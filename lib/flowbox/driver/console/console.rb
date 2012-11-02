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

require 'singleton'
require 'thread'
module FlowBox
  module Driver
    module Console
      class Console
        include Singleton
        def initialize()
          @file = nil
          @output = STDOUT
          @mutex = Mutex.new
        end
        def process(object)
          @mutex.synchronize do
            object.each do |key, data|
              @output.puts "#{key}, #{data}"
            end
            @output.flush()
          end
        end
        def write(data)
          @mutex.synchronize do
            @output.puts "#{data}"
            @output.flush()
          end
        end
        def redirect_to_file(path, mode = 'w')
          @mutex.synchronize do
            if @file != nil
              @file.close()
            end
            @file = File.open(path, mode)
            @output = @file
          end
        end
      end # InputValidator
    end # Console
  end # Driver
end # FlowBox

