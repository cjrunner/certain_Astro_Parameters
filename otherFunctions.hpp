//
//  otherFunctions.hpp
//  certain_Astro_Parameters
//
//  Created by Clifford Campo on 2/21/21.
//  Copyright © 2021 CliffordCampo. All rights reserved.
//
//
//  otherFunctions.hpp
//  astroParameter
//  copied from
//  Created by Clifford Campo on 11/25/20.
//  Copyright © 2020 CliffordCampo. All rights reserved.
//

#ifndef otherFunctions_hpp
#define otherFunctions_hpp
#define GOT_NEITHER_PGRES_COMMAND_OK_OR_PGRES_TUPLES_OK -1
#define GOT_PGRES_COMMAND_OK_ 1
#define GOT_PGRES_TUPLES_OK_ 2
#define TOPOCENTRICRA_MODULO24 24   //Yearly rollover
#define AZIMUTH_MODULO360 360       //Daily  rollover
#define GEOCENTRICRA_MODULO24 24  //Yearly rollover
#define ERA_MODULO360 360           //Daily  rollover
#define LAST_MODULO24 24            //Daily  rollover
#define GAST_MODULO24 24            //Daily  rollover
#include "Constants.hpp"
#include <cmath>
#include <julianToGregorian/julianToGregorian.h>
extern int  processERA(Constants&, void *);
extern int  processGAST(Constants&, void *);
extern int  processLAST(Constants&, void *);
extern int  processSpring(Constants&, void *);
// extern int  processAutumn(Constants&, void *);
extern int  processSummer(Constants&, void *);
extern int  processAutumn(Constants&, void *);
extern int  processWinter(Constants&, void *);
extern int  processSR(Constants&, void *);
extern int  processSS(Constants&, void *);
extern int  processNoonMin(Constants&, void *);
extern int  doERA(Constants &, void *);
extern int  doGAST(Constants&, void *);
extern int  doLAST(Constants&, void *);
extern int  doSpring(Constants&, void *);
extern int  doAutumn(Constants&, void *);
extern int  doSummer(Constants&, void *);
// extern int  doAutumn(Constants&, void *);
extern int  doWinter(Constants&, void *);
extern int  doSR(Constants&, void *);
extern int  doSS(Constants&, void *);
extern int  doNoonMin(Constants&, void *);
extern void printFields(Constants &, int);
extern int getTuples(Constants &c, bool, bool); //first bool if true means we want to display returned tuples. Always place \
copy of tuples in calloc'd memory. Rightmost bool: if true means do PQclear once we've moved FETCH ALL data to our own buffers.
extern int  executeSomeSQL(Constants& , const char *,  bool);
extern void exit_nicely(Constants& c, int rc);
extern void doBegin(Constants& c);                              // 01
extern void doDeclare_srnoonssportal_CursorFor(Constants& c);   // 02
extern void doFetchAll(Constants& c);                           // 03
extern void doClosePortal(Constants& c);                        // 04
extern void doEnd(Constants& c);                                // 05
extern void doExecSQL(Constants& c, const char *someSQL, int expectedOKStatus, bool doPQclear=true);
extern void doFabricateConnectionString(Constants &);
extern void doInterpolation(Constants&, int, string /* msg */, string /* finalBuf */, double /* someModulo */ );
template <typename T>
std::string to_string_with_precision(const T a_value, const int n = 6)
{
    std::ostringstream out;       //Declare working variable, out, as of type ostringstream
    out.precision(n);             //Set the precision for ostringstream out.
    out << std::fixed << a_value; //Make sure the input parameter, a_value, is formated as fixed point.
    return out.str();             //Convert out from ostringstream to a c++-style string and return it as a c++-style string
}
//
// points to dependent variable, found by chebyshev interpolation --(NOT USED)---------+
// points to array of associated dependent variables ------------------------+
// points to array of independent variables -----------------------+         |
// points to THE desired independent variable ---------+           |         |
// points to chebyshev order -----------------+        |           |         |
// points to class ---------+                 |        |           |         |
//                          |                 |        |           |         |
//                          V                 V        V           V         V
extern double doChebyshev(Constants &,    size_t, double *, myParameters *, myParameters *);
extern double cf (double , void *); //The Chebyshev call-back function for gsl_function
void interchange (myParameters *arrayOfInterest, int numberOfRows, int=0, double=0 );
void reviewDo(Constants&, string );
#endif /* otherFunctions_hpp */
