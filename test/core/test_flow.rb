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

require 'minitest/autorun'

class TestCoreFlow< MiniTest::Unit::TestCase
  def setup
    @flow = FlowBox::Core::Flow.new()
  end
  def test_new_flow
    new_flow = FlowBox::Core::Flow.new()
    assert_equal false, new_flow.valid()
  end
  def test_clear_flow
    @flow.clear()
     assert_equal @flow.valid(), false
  end
  def test_zero_flow
    @flow.zero()
    assert_equal "NA", @flow.addr_src_s()
    assert_equal "NA", @flow.addr_dst_s()
    assert_equal "NA", @flow.addr_next_s()
		assert_equal 0, @flow.start_s()
		assert_equal 0, @flow.stop_s()
		assert_equal 0, @flow.sys_start_r()
		assert_equal 0, @flow.sys_stop_r()
		assert_equal 0, @flow.packets()
		assert_equal 0, @flow.bytes()
    assert_equal false, @flow.valid()
		assert_equal 0, @flow.port_src()
		assert_equal 0, @flow.port_dst()
		assert_equal 0, @flow.protocol()
		assert_equal 0, @flow.export_device_id()
		assert_equal 0, @flow.if_in()
		assert_equal 0, @flow.if_out()
		assert_equal 0, @flow.direction()
		assert_equal 0, @flow.addr_length()
  end
  def test_addr
    [
      ["255.255.255.255", 4],
      ["0.0.0.0", 4],
      ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff", 16],
      ["::", 16]
    ].each do |addr, length|
      # src
      @flow.set_addr_src_s(addr)
      @flow.set_addr_length(length)
      received = @flow.addr_src_s()
      assert_equal received, addr, "set src '#{addr}' is not received '#{received}'"
      @flow.set_addr_src_s("::")
      # dst
      @flow.set_addr_dst_s(addr)
      @flow.set_addr_length(length)
      received = @flow.addr_dst_s()
      assert_equal received, addr, "set dst '#{addr}' is not received '#{received}'"  
      @flow.set_addr_dst_s("::")
      # next
      @flow.set_addr_next_s(addr)
      @flow.set_addr_length(length)
      received = @flow.addr_next_s()
      assert_equal received, addr, "set next '#{addr}' is not received '#{received}'" 
      @flow.set_addr_next_s("::")  
    end
  end
  def test_start_s
    [1,10,100,1000, Time.now.to_i].each do |time|
      # start_s
      @flow.set_start_s(time)
      received = @flow.start_s()      
      assert_equal received, time, "set start_s '#{time}' is not received '#{received}'" 
      @flow.set_start_s(0)
      # stop_s
      @flow.set_stop_s(time)
      received = @flow.stop_s()      
      assert_equal received, time, "set stop_s '#{time}' is not received '#{received}'" 
      @flow.set_stop_s(0)
      # sys_start_r
      @flow.set_sys_start_r(time)
      received = @flow.sys_start_r()      
      assert_equal received, time, "set sys_start_r '#{time}' is not received '#{received}'" 
      @flow.set_sys_start_r(0)
      # sys_stop_r
      @flow.set_sys_stop_r(time)
      received = @flow.sys_stop_r()      
      assert_equal received, time, "set sys_stop_r '#{time}' is not received '#{received}'" 
      @flow.set_sys_stop_r(0)
    end
  end

  def test_sys_packets
    10.times do 
      packets = rand(100000)
      @flow.set_packets(packets)
      received = @flow.packets()
      assert_equal received, packets, "set packets '#{packets}' is not received '#{received}'" 
    end
  end
  def test_sys_bytes
    10.times do 
      bytes = rand(100000)
      @flow.set_bytes(bytes)
      received = @flow.bytes()
      assert_equal received, bytes, "set bytes '#{bytes}' is not received '#{received}'" 
    end
  end
  def test_valid
    [true, false].each  do |valid|
      @flow.set_valid(valid)
      received = @flow.valid()
      assert_equal valid,received, "set valid '#{valid}' is not received '#{received}'" 
    end
  end
  def test_port_src
    10.times do 
      port = rand(65535)
      @flow.set_port_src(port)
      received = @flow.port_src()
      assert_equal port,received, "set port src '#{port}' is not received '#{received}'" 
   end
  end
  def test_port_dst
    10.times do 
      port = rand(65535)
      @flow.set_port_dst(port)
      received = @flow.port_dst()
      assert_equal port,received, "set port dst '#{port}' is not received '#{received}'" 
   end
  end
  def test_protocol
    10.times do 
      protocol = rand(254)
      @flow.set_protocol(protocol)
      received = @flow.protocol()
      assert_equal protocol,received, "set protocol '#{protocol}' is not received '#{received}'" 
   end
  end
  def test_export_device_id
    10.times do 
      id = rand(10000)
      @flow.set_export_device_id(id)
      received = @flow.export_device_id()
      assert_equal id, received, "set export_device_id '#{id}' is not received '#{received}'" 
   end
  end
  def test_if_in
    10.times do 
      set = rand(10000)
      @flow.set_if_in(set)
      received = @flow.if_in()
      assert_equal set, received, "set if_in '#{set}' is not received '#{received}'" 
   end
  end
  def test_if_out
    10.times do 
      set = rand(10000)
      @flow.set_if_out(set)
      received = @flow.if_out()
      assert_equal set, received, "set if_out '#{set}' is not received '#{received}'" 
   end
  end
  def test_direction
    10.times do 
      set = rand(255)
      @flow.set_direction(set)
      received = @flow.direction()
      assert_equal set, received, "set direction '#{set}' is not received '#{received}'" 
   end
  end
  def test_border
    10.times do 
      set = rand(255)
      @flow.set_border(set)
      received = @flow.border()
      assert_equal set, received, "set border '#{set}' is not received '#{received}'" 
   end
  end
  def test_addr_length
    [4,16].each do |set|
      @flow.set_addr_length(set)
      received = @flow.addr_length()
      assert_equal set, received, "set addr_length '#{set}' is not received '#{received}'" 
   end
  end
end
