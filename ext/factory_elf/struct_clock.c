// ext/elf_factory/struct_clock.c

#include "struct_clock.h"

// First entry is 2014
static int year_start_offsets[600] = {
  0, 365, 730, 1096, 1461, 1826, 2191, 2557, 2922, 3287, 3652, 4018, 4383, 4748, 5113, 5479, 5844,
  6209, 6574, 6940, 7305, 7670, 8035, 8401, 8766, 9131, 9496, 9862, 10227, 10592, 10957, 11323,
  11688, 12053, 12418, 12784, 13149, 13514, 13879, 14245, 14610, 14975, 15340, 15706, 16071, 16436,
  16801, 17167, 17532, 17897, 18262, 18628, 18993, 19358, 19723, 20089, 20454, 20819, 21184, 21550,
  21915, 22280, 22645, 23011, 23376, 23741, 24106, 24472, 24837, 25202, 25567, 25933, 26298, 26663,
  27028, 27394, 27759, 28124, 28489, 28855, 29220, 29585, 29950, 30316, 30681, 31046, 31411, 31776,
  32141, 32506, 32871, 33237, 33602, 33967, 34332, 34698, 35063, 35428, 35793, 36159, 36524, 36889,
  37254, 37620, 37985, 38350, 38715, 39081, 39446, 39811, 40176, 40542, 40907, 41272, 41637, 42003,
  42368, 42733, 43098, 43464, 43829, 44194, 44559, 44925, 45290, 45655, 46020, 46386, 46751, 47116,
  47481, 47847, 48212, 48577, 48942, 49308, 49673, 50038, 50403, 50769, 51134, 51499, 51864, 52230,
  52595, 52960, 53325, 53691, 54056, 54421, 54786, 55152, 55517, 55882, 56247, 56613, 56978, 57343,
  57708, 58074, 58439, 58804, 59169, 59535, 59900, 60265, 60630, 60996, 61361, 61726, 62091, 62457,
  62822, 63187, 63552, 63918, 64283, 64648, 65013, 65379, 65744, 66109, 66474, 66840, 67205, 67570,
  67935, 68300, 68665, 69030, 69395, 69761, 70126, 70491, 70856, 71222, 71587, 71952, 72317, 72683,
  73048, 73413, 73778, 74144, 74509, 74874, 75239, 75605, 75970, 76335, 76700, 77066, 77431, 77796,
  78161, 78527, 78892, 79257, 79622, 79988, 80353, 80718, 81083, 81449, 81814, 82179, 82544, 82910,
  83275, 83640, 84005, 84371, 84736, 85101, 85466, 85832, 86197, 86562, 86927, 87293, 87658, 88023,
  88388, 88754, 89119, 89484, 89849, 90215, 90580, 90945, 91310, 91676, 92041, 92406, 92771, 93137,
  93502, 93867, 94232, 94598, 94963, 95328, 95693, 96059, 96424, 96789, 97154, 97520, 97885, 98250,
  98615, 98981, 99346, 99711, 100076, 100442, 100807, 101172, 101537, 101903, 102268, 102633,
  102998, 103364, 103729, 104094, 104459, 104824, 105189, 105554, 105919, 106285, 106650, 107015,
  107380, 107746, 108111, 108476, 108841, 109207, 109572, 109937, 110302, 110668, 111033, 111398,
  111763, 112129, 112494, 112859, 113224, 113590, 113955, 114320, 114685, 115051, 115416, 115781,
  116146, 116512, 116877, 117242, 117607, 117973, 118338, 118703, 119068, 119434, 119799, 120164,
  120529, 120895, 121260, 121625, 121990, 122356, 122721, 123086, 123451, 123817, 124182, 124547,
  124912, 125278, 125643, 126008, 126373, 126739, 127104, 127469, 127834, 128200, 128565, 128930,
  129295, 129661, 130026, 130391, 130756, 131122, 131487, 131852, 132217, 132583, 132948, 133313,
  133678, 134044, 134409, 134774, 135139, 135505, 135870, 136235, 136600, 136966, 137331, 137696,
  138061, 138427, 138792, 139157, 139522, 139888, 140253, 140618, 140983, 141349, 141714, 142079,
  142444, 142810, 143175, 143540, 143905, 144271, 144636, 145001, 145366, 145732, 146097, 146462,
  146827, 147193, 147558, 147923, 148288, 148654, 149019, 149384, 149749, 150115, 150480, 150845,
  151210, 151576, 151941, 152306, 152671, 153037, 153402, 153767, 154132, 154498, 154863, 155228,
  155593, 155959, 156324, 156689, 157054, 157420, 157785, 158150, 158515, 158881, 159246, 159611,
  159976, 160342, 160707, 161072, 161437, 161803, 162168, 162533, 162898, 163264, 163629, 163994,
  164359, 164725, 165090, 165455, 165820, 166186, 166551, 166916, 167281, 167647, 168012, 168377,
  168742, 169108, 169473, 169838, 170203, 170569, 170934, 171299, 171664, 172030, 172395, 172760,
  173125, 173491, 173856, 174221, 174586, 174952, 175317, 175682, 176047, 176413, 176778, 177143,
  177508, 177873, 178238, 178603, 178968, 179334, 179699, 180064, 180429, 180795, 181160, 181525,
  181890, 182256, 182621, 182986, 183351, 183717, 184082, 184447, 184812, 185178, 185543, 185908,
  186273, 186639, 187004, 187369, 187734, 188100, 188465, 188830, 189195, 189561, 189926, 190291,
  190656, 191022, 191387, 191752, 192117, 192483, 192848, 193213, 193578, 193944, 194309, 194674,
  195039, 195405, 195770, 196135, 196500, 196866, 197231, 197596, 197961, 198327, 198692, 199057,
  199422, 199788, 200153, 200518, 200883, 201249, 201614, 201979, 202344, 202710, 203075, 203440,
  203805, 204171, 204536, 204901, 205266, 205632, 205997, 206362, 206727, 207093, 207458, 207823,
  208188, 208554, 208919, 209284, 209649, 210015, 210380, 210745, 211110, 211476, 211841, 212206,
  212571, 212937, 213302, 213667, 214032, 214397, 214762, 215127, 215492, 215858, 216223, 216588,
  216953, 217319, 217684, 218049, 218414, 218780 };

static int month_lengths[2][12] = {
  { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 },
  { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }
};

static int month_start_offsets[2][12] = {
  { -1, 30, 58, 89, 119, 150, 180, 211, 242, 272, 303, 333 },
  { -1, 30, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334 }
};

inline int leap_year( int yy ) {
  if ( yy % 400 == 0 ) {
    return 1;
  }
  if ( yy % 100 == 0 ) {
    return 0;
  }
  if ( yy % 4  == 0 ) {
    return 1;
  }
  return 0;
}

inline int leap_year_from_mins( int abs_mins ) {
  return leap_year( clock__yy( abs_mins ) );
}

int clock__yy( int abs_minutes ) {
  int abs_day = (abs_minutes / 1440);
  int y = abs_day / 366; // rough guess, guaranteed correct or one too low
  if ( abs_day >= year_start_offsets[y+1] ) {
    y++;
  }
  return 2014 + y;
}

int clock__mm( int abs_minutes ) {
  int yy = clock__yy( abs_minutes );
  int abs_day = ( abs_minutes / 1440) - year_start_offsets[yy-2014];
  int ly = leap_year( yy );
  int mon = abs_day / 31; // rough guess, guaranteed correct or one too low
  if ( mon < 11 && abs_day > month_start_offsets[ly][mon+1] ) {
    mon++;
  }
  return mon + 1;
}

int clock__dd( int abs_minutes ) {
  int yy = clock__yy( abs_minutes );
  int abs_day = ( abs_minutes / 1440) - year_start_offsets[yy-2014];
  int ly = leap_year( yy );
  int mon = abs_day / 31; // rough guess, guaranteed correct or one too low
  if ( mon < 11 && abs_day > month_start_offsets[ly][mon+1] ) {
    mon++;
  }
  return abs_day - month_start_offsets[ly][mon];
}

int clock__hh( int abs_minutes ) {
  return ( abs_minutes % 1440 ) / 60;
}

int clock__mins( int abs_minutes ) {
  return ( abs_minutes % 60 );
}

int clock__from_yymmdd_hhmm( int y, int m, int d, int h, int mins ) {
  int days = d + year_start_offsets[y-2014]; // FIXME!!!!
  int ly = leap_year( y );
  days += month_start_offsets[ly][m-1];
  return days * 1440 + h * 60 + mins;
}

int clock__in_sanctioned_time( int abs_minutes ) {
  int hh = clock__hh( abs_minutes );
  if ( hh < 9 || hh >= 19 ) {
    return 0;
  }
  return 1;
}

int clock__next_sanctioned_start( int abs_minutes ) {
  int hh = clock__hh( abs_minutes );

  if ( hh < 9 ) {
    return 1440 * (abs_minutes / 1440) + 9 * 60;
  }
  if ( hh == 9 && (abs_minutes % 60) == 0 ) {
    return abs_minutes;
  }
  return 1440 * ( 1 + (abs_minutes / 1440)) + 9 * 60;
}

int clock__next_sanctioned_end( int abs_minutes ) {
  int hh = clock__hh( abs_minutes );
  if ( hh < 19 ) {
    return 1440 * (abs_minutes / 1440) + 19 * 60;
  }
  if ( hh == 19 && (abs_minutes % 60) == 0 ) {
    return abs_minutes;
  }
  return 1440 * ( 1 + (abs_minutes / 1440)) + 19 * 60;
}

int clock__sanctioned_minutes_between( int start_abs_minutes, int end_abs_minutes ) {
  if ( end_abs_minutes < start_abs_minutes ) {
    return 0;
  }

  int total_sanctioned = 0;
  int pos = start_abs_minutes;
  int next_stop;

  while( pos <= end_abs_minutes ) {
    if ( clock__in_sanctioned_time( pos ) ) {
      next_stop = clock__next_sanctioned_end( pos );
      if (next_stop > end_abs_minutes) {
        next_stop = end_abs_minutes + 1;
      }
      total_sanctioned += ( next_stop - pos );
      pos = next_stop;
    } else {
      pos = clock__next_sanctioned_start( pos );
    }
  }

  return total_sanctioned;
}

int clock__non_sanctioned_minutes_between( int start_abs_minutes, int end_abs_minutes ) {
  if ( end_abs_minutes < start_abs_minutes ) {
    return 0;
  }

  int total_non_sanctioned = 0;
  int pos = start_abs_minutes;
  int next_stop;

  while( pos <= end_abs_minutes ) {
    if ( ! clock__in_sanctioned_time( pos ) ) {
      next_stop = clock__next_sanctioned_start( pos );
      if (next_stop > end_abs_minutes) {
        next_stop = end_abs_minutes + 1;
      }
      total_non_sanctioned += ( next_stop - pos );
      pos = next_stop;
    } else {
      pos = clock__next_sanctioned_end( pos );
    }
  }

  return total_non_sanctioned;
}

// This moves the clock on to next sanctioned time, including adjustments for overtime
int clock__add_work_minutes( int abs_minutes, int work_minutes ) {
  int end_abs_mins = abs_minutes + work_minutes - 1;
  int sanctioned_mins = clock__sanctioned_minutes_between( abs_minutes, end_abs_mins );
  int non_sanctioned_mins = clock__non_sanctioned_minutes_between( abs_minutes, end_abs_mins );
  int pos = end_abs_mins + 1;
  if ( non_sanctioned_mins < 1 ) {
    if ( clock__in_sanctioned_time(pos) ) {
      return pos;
    }
    // This should only happen if we hit the end of sanctioned time exactly
    return clock__next_sanctioned_start( pos );
  }

  int rest_remaining = non_sanctioned_mins;
  int next_stop;
  while ( rest_remaining > 0 ) {
    if ( clock__in_sanctioned_time( pos ) ) {
      next_stop = clock__next_sanctioned_end( pos );
      if ( (next_stop - pos) > rest_remaining ) {
        next_stop = pos + rest_remaining;
      }
      rest_remaining -= ( next_stop - pos );
      pos = next_stop;
    } else {
      pos = clock__next_sanctioned_start( pos );
    }
  }

  // This should only happen if we hit the end of sanctioned time exactly
  if (! clock__in_sanctioned_time( pos ) ) {
    pos = clock__next_sanctioned_start( pos );
  }
  return pos;
}

int clock__sanctioned_minutes_remaining( int abs_minutes ) {
  int hh = clock__hh( abs_minutes );
  if ( hh < 9 || hh >= 19 ) {
    return 0;
  }
  return 1440 * (abs_minutes / 1440) + 19 * 60 - abs_minutes;
}
