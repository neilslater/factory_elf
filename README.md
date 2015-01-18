# FactoryElf

Ruby project with solution to the Kaggle competition *Helping Santa's Helpers* from December 2014.

For competition details, and problem data (which you will need to fetch separately to run this code),
see http://www.kaggle.com/c/helping-santas-helpers

The built-in default solution scores 1288199783, which would rank 23rd against
the other orignal competitors. Compared to my best entry, I have removed some overly-complex
additional features which made very little difference to the end result.

## Installation

 * You need a Ruby interpreter, version 1.9.3 or higher, with support for compiling native extensions.

 * Copy this repo locally, and make the directory with the README.md your current directory

 * Install dependencies:


    $ bundle install


 * Place a copy of ```toys_rev2.csv``` in the ```data``` folder.

 * Compile, import, test and run:


    $ bundle exec rake


This should compile the C code, import the problem data from the CSV file (into Ruby's NArray
format), run the unit tests, and then run the default solution. The process should take
around 30 minutes in total. The output is saved to ```data/submission.csv```.

## Usage

To use different parameter values, or try out ideas, you can use Ruby's ```irb``` tool:


    $ irb -Ilib
    2.1.4 :001 > require 'factory_elf'
     => true
    2.1.4 :002 > s = FactoryElf::GreedyScheduler.new( :start_date => "2014-11-17" )
     => #<FactoryElf::GreedyScheduler:0x007fb8a4821478 @opts={...
    2.1.4 :003 > s.run
    0.10%   -   2014-11-23 14:02  toy:2966725  elf:675  rate: 1.175/4.000  build:  298
    0.20%   -   2014-12-07 09:00  toy:5623154  elf:152  rate: 1.926/4.000  build:  598
    0.30%   -   2014-12-20 09:00  toy:6865108  elf:442  rate: 1.925/4.000  build:  595
    ...

For a list of params, or to see what is going on at the top level, please read the source in ```lib/factory_elf/greedy_scheduler.rb```.

To save the current solution as a Kaggle submission, after running the scheduler:

    2.1.4 :004 > s.submission.write_csv( 'data/file_name.csv' )

## Viewing or modifying the scheduler

The object model is not documented, and there are no plans to address that. However, if you
don't mind reading a little Ruby, then the best place to look for how it works at the top level is:

    lib/factory_elf/greedy_scheduler.rb

## Contributing

1. Fork it ( https://github.com/slobo777/factory_elf/fork )
2. Create your feature branch (`git checkout -b my-new-feature`)
3. Commit your changes (`git commit -am 'Add some feature'`)
4. Push to the branch (`git push origin my-new-feature`)
5. Create a new Pull Request
