/* Class used to parse an NMEA sentence into _fields
* 
* Allows access to the fields as strings or numbers.
* First field considered to be the sentence name
* 
*/
#ifndef NMEA_SENTENCE_H
#define NMEA_SENTENCE_H
//////////////////
#include <WString.h>

//////////////////

#define GPRMC "$GPRMC" //$GPRMC,213651.800,A,4728.9319,N,00841.9431,E,0.90,183.08,270116,,,A*65
#define GPGGA "$GPGGA" //$GPGGA,213652.400,4728.9319,N,00841.9431,E,1,8,0.95,1464.5,M,48.0,M,,*5B
#define LXWP0 "$LXWP0" //$LXWP0,N,,404.6,-0.01,,,,,,190,,*4F

//////////////////
// LXWP0 and GPS sentences

#define MAX_FIELDS 16
#define MAX_LINE  100
#define SPLITCHAR ','

class NmeaSentence {
public:
  NmeaSentence(const String& line);
  
  int length() { return _nr; };
  const char* field(int index) { return index < _nr ? _line + _fields[index] : 0; };
  float fieldFloat(int index) { return index < _nr ? atof(_line + _fields[index]) : 0; };
  float fieldChar(int index) { return index < _nr ? (_line + _fields[index])[0] : ' '; };
  const char* name() { return _line; };
  
private:
  unsigned int _fields[MAX_FIELDS];
  char _line[MAX_LINE];
  int _nr;
};

NmeaSentence::NmeaSentence(const String& line) : _nr(0)
{
  line.toCharArray(_line, MAX_LINE);

  int index = 0;
  int pos;
  _fields[_nr] = index;
  while ((pos = line.indexOf(SPLITCHAR, index)) > -1) {
    _line[pos] = 0;
    _nr++;
    index = pos+1;
    _fields[_nr] = index;
    if (_nr >= MAX_FIELDS-1) break;
  }
  if (line.length() > 0) _nr++;
}



#endif
