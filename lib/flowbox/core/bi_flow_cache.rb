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
    class BiFlowCache
      def configure(configure_h)
        conf_check_input(configure_h)
        conf_check_output(configure_h)
        conf_check_output_biflow(configure_h)
        conf_check_stat_interval(configure_h)
        conf_check_prune_interval(configure_h)
        conf_check_export_timeout(configure_h)
        conf_check_import_policy(configure_h)
        conf_push(configure_h)
      end
      def conf_check_input(configure_h)
        input = configure_h[:input]
        case input
        when nil
          raise('no input flow container buffer found')
        when 'null'
          raise('we need an input (null) not accepted')
        end
      end
      def conf_check_output(configure_h)
        output = configure_h[:output]
        case output
        when nil
          raise('no output flow container buffer found')
        end
      end
      def conf_check_output_biflow(configure_h)
        output = configure_h[:output_biflow]
        case output
        when nil
          raise('no output_biflow biflow container buffer found')
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

      def conf_check_prune_interval(configure_h)
        prune_interval = configure_h[:prune_interval]
        case prune_interval
        when nil
          raise('no prune_interval found tag found')
        when prune_interval < 0
         raise('kidding? prune interval > 0')
        end
      end
      def conf_check_export_timeout(configure_h)
        export_timeout = configure_h[:export_timeout]
        case export_timeout
        when nil
          raise('no export_timeout found tag found')
        when export_timeout < 0
         raise('kidding? export_timeout > 0')
        end
      end
      def conf_check_import_policy(configure_h)
        policy = configure_h[:import_policy]
        case policy
        when nil
          raise("no ':import_policy' tag found for BiFlowCache")
        when ImportPolicyAddAll
          return
        when ImportPolicyAddBorderOnly
          return
        when ImportPolicyAddOutBorderInAll
          return
        else
          raise("Unknown import policy #{policy}")
        end
      end
    end
  end
end
