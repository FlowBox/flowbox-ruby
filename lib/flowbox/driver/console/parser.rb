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
  module Driver
    module Console
      class Parser
        @@format = {
          'unknown' => 0, 
          'netflow' => 1
        }
        @@source = {
          'unknown' => 0,
          'csgfile' => 1
        }
        StatHeader = "# time_s, duration_s, real_time_s, real_duration_s, packets, iv_min_size, iv_format, iv_exporter, iv_export_time_too_small, iv_export_time_too_big"
        def initialize()
          @generic_driver = FlowBox::Driver::Generic.new()
          @console = FlowBox::Driver::Console::Console.instance()
          @generic_driver.register_obs_observer(self)
          @generic_driver.register_stat_observer(self)

          @core = nil
          @format = :unknown
          @source = :unknown
        end
        def configure(configure_h)
          @configure = configure_h
          configure_format(configure_h)
          configure_source(configure_h)
          set_core(configure_h)
          @generic_driver.configure_core(@core)
          @generic_driver.configure(configure_h)
        end
        def configure_format(configure_h)
          format = configure_h[:format]
          case format
          when nil
            raise('no format tag found')
          when @@format[format] == nil
            raise("unknown format '#{@@format[format]}'")
          else
            @format = format
          end
        end
        def configure_source(configure_h)
          source = configure_h[:source]
          case source
          when nil
            raise('no source tag found')
          when @@format[source] == nil
            raise("unknown source '#{@@format[source]}'")
          else
            @source = source
          end
        end
        def set_core(configure_h)
          if @source == 'csgfile' and @format == 'netflow'
            @core = FlowBox::Core::ParserUnitCSGFileNetflow.new()
          else
            raise("parser data of type '#{@format}' from '#{@source}' is not yet supported")
          end
        end
        def start()
          @generic_driver.start()
        end
        def finished()
          return @generic_driver.finished()
        end
        def process(object)
          if object[:msg_type] == 'statistic'
            # we would like to have a specific format
            msg  = "Parser STAT "
            msg += "#{object[:time_s]}, "
            msg += "#{object[:duration_s]}, "
            msg += "#{object[:real_time_s]}, "
            msg += "#{object[:real_duration_s]}, "
            msg += "#{object[:packets]}, "
            msg += "#{object[:iv_min_size]}, "
            msg += "#{object[:iv_format]}, "
            msg += "#{object[:iv_exporter]}, "
            msg += "#{object[:iv_export_time_too_small]}, "
            msg += "#{object[:iv_export_time_too_big]} "
            @console.write msg
          elsif object[:msg_type] == 'observation'
            msg  = "Parser OBS #{object[:time_s]}, #{object[:msg]}"
            @console.write msg
          end
        end
      end # Parser
    end # Consol
  end # Driver
end # Flobox

