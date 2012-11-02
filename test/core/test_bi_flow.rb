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

class TestCoreBiFlow< MiniTest::Unit::TestCase
  def setup
    @biflow = FlowBox::Core::BiFlow.new()
  end
  def test_new_bi_flow
    new_bi_flow = FlowBox::Core::BiFlow.new()
    assert_equal false, new_bi_flow.valid()
  end
  def test_new_from_flow
    # TODO!
  end
  def test_reset_bi_flow
    @biflow.reset()
    assert_equal @biflow.valid(), false
    assert_equal "NA", @biflow.in_addr()
    assert_equal "NA", @biflow.out_addr()
    assert_equal "NA", @biflow.next_in_addr()
    assert_equal "NA", @biflow.next_out_addr()

    assert_equal 0, @biflow.in_out_start_s()
    assert_equal 0, @biflow.in_out_stop_s()
    assert_equal 0, @biflow.out_in_start_s()
    assert_equal 0, @biflow.out_in_stop_s()
    assert_equal 0, @biflow.in_out_packets()
    assert_equal 0, @biflow.out_in_packets()
    assert_equal 0, @biflow.in_out_bytes()
    assert_equal 0, @biflow.out_in_bytes()
            
    assert_equal false, @biflow.valid()

    assert_equal 0, @biflow.in_port()
    assert_equal 0, @biflow.out_port()
    assert_equal 0, @biflow.protocol()
    assert_equal(-1 , @biflow.in_out_export_device_id())
    assert_equal(-1, @biflow.out_in_export_device_id())

    assert_equal(-1, @biflow.in_out_if())
    assert_equal(-1, @biflow.out_in_if())
    assert_equal 0, @biflow.addr_length()

    assert_equal 0, @biflow.hkey5()
    assert_equal 0, @biflow.hkey3_in()
    assert_equal 0, @biflow.hkey3_out()
    assert_equal 0, @biflow.hkey1_in()
    assert_equal 0, @biflow.hkey1_out()		
  end
  def test_addr
    [
      ["255.255.255.255", 4],
      ["0.0.0.0", 4],
      ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff", 16],
      ["::", 16]
    ].each do |addr, length|
      # src
      @biflow.set_in_addr(addr)
      @biflow.set_addr_length(length)
      received = @biflow.in_addr()
      assert_equal received, addr, "set in_addr '#{addr}' is not received '#{received}'"
      @biflow.set_in_addr("::")
      # dst
      @biflow.set_out_addr(addr)
      @biflow.set_addr_length(length)
      received = @biflow.out_addr()
      assert_equal received, addr, "set out_addr '#{addr}' is not received '#{received}'"  
      @biflow.set_out_addr("::")
      # next internal
      @biflow.set_next_in_addr(addr)
      @biflow.set_addr_length(length)
      received = @biflow.next_in_addr()
      assert_equal received, addr, "set next_in_addr '#{addr}' is not received '#{received}'" 
      @biflow.set_next_in_addr("::")  
      # next external
      @biflow.set_next_out_addr(addr)
      @biflow.set_addr_length(length)
      received = @biflow.next_out_addr()
      assert_equal received, addr, "set next_out_addr '#{addr}' is not received '#{received}'" 
      @biflow.set_next_out_addr("::")
    end
  end
  def test_timing
    [1,10,100,1000, Time.now.to_i].each do |time|
      # in out start_s
      @biflow.set_in_out_start_s(time)
      received = @biflow.in_out_start_s()      
      assert_equal received, time, "set in_out_start_s '#{time}' is not received '#{received}'" 
      @biflow.set_in_out_start_s(0)
      # out in start s
      @biflow.set_out_in_start_s(time)
      received = @biflow.out_in_start_s()      
      assert_equal received, time, "set out_in_start_s '#{time}' is not received '#{received}'" 
      @biflow.set_in_out_stop_s(0)
      # in out stop_s
      @biflow.set_in_out_stop_s(time)
      received = @biflow.in_out_stop_s()      
      assert_equal received, time, "set in_out_stop_s '#{time}' is not received '#{received}'" 
      @biflow.set_in_out_stop_s(0)
      # out in stop_s
      @biflow.set_out_in_stop_s(time)
      received = @biflow.out_in_stop_s()      
      assert_equal received, time, "set out_in_stop_s '#{time}' is not received '#{received}'" 
      @biflow.set_out_in_stop_s(0)
    end
  end
  def test_packets
    10.times do
      # in->out
      packets = rand(100000)
      @biflow.set_in_out_packets(packets)
      received = @biflow.in_out_packets()
      assert_equal received, packets, "set in_out_packets '#{packets}' is not received '#{received}'" 
      # out->in
      packets = rand(100000)
      @biflow.set_out_in_packets(packets)
      received = @biflow.out_in_packets()
      assert_equal received, packets, "set out_in_packets '#{packets}' is not received '#{received}'"
    end
  end
  def test_bytes
    10.times do
      # in->out
      bytes = rand(100000)
      @biflow.set_in_out_bytes(bytes)
      received = @biflow.in_out_bytes()
      assert_equal received, bytes, "set in_out_bytes '#{bytes}' is not received '#{received}'" 
      # out->in
      bytes = rand(100000)
      @biflow.set_out_in_bytes(bytes)
      received = @biflow.out_in_bytes()
      assert_equal received, bytes, "set out_in_bytes '#{bytes}' is not received '#{received}'"
    end
  end
  def test_valid
    [true, false].each  do |valid|
      @biflow.set_valid(valid)
      received = @biflow.valid()
      assert_equal valid,received, "set valid '#{valid}' is not received '#{received}'" 
    end
  end
  def test_in_port
    10.times do 
      port = rand(65535)
      @biflow.set_in_port(port)
      received = @biflow.in_port()
      assert_equal port,received, "set in_port '#{port}' is not received '#{received}'" 
   end
  end
  def test_out_port
    10.times do 
      port = rand(65535)
      @biflow.set_out_port(port)
      received = @biflow.out_port()
      assert_equal port,received, "set out_port '#{port}' is not received '#{received}'" 
   end
  end
  def test_protocol
    10.times do 
      protocol = rand(254)
      @biflow.set_protocol(protocol)
      received = @biflow.protocol()
      assert_equal protocol,received, "set protocol '#{protocol}' is not received '#{received}'" 
   end
  end
  def test_in_out_export_device_id
    10.times do 
      id = rand(10000)
      @biflow.set_in_out_export_device_id(id)
      received = @biflow.in_out_export_device_id()
      assert_equal id, received, "set in_out_export_device_id '#{id}' is not received '#{received}'" 
   end
  end
  def test_out_in_export_device_id
    10.times do 
      id = rand(10000)
      @biflow.set_out_in_export_device_id(id)
      received = @biflow.out_in_export_device_id()
      assert_equal id, received, "set out_in_export_device_id '#{id}' is not received '#{received}'" 
   end
  end
  def test_out_in_if
    10.times do 
      set = rand(10000)
      @biflow.set_out_in_if(set)
      received = @biflow.out_in_if()
      assert_equal set, received, "set out_in_if '#{set}' is not received '#{received}'" 
   end
  end
  def test_in_out_if
    10.times do 
      set = rand(10000)
      @biflow.set_in_out_if(set)
      received = @biflow.in_out_if()
      assert_equal set, received, "set in_out_if '#{set}' is not received '#{received}'" 
   end
  end
  def test_addr_length
    [4,16].each do |set|
      @biflow.set_addr_length(set)
      received = @biflow.addr_length()
      assert_equal set, received, "set addr_length '#{set}' is not received '#{received}'" 
   end
  end
  # TODO: test hashes
  def test_hash_keys
    biflows = Array.new
    
    [  ["192.168.1.2","10.10.1.2", 4,17, 1, 2], ["192.168.1.2","10.10.1.2", 4,17, 1, 2],
       ["192.168.1.2","10.10.1.2", 4, 6, 1, 2], ["192.168.1.2","10.10.1.2", 4, 6, 1, 250] ,
       ["192.168.1.2","10.10.1.2", 4, 6, 1, 2], ["192.168.1.2","10.10.1.2", 4,17, 1, 2] ,
       ["192.168.1.3","10.10.1.2", 4, 6, 1, 2], ["192.168.1.2","10.10.1.2", 4, 6, 1, 2],
       ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff","ff80:ff23:ffff:ffff:ffff:ffff:ffff:ffff", 16, 6, 1, 2],
       ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff","ff80:ff23:ffff:ffff:ffff:ffff:ffff:ffff", 16, 6, 1, 2]
    
    ].each do |flow|
  
      bf = FlowBox::Core::BiFlow.new
      bf.set_in_addr(flow[0])
      bf.set_out_addr(flow[1])
      bf.set_addr_length(flow[2])
      bf.set_protocol(flow[3])
      bf.set_in_port(flow[4])
      bf.set_out_port(flow[5])
      bf.prepare_hashes()

      biflows.push(bf)
    end
    
    # HKEY 5 
    assert_equal( biflows[0].hkey5,  biflows[1].hkey5, "HKEY 5 of two equal biflows (IPv4) not equal")
    refute_equal( biflows[2].hkey5,  biflows[3].hkey5, "HKEY 5 of two not equal biflows are equal")
    refute_equal( biflows[4].hkey5,  biflows[5].hkey5, "HKEY 5 of two not equal biflows are equal")
    refute_equal( biflows[6].hkey5,  biflows[7].hkey5, "HKEY 5 of two not equal biflows are equal")
    assert_equal( biflows[8].hkey5,  biflows[9].hkey5, "HKEY 5 of two equal biflows (IPv6) not equal")
    
    # HKEY 3
    assert_equal( biflows[0].hkey3_in,  biflows[1].hkey3_in,  "HKEY 3 (in) of two equal biflows (IPv4) not equal")
    assert_equal( biflows[0].hkey3_out, biflows[1].hkey3_out, "HKEY 3 (out) of two equal biflows (IPv4) not equal")
    assert_equal( biflows[2].hkey3_in,  biflows[3].hkey3_in,  "HKEY 3 (in) of two equal (addr,addr_len, protocol, port) (IPv4) not equal")
    refute_equal( biflows[2].hkey3_out, biflows[3].hkey3_out, "HKEY 3 (out) of not equal out_port are equal")
    refute_equal( biflows[4].hkey3_in,  biflows[5].hkey3_in,  "HKEY 3 (in) of not equal protocol are equal")
    refute_equal( biflows[4].hkey3_out, biflows[5].hkey3_out, "HKEY 3 (out) of not equal protocol are equal")
    refute_equal( biflows[6].hkey3_in,  biflows[7].hkey3_in,  "HKEY 3 (in) of not equal in_addr are equal")
    assert_equal( biflows[6].hkey3_out, biflows[7].hkey3_out, "HKEY 3 (out) of equal out_addr are not equal")
    assert_equal( biflows[8].hkey3_in,  biflows[9].hkey3_in,  "HKEY 3 (in) of two equal biflows (IPv6) not equal")
    assert_equal( biflows[8].hkey3_out, biflows[9].hkey3_out, "HKEY 3 (out) of two equal biflows (IPv6) not equal")
     
    # HKEY 1
    assert_equal( biflows[0].hkey1_in,  biflows[1].hkey1_in,  "HKEY 1 (in) of two equal biflows (IPv4) not equal")
    assert_equal( biflows[0].hkey1_out, biflows[1].hkey1_out, "HKEY 1 (out) of two equal biflows (IPv4) not equal")
    assert_equal( biflows[2].hkey1_in,  biflows[3].hkey1_in,  "HKEY 1 (in) of two equal in_addr (IPv4) not equal")
    assert_equal( biflows[2].hkey1_out, biflows[3].hkey1_out, "HKEY 1 (out) of two equal out_addr (IPv4) not equal")
    assert_equal( biflows[4].hkey1_in,  biflows[5].hkey1_in,  "HKEY 1 (in) of two equal in_addr (IPv4) not equal")
    assert_equal( biflows[4].hkey1_out, biflows[5].hkey1_out, "HKEY 1 (out) of two equal out_addr (IPv4) not equal")
    refute_equal( biflows[6].hkey1_in,  biflows[7].hkey1_in,  "HKEY 1 (in) of two not equal in_addr (IPv4) are equal")
    assert_equal( biflows[6].hkey1_out, biflows[7].hkey1_out, "HKEY 1 (out) of two equal out_addr (IPv4) not equal")
    assert_equal( biflows[8].hkey1_in,  biflows[9].hkey1_in,  "HKEY 1 (in) of two equal biflows (IPv6) not equal")
    assert_equal( biflows[8].hkey1_out, biflows[9].hkey1_out, "HKEY 1 (out) of two equal biflows (IPv6) not equal")
  end
end
