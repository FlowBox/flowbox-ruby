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
    class BorderFilter
      def configure(configure_h)
        conf_check_input(configure_h)
        conf_check_output(configure_h)
        conf_check_stat_interval(configure_h)
        conf_check_interfaces(configure_h)
        conf_check_policy(configure_h)
        conf_push(configure_h)
      end
      def conf_check_input(configure_h)
        input = configure_h[:input]
        case input
        when nil
          raise('no input buffer found tag found')
        when 'null'
          raise('we need an input (null) not accepted')
        end
      end
      def conf_check_output(configure_h)
        output = configure_h[:output]
        case output
        when nil
          raise('no output buffer tag found')
        end
      end
      def conf_check_stat_interval(configure_h)
        stat_interval = configure_h[:stat_interval]
        case stat_interval
        when nil
          raise('no stat_interval tag found')
        when stat_interval < 0
         raise('kidding? stat interval < 0')
        end
      end
      def conf_check_interfaces(configure_h)
        interfaces = configure_h[:interfaces]
        case interfaces
        when nil
          raise('no interfaces tag found')
        end
      end
      def conf_check_policy(configure_h)
        policy = configure_h[:policy]
        case policy
        when nil
          raise('no policy tag found')
        when PolicyTag
          return
        when PolicyFilter
          return
        else
          raise("Unknown Policy #{policy}")
        end
      end
    end
  end
end
