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
    class NoiseFilter
      # configuration
      def configure(configure_h)

        conf_check_input(configure_h)
        conf_check_output(configure_h)
        conf_check_stat_interval(configure_h)
        conf_check_filter_params(configure_h)

        conf_push(configure_h)
      end

      def conf_check_input(configure_h)
        input_flow = configure_h[:input_flow]
        input_biflow = configure_h[:input_biflow]
        if input_flow == nil and input_biflow == nil
          raise('no input configured: specify input_flow or input_biflow')
        end

        # check that either flow input or biflow input is specified!
        if (input_flow != nil and input_flow != 'NULL') and (input_biflow != nil and input_biflow != 'NULL')
            raise('either specify flow input or biflow input, but not both!')
        end
      end

      def conf_check_output(configure_h)
        output_flow = configure_h[:output_flow]
        output_biflow = configure_h[:output_biflow]
        output_biflow_filtered = configure_h[:output_biflow_filtered]
        input_flow = configure_h[:input_flow]
        input_biflow = configure_h[:input_biflow]

        # check nil inputs
        if output_flow == nil and output_biflow == nil and output_biflow_filtered == nil
            raise('no output configured: specify output_flow, output_biflow or output_bifilow_filtered')
        end

        # if we have biflow output check that we have biflow input
        if output_biflow != nil and output_biflow != 'NULL'
            if input_biflow == nil or input_biflow == 'NULL'
                raise('biflow output is specified but no biflow input (set input_biflow)')
            end
        end

        # if we have biflow output check that we have biflow input
        if output_biflow_filtered != nil and output_biflow_filtered != 'NULL'
            if input_biflow == nil or input_biflow == 'NULL'
                raise('biflow output is specified but no biflow input (set input_biflow)')
            end
        end

        # if flow output is requested check if we have flow input
        if output_flow != nil and output_flow != 'NULL'
            if input_flow == nil or input_flow == 'NULL'
                raise('flow output is specified but no flow input (set input_flow)')
            end
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

      def conf_check_filter_params(configure_h)
        udp_packets_min = configure_h[:udp_packets_min]
        case udp_packets_min
        when nil
          raise('no udp_packets_min parameter found')
        when udp_packets_min < 0
         raise('kidding? udp_packets_min >= 0')
        end

        tcp_packets_min = configure_h[:tcp_packets_min]
        case tcp_packets_min
        when nil
          raise('no tcp_packets_min parameter found')
        when tcp_packets_min < 0
         raise('kidding? tcp_packets_min >= 0')
        end
      end
    end
  end
end
