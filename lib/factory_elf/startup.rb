require 'csv'

module FactoryElf
  DATA_PATH = File.realpath( File.join( File.dirname(__FILE__), '../..', 'data' ) )
  CSV_PATH = File.join( DATA_PATH, 'toys_rev2.csv' )
  TOYS_PATH = File.join( DATA_PATH, 'toys_narray.dat' )

  if File.exists?( TOYS_PATH )
    toy_data = File.open( TOYS_PATH , 'rb' ) { |file| Marshal.load(file) }
  else
    toy_data = nil
    puts "Cannot load toy data, need to run first-time import."
  end

  TOYS_SIZE_IDX_PATH = File.join( DATA_PATH, 'toys_idx_size_narray.dat' )
  if File.exists?( TOYS_SIZE_IDX_PATH )
    toy_size_idx_data = File.open( TOYS_SIZE_IDX_PATH , 'rb' ) { |file| Marshal.load(file) }
  else
    toy_size_idx_data = nil
  end

  TOYS = toy_data
  TOYS_SIZE_IDX = toy_size_idx_data

  def self.toys
    TOYS
  end

  def self.toys_size_idx
    TOYS_SIZE_IDX
  end

  def self.ready_to_import?
    File.exists?( CSV_PATH )
  end

  def self.import_toys_from_csv
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

    puts "Writing #{TOYS_SIZE_IDX_PATH}"
    File.open( TOYS_SIZE_IDX_PATH, 'wb' ) { |file| Marshal.dump( toys_idx_size, file ) }

    # Removing the constant first avoids warnings
    FactoryElf.send( :remove_const, :TOYS )
    FactoryElf.send( :const_set, :TOYS, toys )
    # Removing the constant first avoids warnings
    FactoryElf.send( :remove_const, :TOYS_SIZE_IDX )
    FactoryElf.send( :const_set, :TOYS_SIZE_IDX, toys_idx_size )
  end

  def self.ready_to_run?
    !! ( TOYS && TOYS_SIZE_IDX )
  end

  def self.count_toys_between min_size, max_size
    mins = 0
    num  = 0
    TOYS[1, 0..9_999_999].each do |x|
      next if x < min_size || x > max_size
      num += 1
      mins += x
    end
    [num,mins]
  end

  def self.toys_size_histogram
    # Based on pre-calculated fibonacci series, combining 1,2,3 to single entry
    fib_series = [[1, 3], [4, 5], [6, 8], [9, 13], [14, 21], [22, 34], [35, 55], [56, 89],
      [90, 144], [145, 233], [234, 377], [378, 610], [611, 987], [988, 1597], [1598, 2584],
      [2585, 4181], [4182, 6765], [6766, 10946], [10947, 17711], [17712, 28657], [28658, 46368],
      [46369, 75025] ]
    fib_series.map do |min_size, max_size|
      [ min_size, max_size, count_toys_between( min_size, max_size ) ].flatten
    end
  end

  # The data never changes, so we can use this
  def self.cached_toys_size_histogram
   [[     1,     3,  345897,     787940], [     4,     5,  314963,    1416480], [     6,     8,   439714,    3067605],
    [     9,    13,  635037,    6939261], [    14,    21,  812065,   14071178], [    22,    34,   970323,   26794823],
    [    35,    55, 1033993,   45682625], [    56,    89,  981565,   69512410], [    90,   144,   825132,   93820399],
    [   145,   233,  611581,  111789266], [   234,   377,  403443,  118712154], [   378,   610,   245215,  116351021],
    [   611,   987,  149990,  115616222], [   988,  1597,  111128,  140247998], [  1598,  2584,   118864,  246156157],
    [  2585,  4181,  167612,  565817250], [  4182,  6765,  263072, 1439550404], [  6766, 10946,   421188, 3729964975],
    [ 10947, 17711,  678757, 9724622662], [ 17712, 28657,  470453, 9432752321], [ 28658, 46368,        7,     230141],
    [ 46369, 75025,       1,      47473]]
  end
end
