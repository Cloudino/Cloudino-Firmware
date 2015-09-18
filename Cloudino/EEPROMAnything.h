#include <Arduino.h>  // for type definitions

template <class T> int EEPROM_clean(int ee, const T& value)
{
    int s=sizeof(value);
    EEPROM.begin(s+ee);
    unsigned int i;
    for (i = 0; i < s; i++)
          EEPROM.write(ee++, 0);
    EEPROM.commit();
    EEPROM.end();
    return i;
}

int EEPROM_clean(int ee)
{
    EEPROM.begin(1024+ee);
    unsigned int i;
    for (i = 0; i < 1024; i++)
          EEPROM.write(ee++, 0);
    EEPROM.commit();
    EEPROM.end();
    return i;
}

template <class T> int EEPROM_write(int ee, const T& value)
{
    EEPROM.begin(sizeof(value)+ee);
    const byte* p = (const byte*)(const void*)&value;
    unsigned int i;
    for (i = 0; i < sizeof(value); i++)
          EEPROM.write(ee++, *p++);
    EEPROM.commit();
    EEPROM.end();
    return i;
}

template <class T> int EEPROM_read(int ee, T& value)
{
    EEPROM.begin(sizeof(value)+ee);
    byte* p = (byte*)(void*)&value;
    unsigned int i;
    for (i = 0; i < sizeof(value); i++)
          *p++ = EEPROM.read(ee++);
    EEPROM.end();   
    return i;
}
