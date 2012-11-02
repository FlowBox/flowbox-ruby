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
    class FanOutFilter
      # configuration
      def configure(configure_h)

        conf_check_input(configure_h)
        conf_check_output_bad(configure_h)
        conf_check_output_good(configure_h)
        conf_check_stat_interval(configure_h)
        conf_check_in_params(configure_h)
        conf_check_out_params(configure_h)

        conf_push(configure_h)
      end

      def conf_check_input(configure_h)
        input = configure_h[:input]
        case input
        when nil
          raise('no input biflow container buffer found')
        when 'null'
          raise('we need an input (null) not accepted')
        end
      end

      def conf_check_output_good(configure_h)
        output = configure_h[:output_good]
        case output
        when nil
          raise('no output_good biflow container buffer found')
        end
      end

      def conf_check_output_bad(configure_h)
        output = configure_h[:output_bad]
        case output
        when nil
          raise('no output_bad biflow container buffer found')
        end
      end

      def conf_check_stat_interval(configure_h)
        stat_interval = configure_h[:stat_interval]
        case stat_interval
        when nil
          raise('no stat_interval found tag found')
        when stat_interval < 0
         raise('kidding? stat interval > 0')
        end
      end

      def conf_check_in_params(configure_h)
        in_bad_min = configure_h[:in_bad_min]
        case in_bad_min
        when nil
          raise('no in_bad_min tag found')
        when in_bad_min < 0
         raise('kidding? in_bad_min >= 0')
        end
        in_ratio_min= configure_h[:in_ratio_min]
        case in_ratio_min
        when nil
          raise('no in_ratio_min tag found')
        when in_ratio_min < 0
         raise('kidding? in_ratio_min >= 0')
        end
      end

      def conf_check_out_params(configure_h)
        out_bad_min = configure_h[:out_bad_min]
        case out_bad_min
        when nil
          raise('no out_bad_min tag found')
        when out_bad_min < 0
         raise('kidding? out_bad_min >= 0')
        end
        out_ratio_min= configure_h[:out_ratio_min]
        case out_ratio_min
        when nil
          raise('no out_ratio_min tag found')
        when out_ratio_min < 0
         raise('kidding? out_ratio_min >= 0')
        end
      end
    end
  end
end
