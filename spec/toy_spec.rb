require 'helpers'

describe FactoryElf::Toy do
  describe "class method" do
    describe "#new" do
      it "instantiates a new toy with supplied id, schedule time and size" do
        t = FactoryElf::Toy.new(1000, 1440, 600)
        expect( t ).to be_a FactoryElf::Toy
        expect( t.toy_id ).to eql 1000
        expect( t.abs_mins ).to eql 1440
        expect( t.size ).to eql 600
      end
    end

    describe "[ toy_id ]" do
      it "returns a Toy instantiated from original data" do
        expected_data = Hash[
                  0 => [         0,       1,     5 ],
          9_099_860 => [ 9_099_860,  460124,   135 ],
          4_860_101 => [ 4_860_101,  325763,    89 ],
          2_879_109 => [ 2_879_109,  207847,    75 ],
          9_375_983 => [ 9_375_983,  470931,    17 ],
          6_969_416 => [ 6_969_416,  428175,  1555 ],
          4_950_610 => [ 4_950_610,  331151,    23 ],
          9_999_999 => [ 9_999_999,  495359,   697 ]
        ]

        expected_data.each do |id, fields|
          t = FactoryElf::Toy[ id ]
          expect( t ).to be_a FactoryElf::Toy
          expect( t.toy_id ).to eql fields[0]
          expect( t.abs_mins ).to eql fields[1]
          expect( t.size ).to eql fields[2]
        end
      end
    end
  end
end
