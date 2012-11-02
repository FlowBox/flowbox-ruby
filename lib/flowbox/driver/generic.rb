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
    class Generic
      attr_reader :finished
      def initialize()
        @core = nil

        @observer_obs_a = []
        @observer_stat_a = []

        @finished = false
        @worker_stat = nil
        @worker_obs = nil
        @worker_data = nil
        @configure = {}
      end

      ## configure the module ##################################################
      def configure(configure_h)
        if @core == nil
          raise "@core == nil: point me to the unit that should be configured"
        elsif configure_h.class != Hash
          raise "configure_h is not of type HASH"
        else
          # delegate work
          @core.configure(configure_h)
        end
      end
      def configure_core(core)
        @core = core
      end

      ## add observers #########################################################
      def register_obs_observer(observer)
        @observer_obs_a << observer
      end
      def register_stat_observer(observer)
        @observer_stat_a << observer
      end

      ## start processing ######################################################
      def start()
        @worker_stat = Thread.new{worker_stat_do()}
        @worker_obs = Thread.new{worker_obs_do()}
        @worker_data = Thread.new{worker_data_do()}
      end

      ## private ###############################################################
      def worker_obs_do()
        begin
          while @finished == false
            answer = @core.obs_get()
            if answer == nil
              # blocking call was canceled due ??
              puts "WARNING Generic::worker_obs_do: Interrupt observed -- skip"
              STDOUT.flush()
            elsif answer.class == Hash
              answer[:msg_type] = 'observation'
              @observer_obs_a.each{|observer| observer.process(answer)}
            else
              # e.g. unknown format 
              puts "WARNING Generic::worker_obs_do: unknown format -- skip"
              STDOUT.flush()
            end
          end
        rescue => e
          p e.backtrace
          $stderr.puts "ERROR Generic::worker_obs_do: '#{e}'"
          $stderr.puts e.backtrace*"\n"
          exit(-1)
        end
      end

      def worker_stat_do()
        begin
          while @finished == false
            answer = @core.stat_get()
            if answer == nil
              # blocking call was canceled due ??
              puts "WARNING Generic::worker_stat_do: Interrupt observed -- skip"
              STDOUT.flush()
            elsif answer.class == Hash
              answer[:msg_type] = 'statistic'
              @observer_stat_a.each{ |observer| observer.process(answer)}
            else
              # e.g. unknown format 
              puts "WARNING Generic::worker_stat_do: unknown format -- skip"
              STDOUT.flush()
            end
          end
        rescue => e
          p e.backtrace
          $stderr.puts "ERROR Generic::worker_stat_do: '#{e}'"
          $stderr.puts e.backtrace*"\n"
          exit(-1)
        end
      end
      def worker_data_do
        @core.data_process()
        @finished = true
      end
    end # Generic
  end # Driver
end # FlowBox

