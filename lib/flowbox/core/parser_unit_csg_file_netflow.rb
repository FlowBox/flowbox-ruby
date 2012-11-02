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
    class ParserUnitCSGFileNetflow

      def configure(configure_h)

        # ParserConfig Variables   
        # parser general
        conf_check_stat_interval_s(configure_h)
        conf_check_export_time_window_s(configure_h)

        # csg-file specific
        conf_check_source(configure_h)
        conf_check_csgfile_input(configure_h)
        conf_check_csgfile_scan_interval_s(configure_h)
        conf_check_csgfile_remove_files_after(configure_h)

        # buffers
        conf_check_output(configure_h)

        # flow exporters
        conf_check_flow_exporters(configure_h)

        # forward configuration
        conf_push(configure_h)
      end

      # ParserConfig Variables

      # parser general
      def conf_check_stat_interval_s(configure_h)
        stat_interval_s = configure_h[:stat_interval_s]
        if stat_interval_s == nil
          raise('no stat_interval_s tag found')
        elsif stat_interval_s < 0
          raise('kidding? stat_interval_s should be > 0')
        end
      end

      def conf_check_export_time_window_s(configure_h)
        export_time_window_s = configure_h[:export_time_window_s]
        if export_time_window_s == nil
          raise('no export_time_window_s tag found')
        elsif export_time_window_s < 0
          raise('kidding? export_time_window_s > 0')
        end
      end
      # csg-file specific
      def conf_check_source(configure_h)
        source = configure_h[:source]
        case source
        when nil
          raise('no source tag found')
        when File.directory?(source) == false
          raise("'#{source}' is not a valid directory")
        end
      end
      def conf_check_csgfile_input(configure_h)
        csgfile_input = configure_h[:csgfile_input]
        case csgfile_input
        when nil
          raise('no csgfile_input tag found')
        when File.directory?(csgfile_input) == false
          raise("'#{csgfile_input}' is not a valid directory")
        end
      end
      def conf_check_csgfile_scan_interval_s(configure_h)
        csgfile_scan_interval_s = configure_h[:csgfile_scan_interval_s]
        if csgfile_scan_interval_s == nil
          raise('no csgfile_scan_interval_s tag found')
        elsif csgfile_scan_interval_s < -1 and csgfile_scan_interval_s != 0
          raise('kidding? csgfile_scan_interval_s > 0 or -1')
        end
      end
      def conf_check_csgfile_remove_files_after(configure_h)
        csgfile_remove_files_after = configure_h[:csgfile_remove_files_after]
        if csgfile_remove_files_after == nil
          raise('no csgfile_remove_files_after tag found')
        end
      end

      # buffers
      def conf_check_output(configure_h)
        output = configure_h[:output]
        case output
        when nil
          raise('no output buffer found tag found')
        end
      end

      # flow exporters
      def conf_check_flow_exporters(configure_h)
        flow_exporters = configure_h[:flow_exporters]
        if flow_exporters == nil
          raise('no flow_exporters tag found')
        elsif flow_exporters.size() == 0
          raise('no flow_exporters data found')
        elsif flow_exporters.is_a?(Array) == false
          raise('flow_exporters data should be an Array')
        end
        flow_exporters.each do |flow_exporter_h|
          raise('no id tag found') if flow_exporter_h[:id] == nil
          raise('no address tag found') if flow_exporter_h[:address] == nil
          raise('no observation_domain tag found') if flow_exporter_h[:observation_domain] == nil
          case flow_exporter_h[:format]
          when 5 
          when 9
          when nil
            raise('no format tag found')
          else
            raise('invalid number (#{flow_exporter_h[:format]}) for format tag found')
          end
          #puts "#{flow_exporter_h[:id]}, #{flow_exporter_h[:address]}, #{flow_exporter_h[:observation_domain]}, #{flow_exporter_h[:format]}"
          STDOUT.flush()
        end
      end
    end
  end
end


