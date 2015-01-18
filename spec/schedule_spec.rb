require 'helpers'

describe FactoryElf::Schedule do
  describe "class method" do
    describe "#new" do
      it "instantiates a new empty schedule" do
        s = FactoryElf::Schedule.new
        expect( s ).to be_a FactoryElf::Schedule
        expect( s.data ).to eql NArray.int(3, 10_000_000)
      end
    end
  end

  describe "instance method" do
    before :each do
      @s = FactoryElf::Schedule.new
    end

    describe "fetch_item" do
      it "returns ScheduleItem object with data from schedule" do
        si = @s.fetch_item( 75 )
        expect( si ).to be_a FactoryElf::ScheduleItem
        expect( si.toy_id ).to eql 75
        expect( si.elf_id ).to eql 0
        expect( si.abs_mins ).to eql 0
        expect( si.duration ).to eql 0
      end

      it "reads data from the schedule NArray as a ScheduleItem" do
        @s.data[0..2, 3_456_789] = [543,80000,45]
        si = @s.fetch_item( 3_456_789 )
        expect( si ).to be_a FactoryElf::ScheduleItem
        expect( si.toy_id ).to eql 3_456_789
        expect( si.elf_id ).to eql 543
        expect( si.abs_mins ).to eql 80000
        expect( si.duration ).to eql 45
      end
    end

    describe "store_item" do
      it "writes data from ScheduleItem to the schedule NArray" do
        si = FactoryElf::ScheduleItem.new( 3_456_789, 543, 80000, 45 )
        @s.store_item( si )
        expect( @s.data[0..2,3_456_789] ).to eql NArray[543,80000,45]
      end

      it "can round-trip ScheduleItems" do
        1000.times do
          toy_id = rand(10_000_000)
          si_orig = FactoryElf::ScheduleItem.new( toy_id, rand(900), 540 + rand(1_000_000), 1 + rand(10000) )
          @s.store_item( si_orig )
          si_copy = @s.fetch_item( toy_id )
          expect( si_copy ).to_not be si_orig
          expect( si_copy.toy_id ).to eql toy_id
          expect( si_copy.elf_id ).to eql si_orig.elf_id
          expect( si_copy.abs_mins ).to eql si_orig.abs_mins
          expect( si_copy.duration ).to eql si_orig.duration
        end
      end
    end
  end
end
