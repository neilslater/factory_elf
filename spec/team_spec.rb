require 'helpers'

describe FactoryElf::Team do
  describe "class method" do
    describe "#new" do
      it "instantiates a new team" do
        t = FactoryElf::Team.new( 540 )
        expect( t ).to be_a FactoryElf::Team
      end
    end
  end

  describe "instance method" do
    before :each do
      @t = FactoryElf::Team.new( 540 )
    end

    describe "#first_worker" do
      it "returns a FactoryElf::Worker" do
        w = @t.first_worker
        expect( w ).to be_a FactoryElf::Worker
      end

      it "returns the first elf (with id 0) by default" do
        w = @t.first_worker
        expect( w.id ).to eql 0
      end
    end

    describe "#update_first_worker" do
      it "writes new values to the team structure" do
        w = @t.first_worker
        w.update_for_toy_build( 540, 600 )
        expect( @t.first_worker.id ).to eql 0
        expect( @t.first_worker.productivity ).to eql 1.0
        expect( @t.first_worker.ready_abs_mins ).to eql 540
        @t.update_first_worker( w )
        expect( @t.first_worker.id ).to eql 0
        expect( @t.first_worker.productivity ).to be_within(0.001).of 1.219
        expect( @t.first_worker.ready_abs_mins ).to eql 1980
      end
    end

    describe "#sort_first_worker" do
      it "has no effect if there has been no update to first worker's availability" do
        5.times do
          @t.sort_first_worker
          expect( @t.first_worker.id ).to eql 0
          expect( @t.first_worker.productivity ).to eql 1.0
          expect( @t.first_worker.ready_abs_mins ).to eql 540
        end
      end

      it "makes next elf available if first worker has been moved" do
        w = @t.first_worker
        w.update_for_toy_build( 540, 600 )
        @t.update_first_worker( w )

        @t.sort_first_worker

        expect( @t.first_worker.id ).to eql 1
        expect( @t.first_worker.productivity ).to eql 1.0
        expect( @t.first_worker.ready_abs_mins ).to eql 540
      end

      it "loops through equivalent times by id" do
        (0..899).each do |expect_id|
          w = @t.first_worker
          expect( w.id ).to eql expect_id
          expect( w.productivity ).to eql 1.0
          expect( w.ready_abs_mins ).to eql 540
          w.update_for_toy_build( 540, 600 )
          @t.update_first_worker( w )
          @t.sort_first_worker
        end

        (0..899).each do |expect_id|
          w = @t.first_worker
          expect( w.id ).to eql expect_id
          expect( w.productivity ).to be_within(0.001).of 1.219
          expect( w.ready_abs_mins ).to eql 1980
          w.update_for_toy_build( 1980, 60 )
          @t.update_first_worker( w )
          @t.sort_first_worker
        end

        (0..899).each do |expect_id|
          w = @t.first_worker
          expect( w.id ).to eql expect_id
          expect( w.productivity ).to be_within(0.001).of 1.239
          expect( w.ready_abs_mins ).to eql 2030
          w.update_for_toy_build( 2100, 60 )
          @t.update_first_worker( w )
          @t.sort_first_worker
        end
      end

      it "sorts by ready time first, then id" do
        (0..899).each do |expect_id|
          w = @t.first_worker
          expect( w.id ).to eql expect_id
          expect( w.productivity ).to eql 1.0
          expect( w.ready_abs_mins ).to eql 540
          w.update_for_toy_build( 540, 900 - expect_id )
          @t.update_first_worker( w )
          @t.sort_first_worker
        end

        (0..899).each do |i|
          expect_id = 899 - i
          w = @t.first_worker
          expect( w.id ).to eql expect_id
          w.update_for_toy_build( 12000, 1000 )
          @t.update_first_worker( w )
          @t.sort_first_worker
        end

        all_w = @t.all_workers
        all_w_properties = all_w.map {|w| [w.ready_abs_mins, w.id, w.productivity] }
        expect( all_w_properties ).to eql all_w_properties.sort
      end
    end
  end
end
