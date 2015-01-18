require 'helpers'

describe FactoryElf::ScheduleItem do
  describe "class method" do
    describe "#new" do
      it "instantiates a new item with supplied toy_id, elf_id, schedule time and duration" do
        si = FactoryElf::ScheduleItem.new(10000, 850, 100000, 300)
        expect( si ).to be_a FactoryElf::ScheduleItem
        expect( si.toy_id ).to eql 10000
        expect( si.elf_id ).to eql 850
        expect( si.abs_mins ).to eql 100000
        expect( si.duration ).to eql 300
      end
    end
  end

  describe "instance method" do
    before :each do
      @si = FactoryElf::ScheduleItem.new(10000, 850, 100000, 300)
    end

    describe "#toy_id=" do
      it "changes toy_id" do
        @si.toy_id = 70
        expect( @si.toy_id ).to eql 70
      end

      it "enforces bounds on the allowed value" do
        expect { @si.toy_id = -1 }.to raise_error ArgumentError
        expect { @si.toy_id = 10_000_000 }.to raise_error ArgumentError
        expect( @si.toy_id ).to eql 10000
      end
    end

    describe "#elf_id=" do
      it "changes elf_id" do
        @si.elf_id = 70
        expect( @si.elf_id ).to eql 70
      end

      it "enforces bounds on the allowed value" do
        expect { @si.elf_id = -1 }.to raise_error ArgumentError
        expect { @si.elf_id = 900 }.to raise_error ArgumentError
        expect( @si.elf_id ).to eql 850
      end
    end

    describe "#abs_mins=" do
      it "changes abs_mins" do
        @si.abs_mins = 700
        expect( @si.abs_mins ).to eql 700
      end

      it "enforces bounds on the allowed value" do
        expect { @si.abs_mins = -1 }.to raise_error ArgumentError
        expect( @si.abs_mins ).to eql 100000
      end
    end

    describe "#duration=" do
      it "changes duration" do
        @si.duration = 700
        expect( @si.duration ).to eql 700
      end

      it "enforces bounds on the allowed value" do
        expect { @si.duration = -1 }.to raise_error ArgumentError
        expect( @si.duration ).to eql 300
      end
    end
  end
end
