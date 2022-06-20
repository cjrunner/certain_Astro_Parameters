//
//  main.cpp
//  certain_Astro_Parameters
//
//  Created by Clifford Campo on 2/21/21.
//  Copyright © 2021 CliffordCampo. All rights reserved.
//


#include <iostream>
// #include "helpMsg.hpp"
#include "Constants.hpp"
// #include "Pclp.hpp" commented-out 2021-02-21T07:20:20
// #include "executeSomeSQL.hpp"
#include "otherFunctions.hpp"
#include "Pclp.hpp" //Added 2021-02-21T07:20:58
// #include "doingGast.hpp"
#include "goExecuteSomeSQL.hpp"
#include "doingGast.hpp"
#include "doBegin.hpp"
#include "doDeclare_srnoonssportal_CursorFor.hpp"
#include "doFetchAll.hpp"
#include <exception>
#include <ctype.h>
#include <fabricateSQL/fabricateSQLr3.h>
extern int  processERA(Constants &, void *);
extern int  processGAST(Constants &, void *);
extern int  processLAST(Constants &, void *);
extern int  processSpring(Constants &, void *);
// extern int  processAutumn(Constants &, bool=false);
extern int  processSummer(Constants &, void *);
extern int  processAutumn(Constants &, void *);
extern int  processWinter(Constants &, void *);
extern int  processSR(Constants &, void *);
extern int  processSS(Constants &, void *);
extern int  processNoonMin(Constants &, void *);
extern int  doERA(Constants &, void *);
extern int  doGAST(Constants &, void *);
extern int  doLAST(Constants &, void *);
extern int  doSpring(Constants &, void *);
extern int  doAutumn(Constants &, void *);
extern int  doSummer(Constants &, void *);
// extern int  doAutumn(Constants &, bool=false);
extern int  doWinter(Constants &, void *);
extern int  doSR(Constants &, void *);
extern int  doSS(Constants &, void *);
extern int  doNoonMin(Constants &, void *);
extern void printFields(Constants &, int);
extern int getTuples(Constants &c, bool, bool); //first bool if true means we want to display returned tuples. Always place \
copy of tuples in calloc'd memory. Rightmost bool: if true means do PQclear once we've moved FETCH ALL data to our own buffers.
extern int  executeSomeSQL(Constants& , const char *,  bool);
extern void exit_nicely(Constants& c, int rc);
extern void doClosePortal(Constants& c);                        // 04
extern void doEnd(Constants& c);                                // 05
extern void doExecSQL(Constants& c, const char *someSQL, int expectedOKStatus, bool doPQclear);
extern void doFabricateConnectionString(Constants &);
extern void doInterpolation(Constants&, int, string /* msg */, string /* finalBuf */, double /* someModulo */ );

struct MyException : public exception {
    const char * what () const throw () {
        return "C++ Exception";
    }
};
void fabricateSQLr3(timings *, const char **, const char *, int t, char *, size_t *, bool);
Constants *C;
Pclp *P;
int main(int argc, const char * argv[]) {
#ifdef BRACKETSEARCH
    cout << "Hello, " << *(argv + 0) << " and World!\n" << *(argv + 0) << \
    " compiled with preprocessor variable BRACKETSEARCH turned O N (doing Binary Search) \n" << endl;
#else
    cout << "Hello, " << *(argv + 0) << " and World!\n" << *(argv + 0) << \
    " compiled with preprocessor variable BRACKETSEARCH turned O F F (doing Linear Search)\n" << endl;
#endif
//    Constants cnst(argc, argv);
    Pclp c(argc, argv); //‘c’ as the parameter because the base class of the Pclp derived class is named Constants. This expression\
    leads to the base class' constructor executing before the Pclp, derived class, constructor executes.

    c.process_CommandLineParamaters(c);  //You should really do this before you establish reference to \
    derived class (Pclp)/base Class (Constants). Process_CommandLineParameters should either have its own class apart from base Class (Constants). Probably needs some rearchitecure here.
    c.doFabricateConnectionString();  //Fabricate the connection string based upon default, or the over-ridden default values \
    provided via the command line.
    
    //    auto pos = c.dict.begin();
    //    auto pos = c.dict.find(c.whatThisEventIsAsString_lc);
    
    if(c.db1__) {
        cout << "Processing event (transformed to lower case from what appears in command line by function " << \
        "processCommandLineParameters.cpp) now known as: " << c.dict.find(c.whatThisEventIsAsString_lc)->first << endl;
    }
    c.eventAsIndexNumber = c.dict.find(c.whatThisEventIsAsString_lc)->second; //Need to get event as indext number so we know \
    which pointer-to-function, obtained from array ptrToFunction, to use for processing this event.
    if (c.db1__) cout << "c.eventAsIndexNumber " << c.eventAsIndexNumber << endl;
    c.rc = c.ptrToFunction[c.eventAsIndexNumber][DOSETUP](c, 0); //Process the event by setting up for the call to libfabricateSQLr3.dylib.
    reviewDo(c, "doSummer1"); //Added 2022-06-09T09:30:18
    c.charactersWrittenToFile = 0; //Reset this

    c.outBufSize = (c.some1DArraySize>>1) + c.some1DArraySize;  //Make outBufSize which is 50% larger than the size of the SQL template.
  //  c.ptr_cbb_results = (char *)c.cbb_results;
    reviewDo(c, "doSummer2"); //Added 2022-06-09T09:30:18
//2022-06-09T09:26:27 changed *c.some2Darray to c.some2Darray[0]. 2022-06-09T10:23:32 Added the scope resolution operator \
Constants:: to some2Darray because both the Constants and Pclp have the variable named some2Darray and these variables \
are both of the same type!
    fabricateSQLr3(c.ptrPerformanceBuffer, (const char **)c.Constants::some2Darray, c.some1Darray, c.numberOf2Dentries, c.ptr_cbb_results, &c.outBufSize, c.db4__);
    c.Constants::j = (int)c.outBufSize; //Constants::j means use the variable j found in class Constants rather than the \
    variable j found in class Pclp:: (i.e., Pclp::j). NOTE: the we expect the last character in this 1D array to be a \
    '\0` (NULL) character
    while ( *(c.ptr_cbb_results + c.Constants::j) != ';'  ) { //Starting with the \
        last character in c.ptr_cbb_results, make sure this character is a `;`, thus terminating the SQL command.  Set this \
        character to zero if the character is not a `;`. Then check the character prior to  (i.e., left of) this last character \
        and check that it's a `;`, then if this character is not `;` set it to `0`. Continue in this manner at most 6 characters \
        have been checked.
        *(c.ptr_cbb_results + c.Constants::j) = '\0';
        c.Constants::j--; //Get the next character which is to the left of this current character
    }
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (c.db3__) printf("05.%d =======> Returned from fabricateSQLr3\n", c.k);
    if (c.db3__) printf("06.%d Call-back buffer, cbb, looks like:\n", c.k);
    if (c.db3__) printf  ("07.%d %s\n", c.k, c.ptr_cbb_results);
    c.fp = fopen(c.outputFileName, "w");
    c.rc=fputs( c.ptr_cbb_results,  c.fp); //This function returns a non-negative value, or else it returns EOF indicating error. \
    On Linux systems and OS X, the character to input to cause an EOF is Ctrl-D. For Windows, it's Ctrl-Z.
    if(c.db3__) printf("outBufSize is %zu, strlen(ptr_cbb_results) is %lu, ∆ is %lu\n", c.outBufSize, strlen(c.ptr_cbb_results), \
                       c.outBufSize - strlen(c.ptr_cbb_results));
    c.charactersWrittenToFile += fwrite(c.ptr_cbb_results, sizeof(char), strlen(c.ptr_cbb_results), c.fp); // Output into the output file the \
    first logical record, the SQL COPY statement.
    if (c.charactersWrittenToFile>0) {
        printf("09a.%d Got a non-zero return code [Good!] from fwrite of %d \n", c.k, c.charactersWrittenToFile);
    } else {
        printf("09b.%d ERROR: got a negative return code [BAD!!] from fwrite or %d \n", c.k, c.charactersWrittenToFile);
    }
    if (c.db3__) {
        printf("10.%d  Done =============================================================================\n", c.k);
        printf("fabricateSQL's total execution time: %lu nsec;\ntime to do constructor processing: %lu nsec;\ntime to do fabricate \
               processing: %lu nsec;\ntime to do delete processing: %lu nsec.\n", \
               c.ptrPerformanceBuffer->totalTime, \
               c.ptrPerformanceBuffer->constructorTime, \
               c.ptrPerformanceBuffer->fabricateTime, \
               c.ptrPerformanceBuffer->deleteTime
               );
    }
    c.dbc->conn = PQconnectdb(c.ptrConstructedConnectionString);
    if(PQstatus(c.dbc->conn) != CONNECTION_OK) {
        std::cerr << "Connection to database " << c.dbc->dbname << " failed because " << PQerrorMessage(c.dbc->conn) << std::endl;
        return -1;
    } else {
        std::cout << "Connection to database " << c.dbc->dbname << " on port " << c.dbc->portId << " was successful!" << std::endl;
    }
    c.rc = goExecuteSomeSQL(c, "BEGIN;", PGRES_COMMAND_OK, BADBEGIN, true);                                                    // 01
    if (c.rc != 0) return c.rc;
    c.rc = goExecuteSomeSQL(c, c.ptr_cbb_results, PGRES_COMMAND_OK, BADGENERALEXECUTE, true);                                  // 02
    if (c.rc != 0) return c.rc;
    c.rc = goExecuteSomeSQL(c, "FETCH ALL IN srnoonssportal;", PGRES_TUPLES_OK, BADFETCHALL, false, true );                    // 03
    if (c.rc != 0) return c.rc;
    c.rc = getTuples(c, true, true);  //Put all data extracted by the SQL fabricated by fabricateSQLr3.dylib into the 2D array \
    named allFetchedData found in the instance of class named Constants
    c.rc = goExecuteSomeSQL(c, "CLOSE srnoonssportal;", PGRES_COMMAND_OK, BADCLOSE, true);                                     // 04
    if (c.rc != 0) return c.rc;
    c.rc = goExecuteSomeSQL(c, "END;", PGRES_COMMAND_OK, BADEND, true);                                                        // 05
    if (c.rc != 0) return c.rc;
//Using data found in 2D array, named allFetchedData, Interpolate the data satisfying the event specified by the -E switch at the \
    point specified by the -e switch.
    c.rc = c.ptrToFunction[c.eventAsIndexNumber][DOPROCESS](c, 0); //Process the event by setting up for the call for performing \
    Chebyshev Interpolation.
    //
    //
    // At this point we can execute the INSERT INTO command formed by the above pointer to Function
    //
    //
    if (c.dbc->conn) {
        PQfinish(c.dbc->conn);
        c.dbc->conn = nullptr; //Indicate disconnected database.
        if (c.db2__) cout << "NOTE: closeout of database connection via dbc->conn occured in main.cpp " << endl;
    }
    return c.rc;
} //End of main.cpp


Constants::Constants(int argc, const char **argv) {
    the_argc = argc;
    the_argv = argv;
    cout << "In constructor for class named Constants. This instance of Class Constants is located at address " <<hex<< this <<endl;
    I_M_HERE = (unsigned long)this;
    dbc = (Dbc *)calloc(sizeOfDbc, 1);   // Dbc is defined in include file:  /usr/local/include/databaseOps/databaseOps.h
    //  cbb_results = (char *)calloc(2000,  1);
    pbSize = sizeof(*ptrPerformanceBuffer);
    ptrPerformanceBuffer = (timings *)calloc(pbSize, 1);
    //Assert database's default values
    
    ptrUserName     = "cjc";              // = "cjc";
    ptrDatabaseName = "LocalWeather";     // = "LocalWeather" ;
    ptrHostName     = "localhost";        // = "localhost";
    ptrPortName     = "5436";             // = "5436";
    
    dbc->dbname     = ptrDatabaseName;
    dbc->portId     = ptrPortName;
    dbc->hostName   = ptrHostName;
    dbc->userId     = ptrUserName;
    dbc->conn       = nullptr;
    dbc->res        = nullptr;
    dbc->debug      = 0;
//  Initialize the First Column of this 2D array of function pointers with the entry points to the “do functions” for SR, SS, … LAST
    ptrToFunction[SUNRISE][THE_SET_OF_DO_FUNCTIONS] = &doSR;
    ptrToFunction[SUNSET][THE_SET_OF_DO_FUNCTIONS]  = &doSS;
    ptrToFunction[NOONMIN][THE_SET_OF_DO_FUNCTIONS] = &doNoonMin;
    ptrToFunction[SPRING][THE_SET_OF_DO_FUNCTIONS]  = &doSpring;
    ptrToFunction[SUMMER][THE_SET_OF_DO_FUNCTIONS]  = &doSummer;
    ptrToFunction[AUTUMN][THE_SET_OF_DO_FUNCTIONS]  = &doAutumn;
    ptrToFunction[WINTER][THE_SET_OF_DO_FUNCTIONS]  = &doWinter;
    ptrToFunction[ERA][THE_SET_OF_DO_FUNCTIONS]     = &doERA;
    ptrToFunction[GAST][THE_SET_OF_DO_FUNCTIONS]    = &doGAST;
    ptrToFunction[LAST][THE_SET_OF_DO_FUNCTIONS]    = &doLAST;
//Initialize the Second Column of this 2D array of function pointers with entry points to the “process functions” for SR, SS, … LAST
    ptrToFunction[SUNRISE][THE_SET_OF_PROCESS_FUNCTIONS] = &processSR;
    ptrToFunction[SUNSET][THE_SET_OF_PROCESS_FUNCTIONS]  = &processSS;
    ptrToFunction[NOONMIN][THE_SET_OF_PROCESS_FUNCTIONS] = &processNoonMin;
    ptrToFunction[SPRING][THE_SET_OF_PROCESS_FUNCTIONS]  = &processSpring;
    ptrToFunction[SUMMER][THE_SET_OF_PROCESS_FUNCTIONS]  = &processSummer;
    ptrToFunction[AUTUMN][THE_SET_OF_PROCESS_FUNCTIONS]  = &processAutumn;
    ptrToFunction[WINTER][THE_SET_OF_PROCESS_FUNCTIONS]  = &processWinter;
    ptrToFunction[ERA][THE_SET_OF_PROCESS_FUNCTIONS]     = &processERA;
    ptrToFunction[GAST][THE_SET_OF_PROCESS_FUNCTIONS]    = &processGAST;
    ptrToFunction[LAST][THE_SET_OF_PROCESS_FUNCTIONS]    = &processLAST;
//
    ptrSQLTemplateERA0 = sqlTemplateERA0;
}
Constants::Constants(void) {
    cout << "Entered Constants::Constants(void), which does very little" << endl;
    I_M_HERE = (unsigned long)this;
    dbc = (Dbc *)calloc(sizeOfDbc, 1);   // Dbc is defined in include file:  /usr/local/include/databaseOps/databaseOps.h
    //  cbb_results = (char *)calloc(2000,  1);
    pbSize = sizeof(*ptrPerformanceBuffer);
    ptrPerformanceBuffer = (timings *)calloc(pbSize, 1);
    //Assert database's default values
    
    ptrUserName     = "cjc";              // = "cjc";
    ptrDatabaseName = "LocalWeather";     // = "LocalWeather" ;
    ptrHostName     = "localhost";        // = "localhost";
    ptrPortName     = "5436";             // = "5436";
    
    dbc->dbname     = ptrDatabaseName;
    dbc->portId     = ptrPortName;
    dbc->hostName   = ptrHostName;
    dbc->userId     = ptrUserName;
    dbc->conn       = nullptr;
    dbc->res        = nullptr;
    dbc->debug      = 0;
    ptrToFunction[SUNRISE][0] = &doSR;
    ptrToFunction[SUNSET][0]  = &doSS;
    ptrToFunction[NOONMIN][0] = &doNoonMin;
    ptrToFunction[SPRING][0]  = &doSpring;
    ptrToFunction[SUMMER][0]  = &doSummer;
    ptrToFunction[AUTUMN][0]  = &doAutumn;
    ptrToFunction[WINTER][0]  = &doWinter;
    ptrToFunction[ERA][0]     = &doERA;
    ptrToFunction[GAST][0]    = &doGAST;
    ptrToFunction[LAST][0]    = &doLAST;
    
    ptrToFunction[SUNRISE][THE_SET_OF_PROCESS_FUNCTIONS] = &processSR;
    ptrToFunction[SUNSET][THE_SET_OF_PROCESS_FUNCTIONS]  = &processSS;
    ptrToFunction[NOONMIN][THE_SET_OF_PROCESS_FUNCTIONS] = &processNoonMin;
    ptrToFunction[SPRING][THE_SET_OF_PROCESS_FUNCTIONS]  = &processSpring;
    ptrToFunction[SUMMER][THE_SET_OF_PROCESS_FUNCTIONS]  = &processSummer;
    ptrToFunction[AUTUMN][THE_SET_OF_PROCESS_FUNCTIONS]  = &processAutumn;
    ptrToFunction[WINTER][THE_SET_OF_PROCESS_FUNCTIONS]  = &processWinter;
    ptrToFunction[ERA][THE_SET_OF_PROCESS_FUNCTIONS]     = &processERA;
    ptrToFunction[GAST][THE_SET_OF_PROCESS_FUNCTIONS]    = &processGAST;
    ptrToFunction[LAST][THE_SET_OF_PROCESS_FUNCTIONS]    = &processLAST;
    ptrSQLTemplateERA0 = &sqlTemplateERA0[0];
}
Constants::~Constants(void) {
//Cleanup and return resources (e.g.  calloc'd memory, open files, postgres resources) to operating system
    cout << "Entered the Constants::~Constants Destructors" << endl;
    j=1;
    if (ptrPerformanceBuffer != nullptr) {
        free(ptrPerformanceBuffer); //Free the storage allocated by the constructor, named Constants::Constants(void)cbb
        if (db5__) cout << j << ". Destructor of class Constants had to free memory pointed to by variable `ptrPerformanceBuffer`. " << endl;
    }
    j++;
    if (allFetchedData != nullptr)  {
        free(allFetchedData); //Free acquired storage containing data that FETCHALL … returned.
        if (db5__) cout << j << ". Destructor of class Constants had to free memory pointed to by variable `allFetchedData`. " << endl;
    }
    j++;
    if (dbc != nullptr)  {
        free(dbc);
        if (db5__) cout << j << ". Destructor of class Constants had to free memory pointed to by variable `dbc`. " << endl;
    }
    j++;
    if (ptrArrayDependent != nullptr)  {
        free(ptrArrayDependent);
        if (db5__) cout << j << ". Destructor of class Constants had to free memory pointed to by variable `ptrArrayDependent`. " << endl;
    }
    j++;
    if(ptrAllResultingDependentVaraiables != nullptr) {
        free(ptrAllResultingDependentVaraiables);
        if (db5__) cout << j << ". Destructor of class Constants had to free memory pointed to by variable `ptrAllResultingDependentVariables`. " << endl;
    }
    j++;
    if (ptrArrayIndependent != nullptr)  {
        free(ptrArrayIndependent);
        if (db5__) cout << j << ". Destructor of class Constants had to free memory pointed to by variable `ptrArrayIndependent`. " << endl;
    }
    j++;
    if (dc != nullptr ) {
        free(dc);
        if(db5__) cout << j << ". Destructor of class Constants had to free memory pointed to by variable `dc`. " << endl;
    }
    j++;
    if (db5__) cout << "All calloc'd buffers that were not, upon endering this destructor, free have been freed" << endl;
    if(fp != nullptr ) {
        fclose(fp);
        fp = nullptr;
        if(db5__) cout << j << ". Found file pointer still opened so this destructor closed file pointer named `fp`" << endl;
    }
    j++;
    if (dbc->res != nullptr) { 
        PQclear(dbc->res);
        dbc->res = nullptr;
        if (db5__) cout << j << ". closeout of database results via dbc->res occured in destructor " << endl;
    } else {
        if (db5__) cout << j << ". Result object, res, was allready freed (res‘ memory freed) upon entry to this desctructor." << endl;
    }
    j++;
    if ( dbc->conn != nullptr) {
        PQfinish(dbc->conn); //Do this so the destructor doesn't try to do another PQfinish and thus cause a abnormal termination.
        dbc->conn = nullptr;
        if (db5__) cout << j << ". Just issued a PQfinish(conn) command to close-out database connection. " << endl;
    } else {
        if (db5__) cout << j << ". Connection object, conn, was allready freed (connection closed) upon entry to this desctructor." << endl;
    }
} //End destructor: Constants::~Constants(void)





