
//
//  otherFunctions.cpp
//  certain_Astro_Parameters
//
//  Created by Clifford Campo on 2/21/21.
//  Copyright © 2021 CliffordCampo. All rights reserved.
//

#include <stdio.h>


//
//  otherFunctions.cpp
//  astroParameter
//  copied from:
//  Created by Clifford Campo on 11/25/20.
//  Copyright © 2020 CliffordCampo. All rights reserved.
//
#include <stdio.h>
#include "otherFunctions.hpp"
#include "Pclp.hpp"
#include <strstream>
#include <iomanip>
#include <sstream>
#include "doingGast.hpp"
void freeCallocdMemory(Constants& v) {
    if (v.ptrArrayIndependent) {
        free(v.ptrArrayIndependent);   //This was acquired in function goExecuteSomeSQL
        v.ptrArrayIndependent = nullptr;
    }
    if (v.ptrArrayDependent) {
        free(v.ptrArrayDependent);      //This was acquired in function goExecuteSomeSQL
        v.ptrArrayDependent = nullptr;
    }
    if (v.ptrAllResultingDependentVaraiables) {
        free(v.ptrAllResultingDependentVaraiables); //This memory was acquired by calling process… function
        v.ptrAllResultingDependentVaraiables = nullptr;
    }
}
void reallocateArrays(Constants &v, bool doInterchange = false) {     //----Added 2022-06-16T08:46:46
    // a) Reassigns ptrArrayIndependent to point to the jdlocal values found in ptrArrayDependent.
    // b) if necessary call iterchange so the jdlocal variables are in ascending order as demanded by the gsl interpolation routines
    // c) Assign to poointer ptrArrayDependent a newly calloc'd memory where the various dependent variables will be stored.
    
    //Free this because we won't be needing to point to an array of zenithdistance values because there is only ONE zenithdistance \
    value we will use and that's what's in v.eventParameter and v.resultJDLOCAL is the local time that we achieved the desired \
    value of ZENITHDISTANCE_SR_OR_SS.
    free(v.ptrArrayIndependent); //Free array of independent variables, containing a 1D array of no longer needed variables (e.g., \
    LAST [used for estimating when LAST = 0 hours], or GAST [used for estimating when GAST = 0 hours], or ERA [used for estimating \
    when ERA = 0º], or zenithdistance [used for estimating when zenithdistance = 90.58333…º for determining sunrise or sunset], or \
    geocentricra = 0, 6, 12, or 18 [used for determining the occurance of Spring, Summer, Fall/Autumn, or Winter, respectively], \
    or topocentricra = 0, 6, 12, or 18 [used for determining the occurance of Spring, Summer, Fall/Autumn, or Winter, respectively].
    //At this point v.resultingDependentVariable points to the local time, in julian day format. This time will become the the \
    independent variable;
    if (doInterchange) interchange (v.ptrArrayDependent, v.numberOfRows); //Interchange the dependent variables (jdlocal) back to \
    original order because we've done this interchange operation twice. jdlocal becomes the independent variable for all subsequent \
    calls to by doInterpolation to doChebyshev. doInterpolation’s job is to put the dependent variables into the dependent variable \
    array.
    v.ptrArrayIndependent = v.ptrArrayDependent; //ReAssign v.ptrArrayIndependent to point to the array of jdlocal times which \
    WAS in the array of Dependent Varaibles (v.ptrArrayDependent)
    //At this point v.resultingDependentVariable points to the local time, in julian day format. This time will become the the \
    independent variable;
    v.ptrArrayDependent = (myParameters *)calloc(sizeof(myParameters), v.numberOfRows); //Now reAllocate a new \
    Array of Dependent Variables to the variable v.ptrArrayDependent. This array will be used by doInterpolation.
}
void genericSeasonProcessing(Constants& v, int Season) {
    for (v.i=0; v.i<v.numberOfRows; v.i++) { // +1
        if ( v.i > 0 && (fabs((v.allFetchedData  + v.i)->geocentricra < fabs((v.allFetchedData + v.i -1)->geocentricra)) ) ) {
            // Above conditional statement checks to see that the current geocentricra value is greater than the previous geocentricra's value \
            2022-06-12T13:04:20  had to change to v.…->geocentricra from v.…-> gast! If current value is less than previous independent value, \
            then add modulus to the current value, thus satisfying gsl interpolation routine's requirement that arrays be monotonically \
            increasing. I have never noticed an instance where geocentricra rolled over.
            (v.allFetchedData + v.i)->geocentricra += (double)v.ptr_eP->modulous; //2022-06-12T13:00:41 had to change to \
            (v.allFetchedData + v.i)->geocentricra from (v.allFetchedData + v.i)->gast !!!
        } else {
            (v.ptrArrayIndependent + v.i)->dbl_value   = (v.allFetchedData + v.i)->geocentricra; //2022-06-12T12:58:39 had to \
            change to (v.allFetchedData + v.i)->geocentricra from (v.allFetchedData + v.i)->gast. A real FuckUp!!
        }  // end of if/else
        (v.ptrArrayIndependent + v.i)->index = v.i;
        (v.ptrArrayDependent + v.i)->dbl_value     = (v.allFetchedData + v.i)->jdlocal;
        (v.ptrArrayDependent + v.i)->index = v.i;
    }     // -1 End of for loop
    //
    //
    //        points to array of associated dependent variables -------------------------------------------+
    //        points to array of independent variables ---------------------------------+                  |
    //        points to THE desired independent variable -------+                       |                  |
    //        points to chebyshev order -----+                  |                       |                  |
    //        points to class --+            |                  |                       |                  |
    //                          |            |                  |                       |                  |
    //                          V            V                  V                       V                  V
    v.resultJDLOCAL=doChebyshev(v,    v.numberOfRows, &v.eventParameter, v.ptrArrayIndependent, v.ptrArrayDependent);
    
    reallocateArrays(v);
    v.ptrAllResultingDependentVaraiables = (namesOfColumns *)calloc(sizeof(namesOfColumns), 1);
    (v.ptrAllResultingDependentVaraiables + JDLOCAL)->jdlocal = v.resultJDLOCAL;
    (v.ptrAllResultingDependentVaraiables + Season)->geocentricra = v.eventParameter;   //This Becomes the aspirational value \
    as presented via the -e command line parameter
    if ( (v.ptrAllResultingDependentVaraiables + Season)->geocentricra >=  (double)v.ptr_eP->modulous )  \
        (v.ptrAllResultingDependentVaraiables + Season)->geocentricra  -=  (double)v.ptr_eP->modulous;  //If necessary, back-out the modulous!
    v.buf.reserve(1500); //Without this reserve method the capacity of the v.buf is 22 characters.
    v.buf.clear(); //Make sure this c++ buffer is clean
    if(v.db2__) {
        cout << "The max size of v.buf is: " << v.buf.max_size() << ",\nthe capacity of v.buf is: " << v.buf.capacity() << endl;
        cout << "After doing v.buf.reserve(1500), the max size of v.buf is now: " << v.buf.max_size() << \
        " characters (i.e. 2^64), the capacity of v.buf is: " << v.buf.capacity() << " characters." << endl;
    }
    v.buf += v.insertIntoStart; //Initialize v.buf with the “INSERT INTO … blah, blah, blah” SQL ”INSERT INTO“  preliminaries
    v.ptrToGregorian = julianToGregorian(&v.resultJDLOCAL, 0) ; //julianToGregorian is a recently (2020-12-02) added dylib. Perhaps \
    I should add gregorianToJuian to my dylib library?
    //    v.buf = v.ptrToGregorian; //Convert the pointer-to-cString to a c++ string in buf. buf is an accumulator of c++ strings
    if (v.db2__) cout << "01. v.ptrToGregorian looks like: " << v.ptrToGregorian << endl;
    v.buf += string(v.ptrToGregorian); // + "', "; // need to convert v.ptrToGregorian from type c-string to type c++string. Then \
    begin accumulating the results of each "Chebyshev'd" parameter in a c++ string buffer named buf, separating each parameter with \
    a comma.
    v.buf += "', ";
    (v.ptrAllResultingDependentVaraiables + SITEID)->siteid = (v.allFetchedData + SITEID)->siteid;  //SITEID doesn't change
    
    //-->TOPOCENTRICRA is Modulous 24 and rolls-over once per year
    doInterpolation(v, TOPOCENTRICRA, "02 TOPOCENTRICRA ", v.ptrC, TOPOCENTRICRA_MODULO24);
    //TOPOCENTRICDEC ranges between +23.4… degrees and -23.4… degrees and becomes zero at Vernal Equinox (Spring)
    doInterpolation(v, TOPOCENTRICDEC, "03 TOPOCENTRICDEC ", v.ptrC, 0);
    //TOPOCENTRICDIS
    doInterpolation(v, TOPOCENTRICDIS, "04 TOPOCENTRICDIS ", v.ptrC, 0);
    //-->AZIMUTH is Modulous 360 and rolls-over daily
    doInterpolation(v, AZIMUTH, "05 AZIMUTH ", v.ptrC, AZIMUTH_MODULO360);
    //ZENITHDISTANCE
    doInterpolation(v, ZENITHDISTANCE, "06 ZENITHDISTANCE ", v.ptrC, 0);
    //-->GEOCENTRICRA is Modulous 24 and rolls-over once per year. However, Summer is defined as when Geocentricra === 6
    //No need to interpolate, we know that for supper the value of geocentricra is 6, the definition of summer.
    // doInterpolation(v, GEOCENTRICRA, "07 GEOCENTRICRA ", v.ptrC, GEOCENTRICRA_MODULO24);
    switch (Season) { //Switch statement of just 4 possibilities is certainly less costly than sprintf.
        case SPRING:
            v.workbuf[GEOCENTRICRA] = "0";
            break;
        case SUMMER:
            v.workbuf[GEOCENTRICRA] = "6";
            break;
        case AUTUMN:
            v.workbuf[GEOCENTRICRA] = "12";
            break;
        case WINTER:
            v.workbuf[GEOCENTRICRA] = "18";
            break;
        default:
            cout << "EXITING due to SPECIFICATION ERROR: Performing season processing but Season Processing incorrectly specified as "  << Season << "!!" << endl;
            exit (-10);
    }
    if (v.db2__) cout << "07 GEOCENTRICRA " << v.workbuf[GEOCENTRICRA] << endl;
    v.buf += v.workbuf[GEOCENTRICRA];    //2022-06-13T08:40:54 added this necessary instruction
    v.buf += ", ";
    //GEOCENTRICDEC
    doInterpolation(v, GEOCENTRICDEC, "08 GEOCENTRICDEC ", v.ptrC, 0);
    //GEOCENTRICDIS
    doInterpolation(v, GEOCENTRICDIS, "09 GEOCENTRICDIS ", v.ptrC, 0);
    //JDUTC
    doInterpolation(v, JDUTC, "10 JDUTC ", v.ptrC, 0);
    //JDLOCAL ONLY ONE VALUE HERE BECAUSE we are looking for just one value of jdlocal, the jdlocal time when ERA === 0
    v.workbuf[JDLOCAL] = to_string_with_precision(v.resultJDLOCAL, v.DESIRED_MAX_PRECISION);    //Convert double to c++ string of \
    15 digit precision
    if (v.db2__) cout << "11 JDLOCAL " << v.workbuf[JDLOCAL] << endl;
    v.buf += v.workbuf[JDLOCAL]; //Concatenate onto buf
    v.buf += ", ";
    //SITEID ONLY ONE VALUE HERE BECAUSE we process only one value of siteid.
    v.workbuf[SITEID] = string(v.ptrWhatThisSiteIdIs); //Converts c-string, as found in “v.ptrWhatThisSiteIdIs”, to a c++ string
    if (v.db2__) cout << "12 SITEID " << v.workbuf[SITEID] << endl;
    v.buf += v.workbuf[SITEID];        // Concatenate siteid onto buf
    v.buf += ", ";                     // Concatenate separator onto buf
    //-->ERA is a Modulous 360 parameter  ONLY ONE VALUE HERE BECAUSE we are looking for predefined value ERA = 0.
    doInterpolation(v, ERA_, "13 ERA ", v.ptrC, ERA_MODULO360);
    //-->LAST is a Modulous 24 parameter
    doInterpolation(v, LAST_, "14 LAST ", v.ptrC, LAST_MODULO24);
    //-->GAST is a Modulous 24 parameter;
    doInterpolation(v, GAST_, "15 GAST ", v.ptrC, GAST_MODULO24);
    //        if(v.eventParameter >= GAST_MODULO24) v.eventParameter -= GAST_MODULO24; //IF necessary, BackOut the modulous
    //        v.workbuf[GAST] = to_string(v.eventParameter);  //Convert double to c++ string of 15 digit precision
    //        if (v.db2__) cout << "15 GAST " << v.workbuf[GAST] << endl;
    //        v.buf += v.workbuf[GAST]; //Concatenate GAST onto buf
    //        v.buf += ", ";
    //VCELX
    doInterpolation(v, VCELX, "16 VCELX ", v.ptrC, 0);
    //VCELY
    doInterpolation(v, VCELY, "17 VCELY ", v.ptrC, 0);
    //VCELZ
    doInterpolation(v, VCELZ, "18 VCELZ ", v.ptrEndCap, 0);  //VCELZ gets the special SQL command-terminating ", 'GAST0');"
    //  v.iI = v.intermediateConnectionString.c_str();  // Then convert the c++ string object into a c-string array of characters, \
    which is what the PostgreSQL prefers because the PostgreSQL libpq routines are written in standard C and know nothing about \
    c++ object-oriented coding techniques.
    v.iI = v.buf.c_str(); //Covert the c++ string to ordinary c-string because PostgreSQL database DOES NOT understand c++ strings!
    if (v.db2__) {
        cout <<   "v.numberOfEntries: " << v.numberOfEntries  << "\nv.numberOf_BS_Entries: " << v.numberOf_BS_Entries << endl;
        cout << "The size of v.buf is " << v.buf.size() << " characters " << endl;
    }
    v.charactersWrittenToFile += fwrite(v.iI, 1, strlen(v.iI), v.fp); // Output into the output file the \
    first logical record, the INSERT INTO statement.
    if (v.executeTheSQL) doExecSQL(v, v.iI, PGRES_COMMAND_OK, true);
    freeCallocdMemory(v);
}
int doGAST(Constants &v, void *ptr_to_void) {
    //NOTE: LAST and GAST share the same 1D and 2D templates because they are so similar.
    v.Constants::some2Darray = (char **)v.ptr2DTemplateGAST0;  //Need to create a unique v.sunTemplateGAST0 rather than “borrow” \
    v.sunTemplateERA0
    v.some2DArraySize = sizeof(v.ptr2DTemplateGAST0[0]);
    v.numberOf2Dentries = (v.some2DArraySize>>1);
    v.some1Darray = v.ptr1DTemplateGAST0;
    v.some1DArraySize = (int)strlen(v.ptr1DTemplateGAST0);
    v.ptrWhatThisEventIs = "GAST";
    v.ptrWhatThisEventDefinitionIs = "0"; //This means we're looking for when GAST == 0
    v.ptrWhatThisFieldIs = "gast";
    v.ptrEndCap = ", 'GAST0');";  //2022-03-13T16:40:29: Added the ', ', the ')', the festooning of GAST0 with ‘'’, and the ';'
    if(v.db2__)  reviewDo(v, "doGAST");
    return 0;
} //End of doGAST
int processGAST(Constants &v,void *ptr_to_void) {
    v.ptr_eP = &v.myEvents[GAST]; //
    v.eventParameter = v.ptr_eP->modulous; //This becomes the aspirational value, or independent parameter, for our \
    Chebyshev Polynomial Interpolation activity.
    if (!v.ptrArrayDependent) v.ptrArrayDependent = (myParameters *)calloc(sizeof(myParameters), v.numberOfRows); //jdlocal will \
    be in ArrayDependent
    if (!v.ptrArrayIndependent) v.ptrArrayIndependent = (myParameters *)calloc(sizeof(myParameters), v.numberOfRows); // Either \
    era, gast, last, zenithdistance [sunrise, sunset, noon], or geocentricra [summer, fall, winter, spring] will be in \
    arrayIndependent because we want to find the specific local time, jdlocal, when era=360, gast = 24 hours, last = 24 hours, \
    zenithdistance=90.5833… [sunrise or sunset], zenithdistance is a minimum value [noon], geocentricra is 6 [summer], \
    12 [fall/autumn], 18 [winter], or 0 [spring]. To accomplish this we set the independent variable (GAST) to zero and then \
    provide doChebyshev, for performing Chebyshev Polynomial Interpolation, with the jdlocal values surrounding the location where \
    what we believe the sought-after gast value lies.
    for (v.i=0; v.i<v.numberOfRows; v.i++) { // +1
        if ( v.i > 0 && (fabs((v.allFetchedData  + v.i)->gast < fabs((v.allFetchedData + v.i -1)->gast)) ) ) { //If current \
            value is less than previous independent value, then add modulus to the current value, thus satisfying \
            gsl interpolation routine's requirement that arrays be monotonically increasing.
            (v.allFetchedData + v.i)->gast += (double)v.ptr_eP->modulous;
        } else {
            (v.ptrArrayIndependent + v.i)->dbl_value   = (v.allFetchedData + v.i)->gast;
        }  // end of if/else
        (v.ptrArrayIndependent + v.i)->index = v.i;
        (v.ptrArrayDependent + v.i)->dbl_value     = (v.allFetchedData + v.i)->jdlocal;
        (v.ptrArrayDependent + v.i)->index = v.i;
    }     // -1 End of for loop
    //
    //
    //        points to array of associated dependent variables -------------------------------------------+
    //        points to array of independent variables ---------------------------------+                  |
    //        points to THE desired independent variable -------+                       |                  |
    //        points to chebyshev order -----+                  |                       |                  |
    //        points to class --+            |                  |                       |                  |
    //                          |            |                  |                       |                  |
    //                          V            V                  V                       V                  V
    v.resultJDLOCAL=doChebyshev(v,    v.numberOfRows, &v.eventParameter, v.ptrArrayIndependent, v.ptrArrayDependent);
    reallocateArrays(v);                                                                                //Added 2022-06-16T08:47:07
    v.ptrIndependentVariable = &v.resultingDependentVariable; //From now on, jdlocal becomes the independent variable
    //
    if(!v.ptrAllResultingDependentVaraiables) v.ptrAllResultingDependentVaraiables=(namesOfColumns *)calloc(sizeof(namesOfColumns), 1);
    (v.ptrAllResultingDependentVaraiables + JDLOCAL)->jdlocal = v.resultJDLOCAL;
    (v.ptrAllResultingDependentVaraiables + GAST)->gast = v.eventParameter;   //This Becomes the aspirational value as presented via \
    the -e command line parameter
    if ( (v.ptrAllResultingDependentVaraiables + GAST)->gast >=  (double)v.ptr_eP->modulous )  \
        (v.ptrAllResultingDependentVaraiables + GAST)->gast  -=  (double)v.ptr_eP->modulous;  //If necessary, back-out the modulous!
    v.buf.reserve(1500); //Without this reserve method the capacity of the v.buf is 22 characters.
    v.buf.clear(); //Make sure this c++ buffer is clean
    if(v.db2__) {
        cout << "The max size of v.buf is: " << v.buf.max_size() << ",\nthe capacity of v.buf is: " << v.buf.capacity() << endl;
        cout << "After doing v.buf.reserve(1500), the max size of v.buf is now: " << v.buf.max_size() << \
        " characters (i.e. 2^64), the capacity of v.buf is: " << v.buf.capacity() << " characters." << endl;
    }
    v.buf += v.insertIntoStart; //Initialize v.buf with the “INSERT INTO … blah, blah, blah” SQL ”INSERT INTO“  preliminaries
    v.ptrToGregorian = julianToGregorian(&v.resultJDLOCAL, 0) ; //julianToGregorian is a recently (2020-12-02) added dylib. Perhaps \
    I should add gregorianToJuian to my dylib library?
    //    v.buf = v.ptrToGregorian; //Convert the pointer-to-cString to a c++ string in buf. buf is an accumulator of c++ strings
    if (v.db2__) cout << "01. v.ptrToGregorian looks like: " << v.ptrToGregorian << endl;
    v.buf += string(v.ptrToGregorian); // + "', "; // need to convert v.ptrToGregorian from type c-string to type c++string. Then \
    begin accumulating the results of each "Chebyshev'd" parameter in a c++ string buffer named buf, separating each parameter with \
    a comma.
    v.buf += "', ";
    (v.ptrAllResultingDependentVaraiables + SITEID)->siteid = (v.allFetchedData + SITEID)->siteid;  //SITEID doesn't change
    
    
    //-->TOPOCENTRICRA is Modulous 24 and rolls-over once per year
    doInterpolation(v, TOPOCENTRICRA, "02 TOPOCENTRICRA ", v.ptrC, TOPOCENTRICRA_MODULO24);
    //TOPOCENTRICDEC ranges between +23.4… degrees and -23.4… degrees and becomes zero at Vernal Equinox (Spring)
    doInterpolation(v, TOPOCENTRICDEC, "03 TOPOCENTRICDEC ", v.ptrC, 0);
    //TOPOCENTRICDIS
    doInterpolation(v, TOPOCENTRICDIS, "04 TOPOCENTRICDIS ", v.ptrC, 0);
    //-->AZIMUTH is Modulous 360 and rolls-over daily
    doInterpolation(v, AZIMUTH, "05 AZIMUTH ", v.ptrC, AZIMUTH_MODULO360);
    //ZENITHDISTANCE
    doInterpolation(v, ZENITHDISTANCE, "06 ZENITHDISTANCE ", v.ptrC, 0);
    //-->GEOCENTRICRA is Modulous 24 and rolls-over once per year
    doInterpolation(v, GEOCENTRICRA, "07 GEOCENTRICRA ", v.ptrC, GEOCENTRICRA_MODULO24);
    //GEOCENTRICDEC
    doInterpolation(v, GEOCENTRICDEC, "08 GEOCENTRICDEC ", v.ptrC, 0);
    //GEOCENTRICDIS
    doInterpolation(v, GEOCENTRICDIS, "09 GEOCENTRICDIS ", v.ptrC, 0);
    //JDUTC
    doInterpolation(v, JDUTC, "10 JDUTC ", v.ptrC, 0);
    //JDLOCAL ONLY ONE VALUE HERE BECAUSE we are looking for just one value of jdlocal, the jdlocal time when ERA === 0
    v.workbuf[JDLOCAL] = to_string_with_precision(v.resultJDLOCAL, v.DESIRED_MAX_PRECISION);    //Convert double to c++ string of \
    15 digit precision
    if (v.db2__) cout << "11 JDLOCAL " << v.workbuf[JDLOCAL] << endl;
    v.buf += v.workbuf[JDLOCAL]; //Concatenate onto buf
    v.buf += ", ";
    //SITEID ONLY ONE VALUE HERE BECAUSE we process only one value of siteid.
    v.workbuf[SITEID] = string(v.ptrWhatThisSiteIdIs); //Convert c-string to c++ string
    if (v.db2__) cout << "12 SITEID " << v.workbuf[SITEID] << endl;
    v.buf += v.workbuf[SITEID];        //Concatenate onto buf
    v.buf += ", ";
    //-->ERA is a Modulous 360 parameter  ONLY ONE VALUE HERE BECAUSE we are looking for predefined value ERA = 0.
    doInterpolation(v, ERA_, "13 ERA ", v.ptrC, ERA_MODULO360);
    //-->LAST is a Modulous 24 parameter
    doInterpolation(v, LAST_, "14 LAST ", v.ptrC, LAST_MODULO24);
    //-->GAST is a Modulous 24 parameter
    if(v.eventParameter >= GAST_MODULO24) v.eventParameter -= GAST_MODULO24; //IF necessary, BackOut the modulous
    v.workbuf[GAST] = to_string(v.eventParameter);  //Convert double to c++ string of 15 digit precision
    if (v.db2__) cout << "15 GAST " << v.workbuf[GAST] << endl;
    v.buf += v.workbuf[GAST]; //Concatenate GAST onto buf
    v.buf += ", ";
    //VCELX
    doInterpolation(v, VCELX, "16 VCELX ", v.ptrC, 0);
    //VCELY
    doInterpolation(v, VCELY, "17 VCELY ", v.ptrC, 0);
    //VCELZ
    doInterpolation(v, VCELZ, "18 VCELZ ", v.ptrEndCap, 0);  //VCELZ gets the special SQL command-terminating ", 'GAST0');"
    //  v.iI = v.intermediateConnectionString.c_str();  // Then convert the c++ string object into a c-string array of characters, \
    which is what the PostgreSQL prefers because the PostgreSQL libpq routines are written in standard C and know nothing about \
    c++ object-oriented coding techniques.
    v.iI = v.buf.c_str(); //Covert the c++ string to ordinary c-string because PostgreSQL database DOES NOT understand c++ strings!
    if (v.db2__) {
        cout <<   "v.numberOfEntries: " << v.numberOfEntries  << "\nv.numberOf_BS_Entries: " << v.numberOf_BS_Entries << endl;
        cout << "The size of v.buf is " << v.buf.size() << " characters " << endl;
    }
    v.charactersWrittenToFile += fwrite(v.iI, 1, strlen(v.iI), v.fp); // Output into the output file the \
    first logical record, the INSERT INTO statement.
    if (v.executeTheSQL) doExecSQL(v, v.iI, PGRES_COMMAND_OK, true);
    freeCallocdMemory(v);
    return 0;
}  //End of processGAST
int doLAST(Constants &v, void *ptr_to_void) {
    //NOTE: LAST and GAST share the same 1D and 2D templates because they are so similar.
    v.Constants::some2Darray = (char **)v.ptr2DTemplateGAST0;  //Need to create a unique v.sunTemplateGAST0 rather than “borrow” v.sunTemplateERA0
    v.some2DArraySize = sizeof(v.ptr2DTemplateGAST0[0]);
    v.numberOf2Dentries = (v.some2DArraySize>>1);  //Here we're interested in just the number of rows; number of columns is 2.
    v.some1Darray = v.ptr1DTemplateGAST0;
    v.some1DArraySize = (int)strlen(v.ptr1DTemplateGAST0);
    v.ptrWhatThisEventIs = "LAST";
    v.ptrWhatThisEventDefinitionIs = "0";
    v.ptrWhatThisFieldIs = "last";
    v.ptrEndCap = ", 'LAST0');"; //2022-03-14T16:51:25: Added the ', ', the ')', the festooning of LAST0 with ‘'’, and the ';'
    if(v.db2__)  reviewDo(v, "doLAST");
    return 0;
} //End of doLAST
int processLAST(Constants &v,void *ptr_to_void) {
    v.ptr_eP = &v.myEvents[LAST]; //
    v.eventParameter = v.ptr_eP->modulous;
    //    v.ptrArrayDependent = (myParameters *)calloc(sizeof(myParameters), v.numberOfRows); //jdlocal will be in ArrayDependent
    //    v.ptrArrayIndependent = (myParameters *)calloc(sizeof(myParameters), v.numberOfRows); // Either era, gast, last, zenithdistance \
    [sunrise, sunset, noon], or geocentricra [summer, fall, winter, spring] will be in arrayIndepented because we want to find the \
    specific time, jdlocal, when era=360, gast = 24 hours, last = 24 hours, zenithdistance=90.5833… [sunrise or sunset], \
    zenithdistance is a minimum value [noon], geocentricra is 6 [summer], 12 [fall/autumn], 18 [winter], or 0 [spring].  \
    To accomplish this we set the independent variable (LAST) to zero and then provide doChebyshev, for performing \
    Chebyshev Polynomial Interpolation, with the jdlocal values surrounding the location where what we believe the sought-after \
    gast value lies.
    for (v.i=0; v.i<v.numberOfRows; v.i++) { // +1
        if ( v.i > 0 && (fabs((v.allFetchedData  + v.i)->last < fabs((v.allFetchedData + v.i -1)->last)) ) ) { //If current \
            value is less than previous independent value, then add modulus to the current value, thus satisfying \
            gsl interpolation routine's requirement that arrays be monotonically increasing.
            (v.allFetchedData + v.i)->last += (double)v.ptr_eP->modulous;
        } else {
            (v.ptrArrayIndependent + v.i)->dbl_value   = (v.allFetchedData + v.i)->last;
        }  // end of if/else
        (v.ptrArrayIndependent + v.i)->index = v.i;
        (v.ptrArrayDependent + v.i)->dbl_value     = (v.allFetchedData + v.i)->jdlocal;
        (v.ptrArrayDependent + v.i)->index = v.i;
    }     // -1 End of for loop
    //
    //
    //        points to array of associated dependent variables -------------------------------------------+
    //        points to array of independent variables ---------------------------------+                  |
    //        points to THE desired independent variable -------+                       |                  |
    //        points to chebyshev order -----+                  |                       |                  |
    //        points to class --+            |                  |                       |                  |
    //                          |            |                  |                       |                  |
    //                          V            V                  V                       V                  V
    v.resultJDLOCAL=doChebyshev(v,    v.numberOfRows, &v.eventParameter, v.ptrArrayIndependent, v.ptrArrayDependent);
    reallocateArrays(v); //Added 2022-06-16T09:03:30
    v.ptrIndependentVariable = &v.resultingDependentVariable; //From now on, make jdlocal the independent variable
    v.ptrAllResultingDependentVaraiables = (namesOfColumns *)calloc(sizeof(namesOfColumns), 1);
    (v.ptrAllResultingDependentVaraiables + JDLOCAL)->jdlocal = v.resultJDLOCAL;
    (v.ptrAllResultingDependentVaraiables + LAST)->last = v.eventParameter;   //This Becomes the aspirational value as presented via \
    the -e command line parameter
    if ( (v.ptrAllResultingDependentVaraiables + LAST)->last >=  (double)v.ptr_eP->modulous )  \
        (v.ptrAllResultingDependentVaraiables + LAST)->last -= (double)v.ptr_eP->modulous;  //Back-out the modulous if necessary
    v.buf.reserve(1500); //Without this the capacity of the v.buf is 22 characters.
    v.buf.clear(); //Make sure this c++ buffer is clean
    if(v.db2__) {
        cout << "The max size of v.buf is: " << v.buf.max_size() << ",\nthe capacity of v.buf is: " << v.buf.capacity() << endl;
        cout << "After doing v.buf.reserve(1500), the capacity of v.buf is now: " << v.buf.max_size() << \
        " characters (i.e. 2^64), the capacity of v.buf is: " << v.buf.capacity() << " characters." << endl;
    }
    v.buf += v.insertIntoStart; //Initialize v.buf with the “INSERT INTO … blah, blah, blah” SQL ”INSERT INTO“  preliminaries
    v.ptrToGregorian = julianToGregorian(&v.resultJDLOCAL, 0) ; //julianToGregorian is a recently (2020-12-02) added dylib. Perhaps \
    I should add gregorianToJuian to my dylib library?
    //    v.buf = v.ptrToGregorian; //Convert the pointer-to-cString to a c++ string in buf. buf is an accumulator of c++ strings
    if (v.db2__) cout << "01. v.ptrToGregorian looks like: " << v.ptrToGregorian << endl;
    v.buf += string(v.ptrToGregorian); // + "', "; // need to convert v.ptrToGregorian from type c-string to type c++string. Then \
    begin accumulating the results of each "Chebyshev'd" parameter in a c++ string buffer named buf, separating each parameter with \
    a comma.
    v.buf += "', ";
    (v.ptrAllResultingDependentVaraiables + SITEID)->siteid = (v.allFetchedData + SITEID)->siteid;  //SITEID doesn't change
    
    //-->TOPOCENTRICRA is Modulous 24 and rolls-over once per year
    doInterpolation(v, TOPOCENTRICRA, "02 TOPOCENTRICRA ", v.ptrC, TOPOCENTRICRA_MODULO24);
    //TOPOCENTRICDEC ranges between +23.4… degrees and -23.4… degrees and becomes zero at Vernal Equinox (Spring)
    doInterpolation(v, TOPOCENTRICDEC, "03 TOPOCENTRICDEC ", v.ptrC, 0);
    //TOPOCENTRICDIS
    doInterpolation(v, TOPOCENTRICDIS, "04 TOPOCENTRICDIS ", v.ptrC, 0);
    //-->AZIMUTH is Modulous 360 and rolls-over daily
    doInterpolation(v, AZIMUTH, "05 AZIMUTH ", v.ptrC, AZIMUTH_MODULO360);
    //ZENITHDISTANCE
    doInterpolation(v, ZENITHDISTANCE, "06 ZENITHDISTANCE ", v.ptrC, 0);
    //-->GEOCENTRICRA is Modulous 24 and rolls-over once per year
    doInterpolation(v, GEOCENTRICRA, "07 GEOCENTRICRA ", v.ptrC, GEOCENTRICRA_MODULO24);
    //GEOCENTRICDEC
    doInterpolation(v, GEOCENTRICDEC, "08 GEOCENTRICDEC ", v.ptrC, 0);
    //GEOCENTRICDIS
    doInterpolation(v, GEOCENTRICDIS, "09 GEOCENTRICDIS ", v.ptrC, 0);
    //JDUTC
    doInterpolation(v, JDUTC, "10 JDUTC ", v.ptrC, 0);
    //JDLOCAL ONLY ONE VALUE HERE BECAUSE we are looking for just one value of jdlocal, the jdlocal time when ERA === 0
    v.workbuf[JDLOCAL] = to_string_with_precision(v.resultJDLOCAL, v.DESIRED_MAX_PRECISION);    //Convert double to c++ string of \
    15 digit precision
    if (v.db2__) cout << "11 JDLOCAL " << v.workbuf[JDLOCAL] << endl;
    v.buf += v.workbuf[JDLOCAL]; //Concatenate onto buf
    v.buf += ", ";
    //SITEID ONLY ONE VALUE HERE BECAUSE we process only one value of siteid.
    v.workbuf[SITEID] = string(v.ptrWhatThisSiteIdIs); //Convert c-string to c++ string
    if (v.db2__) cout << "12 SITEID " << v.workbuf[SITEID] << endl;
    v.buf += v.workbuf[SITEID];        //Concatenate onto buf
    v.buf += ", ";
    //-->ERA is a Modulous 360 parameter  ONLY ONE VALUE HERE BECAUSE we are looking for predefined value ERA = 0.
    doInterpolation(v, ERA_, "13 ERA ", v.ptrC, ERA_MODULO360);
    //-->LAST is a Modulous 24 parameter
    if(v.eventParameter >= LAST_MODULO24) v.eventParameter -= LAST_MODULO24; //IF necessary, BackOut the modulous
    v.workbuf[LAST] = to_string(v.eventParameter);  //Convert double to c++ string of 15 digit precision
    if (v.db2__) cout << "14 LAST " << v.workbuf[LAST] << endl;
    v.buf += v.workbuf[LAST]; //Concatenate LAST onto buf
    v.buf += ", ";
    //-->GAST is a Modulous 24 parameter
    doInterpolation(v, GAST_, "15 GAST ", v.ptrC, GAST_MODULO24);
    //VCELX
    doInterpolation(v, VCELX, "16 VCELX ", v.ptrC, 0);
    //VCELY
    doInterpolation(v, VCELY, "17 VCELY ", v.ptrC, 0);
    //VCELZ
    doInterpolation(v, VCELZ, "18 VCELZ ", v.ptrEndCap, 0);
    //  v.iI = v.intermediateConnectionString.c_str();  // Then convert the c++ string object into a c-string array of characters, \
    which is what the PostgreSQL prefers because the PostgreSQL libpq routines are written in standard C and know nothing about \
    c++ object-oriented coding techniques.
    v.iI = v.buf.c_str(); //Covert the c++ string to ordinary c-string because PostgreSQL database DOES NOT understand c++ strings!
    if (v.db2__) {
        cout <<   "v.numberOfEntries: " << v.numberOfEntries  << "\nv.numberOf_BS_Entries: " << v.numberOf_BS_Entries << endl;
        cout << "The size of v.buf is " << v.buf.size() << " characters " << endl;
    }
    v.charactersWrittenToFile += fwrite(v.iI, 1, strlen(v.iI), v.fp); // Output into the output file the \
    first logical record, the INSERT INTO statement.
    if (v.executeTheSQL) doExecSQL(v, v.iI, PGRES_COMMAND_OK, true);
    freeCallocdMemory(v);
    return 0;
}  //End of processLAST

int doSR(Constants &v, void* myb) {
    v.some2Darray = (char **)v.sunRiseArray; //sunRiseArray is in Constants.hpp
    v.some1Darray = (char *)v.ptrSQLTemplateSR;
    v.some2DArraySize = sizeof(v.sunRiseArray[0]); //should be 10 for 10 rows
    v.numberOf2Dentries = (v.some2DArraySize>>1);
    v.some1DArraySize = (int)strlen(v.ptrSQLTemplateSR);
    v.ptrWhatThisEventIs = "SR";
    v.ptrWhatThisEventDefinitionIs = ZENITHDISTANCE_SR_OR_SS;
    v.ptrWhatThisFieldIs = "zenithdistance";
    v.ptrEndCap = ", 'SunRise');";
    if ( v.db2__) reviewDo(v, "doSR");
    return 0;
} //End of doSR
// ---------------------------------------------------------start interchange----------------------------------------------------

void interchange (/* Constants &v ,*/ myParameters *arrayOfInterest, int numberOfRows, int index, double workDoubleUpper ) {
    /*--- INTERCHANGE first Element With Last Element; then INTERCHANGE next-to-first element with next-to-last element; etc. … … …
     because gsl_chebyshev DEMANDS that Independent Variable be in monotonically increasing
     Assumes that the values in arrayOfInterest have already been sorted in either ascending or descending order. So if
     arrayOfInterest was sorted in ascending order on entry to interchange, then arrayOfInterest will be in descending order
     on exit from function interchange
     */
    index = numberOfRows>>1;
    index--;
    while (index >= 0) {
        workDoubleUpper  = (arrayOfInterest + index)->dbl_value;  //Put the upper value into holding area called workDouble.
        (arrayOfInterest + index)->dbl_value = (arrayOfInterest + (numberOfRows-1) - index)->dbl_value; //Get the last value; Make \
        what was the upper value the last value
        (arrayOfInterest + (numberOfRows-1) - index)->dbl_value = workDoubleUpper;
        index--;
    }
}
// ----------------------------------------------------------end interchange----------------------------------------------------
int processSR(Constants &v, void *ptr_to_void) { // sunrise === zenithdistance = 90.5833
    v.ptr_eP->modulous = 0; //zenithdistance has no modulus
    v.ptr_eP = &v.myEvents[SUNRISE];
    v.eventParameter = atof(ZENITHDISTANCE_SR_OR_SS);
    //Callocing of two arrays in now done in goExecuteSomeSQL
    //  v.ptrArrayDependent = (myParameters *)calloc(sizeof(myParameters), v.numberOfRows); //jdlocal will be in ArrayDependent
    //  v.ptrArrayIndependent = (myParameters *)calloc(sizeof(myParameters), v.numberOfRows); // Either era, gast, last, zenithdistance \
    [sunrise, sunset, noon], or geocentricra [summer, fall, winter, spring] will be in arrayIndepented because we want to find the \
    specific time, jdlocal, when era=360, gast = 24 hours, last = 24 hours, zenithdistance=90.5833… [sunrise or sunset], \
    zenithdistance is a minimum value [noon], geocentricra is 6 [summer], 12 [fall/autumn], 18 [winter], or 0 [spring].  \
    To accomplish this we set the independent variable (zenithdistance) to 90.5833 and then provide doChebyshev, for performing \
    Chebyshev Polynomial Interpolation, with the jdlocal values surrounding the location where what we believe the sought-after \
    gast value lies.
    //NOTE that zenithdistance decreases as we approach time of sunrise (zenithdistance === 90.5833) and keeps decreasing until we \
    reach noon at which point zenithdistance begins to increase again. Then, half-a-day-later (~ 12 hours), zenith distance begins \
    to decrease again. For sunrise and sunset processing, we will rely upon the SQL SELECT statement, that collected the data, that \
    this function will have the data sorted in a manner satisfactory to the gsl routines' requirements that data be sorted in a \
    monotonically increasing manner.
    /*   The following code is commented out because it came from the legacy processERA function and is not germaine to processSR */
    for (v.i=0; v.i < v.numberOfRows; v.i++) { // +1
        (v.ptrArrayIndependent + v.i)->dbl_value   = (v.allFetchedData + v.i)->zenithdistance;
        (v.ptrArrayIndependent + v.i)->index = v.i;
        (v.ptrArrayDependent + v.i)->dbl_value     = (v.allFetchedData + v.i)->jdlocal;
        (v.ptrArrayDependent + v.i)->index = v.i;
    }     // -1 End of for loop
    
    interchange ( v.ptrArrayIndependent, v.numberOfRows); //Interchange the independent variables effecting reversing the order
    interchange ( v.ptrArrayDependent, v.numberOfRows);     //Interchange the dependent variables effecting reversing the order
    //---
    //
    //
    //        points to array of associated dependent variables -------------------------------------------+
    //        points to array of independent variables ---------------------------------+                  |
    //        points to THE desired independent variable -------+                       |                  |
    //        points to chebyshev order -----+                  |                       |                  |
    //        points to class --+            |                  |                       |                  |
    //                          |            |                  |                       |                  |
    //                          V            V                  V                       V                  V
    v.resultJDLOCAL=doChebyshev(v,    v.numberOfRows, &v.eventParameter, v.ptrArrayIndependent, v.ptrArrayDependent);
    reallocateArrays(v, true);
    v.ptrIndependentVariable = &v.resultingDependentVariable; //From now on, make jdlocal the independent variable
    v.ptrAllResultingDependentVaraiables = (namesOfColumns *)calloc(sizeof(namesOfColumns), 1);
    (v.ptrAllResultingDependentVaraiables + JDLOCAL)->jdlocal = v.resultJDLOCAL;
    (v.ptrAllResultingDependentVaraiables + ZENITHDISTANCE)->zenithdistance = v.eventParameter;   //ZENITHDISTANCE Becomes the \
    aspirational value as presented via the -e command line parameter. With zenithdistance there are not sudden downward jumps \
    toward zeron when we've reached the “modulo limit”
    //----
    //At this point v.resultingDependentVariable points to the local time, in julian day format. This time will become the the \
    independent variable;
    
    v.buf.reserve(1500); //Without this the capacity of the v.buf is 22 characters.
    v.buf.clear(); //Make sure this c++ buffer is clean
    if(v.db2__) {
        cout << "The max size of v.buf is: " << v.buf.max_size() << ",\nthe capacity of v.buf is: " << v.buf.capacity() << endl;
        cout << "After doing v.buf.reserve(1500), the capacity of v.buf is now: " << v.buf.max_size() << \
        " characters (i.e. 2^64), the capacity of v.buf is: " << v.buf.capacity() << " characters." << endl;
    } //
    v.buf += v.insertIntoStart; //Initialize v.buf with the “INSERT INTO … blah, blah, blah” SQL ”INSERT INTO“  preliminaries
    v.ptrToGregorian = julianToGregorian(&v.resultJDLOCAL, 0) ; //julianToGregorian is a recently (2020-12-02) added dylib. Perhaps \
    I should add gregorianToJuian to my dylib library?
    //    v.buf = v.ptrToGregorian; //Convert the pointer-to-cString to a c++ string in buf. buf is an accumulator of c++ strings
    if (v.db2__) cout << "01. v.ptrToGregorian looks like: " << v.ptrToGregorian << endl;
    v.buf += string(v.ptrToGregorian); // + "', "; // need to convert v.ptrToGregorian from type c-string to type c++string. Then \
    begin accumulating the results of each "Chebyshev'd" parameter in a c++ string buffer named buf, separating each parameter with \
    a comma.
    v.buf += "', ";
    (v.ptrAllResultingDependentVaraiables + SITEID)->siteid = (v.allFetchedData + SITEID)->siteid;  //SITEID doesn't change
    
    //-->TOPOCENTRICRA is Modulous 24 and rolls-over once per year
    doInterpolation(v, TOPOCENTRICRA, "02 TOPOCENTRICRA ", v.ptrC, TOPOCENTRICRA_MODULO24);
    //TOPOCENTRICDEC ranges between +23.4… degrees and -23.4… degrees and becomes zero at Vernal Equinox (Spring)
    doInterpolation(v, TOPOCENTRICDEC, "03 TOPOCENTRICDEC ", v.ptrC, 0);
    //TOPOCENTRICDIS
    doInterpolation(v, TOPOCENTRICDIS, "04 TOPOCENTRICDIS ", v.ptrC, 0);
    //-->AZIMUTH is Modulous 360 and rolls-over daily
    doInterpolation(v, AZIMUTH, "05 AZIMUTH ", v.ptrC, AZIMUTH_MODULO360);
    //ZENITHDISTANCE
    // --  doInterpolation(v, ZENITHDISTANCE, "06 ZENITHDISTANCE ", v.ptrC, 0);  //This is what we did for processERA, upon which processSR is based.
    v.workbuf[ZENITHDISTANCE]=ZENITHDISTANCE_SR_OR_SS; // Now ZENITHDISTANCE, the sixth (06) column in table named \
    “tbl_sun_loc_site” of database named “LocalWeather”,  is what we specified it to be
    if (v.db2__) cout << "06 ZENITHDISTANCE " << v.workbuf[ZENITHDISTANCE] << endl;
    v.buf += v.workbuf[ZENITHDISTANCE];
    v.buf += ", ";
    //-->GEOCENTRICRA is Modulous 24 and rolls-over once per year
    doInterpolation(v, GEOCENTRICRA, "07 GEOCENTRICRA ", v.ptrC, GEOCENTRICRA_MODULO24);
    //GEOCENTRICDEC
    doInterpolation(v, GEOCENTRICDEC, "08 GEOCENTRICDEC ", v.ptrC, 0);
    //GEOCENTRICDIS
    doInterpolation(v, GEOCENTRICDIS, "09 GEOCENTRICDIS ", v.ptrC, 0);
    //JDUTC
    doInterpolation(v, JDUTC, "10 JDUTC ", v.ptrC, 0);
    //JDLOCAL ONLY ONE VALUE HERE BECAUSE we are looking for just one value of jdlocal, the jdlocal time when ERA === 0
    v.workbuf[JDLOCAL] = to_string_with_precision(v.resultJDLOCAL, v.DESIRED_MAX_PRECISION); //Convert type double to c++ string \
    having precision as specified in v.DESIRED_MAX_PRECISION [usually 15 digits]
    if (v.db2__) cout << "11 JDLOCAL " << v.workbuf[JDLOCAL] << endl;
    v.buf += v.workbuf[JDLOCAL]; //Concatenate onto buf
    v.buf += ", ";
    //SITEID ONLY ONE VALUE HERE BECAUSE we process only one value of siteid.
    v.workbuf[SITEID] = string(v.ptrWhatThisSiteIdIs); //Convert c-string to c++ string
    if (v.db2__) cout << "12 SITEID " << v.workbuf[SITEID] << endl;
    v.buf += v.workbuf[SITEID];        //Concatenate onto buf
    v.buf += ", ";
    //-->ERA is a Modulous 360 parameter  ONLY ONE VALUE HERE BECAUSE we are looking for predefined value ERA = 0.
    //    if(v.eventParameter >= ERA_MODULO360) v.eventParameter -= ERA_MODULO360; //IF necessary, BackOut the modulous
    //    v.workbuf[ERA] = to_string(v.eventParameter);  //Convert double to c++ string of 15 digit precision
    //    if (v.db2__) cout << "13 ERA " << v.workbuf[ERA] << endl;
    //    v.buf += v.workbuf[ERA]; //Concatenate ERA onto buf
    //    v.buf += ", ";
    doInterpolation(v, ERA_, "13 ERA ", v.ptrC, ERA_MODULO360);
    //-->LAST is a Modulous 24 parameter
    doInterpolation(v, LAST_, "14 LAST ", v.ptrC, LAST_MODULO24);
    //-->GAST is a Modulous 24 parameter
    doInterpolation(v, GAST_, "15 GAST ", v.ptrC, GAST_MODULO24);
    //VCELX
    doInterpolation(v, VCELX, "16 VCELX ", v.ptrC, 0);
    //VCELY
    doInterpolation(v, VCELY, "17 VCELY ", v.ptrC, 0);
    //VCELZ
    doInterpolation(v, VCELZ, "18 VCELZ ", v.ptrEndCap, 0);
    //Like `v.iI = v.intermediateConnectionString.c_str();`,  convert the c++ string object into a legacy c-string array \
    of characters, which is what the PostgreSQL prefers because the PostgreSQL libpq routines are written in standard, legacy, C \
    and knows nothing about c++ objects and other complexities.
    v.iI = v.buf.c_str(); //Covert the c++ string to ordinary c-string because PostgreSQL database DOES NOT understand c++ strings!
    if (v.db2__) {
        cout <<   "v.numberOfEntries: " << v.numberOfEntries  << "\nv.numberOf_BS_Entries: " << v.numberOf_BS_Entries << endl;
        cout << "The size of v.buf is " << v.buf.size() << " characters " << endl;
    }
    v.charactersWrittenToFile += fwrite(v.iI, 1, strlen(v.iI), v.fp); // Output into the output file the \
    first logical record, the INSERT INTO statement.
    if (v.executeTheSQL) doExecSQL(v, v.iI, PGRES_COMMAND_OK, true);
    freeCallocdMemory(v);
    return 0;
    
} //End of processSR

int doERA(Constants &v, void *myb) {
    v.Constants::some2Darray = (char **)v.sunTemplateERA0;
    v.some1Darray = v.ptrSQLTemplateERA0;
    v.some2DArraySize = sizeof(v.ptr2DTemplateGAST0[0]);
    v.numberOf2Dentries = (v.some2DArraySize>>1);
    v.some1Darray = v.ptr1DTemplateGAST0;
    v.some1DArraySize = (int)strlen(v.ptr1DTemplateGAST0);
    v.ptrWhatThisEventIs = "ERA";                      //This is the parameter associated with the -E command line keword/switch
    v.ptrWhatThisEventDefinitionIs = "0";              //Or Modulo 360
    v.ptrWhatThisFieldIs = "era";                      //This is the field name within table tbl_sun_loc_site
    v.ptrEndCap = ", 'ERA0');"; //2022-03-14T16:51:25: Added the ', ', the ')', the festooning of ERA0 with ‘'’, and the ';'
    if (v.db2__) reviewDo(v, "doERA");
    return 0;
} //End of doERA
int processERA(Constants &v, void *ptr_to_void) { // +0
    v.ptr_eP = &v.myEvents[ERA];
    v.eventParameter = v.ptr_eP->modulous;
    //Callocing in now done in goExecuteSomeSQL.cpp
    //   v.ptrArrayDependent = (myParameters *)calloc(sizeof(myParameters), v.numberOfRows); //jdlocal will be in ArrayDependent
    //   v.ptrArrayIndependent = (myParameters *)calloc(sizeof(myParameters), v.numberOfRows); // Either era, gast, last, zenithdistance \
    [sunrise, sunset, noon], or geocentricra [summer, fall, winter, spring] will be in arrayIndepented because we want to find the \
    specific time, jdlocal, when era=360, gast = 24 hours, last = 24 hours, zenithdistance=90.5833… [sunrise or sunset], \
    zenithdistance is a minimum value [noon], geocentricra is 6 [summer], 12 [fall/autumn], 18 [winter], or 0 [spring].  \
    To accomplish this we set the independent variable (ERA) to zero and then provide doChebyshev, for performing \
    Chebyshev Polynomial Interpolation, with the jdlocal values surrounding the location where what we believe the sought-after \
    gast value lies.
    for (v.i=0; v.i<v.numberOfRows; v.i++) { // +1
        if ( v.i > 0 && (fabs((v.allFetchedData  + v.i)->era < fabs((v.allFetchedData + v.i -1)->era)) ) ) { //If current \
            value is less than previous independent value, then add modulus to the current value, thus satisfying \
            gsl interpolation routine's requirement that arrays be monotonically increasing.
            (v.allFetchedData + v.i)->era += (double)v.ptr_eP->modulous;
        } else {
            (v.ptrArrayIndependent + v.i)->dbl_value   = (v.allFetchedData + v.i)->era;
        }  // end of if/else
        (v.ptrArrayIndependent + v.i)->index = v.i;
        (v.ptrArrayDependent + v.i)->dbl_value     = (v.allFetchedData + v.i)->jdlocal;
        (v.ptrArrayDependent + v.i)->index = v.i;
    }     // -1 End of for loop
    //
    //
    //        points to array of associated dependent variables -------------------------------------------+
    //        points to array of independent variables ---------------------------------+                  |
    //        points to THE desired independent variable -------+                       |                  |
    //        points to chebyshev order -----+                  |                       |                  |
    //        points to class --+            |                  |                       |                  |
    //                          |            |                  |                       |                  |
    //                          V            V                  V                       V                  V
    v.resultJDLOCAL=doChebyshev(v,    v.numberOfRows, &v.eventParameter, v.ptrArrayIndependent, v.ptrArrayDependent);
    reallocateArrays(v);
    v.ptrAllResultingDependentVaraiables = (namesOfColumns *)calloc(sizeof(namesOfColumns), 1);
    (v.ptrAllResultingDependentVaraiables + JDLOCAL)->jdlocal = v.resultJDLOCAL;
    (v.ptrAllResultingDependentVaraiables + ERA)->era = v.eventParameter;   //This Becomes the aspirational value as presented via \
    the -e command line parameter
    if ( (v.ptrAllResultingDependentVaraiables + ERA)->era >= ERA_MODULO360 )  \
        (v.ptrAllResultingDependentVaraiables + ERA)->era -= ERA_MODULO360;  //Back-out the modulous if necessary
    v.buf.reserve(1500); //Without this the capacity of the v.buf is 22 characters.
    v.buf.clear(); //Make sure this c++ buffer is clean
    if(v.db2__) {
        cout << "The max size of v.buf is: " << v.buf.max_size() << ",\nthe capacity of v.buf is: " << v.buf.capacity() << endl;
        cout << "After doing v.buf.reserve(1500), the capacity of v.buf is now: " << v.buf.max_size() << \
        " characters (i.e. 2^64), the capacity of v.buf is: " << v.buf.capacity() << " characters." << endl;
    } //
    v.buf += v.insertIntoStart; //Initialize v.buf with the “INSERT INTO … blah, blah, blah” SQL ”INSERT INTO“  preliminaries
    v.ptrToGregorian = julianToGregorian(&v.resultJDLOCAL, 0) ; //julianToGregorian is a recently (2020-12-02) added dylib. Perhaps \
    I should add gregorianToJuian to my dylib library?
    //    v.buf = v.ptrToGregorian; //Convert the pointer-to-cString to a c++ string in buf. buf is an accumulator of c++ strings
    if (v.db2__) cout << "01. v.ptrToGregorian looks like: " << v.ptrToGregorian << endl;
    v.buf += string(v.ptrToGregorian); // + "', "; // need to convert v.ptrToGregorian from type c-string to type c++string. Then \
    begin accumulating the results of each "Chebyshev'd" parameter in a c++ string buffer named buf, separating each parameter with \
    a comma.
    v.buf += "', ";
    (v.ptrAllResultingDependentVaraiables + SITEID)->siteid = (v.allFetchedData + SITEID)->siteid;  //SITEID doesn't change
    
    //-->TOPOCENTRICRA is Modulous 24 and rolls-over once per year
    doInterpolation(v, TOPOCENTRICRA, "02 TOPOCENTRICRA ", v.ptrC, TOPOCENTRICRA_MODULO24);
    //TOPOCENTRICDEC ranges between +23.4… degrees and -23.4… degrees and becomes zero at Vernal Equinox (Spring)
    doInterpolation(v, TOPOCENTRICDEC, "03 TOPOCENTRICDEC ", v.ptrC, 0);
    //TOPOCENTRICDIS
    doInterpolation(v, TOPOCENTRICDIS, "04 TOPOCENTRICDIS ", v.ptrC, 0);
    //-->AZIMUTH is Modulous 360 and rolls-over daily
    doInterpolation(v, AZIMUTH,        "05 AZIMUTH ", v.ptrC, AZIMUTH_MODULO360);
    //ZENITHDISTANCE
    doInterpolation(v, ZENITHDISTANCE, "06 ZENITHDISTANCE ", v.ptrC, 0);
    //-->GEOCENTRICRA is Modulous 24 and rolls-over once per year
    doInterpolation(v, GEOCENTRICRA,    "07 GEOCENTRICRA ", v.ptrC, GEOCENTRICRA_MODULO24);
    //GEOCENTRICDEC
    doInterpolation(v, GEOCENTRICDEC,   "08 GEOCENTRICDEC ", v.ptrC, 0);
    //GEOCENTRICDIS
    doInterpolation(v, GEOCENTRICDIS,   "09 GEOCENTRICDIS ", v.ptrC, 0);
    //JDUTC
    doInterpolation(v, JDUTC, "10 JDUTC ", v.ptrC, 0);
    //JDLOCAL ONLY ONE VALUE HERE BECAUSE we are looking for just one value of jdlocal, the jdlocal time when ERA === 0
    v.workbuf[JDLOCAL] = to_string_with_precision(v.resultJDLOCAL, v.DESIRED_MAX_PRECISION);    //Convert double to c++ string of \
    15 digit precision
    if (v.db2__) cout << "11 JDLOCAL " << v.workbuf[JDLOCAL] << endl;
    v.buf += v.workbuf[JDLOCAL]; //Concatenate onto buf
    v.buf += ", ";
    //SITEID ONLY ONE VALUE HERE BECAUSE we process only one value of siteid.
    v.workbuf[SITEID] = string(v.ptrWhatThisSiteIdIs); //Convert c-string to c++ string
    if (v.db2__) cout << "12 SITEID " << v.workbuf[SITEID] << endl;
    v.buf += v.workbuf[SITEID];        //Concatenate onto buf
    v.buf += ", ";
    //-->ERA is a Modulous 360 parameter  ONLY ONE VALUE HERE BECAUSE we are looking for predefined value ERA = 0.
    if(v.eventParameter >= ERA_MODULO360) v.eventParameter -= ERA_MODULO360; //IF necessary, BackOut the modulous
    v.workbuf[ERA] = to_string(v.eventParameter);  //Convert double to c++ string of 15 digit precision
    if (v.db2__) cout << "13 ERA " << v.workbuf[ERA] << endl;
    v.buf += v.workbuf[ERA]; //Concatenate ERA onto buf
    v.buf += ", ";
    //-->LAST is a Modulous 24 parameter
    doInterpolation(v, LAST_, "14 LAST ", v.ptrC, LAST_MODULO24);
    //-->GAST is a Modulous 24 parameter
    doInterpolation(v, GAST_, "15 GAST ", v.ptrC, GAST_MODULO24);
    //VCELX
    doInterpolation(v, VCELX, "16 VCELX ", v.ptrC, 0);
    //VCELY
    doInterpolation(v, VCELY, "17 VCELY ", v.ptrC, 0);
    //VCELZ
    doInterpolation(v, VCELZ, "18 VCELZ ", v.ptrEndCap, 0);
    //Like `v.iI = v.intermediateConnectionString.c_str();`,  convert the c++ string object into a legacy c-string array \
    of characters, which is what the PostgreSQL prefers because the PostgreSQL libpq routines are written in standard, legacy, C \
    and knows nothing about c++ objects and other complexities.
    v.iI = v.buf.c_str(); //Covert the c++ string to ordinary c-string because PostgreSQL database DOES NOT understand c++ strings!
    if (v.db2__) {
        cout <<   "v.numberOfEntries: " << v.numberOfEntries  << "\nv.numberOf_BS_Entries: " << v.numberOf_BS_Entries << endl;
        cout << "The size of v.buf is " << v.buf.size() << " characters " << endl;
    }
    v.charactersWrittenToFile += fwrite(v.iI, 1, strlen(v.iI), v.fp); // Output into the output file the \
    first logical record, the INSERT INTO statement.
    if (v.executeTheSQL) doExecSQL(v, v.iI, PGRES_COMMAND_OK, true);
    freeCallocdMemory(v);
    return 0;
} // End of processERA
int doSpring(Constants &v, void* myb) {
    ; // Spring is defined when geocentricra = 0 hours or 0º. \
    Watch out for geocentricra modulo 24 "rollover"
    v.some2Darray                  = (char **)v.sunTemplate4Seasons; //NTS: Need to create a unique 2D array for arrival of season processing
    v.some1Darray                  = v.sqlTemplate4Seasons;
    v.some1DArraySize              = (int)strlen(v.sqlTemplate4Seasons);
    v.some2DArraySize              = sizeof(v.sunTemplate4Seasons);
    v.numberOf2Dentries            = (v.some2DArraySize>>1);
    v.ptrWhatThisEventIs           = "Spring";
    v.ptrWhatThisGeocentricraSeasonIs = "24";  // 360º is 0º for this geocentricRA. geocentricRA is a modulo 360 parameter which \
    rolls-over once per year.
    v.ptrWhatThisFieldIs = "geocentricra";
    v.ptrEndCap = ", 'Spring');"; //2022-03-14T16:53:27: Added the ', ', the ')', the festooning of Spring with ‘'’, and the ';'
    if ( v.db2__) reviewDo(v, "doSpring");
    return 0;
} //End of doSpring

int processSpring(Constants &v, void* myb) {
    v.ptr_eP = &v.myEvents[SPRING]; //
    v.eventParameter = v.ptr_eP->modulous; //This becomes the aspirational value, or independent parameter, for our \
    Chebyshev Polynomial Interpolation activity. Note that through SQL Magic geocentricra has alreading added in the modulous = 24 \
    which defines Spring. \
    [sunrise, sunset, noon], or geocentricra [summer, fall, winter, spring] will be in arrayIndependent because we want to find the \
    specific local time, jdlocal, when era=360, gast = 24 hours, last = 24 hours, zenithdistance=90.5833… [sunrise or sunset], \
    zenithdistance is a minimum value [noon], geocentricra is 6 [summer], 12 [fall/autumn], 18 [winter], or 0 [spring]. \
    To accomplish this we set the independent variable (GAST) to zero and then provide doChebyshev, for performing \
    Chebyshev Polynomial Interpolation, with the jdlocal values surrounding the location where what we believe the sought-after \
    gast value lies.
    genericSeasonProcessing(v, SPRING);
    ; // TO BE DEFINED
    return 0;
} //End of processSpring

int doSummer(Constants &v, void* myb) {
    // Summer is defined when geocentricra = 6 hours or 90º
    
    v.some2Darray = (char **)v.sunTemplate4Seasons;   //2022-06-11T10:35:27 NOT DIFFERENCE from v.some_2Darray
    v.some1Darray = v.sqlTemplate4Seasons;
    v.some1DArraySize = (int)strlen(v.sqlTemplate4Seasons);
    v.some2DArraySize = sizeof(v.sunTemplate4Seasons[0]); //2022-06-08T13:30:28
    v.numberOf2Dentries = (v.some2DArraySize>>1);
    v.ptrWhatThisEventIs = "Summer";
    v.ptrWhatThisGeocentricraSeasonIs = "6";  //When geocentricra = 6, then it's summer.
    v.ptrWhatThisFieldIs = "geocentricra";
    v.ptrEndCap = ", 'Summer');";
    if ( v.db2__) reviewDo(v, "doSummer");
    return 0;
} //End of doSummer

int processSummer(Constants &v, void* myb) {  //Added 2022-06-04T12:27:31 Made from processGAST
    v.ptr_eP = &v.myEvents[SUMMER]; //
    v.eventParameter = v.ptr_eP->eventDefinition; //This becomes the aspirational value, or independent parameter, for our \
    Chebyshev Polynomial Interpolation activity. \
    [sunrise, sunset, noon], or geocentricra [summer, fall, winter, spring] will be in arrayIndependent because we want to find the \
    specific local time, jdlocal, when era=360, gast = 24 hours, last = 24 hours, zenithdistance=90.5833… [sunrise or sunset], \
    zenithdistance is a minimum value [noon], geocentricra is 6 [summer], 12 [fall/autumn], 18 [winter], or 0 [spring]. \
    To accomplish this we set the independent variable (GAST) to zero and then provide doChebyshev, for performing \
    Chebyshev Polynomial Interpolation, with the jdlocal values surrounding the location where what we believe the sought-after \
    gast value lies.
    genericSeasonProcessing(v, SUMMER);
    return 0;
    
} //End of processSummer

int doAutumn(Constants &v, void* myb) {
    ; // Autumn (aka fall) is defined when geocentricra = 12 hours or 180º
    v.Constants::some2Darray = (char **)v.sunTemplate4Seasons; //NTS: Need to create a unique v.sunTemplateAUTUMN12 -- where 12 represents the \
    geocentricRA at autumn/fall equinox -- rather than “borrow” v.sunTemplateERA0
    v.some1Darray = v.sqlTemplate4Seasons;
    v.some1DArraySize = (int)strlen(v.sqlTemplate4Seasons);
    v.some2DArraySize = sizeof(v.sunTemplate4Seasons[0]);
    v.numberOf2Dentries = (v.some2DArraySize>>1);
    v.ptrWhatThisEventIs = "Autumn";
    v.ptrWhatThisEventDefinitionIs = "12";
    v.ptrWhatThisGeocentricraSeasonIs = "12";
    v.ptrWhatThisFieldIs = "geocentricra";
    v.ptrEndCap = ", 'Autumn');";
    if ( v.db2__) reviewDo(v, "doAutumn");
    return -v.autumn;
} //End of doAutumn
int processAutumn(Constants &v, void* myb) {
    v.ptr_eP = &v.myEvents[AUTUMN]; //
    v.eventParameter = v.ptr_eP->eventDefinition; //This becomes the aspirational value, or independent parameter, for our \
    Chebyshev Polynomial Interpolation activity. \
    [sunrise, sunset, noon], or geocentricra [summer, fall, winter, spring] will be in arrayIndependent because we want to find the \
    specific local time, jdlocal, when era=360, gast = 24 hours, last = 24 hours, zenithdistance=90.5833… [sunrise or sunset], \
    zenithdistance is a minimum value [noon], geocentricra is 6 [summer], 12 [fall/autumn], 18 [winter], or 0 [spring]. \
    To accomplish this we set the independent variable (GAST) to zero and then provide doChebyshev, for performing \
    Chebyshev Polynomial Interpolation, with the jdlocal values surrounding the location where what we believe the sought-after \
    gast value lies.
    genericSeasonProcessing(v, AUTUMN);
    return 0;
} //End of processAutumn
int doWinter(Constants &v, void* myb) {
    ; // Winter is defined when geocentricra = 18 hours or 270º
    v.Constants::some2Darray = (char **)v.sunTemplate4Seasons; //NTS: Need to create a unique v.sunTemplateWINTER18 -- where 18 represents the \
    geocentricRA at winter solstice -- rather than “borrow” v.sunTemplateERA0
    v.some1Darray     = v.sqlTemplate4Seasons;
    v.some1DArraySize = (int)strlen(v.sqlTemplate4Seasons);
    v.some2DArraySize = sizeof(v.sunTemplate4Seasons);
    v.numberOf2Dentries = (v.some2DArraySize>>1);
    v.ptrWhatThisEventIs = "Winter";
    v.ptrWhatThisEventDefinitionIs = "18";
    v.ptrWhatThisGeocentricraSeasonIs = "18";
    v.ptrWhatThisFieldIs = "geocentricra";
    v.ptrEndCap = ", 'Winter');";
    if ( v.db2__) reviewDo(v, "doWinter");
    return -v.winter;
} //End of doWinter
int processWinter(Constants &v, void* myb) {
    v.ptr_eP = &v.myEvents[WINTER]; //
    v.eventParameter = v.ptr_eP->eventDefinition; //This becomes the aspirational value, or independent parameter, for our \
    Chebyshev Polynomial Interpolation activity. \
    [sunrise, sunset, noon], or geocentricra [summer, fall, winter, spring] will be in arrayIndependent because we want to find the \
    specific local time, jdlocal, when era=360, gast = 24 hours, last = 24 hours, zenithdistance=90.5833… [sunrise or sunset], \
    zenithdistance is a minimum value [noon], geocentricra is 6 [summer], 12 [fall/autumn], 18 [winter], or 0 [spring]. \
    To accomplish this we set the independent variable (GAST) to zero and then provide doChebyshev, for performing \
    Chebyshev Polynomial Interpolation, with the jdlocal values surrounding the location where what we believe the sought-after \
    gast value lies.
    genericSeasonProcessing(v, WINTER);
    return 0;
} //End of processWinter

void printFields(Constants &c, int nFields) {
    for(c.k=0; c.k < nFields; c.k++) {
        printf ( "%-20s", PQfname(c.dbc->res, c.k) ); //Every modulo rows print a column header
    }
    printf("\n");
} //End of printFields


int doSS(Constants &v, void* myb) {
    v.Constants::some2Darray = (char **)v.sunSetArray;
    v.some1Darray = (char *)v.ptrSQLTemplateSS;
    v.some1DArraySize = (int)strlen(v.ptrSQLTemplateSS);
    v.some2DArraySize = sizeof(v.sunSetArray);
    v.numberOf2Dentries = (v.some2DArraySize>>1);
    v.ptrWhatThisEventDefinitionIs = ZENITHDISTANCE_SR_OR_SS;
    v.ptrWhatThisFieldIs = "zenithdistance";
    v.ptrWhatThisEventIs = "SS";
    v.ptrEndCap = ", 'SunSet');";
    if ( v.db2__) reviewDo(v, "doSS");
    return 0;
} //End of doSS

int processSS(Constants &v, void* myb) {
    v.ptr_eP->modulous = 0; //zenithdistance, the parameter that determines sunrise or sunset, has no modulus
    v.ptr_eP = &v.myEvents[SUNSET];
    v.eventParameter = atof(ZENITHDISTANCE_SR_OR_SS);
    //The “callocing” of arrays ptrArrayDependent and ptrArrayIndependent, is now done in goExecuteSomeSQL.cpp
    //    v.ptrArrayDependent = (myParameters *)calloc(sizeof(myParameters), v.numberOfRows); //jdlocal will be in ArrayDependent
    //    v.ptrArrayIndependent = (myParameters *)calloc(sizeof(myParameters), v.numberOfRows); // Either era, gast, last, zenithdistance \
    [sunrise, sunset, noon], or geocentricra [summer, fall, winter, spring] will be in arrayIndepented because we want to find the \
    specific time, jdlocal, when era=360, gast = 24 hours, last = 24 hours, zenithdistance=90.5833… [sunrise or sunset], \
    zenithdistance is a minimum value [noon], geocentricra is 6 [summer], 12 [fall/autumn], 18 [winter], or 0 [spring].  \
    To accomplish this we set the independent variable (zenithdistance) to 90.5833 and then provide doChebyshev, for performing \
    Chebyshev Polynomial Interpolation, with the jdlocal values surrounding the location where what we believe the sought-after \
    gast value lies.
    //     NOTE that zenithdistance decreases as we approach time of sunrise (zenithdistance === 90.5833) and keeps decreasing until we reach noon
    //     at which point zenithdistance begins to increase again. Then, half-a-day-later (~ 12 hours),  zenith distance begins to decrease again.
    //     For sunrise and sunset processing, we will rely upon the SQL SELECT statement, that collected the data, that this function will have the data
    
    
    for (v.i=0; v.i < v.numberOfRows; v.i++) { // +1
        if ( v.i > 0 && (fabs((v.allFetchedData  + v.i)->zenithdistance < fabs((v.allFetchedData + v.i -1)->zenithdistance)) ) ) { //If current \
            value is less than previous independent value, then add modulus to the current value, thus satisfying \
            gsl interpolation routine's requirement that arrays be monotonically increasing.
            (v.allFetchedData + v.i)->zenithdistance  += (double)v.ptr_eP->modulous; //Should NEVER NEED TO DO THIS FOR SR OR SS \
            events; in fact (double)v.ptr_eP->modulous should be set to zero for SR or SS
        } else {
            (v.ptrArrayIndependent + v.i)->dbl_value   = (v.allFetchedData + v.i)->zenithdistance;
        }  // end of if/else
        (v.ptrArrayIndependent + v.i)->index = v.i;
        (v.ptrArrayDependent + v.i)->dbl_value     = (v.allFetchedData + v.i)->jdlocal;
        (v.ptrArrayDependent + v.i)->index = v.i;
    }     // -1 End of for loop
    
    //
    //
    //        points to array of associated dependent variables -------------------------------------------+
    //        points to array of independent variables ---------------------------------+                  |
    //        points to THE desired independent variable -------+                       |                  |
    //        points to chebyshev order -----+                  |                       |                  |
    //        points to class --+            |                  |                       |                  |
    //                          |            |                  |                       |                  |
    //                          V            V                  V                       V                  V
    v.resultJDLOCAL=doChebyshev(v,    v.numberOfRows, &v.eventParameter, v.ptrArrayIndependent, v.ptrArrayDependent);
    free(v.ptrArrayIndependent); //Free array of independent variables, containing a 1D array of nolonger needed zenithdistances
    v.ptrArrayIndependent = v.ptrArrayDependent; //ReAssign v.ptrArrayIndependent to point to the array of jdlocal times which \
    WAS in the array of Dependent Varaibles (v.ptrArrayDependent)
    //At this point v.resultingDependentVariable points to the local time, in julian day format. This time will become the the \
    independent variable;
    v.ptrArrayDependent = (myParameters *)calloc(sizeof(myParameters), v.numberOfRows); //Now reAllocate a new \
    Array of Dependent Variables
    //   free(v.ptrArrayIndependent); //Free this because we won't be needing to point to an array of zenithdistance values because \
    there is only ONE zenith distance value we will use and that's what's in v.eventParameter and v.resultJDLOCAL is the local time \
    that we achieved the desired value of ZENITHDISTANCE_SR_OR_SS.
    v.ptrIndependentVariable = &v.resultingDependentVariable; //From now on, make jdlocal the independent variable
    
    
    
    v.ptrAllResultingDependentVaraiables = (namesOfColumns *)calloc(sizeof(namesOfColumns), 1);
    (v.ptrAllResultingDependentVaraiables + JDLOCAL)->jdlocal = v.resultJDLOCAL;
    (v.ptrAllResultingDependentVaraiables + ZENITHDISTANCE)->zenithdistance = v.eventParameter;   //ZENITHDISTANCE Becomes the \
    aspirational value as presented via the -e command line parameter. With zenithdistance there are not sudden downward jumps \
    toward zeron when we've reached the “modulo limit”
    /*
     if ( (v.ptrAllResultingDependentVaraiables + ERA)->era >= ERA_MODULO360 )  \
     (v.ptrAllResultingDependentVaraiables + ERA)->era -= ERA_MODULO360;  //Back-out the modulous if necessary
     */
    v.buf.reserve(1500); //Without this the capacity of the v.buf is 22 characters.
    v.buf.clear(); //Make sure this c++ buffer is clean
    if(v.db2__) {
        cout << "The max size v.buf can aspire to is: " << v.buf.max_size() << ",\nthe capacity of v.buf is: " << v.buf.capacity() << endl;
        cout << "After doing v.buf.reserve(1500), the capacity of v.buf is now: " << v.buf.max_size() << \
        " characters (i.e. 2^64), the capacity to whic v.buf can aspire is: " << v.buf.capacity() << " characters." << endl;
    } //
    v.buf += v.insertIntoStart; //Initialize v.buf with the “INSERT INTO … blah, blah, blah” SQL ”INSERT INTO“  preliminaries
    v.ptrToGregorian = julianToGregorian(&v.resultJDLOCAL, 0) ; //julianToGregorian is a recently (2020-12-02) added dylib. Perhaps \
    I should add gregorianToJuian to my dylib library?
    //    v.buf = v.ptrToGregorian; //Convert the pointer-to-cString to a c++ string in buf. buf is an accumulator of c++ strings
    if (v.db2__) cout << "01. v.ptrToGregorian looks like: " << v.ptrToGregorian << endl;
    v.buf += string(v.ptrToGregorian); // + "', "; // need to convert v.ptrToGregorian from type c-string to type c++string. Then \
    begin accumulating the results of each "Chebyshev'd" parameter in a c++ string buffer named buf, separating each parameter with \
    a comma.
    v.buf += "', ";
    (v.ptrAllResultingDependentVaraiables + SITEID)->siteid = (v.allFetchedData + SITEID)->siteid;  //SITEID doesn't change
    
    
    
    //Move all the returned topocentricra data, scattered around multicolumn array allFetchedData into the single column array, \
    as demanded by the gsl Chebyschev Polynomial Interpolation Routine, pointed to by ptrArrayDependent. This single column array, \
    calloced above, will be used over-and-over to contain the many various dependent variables.
    
    
    
    //-->TOPOCENTRICRA is Modulous 24 and rolls-over once per year
    doInterpolation(v, TOPOCENTRICRA, "02 TOPOCENTRICRA ", v.ptrC, TOPOCENTRICRA_MODULO24);
    //TOPOCENTRICDEC ranges between +23.4… degrees and -23.4… degrees and becomes zero at Vernal Equinox (Spring)
    doInterpolation(v, TOPOCENTRICDEC, "03 TOPOCENTRICDEC ", v.ptrC, 0);
    //TOPOCENTRICDIS
    doInterpolation(v, TOPOCENTRICDIS, "04 TOPOCENTRICDIS ", v.ptrC, 0);
    //-->AZIMUTH is Modulous 360 and rolls-over daily
    doInterpolation(v, AZIMUTH, "05 AZIMUTH ", v.ptrC, AZIMUTH_MODULO360);
    //ZENITHDISTANCE
    // --  doInterpolation(v, ZENITHDISTANCE, "06 ZENITHDISTANCE ", v.ptrC, 0);  //This is what we did for processERA, upon which processSR is based.
    v.workbuf[ZENITHDISTANCE]=ZENITHDISTANCE_SR_OR_SS; // Now ZENITHDISTANCE, the sixth (06) column in table named \
    “tbl_sun_loc_site” of database named “LocalWeather”,  is what we specified it to be
    if (v.db2__) cout << "06 ZENITHDISTANCE " << v.workbuf[ZENITHDISTANCE] << endl;
    v.buf += v.workbuf[ZENITHDISTANCE];
    v.buf += ", ";
    //-->GEOCENTRICRA is Modulous 24 and rolls-over once per year
    doInterpolation(v, GEOCENTRICRA, "07 GEOCENTRICRA ", v.ptrC, GEOCENTRICRA_MODULO24);
    //GEOCENTRICDEC
    doInterpolation(v, GEOCENTRICDEC, "08 GEOCENTRICDEC ", v.ptrC, 0);
    //GEOCENTRICDIS
    doInterpolation(v, GEOCENTRICDIS, "09 GEOCENTRICDIS ", v.ptrC, 0);
    //JDUTC
    doInterpolation(v, JDUTC, "10 JDUTC ", v.ptrC, 0);
    //JDLOCAL ONLY ONE VALUE HERE BECAUSE we are looking for just one value of jdlocal, the jdlocal time when ERA === 0
    v.workbuf[JDLOCAL] = to_string_with_precision(v.resultJDLOCAL, v.DESIRED_MAX_PRECISION);    //Convert double to c++ string of \
    15 digit precision
    if (v.db2__) cout << "11 JDLOCAL " << v.workbuf[JDLOCAL] << endl;
    v.buf += v.workbuf[JDLOCAL]; //Concatenate onto buf
    v.buf += ", ";
    //SITEID ONLY ONE VALUE HERE BECAUSE we process only one value of siteid.
    v.workbuf[SITEID] = string(v.ptrWhatThisSiteIdIs); //Convert c-string to c++ string
    if (v.db2__) cout << "12 SITEID " << v.workbuf[SITEID] << endl;
    v.buf += v.workbuf[SITEID];        //Concatenate onto buf
    v.buf += ", ";
    //-->ERA is a Modulous 360 parameter  ONLY ONE VALUE HERE BECAUSE we are looking for predefined value ERA = 0.
    //    if(v.eventParameter >= ERA_MODULO360) v.eventParameter -= ERA_MODULO360; //IF necessary, BackOut the modulous
    //    v.workbuf[ERA] = to_string(v.eventParameter);  //Convert double to c++ string of 15 digit precision
    //    if (v.db2__) cout << "13 ERA " << v.workbuf[ERA] << endl;
    //    v.buf += v.workbuf[ERA]; //Concatenate ERA onto buf
    //    v.buf += ", ";
    doInterpolation(v, ERA_, "13 ERA ", v.ptrC, ERA_MODULO360);
    //-->LAST is a Modulous 24 parameter
    doInterpolation(v, LAST_, "14 LAST ", v.ptrC, LAST_MODULO24);
    //-->GAST is a Modulous 24 parameter
    doInterpolation(v, GAST_, "15 GAST ", v.ptrC, GAST_MODULO24);
    //VCELX
    doInterpolation(v, VCELX, "16 VCELX ", v.ptrC, 0);
    //VCELY
    doInterpolation(v, VCELY, "17 VCELY ", v.ptrC, 0);
    //VCELZ
    doInterpolation(v, VCELZ, "18 VCELZ ", v.ptrEndCap, 0);
    //Like `v.iI = v.intermediateConnectionString.c_str();`,  convert the c++ string object into a legacy c-string array \
    of characters, which is what the PostgreSQL prefers because the PostgreSQL libpq routines are written in standard, legacy, C \
    and knows nothing about c++ objects and other complexities.
    v.iI = v.buf.c_str(); //Covert the c++ string to ordinary c-string because PostgreSQL database DOES NOT understand c++ strings!
    if (v.db2__) {
        cout <<   "v.numberOfEntries: " << v.numberOfEntries  << "\nv.numberOf_BS_Entries: " << v.numberOf_BS_Entries << endl;
        cout << "The size of v.buf is " << v.buf.size() << " characters " << endl;
    }
    v.charactersWrittenToFile += fwrite(v.iI, 1, strlen(v.iI), v.fp); // Output into the output file the \
    first logical record, the INSERT INTO statement.
    if (v.executeTheSQL) doExecSQL(v, v.iI, PGRES_COMMAND_OK, true);
    freeCallocdMemory(v);
    return 0;
} //End of processSS

int doNoonMin(Constants &v, void* myb) {
    ;
    v.Constants::some2Darray = (char **)v.noonArray;
    v.some1Darray = (char *)v.ptrSQLTemplateNOON;
    v.some1DArraySize = (int)strlen(v.ptrSQLTemplateNOON);
    v.some2DArraySize = sizeof(v.noonArray);
    v.ptrWhatThisEventIs = "NoonMin";
    if ( v.db2__) reviewDo(v, "doNoonMin");
    return 0;
} //End of doNoonMin
int processNoonMin(Constants &v, void* myb) {
    return 0;
} //End of processNoonMin

int getTuples(Constants &c, bool output, bool doPQclear) {
    c.rc = 0; //Assume success;
    c.numberOfColumns = PQnfields(c.dbc->res);
    c.numberOfRows = PQntuples(c.dbc->res);
    if (c.numberOfColumns == 0 || c.numberOfRows == 0) {
        cerr << "Either no rows were returned and/or no columns were returned from FETCH ALL operation! number of columns = " \
        << c.numberOfColumns << ", and the number of rows = " << c.numberOfRows << endl;
        c.rc = -(30+1); //Set Negative, bad, return code.
        return (c.rc); //Quit
    }
    c.allFetchedData = (namesOfColumns *)calloc(c.numberOfRows, sizeof(namesOfColumns));
    c.k = sizeof(namesOfColumns);
    for (c.i=0; c.i < c.numberOfRows; c.i++) {
        if (c.i%c.modulo_lines_perPage == 0) printf("%s\n", c.titles); //Print-out the title-line per modulo_lines_perPage.
        (c.allFetchedData + c.i)->id = atoi(PQgetvalue(c.dbc->res, c.i, ID));
        if (output) printf("%-8s", PQgetvalue(c.dbc->res, c.i, ID) );
        c.ptrWorkChar = PQgetvalue(c.dbc->res, c.i, LT);
        c.j=0;
        while (*(c.ptrWorkChar) != '\0' ) {
            //--           c.lastCharacter = *(c.ptrWorkChar);  //-- let's try a more
            if (c.j > NUMBER_OF_CHARACTERS_DESCRIBING_ISO_8601_DATETIME) { //19 characters are required for date/time formated \
                ISO 8601 as: "YYYY-MM-DD hh:mm:ss"
                cerr << "LINE" << __LINE__ << ", of file " << __FILE__ << ", compiled on " << __DATE__ << "/" << __TIME__ << \
                ": ERROR — can't find the end of the DATE/TIME in *(c.allFetchedData)->lt" << endl;
                break;
            }
            *((c.allFetchedData + c.i)->lt + c.j++)  = *(c.ptrWorkChar++);
            //--           *((c.allFetchedData)->lt + c.j)  = c.lastCharacter;
            //--           c.ptrWorkChar++;
            //--           c.j++;
        }
        *((c.allFetchedData + c.i)->lt + c.j)  = '\0'; //Terminate with a null character the date/time field, which we constructed \
        in the above while loop.
        if (output) printf("%-20s", PQgetvalue(c.dbc->res, c.i, LT) ); //This results in GOOD DATA being output.
        //------
        //        c.ptrWorkChar =  PQgetvalue(c.dbc->res, c.i, TOPOCENTRICRA);
        //        c.workDouble = atof(c.ptrWorkChar);
        
        (c.allFetchedData + c.i)->topocentricra = atof(PQgetvalue(c.dbc->res, c.i, TOPOCENTRICRA)); //Convert c-string to double
        // current value ---------------------+                                   +----- previous value
        //                                    |                                   |
        //                                    V                                   V
        if (c.i > 0 && (fabs((c.allFetchedData + c.i)->topocentricra) ) < fabs((c.allFetchedData + c.i-1)->topocentricra) ) {
            (c.allFetchedData + c.i)->topocentricra += TOPOCENTRICRA_MODULO24; //Modulus adjust topocentricra because the \
            gsl interpolation routines demand that variables be monotonically increasing and have no sudden jumps like in a \
            "saw-tooth" pattern.
        }
        if (output) printf("%-16lf", (c.allFetchedData + c.i)->topocentricra );
        //------
        (c.allFetchedData + c.i)->topocentricdec = atof(PQgetvalue(c.dbc->res, c.i, TOPOCENTRICDEC));
        if (output) printf("%-16lf", (c.allFetchedData + c.i)->topocentricdec );
        //------
        (c.allFetchedData + c.i)->topocentricdis = atof(PQgetvalue(c.dbc->res, c.i, TOPOCENTRICDIS));
        if (output) printf("%-16lf", (c.allFetchedData + c.i)->topocentricdis  );
        //------
        (c.allFetchedData + c.i)->azimuth = atof(PQgetvalue(c.dbc->res, c.i, AZIMUTH));
        // current value ---------------------+                                   +----- previous value
        //                                    |                                   |
        //                                    V                                   V
        if (c.i > 0 && (fabs((c.allFetchedData + c.i)->azimuth) < fabs((c.allFetchedData + c.i-1)->azimuth))) {
            (c.allFetchedData + c.i)->azimuth += AZIMUTH_MODULO360; //Modulus adjust azimuth because the gsl interpolation routines \
            demand that variables be monotonically increasing.
        }
        if (output) printf("%-16lf", (c.allFetchedData + c.i)->azimuth );
        //------
        (c.allFetchedData + c.i)->zenithdistance = atof(PQgetvalue(c.dbc->res, c.i, ZENITHDISTANCE));
        if (output) printf("%-16lf",(c.allFetchedData + c.i)->zenithdistance);
        //------
        (c.allFetchedData + c.i)->geocentricra = atof(PQgetvalue(c.dbc->res, c.i, GEOCENTRICRA));
        // current value ---------------------+                                   +----- previous value
        //                                    |                                   |
        //                                    V                                   V
        if (c.i > 0 && (fabs((c.allFetchedData + c.i)->geocentricra) < fabs((c.allFetchedData + c.i-1)->geocentricra) ) ) {
            (c.allFetchedData + c.i)->geocentricra += GEOCENTRICRA_MODULO24; //Modulus adjust geopocentricra because the \
            gsl interpolation routines demand that variables be monotonically increasing.
        }
        if (output) printf("%-16lf", (c.allFetchedData + c.i)->geocentricra );
        //------
        (c.allFetchedData + c.i)->geocentricdec = atof(PQgetvalue(c.dbc->res, c.i, GEOCENTRICDEC));
        if (output) printf("%-16s", PQgetvalue(c.dbc->res, c.i, GEOCENTRICDEC) );
        //------
        (c.allFetchedData + c.i)->geocentricdis = atof(PQgetvalue(c.dbc->res, c.i, GEOCENTRICDIS));
        if (output) printf("%-16s", PQgetvalue(c.dbc->res, c.i, GEOCENTRICDIS) );
        //------
        (c.allFetchedData + c.i)->jdutc = atof(PQgetvalue(c.dbc->res, c.i, JDUTC));
        if (output) printf("%-18s", PQgetvalue(c.dbc->res, c.i, JDUTC) );
        //------
        (c.allFetchedData + c.i)->jdlocal = atof(PQgetvalue(c.dbc->res, c.i, JDLOCAL));
        if (output) printf("%-18lf", (c.allFetchedData + c.i)->jdlocal );
        //------
        (c.allFetchedData + c.i)->siteid = (unsigned short int)atoi(PQgetvalue(c.dbc->res, c.i, SITEID));
        if (output) printf("%-5s", PQgetvalue(c.dbc->res, c.i, SITEID) );
        //------
        (c.allFetchedData + c.i)->era = atof(PQgetvalue(c.dbc->res, c.i, ERA_));
        // current value ---------------------+                                   +----- previous value
        //                                    |                                   |
        //                                    V                                   V
        if (c.i > 0 && (fabs((c.allFetchedData + c.i)->era) < fabs((c.allFetchedData + c.i-1)->era))) {
            (c.allFetchedData + c.i)->era += ERA_MODULO360; //Modulus adjust era because the gsl interpolation routines \
            demand that variables be monotonically increasing.
        }
        if (output) printf("%-16lf", (c.allFetchedData + c.i)->era );
        //------
        (c.allFetchedData + c.i)->last = atof(PQgetvalue(c.dbc->res, c.i, LAST_));
        // current value ---------------------+                                   +----- previous value
        //                                    |                                   |
        //                                    V                                   V
        if (c.i > 0 && (fabs((c.allFetchedData + c.i)->last) < fabs((c.allFetchedData + c.i-1)->last))) {
            (c.allFetchedData + c.i)->last += LAST_MODULO24; //Modulus adjust last because the gsl interpolation routines \
            demand that variables be monotonically increasing.
        }
        if (output) printf("%-16lf", (c.allFetchedData + c.i)->last);
        //------
        (c.allFetchedData + c.i)->gast = atof(PQgetvalue(c.dbc->res, c.i, GAST_));
        // current value ---------------------+                                   +----- previous value
        //                                    |                                   |
        //                                    V                                   V
        if (c.i > 0 && (fabs((c.allFetchedData + c.i)->gast) < fabs((c.allFetchedData + c.i-1)->gast))) {
            (c.allFetchedData + c.i)->gast += GAST_MODULO24; //Modulus adjust gast because the gsl interpolation routines \
            demand that variables be monotonically increasing.
        }
        if (output) printf("%-16f",  (c.allFetchedData + c.i)->gast  );
        //------
        (c.allFetchedData + c.i)->vcelx = atof(PQgetvalue(c.dbc->res, c.i, VCELX));
        if (output) printf("%-16lf", (c.allFetchedData + c.i)->vcelx );
        //------
        (c.allFetchedData + c.i)->vcely = atof(PQgetvalue(c.dbc->res, c.i, VCELY));
        if (output) printf("%-16lf", (c.allFetchedData + c.i)->vcely );
        //------
        (c.allFetchedData + c.i)->vcelz = atof(PQgetvalue(c.dbc->res, c.i, VCELZ));
        if (output) printf("%-16lf", (c.allFetchedData + c.i)->vcelz );
        //------
        if (strlen(PQgetvalue(c.dbc->res, c.i, EVENTS)) > 0) { //Usually this value is NULL character.
            strcpy((c.allFetchedData + c.i)->events, PQgetvalue(c.dbc->res, c.i, EVENTS));
            if (output) {
                printf("%-9s", PQgetvalue(c.dbc->res, c.i, EVENTS) );
            }
        }
        printf("\n");
        //        c.allFetchedData++; //Go to next row of data
    } //end of for-loop
    if (doPQclear) {
        PQclear(c.dbc->res);
        c.dbc->res = nullptr;
    }
    return c.rc;
} //End of getTuples
void reviewDo(Constants& v, string s) {
    /*
     size_t work4 = sizeof(v.sunTemplate4Seasons[0][0]);   //work4 = 8
     cout << "Start Addr. --> hex value of v.some2Darray looks like: " << std::hex << v.some2Darray << std::dec << \
     "sizeof((char *)v.some2Darray is: " << work4 << " bytes." << \
     "\nStart Addr. --> hex value of v.sunTemplate4Seasons[0][0] looks like: " << std::hex << v.sunTemplate4Seasons[0][0] << \
     std::dec << ". sizeof((char *)v.sunTemplate4Seasons[0][0] is: " << work4 << " bytes." << endl;
     */
    v.j=0;
    cout << "In function " << s << ". v.some2Darray looks like: " << endl;
    while ( 1 ) {
        if (v.j == SOMETHINGREASONABLYLARGE) {
            cout << "Terminating because we never found this array’s TERM after " << SOMETHINGREASONABLYLARGE << " attempts!" << endl;
            exit (-3);
        }
        cout <<  v.j << ". "  << **(char ***)(v.some2Darray + (v.j + 0)) << ", " << **(char ***)(v.some2Darray + (v.j + 1)) << std::dec << std::endl;
        if ( strcmp(**(char ***)(v.some2Darray + (v.j +0)),  v.ptrThisTerm) == 0)  { //Have we reached the end of this 2D array?
            v.some2DArraySize = v.j + NUMBEROFCOLUMNSIN2DARRAY;    //Set the number of rows of this 2D array
            break;
        }
        v.j += NUMBEROFCOLUMNSIN2DARRAY;
    }
    v.numberOfArrayRows = (unsigned int)v.some2DArraySize/2;
    v.numberOf2Dentries = v.numberOfArrayRows;
    cout << "Two-dimensional array, named some2Darray, contains " << v.some2DArraySize << " pointer entries, or consists of " << \
    v.numberOfArrayRows << " rows of pointers @ 2 pointers per row." << endl;
    cout << "v.some1Darray looks like: " << endl;
    cout << (v.some1Darray) << endl;
    cout << "v.some1DarraySize: " << std::dec << v.some1DArraySize << " characters, forming our SQL SELECT template." << endl;
    cout << "  End--> hex value of v.some2Darray looks like: " << std::hex << v.some2Darray << std::dec << endl;
} //End of reviewDo
int executeSomeSQL(Constants& c, const char *sql, bool doClear) {
    executeSQL((char *)sql, c.dbc);
    c.er = PQresultStatus(c.dbc->res);
    switch (c.er) {
        case PGRES_COMMAND_OK:  // (c.er=1) For SQL commands like BEGIN, END, COMMIT (need to investigate INSERT INTO?, UPDATE?)
            if (c.db2__) printf("Line %d, File %s, Compiled on %s/%s: got PGRES_COMMAND_OK\n",__LINE__, __FILE__, __DATE__, __TIME__);
            c.rc = GOT_PGRES_COMMAND_OK_;
            if (doClear) {
                if (c.dbc->res) {
                    PQclear(c.dbc->res);
                    c.dbc->res = nullptr; //set c.dbc->res to zero (i.e., nullptr) so we know we've done a PQclear.
                }
            }
            break;
        case PGRES_TUPLES_OK: // (c.er=2) For SQL commands that return data, like SELECT
            if (c.db2__) printf("Line %d, File %s, Compiled on %s/%s: got PGRES_TUPLES_OK\n",__LINE__, __FILE__, __DATE__, __TIME__);
            c.rc = GOT_PGRES_TUPLES_OK_;
            //         pg_result *pr = c.dbc->res; //pg_result is defined as a via in #include <internal/libpq-int.h>
            //          c.numberOfRows = pr->ntups;
            //We don't want to do PQclear just yet because we will loose all the tuples that we returned
            break;
        default:
            if (c.db2__ && !(c.er == PGRES_TUPLES_OK || c.er == PGRES_COMMAND_OK )) {
                if (c.dbc->res) {
                    printf("Error: LINE: %d, File: %s, compiled on: %s %s: PGresult indicates some sort of error: %s\n \
                           PGresult has a value of %d\n", __LINE__, __FILE__, __DATE__, __TIME__, PQerrorMessage(c.dbc->conn), \
                           PQresultStatus(c.dbc->res));
                    
                    PQclear(c.dbc->res); //In case of error, do clear to release any memory that libpq-fe might have done.
                    c.dbc->res = nullptr; //set c.dbc->res to zero (i.e., nullptr) so we know we've done a PQclear.
                }
                c.rc = GOT_NEITHER_PGRES_COMMAND_OK_OR_PGRES_TUPLES_OK;
            }
    } //End of Switch
    return c.rc;
} //End of executeSomeSQL
void exit_nicely(Constants& c, int rc) {
    if (c.dbc->res) {
        PQclear(c.dbc->res);
        c.dbc->res = nullptr; //Clear c.dbc->res so the desetructor doesn't try to clear c.dbc->res again in the destructor causing \
        abnormal termination.
    }
    if (c.dbc->conn) {
        PQfinish(c.dbc->conn);
        c.dbc->conn = nullptr; //Clear c.dbc->conn so the destructor doesn't try to do another PQfinish and thus cause an \
        abnormal termination.
        c.dbc->conn = nullptr;
        cout << "In exit_nicely with a return code of " << rc << endl;
    }
    c.~Constants(); //Call destructor to free up calloc'd data and close any opened files.
    exit (rc);
} //End of exit_nicely
void doBegin(Constants& c) { //1
    c.dbc->conn = PQconnectdb(c.ptrConstructedConnectionString);
    if (PQstatus(c.dbc->conn) != CONNECTION_OK) {
        cerr << "Attempted conndction to database LocalWeather failed because " << PQerrorMessage(c.dbc->conn) << endl;
        PQfinish(c.dbc->conn);
        exit (1);
    }
    c.dbc->res = PQexec(c.dbc->conn, "BEGIN");
    if (PQresultStatus(c.dbc->res) != PGRES_COMMAND_OK) {
        cerr << "”BEGIN“ command failed with error indicating:" << PQerrorMessage(c.dbc->conn) << \
        "\nWill execute  ‘PQclear(c.dbc->res)’ then exit with exit code -6" << endl;
        PQclear(c.dbc->res);
        c.dbc->res = nullptr; //Setting c.dbc->res to 0 (nullptr) indicates we've done a PQclear.
        exit_nicely(c, -5);
    }
    if (c.db2__) cout << "BEGIN command seems to have worked. On to ”DECLARE someportal CURSOR FOR SELECT … ;“" << endl;
    PQclear(c.dbc->res);
    c.dbc->res=nullptr; //Now we know c.dbc->res has been PQclear'd
} //End of doBegin
// DODOUBLESEMICOLON is defined via this project's build settings: See `Other C++ Flags`
// DODOUBLESEMICOLON
void doDeclare_srnoonssportal_CursorFor(Constants& c) {                    //2
    c.k=0;
    c.j=0;
#ifdef DODOUBLESEMICOLON
    
    c.ptrWorkChar = strchr(c.ptr_cbb_results, ';');
    if (c.ptrWorkChar == (char *)-1) {
        cerr << "Terminating because we couldn't find a terminating ';' in SQL string\n" << c.ptr_cbb_results << endl;
    } else {
        cout << "Found \"; ;;\" at offset " << c.ptrWorkChar - c.ptr_cbb_results << " bytes from the start of " << \
        "c.ptr_cbb_results.\nBy setting the 2 subsequent characters to NULL to definitely make sure we have end of this SQL " << \
        "string!\nKeep seeing things like ShmemIndex following ”; ;;“ string of characters, thus causing SQL errors because " << \
        "Postgresql thinks ShmemIndex is part of the SQL, which it is not, it's just the Xcode debugger somehow or other " << \
        "getting access to my data Constants object's private area and corrupting the SQL fabrication buffer.\nNOTE that this " << \
        "program, named astroParameter, DOES NOT do shared memory; however, I don't know about the Postres' libpq " << \
        "Routines doing shared memory activities." << endl;
        *(c.ptrWorkChar + 1) = '\0';
        *(c.ptrWorkChar + 2) = '\0';
    }
#endif
    if (c.dbc->res) PQclear(c.dbc->res); //If an active c.dbc->res exists then clear it, before doing the next PQexec, so we avoid \
    memory leaks and other, difficult to debug, errors.
    c.dbc->res=nullptr;
    c.dbc->res = PQexec(c.dbc->conn, c.ptr_cbb_results);  //c.ptr_cbb_results is where fabricateSQLr3.dylib constructs the SQL
    if (PQresultStatus(c.dbc->res) != PGRES_COMMAND_OK) {
        cerr << "SQL command \n" << c.ptr_cbb_results << "\n command failed with error indicating==>" << \
        PQerrorMessage(c.dbc->conn) << "\nWill execute  ‘PQclear(c.dbc->res)’ then exit with exit code -6" << endl;
        PQclear(c.dbc->res);
        c.dbc->res = nullptr; //Setting c.dbc->res to 0 (nullptr) indicates we've done a PQclear.
        c.~Constants(); //Call destructor to cleanup
        exit_nicely(c, -6);
    }
    if (c.db2__) cout << "”DECLARE srnoonssportal CURSOR FOR SELECT … ;“ command seems to have worked.\n" << \
        "On to ”FETCH ALL srnoonssportal;“" << endl;
} //End of doDeclare_srnoonssportal_CursorFor
void doFetchAll(Constants& c) {                                            //3
#ifdef PG_RESULT   //Show the memory to which ”c.dbc->res“ points. The following struct is found in file \
/Users/cjc/C/variousPostgreSQL/postgresql-9.6.10/postgresql-9.6.10/tmp_install/usr/local/pgsql/pgsql090610/include/internal/libpq-int.h
    struct pg_result
    {
        int         ntups;          //00 Number of Rows
        int         numAttributes;  //04 Number of Columns
        PGresAttDesc *attDescs;     //08
        PGresAttValue **tuples;     /*10 each PGresTuple is an array of
                                     * PGresAttValue's */
        int         tupArrSize;     /*14 allocated size of tuples array */
        int         numParameters;  //18
        PGresParamDesc *paramDescs; //20
        ExecStatusType resultStatus; //
        char        cmdStatus[CMDSTATUS_LEN];       /* cmd status from the query */
        int         binary;         /* binary tuple values if binary == 1,
                                     * otherwise text */
        
        /*
         * These fields are copied from the originating PGconn, so that operations
         * on the PGresult don't have to reference the PGconn.
         */
        PGNoticeHooks noticeHooks;
        PGEvent    *events;
        int         nEvents;
        int         client_encoding;    /* encoding id */
        
        /*
         * Error information (all NULL if not an error result).  errMsg is the
         * "overall" error message returned by PQresultErrorMessage.  If we have
         * per-field info then it is stored in a linked list.
         */
        char       *errMsg;         /* error message, or NULL if no error */
        PGMessageField *errFields;  /* message broken into fields */
        char       *errQuery;       /* text of triggering query, if available */
        
        /* All NULL attributes in the query result point to this null string */
        char        null_field[1];
        
        /*
         * Space management information.  Note that attDescs and error stuff, if
         * not null, point into allocated blocks.  But tuples points to a
         * separately malloc'd block, so that we can realloc it.
         */
        PGresult_data *curBlock;    /* most recently allocated block */
        int         curOffset;      /* start offset of free space in block */
        int         spaceLeft;      /* number of free bytes remaining in block */
    };
#endif
    
    
    if(c.dbc->res) PQclear(c.dbc->res);
    c.dbc->res = nullptr;
    c.dbc->res = PQexec(c.dbc->conn, "FETCH ALL IN srnoonssportal");
    if (PQresultStatus(c.dbc->res) != PGRES_TUPLES_OK) {
        cerr << "FETCH ALL IN srnoonssportal command failed with error indicating:" << PQerrorMessage(c.dbc->conn) << \
        "\nWill execute  ‘PQclear(c.dbc->res)’ then exit with exit code -7" << endl;
        PQclear(c.dbc->res);
        c.dbc->res = nullptr; //Setting c.dbc->res to 0 (nullptr) indicates we've done a PQclear.
        exit_nicely(c, -7);
    }
    //NOTE:         DO NOT do a PQclear(c.dbc->res) until all data returned by SELECT query has been transferred from Postgres buffers into \
    this program's working buffers
    if (c.db2__) cout << "FETCH ALL IN srnoonssportal command seems to have worked. We now have in memory the data sought" << endl;
    c.numberOfColumns = PQnfields(c.dbc->res);
    c.numberOfRows    = PQntuples(c.dbc->res);
} //End of doFetchALL
void doClosePortal(Constants& c) {                                          //4
    if ( c.dbc->res ) PQclear(c.dbc->res);
    c.dbc->res = nullptr;
    c.dbc->res = PQexec(c.dbc->conn, c.ptrCLOSE);
    PQclear(c.dbc->res);
    c.dbc->res = nullptr; //Setting c.dbc->res to 0 (nullptr) indicates we've done a PQclear.
} //End of doClosePortal
void doEnd(Constants& c) {                                                  //5
    if(c.dbc->res) PQclear(c.dbc->res); c.dbc->res = nullptr;
    c.dbc->res = PQexec(c.dbc->conn, c.ptrEND);
    if (PQresultStatus(c.dbc->res) != PGRES_COMMAND_OK) {
        cerr << "END command failed with error indicating:" << PQerrorMessage(c.dbc->conn) << \
        "\nWill execute  ‘PQclear(c.dbc->res)’ then exit with exit code -8" << endl;
        PQclear(c.dbc->res);
        c.dbc->res = nullptr; //Setting c.dbc->res to 0 (nullptr) indicates we've done a PQclear.
        exit_nicely(c, -8);
    }
    if (c.db2__) cout << "END command seems to have worked. On to executing SQL DECLARE srnoonssportal CURSOR FOR SELECT … ;" << endl;
} //End of doEnd
void doExecSQL(Constants& c, const char *someSQL, int expectedOKStatus, bool doPQclear) {
    if ( c.dbc->res ) { //c.dbc->res should be unused (cleared) before doing PQexec because c.dbc->res is “memory malloc'd” by postgres database, \
        otherwise memory leaks occur with other kinds of unusual, difficult to debug, errors. HOWEVER, when the resulting \
        PQresultStatus is PGRES_TUPLES_OK then we should ***delay returning postgres-malloc'd memory*** until this program moves \
        the returned data/tuples to its ***own block of managed calloc'd memory***.
        PQclear(c.dbc->res);
        c.dbc->res = nullptr;
    }
    c.dbc->res = PQexec(c.dbc->conn, someSQL);
    if (expectedOKStatus == PGRES_TUPLES_OK ) {  //Usually get PGRES_TUPLES_OK after “FETCH ALL …” SQL commands.
        if (PQresultStatus(c.dbc->res) != PGRES_TUPLES_OK) { //Did we get expected status?
            cerr << "Line " << __LINE__ << " of file " << __FILE__ << ", compiled on " << __DATE__ << "T" << __TIME__ << \
            "FETCH ALL IN srnoonssportal command failed with error indicating:" << PQerrorMessage(c.dbc->conn) << \
            "\nWill execute  ‘PQclear(c.dbc->res)’ then exit with exit code -7" << endl;
            if (doPQclear) {
                if (c.dbc->res) PQclear(c.dbc->res); //It would be unusual to do PQclear with an expectedOKstatus of PGRES_TUPLES_OK because \
                we would not have injested the returned tuples into this process's memory prior to removing these tuples from the \
                database's memory.
                c.dbc->res = nullptr; //Setting c.dbc->res to 0 (nullptr) indicates we've done a PQclear.
            }
            exit_nicely(c, -7);
        }
    } else if ( expectedOKStatus == PGRES_COMMAND_OK ) { //Usually get PGRES_COMMAND_OK for non-SELECT types of SQL commands, like “INSERT INTO …” or “BEGIN”, or “END”, etc.
        if (PQresultStatus(c.dbc->res) != PGRES_COMMAND_OK) { //Did we get expected status of PGRES_COMMAND_OK?
            cerr << "Line " << __LINE__ << " of file " << __FILE__ << ", compiled on " << __DATE__ << "T" << __TIME__ << \
            "\nWhile attempting to execute SQL of:\n" << someSQL << "\nWe got an error of " << PQerrorMessage(c.dbc->conn) << \
            "\nWill execute  ‘PQclear(c.dbc->res)’ and then exit with exit code -9" << endl;
            if (c.dbc->res) PQclear(c.dbc->res);
            c.dbc->res = nullptr; //Setting c.dbc->res to 0 (nullptr) indicates we've done a PQclear.
            exit_nicely(c, -9);
        }
        if (c.db2__) cout << "Got expected status of " << "PGRES_COMMAND_OK, so we're “good-to-go”" << endl;
        if (doPQclear) {
            if (c.dbc->res) PQclear(c.dbc->res);
            c.dbc->res = nullptr;
        }
    } else {
        cerr << "Got unexpected status of " << PQresultStatus(c.dbc->res) << ". We were expecting PQstatus of PGRES_TUPLES_OK [i.e. 2] \
        or PGRES_COMMAND_OK [i.e., 1]. We're exiting with exit code of -10." << endl;
        exit_nicely(c, -10);
    }
    
} //End of doExecSAL
// doInterpolation(v, TOPOCENTRICRA, “01 TOPOCENTRICRA”, “, “, TOPOCENTRICRA_MODULO24);

void doInterpolation(Constants& v, int dependentVariableIndex, string msg, string finalBuf, double someModulo) {
    //                                                            We ***ASSUME*** that the ptrArrayDependent has already been allocated
    for (v.i=0;  v.i < v.numberOfRows; v.i++) {
        switch (dependentVariableIndex) {
            case TOPOCENTRICRA:
                
                (v.ptrArrayDependent   + v.i)->dbl_value  = (v.allFetchedData + v.i)->topocentricra;
                
                break;
            case TOPOCENTRICDEC:
                
                (v.ptrArrayDependent   + v.i)->dbl_value  = (v.allFetchedData + v.i)->topocentricdec;
                
                break;
            case TOPOCENTRICDIS:
                
                (v.ptrArrayDependent   + v.i)->dbl_value  = (v.allFetchedData + v.i)->topocentricdis;
                
                break;
            case AZIMUTH:
                
                (v.ptrArrayDependent   + v.i)->dbl_value  = (v.allFetchedData + v.i)->azimuth;
                
                break;
            case ZENITHDISTANCE:
                
                (v.ptrArrayDependent   + v.i)->dbl_value  = (v.allFetchedData + v.i)->zenithdistance;
                
                break;
            case GEOCENTRICRA:
                
                (v.ptrArrayDependent   + v.i)->dbl_value  = (v.allFetchedData + v.i)->geocentricra;
                
                break;
            case GEOCENTRICDEC:
                
                (v.ptrArrayDependent   + v.i)->dbl_value  = (v.allFetchedData + v.i)->geocentricdec;
                break;
            case GEOCENTRICDIS:
                
                (v.ptrArrayDependent   + v.i)->dbl_value  = (v.allFetchedData + v.i)->geocentricdis;
                
                break;
            case JDUTC:
                
                (v.ptrArrayDependent   + v.i)->dbl_value  = (v.allFetchedData + v.i)->jdutc;
                
                break;
            case JDLOCAL:
                
                (v.ptrArrayDependent   + v.i)->dbl_value  = (v.allFetchedData + v.i)->jdlocal;
                
                break;
            case ERA_:
                
                (v.ptrArrayDependent   + v.i)->dbl_value  = (v.allFetchedData + v.i)->era;
                
                break;
            case LAST_:
                
                (v.ptrArrayDependent   + v.i)->dbl_value  = (v.allFetchedData + v.i)->last;
                
                break;
            case GAST_:
                
                (v.ptrArrayDependent   + v.i)->dbl_value  = (v.allFetchedData + v.i)->gast;
                
                break;
            case VCELX:
                
                (v.ptrArrayDependent   + v.i)->dbl_value  = (v.allFetchedData + v.i)->vcelx;
                
                break;
            case VCELY:
                
                (v.ptrArrayDependent   + v.i)->dbl_value  = (v.allFetchedData + v.i)->vcely;
                
                break;
            case VCELZ:
                
                (v.ptrArrayDependent   + v.i)->dbl_value  = (v.allFetchedData + v.i)->vcelz;
                
                break;
            default:
                cerr << "LINE " << __LINE__ << ", of file " << __FILE__  << ", compiled on " << __DATE__ << "T" << __TIME__ << \
                ": Bad dependentVariableIndex parameter, in function  doInterpolateion. Bad value is: " << \
                dependentVariableIndex << ". We're exiting with exit code of " << BAD_doInterpolation_PARAMETER << endl;
                exit (BAD_doInterpolation_PARAMETER);
                
        }
        //        (v.ptrArrayDependent   + v.i)->dbl_value  = (v.allFetchedData + v.i)->topocentricra;
        //        (v.ptrArrayDependent   + v.i)->index = v.i;
    }
    //    if (v.ongoingResult >= TOPOCENTRICRA_MODULO24) v.ongoingResult -= TOPOCENTRICRA_MODULO24;
    if ( (someModulo > 0) && (v.ongoingResult >= someModulo) ) v.ongoingResult -= someModulo; //Make sure good data is going in.
    v.ongoingResult = doChebyshev(v,    v.numberOfRows, &v.resultJDLOCAL, v.ptrArrayIndependent, v.ptrArrayDependent);
    if ( (someModulo > 0) && (v.ongoingResult >= someModulo) ) v.ongoingResult -=  someModulo; //Make sure good data is coming out.\
    Correct v.ongoingResult if a modulous is defined (i.e., someModulo > 0) and the value returned by doChebyshev exceeds this \
    parameter's modulous.
    v.workbuf[dependentVariableIndex] = to_string_with_precision(v.ongoingResult, v.DESIRED_MAX_PRECISION); //workbuf MUST contain \
    sufficient number of entires to contain all 18 (one for each column in tbl_sun_loc_site) or so possible entires!!
    if (v.db2__) cout <<  msg  << v.workbuf[dependentVariableIndex] << endl;
    v.buf +=  v.workbuf[dependentVariableIndex]; //Concatenate TOPOCENTRICRA onto buf
    v.buf += finalBuf;
} //End of doInterpolation


DoingGast::DoingGast(bool ) {
    
}


//Turn this value positive so we can tell that it has been modified.
//Turn this value positive so we can tell that it has been modified.
