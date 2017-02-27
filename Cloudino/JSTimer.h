/*
  Timer.h - Library for Cloudino Platform.
  Created by Javier Solis, javier.solis@infotec.mx, softjei@gmail.com, July 8, 2015
  Released into the public domain.
*/

#ifndef TIMER_H
#define TIMER_H

#include <Arduino.h> 

class Timer {
public:
    // constructor
    Timer();

    typedef String timer_callback;
    typedef void (*timer_global_callback)(timer_callback);


    struct TimerInstance{
      TimerInstance(unsigned long time, timer_callback fn, int n, timer_callback fn2): time(time), fn(fn), n(n), fn2(fn2)
      {
        ltime=millis();
      }
      int id;
      unsigned long ltime=0;
      unsigned long time=0;
      int n;
      timer_callback fn;
      timer_callback fn2;
      TimerInstance *next=NULL;;
    };    

    // this function must be called inside loop()
    void loop();

    // call function f every d milliseconds
    int setInterval(unsigned long d, timer_callback f);

    // call function f once after d milliseconds
    int setTimeout(unsigned long d, timer_callback f);

    // call function f every d milliseconds for n times
    int setTimer(unsigned long d, timer_callback f, int n);

    // call function f every d milliseconds for n times and the end call function f2
    int setTimer(unsigned long d, timer_callback f, int n, timer_callback f2);

    // destroy the specified timer
    void deleteTimer(int id);

    void setGlobalCallBack(timer_global_callback fn)
    {
      funct=fn;
    }

    void clear();

protected:
    int _idc=0;  //contadore de IDs
    TimerInstance *_ini=NULL;
    TimerInstance *_end=NULL;

    timer_global_callback funct;

    /**
    * Regresa el siguiente elemento en la lista
    */
    TimerInstance *_deleteTimer(int id)
    {
        TimerInstance *prev=NULL;
        TimerInstance *aux=_ini;
        while(aux!=NULL)
        {
            if(aux->id==id){
                TimerInstance *next=aux->next;
                if(prev==NULL)
                {
                    _ini=next;
                }else
                {
                    prev->next=next;
                }
                if(next==NULL)
                {
                    _end=prev;
                }
                delete aux;
                return next;
            }
            prev=aux;
            aux=aux->next;
        }
        return NULL;
    }    
};

#endif
