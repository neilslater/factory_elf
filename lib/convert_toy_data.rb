require "csv"
require "factory_elf"

DATA_PATH = File.realpath( File.join( File.dirname(__FILE__), '..', 'data' ) )
CSV_PATH = File.join( DATA_PATH, 'toys_rev2.csv' )
TOYS_PATH = File.join( DATA_PATH, 'toys_narray.dat' )
TOYS_IDX_SIZE_PATH = File.join( DATA_PATH, 'toys_idx_size_narray.dat' )

toys = NArray.int( 2, 10000000 )
toys_idx_size = NArray.int( 10000000 )

puts "Reading #{CSV_PATH}"

csv = CSV.open( CSV_PATH )
header = csv.readline
unless header == ["ToyId","Arrival_time","Duration"]
  raise "Did not recognise header #{header.inspect}"
end

csv.each do |line|
  id  = line[0].to_i
  dt  = line[1].split(/\s+/).map(&:to_i)
  dur = line[2].to_i
  abs_mins = FactoryElf::Clock.from_yymmdd_hhmm( *dt )
  toys[0, id-1] = abs_mins
  toys[1, id-1] = dur
  if (id % 100000 == 0 )
    p [ line, [ abs_mins, dur ] ]
  end
end

puts "Sorting by size"
toys_sort_metric = NArray.dfloat(10_000_000)
toys_sort_metric += toys[1,0..9_999_999]
toys_sort_metric *= 1000000
toys_sort_metric -= toys[0,0..9_999_999]
toys_idx_size = toys_sort_metric.sort_index

puts "Writing #{TOYS_PATH}"
File.open( TOYS_PATH, 'wb' ) { |file| Marshal.dump( toys, file ) }

puts "Writing #{TOYS_IDX_SIZE_PATH}"
File.open( TOYS_IDX_SIZE_PATH, 'wb' ) { |file| Marshal.dump( toys_idx_size, file ) }
