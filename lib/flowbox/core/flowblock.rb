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

module FlowBox
  module Core
    class Flowblock

      # configuration
      def configure(configure_h)
        conf_set_input(configure_h)
        conf_set_output(configure_h)
      end

      def conf_set_input(configure_h)
        input = configure_h[:input]
        case input
        when nil
          raise('no input buffer found tag found')
        when 'NULL'
          raise("no 'NULL' for input buffer not allowed")
        end
        set_input(input);
      end

      def conf_set_output(configure_h)
        output = configure_h[:output]
        case output
        when nil
          raise('no output buffer found tag found')
        end
        set_output(output);
      end
    end
  end
end
