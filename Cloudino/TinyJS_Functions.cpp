/*
 * TinyJS
 *
 * A single-file Javascript-alike engine
 *
 * - Useful language functions
 *
 * Authored By Gordon Williams <gw@pur3.co.uk>
 * 
 * Ported to ESP8266 - Arduino by Javier Solis, javier.solis@infotec.mx, softjei@gmail.com
 * Nov 2015 
 *
 * Copyright (C) 2009 Pur3 Ltd
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is furnished to do
 * so, subject to the following conditions:

 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "TinyJS_Functions.h"
#include <math.h>

#define NPOS -1

void addDefaulFunct(UsrData *data);
void registerFunctions(CTinyJS *js, Timer *timer);
void js_reset(CScriptVar *v, void *userdata);

void js_print(CScriptVar *v, void *userdata) {
  TRACE_OBJ.print("  ");
  TRACE_OBJ.println(v->getParameter("text")->getString().c_str());
}

void js_dump(CScriptVar *v, void *userdata) {
    CTinyJS *js = (CTinyJS*)userdata;
    TRACE_OBJ.println("FreeHeap:"+String(ESP.getFreeHeap()));
    js->root->trace(">  ");
}

void js_restart(CScriptVar *v, void *userdata) {
  ESP.restart();
}

#ifdef CDINOJS
//CLOUDINO Functions
void scCloudinoPost(CScriptVar *v, void *userdata)
{
    MessageProc *proc = (MessageProc*)userdata;
    proc->messageHandler(v->getParameter("topic")->getString(),v->getParameter("data")->getString());
}

void scCloudinoPrint(CScriptVar *v, void *userdata)
{
    MessageProc *proc = (MessageProc*)userdata;
    proc->logHandler(v->getParameter("txt")->getString());
}
#endif

void js_pinMode(CScriptVar *v, void *userdata) {
  pinMode(v->getParameter("pin")->getInt(), v->getParameter("mode")->getInt());
}

void js_digitalWrite(CScriptVar *v, void *userdata) {
  digitalWrite(v->getParameter("pin")->getInt(), v->getParameter("value")->getInt());
}

void js_digitalRead(CScriptVar *v, void *userdata) {
  int value=digitalRead(v->getParameter("pin")->getInt());
  v->getReturnVar()->setInt(value);  
}

void js_analogRead(CScriptVar *v, void *userdata) {
  int value=analogRead(A0);
  v->getReturnVar()->setInt(value);
}

void js_analogWrite(CScriptVar *v, void *userdata) {
  analogWrite(v->getParameter("pin")->getInt(), v->getParameter("value")->getInt());
}

void js_setInterval(CScriptVar *v, void *userdata)
{
    Timer *timer = (Timer*)userdata;
    int ret=timer->setInterval(v->getParameter("time_ms")->getLong(),v->getParameter("funct")->getString());
    v->getReturnVar()->setInt(ret);
}

void js_clearTimer(CScriptVar *v, void *userdata)
{
    Timer *timer = (Timer*)userdata;
    timer->deleteTimer(v->getParameter("id")->getInt());
}

void js_setTimeout(CScriptVar *v, void *userdata)
{
    Timer *timer = (Timer*)userdata;
    int ret=timer->setTimeout(v->getParameter("time_ms")->getLong(),v->getParameter("funct")->getString());
    v->getReturnVar()->setInt(ret);
}

// ----------------------------------------------- Actual Functions
void scTrace(CScriptVar *c, void *userdata) {
    CTinyJS *js = (CTinyJS*)userdata;
    js->root->trace();
}

void scObjectDump(CScriptVar *c, void *) {
    c->getParameter("this")->trace("> ");
}

void scObjectClone(CScriptVar *c, void *) {
    CScriptVar *obj = c->getParameter("this");
    c->getReturnVar()->copyValue(obj);
}

void scMathRand(CScriptVar *c, void *) {
    c->getReturnVar()->setDouble((double)rand()/RAND_MAX);
}

void scMathRandInt(CScriptVar *c, void *) {
    int min = c->getParameter("min")->getInt();
    int max = c->getParameter("max")->getInt();
    int val = min + (int)(rand()%(1+max-min));
    c->getReturnVar()->setInt(val);
}

void scCharToInt(CScriptVar *c, void *) {
    String str = c->getParameter("ch")->getString();;
    int val = 0;
    if (str.length()>0)
        val = (int)str.c_str()[0];
    c->getReturnVar()->setInt(val);
}

void scStringIndexOf(CScriptVar *c, void *) {
    String str = c->getParameter("this")->getString();
    String search = c->getParameter("search")->getString();
    size_t p = str.indexOf(search);
    int val = (p==NPOS) ? -1 : p;
    c->getReturnVar()->setInt(val);
}

void scStringLastIndexOf(CScriptVar *c, void *) {
    String str = c->getParameter("this")->getString();
    String search = c->getParameter("search")->getString();
    size_t p = str.lastIndexOf(search);
    int val = (p==NPOS) ? -1 : p;
    c->getReturnVar()->setInt(val);
}

void scStringSubstring(CScriptVar *c, void *) {
    String str = c->getParameter("this")->getString();
    int lo = c->getParameter("lo")->getInt();
    int hi = c->getParameter("hi")->getInt();
    int l = hi-lo;
    if (l>0 && lo>=0 && lo+l<=(int)str.length())
    {
      char tmp[l+1];
      tmp[l]='\0';
      for(int x=lo;x<hi;x++)
      {
        tmp[x-lo]=str.charAt(x);
      }
      c->getReturnVar()->setString(tmp);
    }else
      c->getReturnVar()->setString("");
}

void scStringCharAt(CScriptVar *c, void *) {
    String str = c->getParameter("this")->getString();
    int p = c->getParameter("pos")->getInt();
    char str2[]={str.charAt(p),'\0'};
    
    if (p>=0 && p<(int)str.length())
      c->getReturnVar()->setString(str2);
    else
      c->getReturnVar()->setString("");
}

void scStringCharCodeAt(CScriptVar *c, void *) {
    String str = c->getParameter("this")->getString();
    int p = c->getParameter("pos")->getInt();
    if (p>=0 && p<(int)str.length())
      c->getReturnVar()->setInt(str.charAt(p));
    else
      c->getReturnVar()->setInt(0);
}

void scStringSplit(CScriptVar *c, void *) {
    String str = c->getParameter("this")->getString();
    String sep = c->getParameter("separator")->getString();
    CScriptVar *result = c->getReturnVar();
    result->setArray();
    int length = 0;

    size_t pos = str.indexOf(sep);
    while (pos != NPOS) {
      result->setArrayIndex(length++, new CScriptVar(str.substring(0,pos)));
      str = str.substring(pos+1);
      pos = str.indexOf(sep);
    }

    if (str.length()>0)
      result->setArrayIndex(length++, new CScriptVar(str));
}

void scStringFromCharCode(CScriptVar *c, void *) {
    char str[2];
    str[0] = c->getParameter("char")->getInt();
    str[1] = 0;
    c->getReturnVar()->setString(str);
}

void scIntegerParseInt(CScriptVar *c, void *) {
    String str = c->getParameter("str")->getString();
    int val = atoi(str.c_str());
    c->getReturnVar()->setInt(val);
}

void scDoubleParseFloat(CScriptVar *c, void *) {
    String str = c->getParameter("str")->getString();
    float val = atof(str.c_str());
    c->getReturnVar()->setDouble(val);
}

void scIntegerValueOf(CScriptVar *c, void *) {
    String str = c->getParameter("str")->getString();

    int val = 0;
    if (str.length()==1)
      val = str[0];
    c->getReturnVar()->setInt(val);
}

void scJSONStringify(CScriptVar *c, void *) {
    String result;
    c->getParameter("obj")->getJSON(result);
    c->getReturnVar()->setString(result);
}

void scExec(CScriptVar *c, void *data) {
    CTinyJS *tinyJS = (CTinyJS *)data;
    String str = c->getParameter("jsCode")->getString();
    tinyJS->execute(str);
}

void scEval(CScriptVar *c, void *data) {
    CTinyJS *tinyJS = (CTinyJS *)data;
    String str = c->getParameter("jsCode")->getString();
    c->setReturnVar(tinyJS->evaluateComplex(str).var);
}

void scDHT11Read(CScriptVar *c, void *data) {
    CTinyJS *tinyJS = (CTinyJS *)data;
    int pin = c->getParameter("pin")->getInt();
    CScriptVar *ret=c->getReturnVar();
    
    CScriptVar *temperature = new CScriptVar();
    temperature->setInt(12);
    ret->addChild("temperature", temperature);
    
    CScriptVar *humidity = new CScriptVar();
    humidity->setInt(14);
    ret->addChild("humidity", humidity);

    //c->setReturnVar(tinyJS->evaluateComplex(str).var);
}


void scArrayContains(CScriptVar *c, void *data) {
  CScriptVar *obj = c->getParameter("obj");
  CScriptVarLink *v = c->getParameter("this")->firstChild;

  bool contains = false;
  while (v) {
      if (v->var->equals(obj)) {
        contains = true;
        break;
      }
      v = v->nextSibling;
  }

  c->getReturnVar()->setInt(contains);
}

void scArrayRemove(CScriptVar *c, void *data) {
  CScriptVar *obj = c->getParameter("obj");
  Vector<int> removedIndices;
  CScriptVarLink *v;
  // remove
  v = c->getParameter("this")->firstChild;
  while (v) {
      if (v->var->equals(obj)) {
        removedIndices.push_back(v->getIntName());
      }
      v = v->nextSibling;
  }
  // renumber
  v = c->getParameter("this")->firstChild;
  while (v) {
      int n = v->getIntName();
      int newn = n;
      for (size_t i=0;i<removedIndices.size();i++)
        if (n>=removedIndices[i])
          newn--;
      if (newn!=n)
        v->setIntName(newn);
      v = v->nextSibling;
  }
}

void scArrayJoin(CScriptVar *c, void *data) {
  String sep = c->getParameter("separator")->getString();
  CScriptVar *arr = c->getParameter("this");

  String sstr;
  int l = arr->getArrayLength();
  for (int i=0;i<l;i++) {
    if (i>0) sstr+=sep;
    sstr+=arr->getArrayIndex(i)->getString();
  }

  c->getReturnVar()->setString(sstr);
}


//MATH
#define k_E                 exp(1.0)
#define k_PI                3.1415926535897932384626433832795

#define F_ABS(a)            ((a)>=0 ? (a) : (-(a)))
#define F_MIN(a,b)          ((a)>(b) ? (b) : (a))
#define F_MAX(a,b)          ((a)>(b) ? (a) : (b))
#define F_SGN(a)            ((a)>0 ? 1 : ((a)<0 ? -1 : 0 ))
#define F_RNG(a,min,max)    ((a)<(min) ? min : ((a)>(max) ? max : a ))
#define F_ROUND(a)          ((a)>0 ? (int) ((a)+0.5) : (int) ((a)-0.5) )

#define scIsInt(a)          ( c->getParameter(a)->isInt() )
#define scIsDouble(a)       ( c->getParameter(a)->isDouble() )  
#define scGetInt(a)         ( c->getParameter(a)->getInt() )
#define scGetDouble(a)      ( c->getParameter(a)->getDouble() )  
#define scReturnInt(a)      ( c->getReturnVar()->setInt(a) )
#define scReturnDouble(a)   ( c->getReturnVar()->setDouble(a) )  

//Math.abs(x) - returns absolute of given value
void scMathAbs(CScriptVar *c, void *userdata) {
    if ( scIsInt("a") ) {
      scReturnInt( F_ABS( scGetInt("a") ) );
    } else if ( scIsDouble("a") ) {
      scReturnDouble( F_ABS( scGetDouble("a") ) );
    }
}

//Math.round(a) - returns nearest round of given value
void scMathRound(CScriptVar *c, void *userdata) {
    if ( scIsInt("a") ) {
      scReturnInt( F_ROUND( scGetInt("a") ) );
    } else if ( scIsDouble("a") ) {
      scReturnDouble( F_ROUND( scGetDouble("a") ) );
    }
}

//Math.min(a,b) - returns minimum of two given values 
void scMathMin(CScriptVar *c, void *userdata) {
    if ( (scIsInt("a")) && (scIsInt("b")) ) {
      scReturnInt( F_MIN( scGetInt("a"), scGetInt("b") ) );
    } else {
      scReturnDouble( F_MIN( scGetDouble("a"), scGetDouble("b") ) );
    }
}

//Math.max(a,b) - returns maximum of two given values  
void scMathMax(CScriptVar *c, void *userdata) {
    if ( (scIsInt("a")) && (scIsInt("b")) ) {
      scReturnInt( F_MAX( scGetInt("a"), scGetInt("b") ) );
    } else {
      scReturnDouble( F_MAX( scGetDouble("a"), scGetDouble("b") ) );
    }
}

/*
//Math.range(x,a,b) - returns value limited between two given values  
void scMathRange(CScriptVar *c, void *userdata) {
    if ( (scIsInt("x")) ) {
      scReturnInt( F_RNG( scGetInt("x"), scGetInt("a"), scGetInt("b") ) );
    } else {
      scReturnDouble( F_RNG( scGetDouble("x"), scGetDouble("a"), scGetDouble("b") ) );
    }
}

//Math.sign(a) - returns sign of given value (-1==negative,0=zero,1=positive)
void scMathSign(CScriptVar *c, void *userdata) {
    if ( scIsInt("a") ) {
      scReturnInt( F_SGN( scGetInt("a") ) );
    } else if ( scIsDouble("a") ) {
      scReturnDouble( F_SGN( scGetDouble("a") ) );
    }
}
//*/


//Math.PI() - returns PI value
void scMathPI(CScriptVar *c, void *userdata) {
    scReturnDouble(k_PI);
}

/*
//Math.toDegrees(a) - returns degree value of a given angle in radians
void scMathToDegrees(CScriptVar *c, void *userdata) {
    scReturnDouble( (180.0/k_PI)*( scGetDouble("a") ) );
}

//Math.toRadians(a) - returns radians value of a given angle in degrees
void scMathToRadians(CScriptVar *c, void *userdata) {
    scReturnDouble( (k_PI/180.0)*( scGetDouble("a") ) );
}

//Math.sin(a) - returns trig. sine of given angle in radians
void scMathSin(CScriptVar *c, void *userdata) {
    scReturnDouble( sin( scGetDouble("a") ) );
}


//Math.asin(a) - returns trig. arcsine of given angle in radians
void scMathASin(CScriptVar *c, void *userdata) {
    scReturnDouble( asin( scGetDouble("a") ) );
}

//Math.cos(a) - returns trig. cosine of given angle in radians
void scMathCos(CScriptVar *c, void *userdata) {
    scReturnDouble( cos( scGetDouble("a") ) );
}

//Math.acos(a) - returns trig. arccosine of given angle in radians
void scMathACos(CScriptVar *c, void *userdata) {
    scReturnDouble( acos( scGetDouble("a") ) );
}

//Math.tan(a) - returns trig. tangent of given angle in radians
void scMathTan(CScriptVar *c, void *userdata) {
    scReturnDouble( tan( scGetDouble("a") ) );
}

//Math.atan(a) - returns trig. arctangent of given angle in radians
void scMathATan(CScriptVar *c, void *userdata) {
    scReturnDouble( atan( scGetDouble("a") ) );
}

//Math.sinh(a) - returns trig. hyperbolic sine of given angle in radians
void scMathSinh(CScriptVar *c, void *userdata) {
    scReturnDouble( sinh( scGetDouble("a") ) );
}

//Math.asinh(a) - returns trig. hyperbolic arcsine of given angle in radians
void scMathASinh(CScriptVar *c, void *userdata) {
    scReturnDouble( asinh( scGetDouble("a") ) );
}

//Math.cosh(a) - returns trig. hyperbolic cosine of given angle in radians
void scMathCosh(CScriptVar *c, void *userdata) {
    scReturnDouble( cosh( scGetDouble("a") ) );
}

//Math.acosh(a) - returns trig. hyperbolic arccosine of given angle in radians
void scMathACosh(CScriptVar *c, void *userdata) {
    scReturnDouble( acosh( scGetDouble("a") ) );
}

//Math.tanh(a) - returns trig. hyperbolic tangent of given angle in radians
void scMathTanh(CScriptVar *c, void *userdata) {
    scReturnDouble( tanh( scGetDouble("a") ) );
}

//Math.atan(a) - returns trig. hyperbolic arctangent of given angle in radians
void scMathATanh(CScriptVar *c, void *userdata) {
    scReturnDouble( atan( scGetDouble("a") ) );
}

//Math.E() - returns E Neplero value
void scMathE(CScriptVar *c, void *userdata) {
    scReturnDouble(k_E);
}
/*
//Math.log(a) - returns natural logaritm (base E) of given value
void scMathLog(CScriptVar *c, void *userdata) {
    scReturnDouble( log( scGetDouble("a") ) );
}

//Math.log10(a) - returns logaritm(base 10) of given value
void scMathLog10(CScriptVar *c, void *userdata) {
    scReturnDouble( log10( scGetDouble("a") ) );
}


//Math.exp(a) - returns e raised to the power of a given number
void scMathExp(CScriptVar *c, void *userdata) {
    scReturnDouble( exp( scGetDouble("a") ) );
}
*/
/*
//Math.pow(a,b) - returns the result of a number raised to a power (a)^(b)
void scMathPow(CScriptVar *c, void *userdata) {
    scReturnDouble( pow( scGetDouble("a"), scGetDouble("b") ) );
}
*/
//Math.sqrt(a) - returns square root of given value
void scMathSqrt(CScriptVar *c, void *userdata) {
    scReturnDouble( sqrtf( scGetDouble("a") ) );
}
//REQUIRE

void js_require(CScriptVar *v, void *userdata) {
  UsrData *data = (UsrData*)userdata;
  CTinyJS *js=data->js;
  Timer *timer=data->timer;
#ifdef CDINOJS  
  MessageProc *proc=data->proc;
#endif

  String name=v->getParameter("name")->getString();

  //Serial.println("name:"+name);
  
  if(name=="Object")
  {
    js->addNative(F("function Object.dump()"), scObjectDump, 0);
    js->addNative(F("function Object.clone()"), scObjectClone, 0);
#ifdef CDINOJS    
  }else if(name=="Cloudino")
  {
    js->addNative(F("function Cloudino.post(topic,data)"), scCloudinoPost, proc);
    js->addNative(F("function Cloudino.print(txt)"), scCloudinoPrint, proc);
    //js->addNative(F("function Cloudino.on(topic, funct)"), scCloudinoOn, proc); 
    js->execute(F("Cloudino._tmp='';Cloudino._on={};Cloudino.on=function (topic, funct){Cloudino._on[topic]=funct;};"));    
#endif    
  }else if(name=="Rand")
  {
    js->addNative(F("function Math.rand()"), scMathRand, 0);
    js->addNative(F("function Math.randInt(min, max)"), scMathRandInt, 0);    
  }else if(name=="String")
  {
    //js->addNative("function charToInt(ch)", scCharToInt, 0); //  convert a character to an int - get its value
    js->addNative(F("function String.indexOf(search)"), scStringIndexOf, 0); // find the position of a string in a string, -1 if not
    js->addNative(F("function String.lastIndexOf(search)"), scStringLastIndexOf, 0); // find the last position of a string in a string, -1 if not
    js->addNative(F("function String.substring(lo,hi)"), scStringSubstring, 0);
    js->addNative(F("function String.charAt(pos)"), scStringCharAt, 0);
    js->addNative(F("function String.charCodeAt(pos)"), scStringCharCodeAt, 0);
    js->addNative(F("function String.fromCharCode(char)"), scStringFromCharCode, 0);
    js->addNative(F("function String.split(separator)"), scStringSplit, 0);    
//  }else if(name=="Integer")
//  {
//    js->addNative(F("function Integer.parseInt(str)"), scIntegerParseInt, 0); // string to int
//    js->addNative(F("function Integer.valueOf(str)"), scIntegerValueOf, 0); // value of a single character
  }else if(name=="Array")
  {
    js->addNative(F("function Array.contains(obj)"), scArrayContains, 0);
    js->addNative(F("function Array.remove(obj)"), scArrayRemove, 0);
    js->addNative(F("function Array.join(separator)"), scArrayJoin, 0);
  }else if(name=="GPIO")
  {
    //Serial.println("GPIO:");
    js->execute(F("HIGH=1;LOW=0;INPUT=0;OUTPUT=1;INPUT_PULLUP=2;"));
    js->addNative(F("function pinMode(pin,mode)"), js_pinMode, 0);
    js->addNative(F("function digitalWrite(pin,value)"), js_digitalWrite, 0);
    js->addNative(F("function digitalRead(pin)"), js_digitalRead, 0);
    js->addNative(F("function analogRead()"), js_analogRead, 0);
    js->addNative(F("function analogWrite(pin,value)"), js_analogWrite, 0);
  }else if(name=="Timer")
  {
    js->addNative(F("function setInterval(funct,time_ms)"), js_setInterval, timer);
    js->addNative(F("function clearInterval(id)"), js_clearTimer, timer);
    js->addNative(F("function setTimeout(funct,time_ms)"), js_setTimeout, timer);
    js->execute(F("clearTimeout=clearInterval;"));        
  }else if(name=="JSON")
  {
    js->addNative(F("function JSON.stringify(obj, replacer)"), scJSONStringify, 0); // convert to JSON. replacer is ignored at the moment
  }else if(name=="DHT11")
  {
    js->addNative(F("function DHT11.read(pin)"), scDHT11Read, js); // Get DHT11 Values
  }else if(name=="Math")
  {
    js->addNative(F("function Math.abs(a)"), scMathAbs, 0);
    js->addNative(F("function Math.round(a)"), scMathRound, 0);
    js->addNative(F("function Math.min(a,b)"), scMathMin, 0);
    js->addNative(F("function Math.max(a,b)"), scMathMax, 0);
    //js->addNative(F("function Math.range(x,a,b)"), scMathRange, 0);
    //js->addNative(F("function Math.sign(a)"), scMathSign, 0);
    
    js->addNative(F("function Math.PI()"), scMathPI, 0);
/*    
    js->addNative(F("function Math.toDegrees(a)"), scMathToDegrees, 0);
    js->addNative(F("function Math.toRadians(a)"), scMathToRadians, 0);
    js->addNative(F("function Math.sin(a)"), scMathSin, 0);
    js->addNative(F("function Math.cos(a)"), scMathCos, 0);
    js->addNative(F("function Math.tan(a)"), scMathTan, 0);
    js->addNative(F("function Math.asin(a)"), scMathASin, 0);
    js->addNative(F("function Math.acos(a)"), scMathACos, 0);
    js->addNative(F("function Math.atan(a)"), scMathATan, 0);        
       
    js->addNative(F("function Math.E()"), scMathE, 0);
    js->addNative(F("function Math.log(a)"), scMathLog, 0);
    js->addNative(F("function Math.log10(a)"), scMathLog10, 0);
    js->addNative(F("function Math.exp(a)"), scMathExp, 0);
    js->addNative(F("function Math.pow(a,b)"), scMathPow, 0);
*/    
    js->addNative(F("function Math.sqrt(a)"), scMathSqrt, 0); 
  }
  
}

void addDefaulFunct(UsrData *data) 
{
  CTinyJS *js=data->js;
  //Timer *timer=data->timer;
  
  //DEFAULT
  js->addNative(F("function print(text)"), js_print, 0);  
  js->addNative(F("function dump()"), js_dump, js);
  js->addNative(F("function reset()"), js_reset, data);
  js->addNative(F("function restart()"), js_restart, 0);
  js->addNative(F("function parseInt(str)"), scIntegerParseInt, 0); // string to int
  js->addNative(F("function parseFloat(str)"), scDoubleParseFloat, 0); // string to float
  //js->addNative(F("function exec(jsCode)"), scExec, js); // execute the given code
  js->addNative(F("function eval(jsCode)"), scEval, js); // execute the given string (an expression) and return the result
    // JSON.parse is left out as you can (unsafely!) use eval instead    
  //js->addNative(F("function trace()"), scTrace, js);
  js->addNative(F("function require(name)"), js_require, data); 
}

void js_reset(CScriptVar *v, void *userdata) {
  UsrData *data=(UsrData*)userdata;
  CTinyJS *js = data->js;
  Timer *timer=data->timer;
#ifdef CDINOJS
  MessageProc *proc=data->proc;
#endif  
  js->reset();
  timer->clear();
#ifdef CDINOJS  
  proc->clear();
#endif  
  addDefaulFunct(data);
}

// ----------------------------------------------- Register Functions
#ifdef CDINOJS
void registerFunctions(CTinyJS *js, Timer *timer, MessageProc *proc) {
#endif  
#ifndef CDINOJS
void registerFunctions(CTinyJS *js, Timer *timer) {
#endif
  UsrData *data=new UsrData();
  data->js=js;
  data->timer=timer;
#ifdef CDINOJS  
  data->proc=proc;
#endif  
  addDefaulFunct(data);
}

