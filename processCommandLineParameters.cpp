//
//  processCommandLineParameters.cpp
//  certain_Astro_Parameters
//
//  Created by Clifford Campo on 2/21/21.
//  Copyright © 2021 CliffordCampo. All rights reserved.
//
//
//  processCommandLineParameters.cpp
//  astroParameter
//  copied from:
//  Created by Clifford Campo on 12/6/20.
//  Copyright © 2020 CliffordCampo. All rights reserved.
//
#include "Constants.hpp"
#include "Pclp.hpp"

extern int processSiteID(Pclp&);
extern int processSomeDate(Pclp&);
extern int processBeforeDataPoints(Pclp&);
extern int processAfterDataPoints(Pclp&);
extern int processField(Pclp&);
extern int processEvent(Pclp&);
extern int processEventValue(Pclp&);
extern int processSunRiseDefinition(Pclp&);
extern int processDB1(Pclp&);
extern int processDB2(Pclp&);
extern int processDB3(Pclp&);
extern int processDB4(Pclp&);
extern int processDB5(Pclp&);
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int Pclp::process_CommandLineParamaters( Pclp& c ) {
    cout << "Returned from Pclp constructor. Inspecting the first command line parameter following this program‘s name: " << \
    *(my_argv + NEXTCOMMANDLINEPARAMETER) << endl;
    if( (strcmp(*(my_argv + NEXTCOMMANDLINEPARAMETER), "-h") == 0 ) || (strcmp(*(my_argv + 1), "--help") == 0  ) ) {//If the first \
parameter is request for the help message, then output the help message and then quit.
        helpMsg();
        c.~Pclp(); //Before exiting, do the Destructor process so we clean things up properly before exiting.
        exit(0);
    }
    k=NEXTCOMMANDLINEPARAMETER; //Preposition k so it focuses on the first command line parameter past the pointer to this \
program's name.
    if( (strcmp(*(my_argv + k), "-1" )) == 0 ) {
        k += arrayOfPointerToFunction[_DB1_](c);
        //       db1__ = 1; //Indicate that the "-1" command line parameter was encountered.
        //       k++; // Skip past this "-1" value that immediately follows the fully-qualified name of this program.
    }
    while (k < my_argc-1) { //Process each command line keywork/parameter pair, as they are encountered.
        auto posCommandLineArgument = commandLineArguments.find( *(my_argv + k) ); //Note find works with a pointer to c-string!
        if (posCommandLineArgument != commandLineArguments.end() ) {
            // This works, too!           this->theKeywordWeFound = this->commandLineArguments.find( *(this->my_argv + k) )->first;\
            JUST WHAT IS the magic type of `auto`?
            theKeywordWeFound = posCommandLineArgument->first;
            theAssociatedValue = posCommandLineArgument->second;
            //          this->theAssociatedValue = this->commandLineArguments.find( *(this->my_argv + k) )->second;
            if (c.db1__) cout << "Command line argument keyword " << theKeywordWeFound <<  \
                " has an associated jump-table index value of " << dec << theAssociatedValue << endl;
            //See https://www.codeguru.com/cpp/cpp/article.php/c17401/C-Tutorial-PointertoMember-Function.htm for pointers-to-member-functions
            rc = arrayOfPointerToFunction[theAssociatedValue](c);
            if (db1__) cout << "Return code from " << "arrayOfPointerToFunction[" << theAssociatedValue << "](c) is " << rc << endl;
            //           myptToMemberFunction = clpt[theAssociatedValue].ptpf; //Get the pointer to the desired function.
            //           cout << "desired function is at location " << hex << myptToMemberFunction << dec << endl;
            /*
             if (theAssociatedValue == _DB1_) rc = process_DB1();
             else if (theAssociatedValue == _DB2_) rc = process_DB2();
             else if (theAssociatedValue == _DB3_) rc = process_DB3();
             else if (theAssociatedValue == _DB4_) rc = process_DB4();
             else if (theAssociatedValue == _DB5_) rc = process_DB5();
             else if (theAssociatedValue == _SITEID_) rc = process_SiteID();
             else if (theAssociatedValue == _SOMEDATE_) rc = process_SomeDate();
             else if (theAssociatedValue == _EVENT_) rc = process_Event();
             else if (theAssociatedValue == _EVENTVALUE_) rc = process_EventValue();
             else if (theAssociatedValue == _FILE_) rc = process_File();
             else if (theAssociatedValue == _FIELD_) rc = process_Field();
             else if (theAssociatedValue == _HOST_) rc = process_Host();
             else if (theAssociatedValue == _DATABASE_) rc = process_Database();
             else if (theAssociatedValue == _MODULO_) rc = process_Modulo();
             else if (theAssociatedValue == _MAXPRECISION_) rc = process_MaxPrecision();
             else if (theAssociatedValue == _PORT_) rc = process_Port();
             else if (theAssociatedValue == _PREAMBLE_) rc = process_Preamble();
             else if (theAssociatedValue == _POSTAMBLE_) rc = process_Postamble();
             else if (theAssociatedValue == _USERNAME_) rc = process_UserName();
             else if (theAssociatedValue == _SUNRISEDEFINITION_) rc = process_SunRiseDefinition();
             else if (theAssociatedValue == _SUNSETDEFINITION_) rc = process_SunSetDefinition();
             else if (theAssociatedValue == _BEFOREDATAPOINTS_) rc = process_BeforeDataPoints();
             else if (theAssociatedValue == _AFTERDATAPOINTS_) rc = process_AfterDataPoints();
             else if (theAssociatedValue == _NOSQL_) rc = process_NoSQL();
             else rc = process_Help();
             */
            if (rc < 0) {
                return (rc);
            }
            k += rc; //If c.rc = 2, then skip past the parameter associated with the command line keyword/switch, to obtain \
            the next command line keword. If c.rc=1, then the command line keyword has no associated parameter and we will \
            process the next command line keyword.
        } else {
            cerr << "We could not locate the command line switch " << (*my_argv + k) << \
            " in our dictionary of possible command line switch statements. executing the help message, showing the valid " << \
            "command line parameters, and then exiting with exit code of " << dec << EXITCODE_HELPMESSAGE << endl;
            helpMsg();
            return EXITCODE_HELPMESSAGE;
        } //End of if/else
    } //End  of while
    return rc;
} // End of member function Pclp::processCommandLineParamaters

int Pclp::process_Event(void) {
    ptrWhatThisEventIs = (char *)*(my_argv + NEXTCOMMANDLINEPARAMETER + k);
    if (this->db1__) cout << "EVENT: " << " processing command line switch " << this->theKeywordWeFound  << \
        " and associated parameter's value of " << this->ptrWhatThisEventIs << endl;
    this->i=0;
    //   work_c=**(this->my_argv + NEXTCOMMANDLINEPARAMETER + k + i);
    //   work_c = *(ptrWhatThisEventIs + i);
    this->ptr_eventInLowerCaseBuffer = &this->eventInLowerCaseBuffer[0];
    while ( *(this->ptrWhatThisEventIs + this->i) ) {
        this->work_c = *(this->ptrWhatThisEventIs + this->i);
        if (this->work_c >= 'A' &&  this->work_c <= 'Z') { //Is this an upper case character?
            this->work_c += 32; //Change upper case to lower case
            *(this->ptr_eventInLowerCaseBuffer + this->i) = this->work_c;
        } else if ((this->work_c >= '0' &&  this->work_c <= '9') || ((this->work_c >= 'a' &&  this->work_c <= 'z'))) {
            *(this->ptr_eventInLowerCaseBuffer + this->i) = this->work_c;
        } else {
            cout << "Character `" << this->work_c << "` is an illegal character for specifying an event. We are exiting with " << \
            "a return code of " << EXIT_ILLEGAL_EVENT_CHARACTER << endl;
            return (EXIT_ILLEGAL_EVENT_CHARACTER); //Use return-to-main rather than exit so we're given an opportunity to execute \
            the distructor
        }
        this->i++;
    } //End of while statements
    //Turn this value positive so we can tell that it has been modified.
    this->whatThisEventIsAsString = string(this->ptrWhatThisEventIs); //Convert this command line-specified event, as type const char *, to \
    a string object so we can do some serious homework on it and convert this event into an index into an array of \
    pointers to function.
    this->whatThisEventIsAsString_lc = string(this->whatThisEventIsAsString); //Make a copy of whatThisEventIsAsString so we can \
    retain the original, as keyed into the command line, into an all lower-case version which we will use for \
    processing.
    transform(this->whatThisEventIsAsString_lc.begin(), this->whatThisEventIsAsString_lc.end(), \
              this->whatThisEventIsAsString_lc.begin(), ::tolower); //Convert Event ”all lower case“ minimizing the \
    number of upper-case/lower-case variations. Note, “_lc” stands for lower case.
    this->str_eventInLowerCase = this->whatThisEventIsAsString_lc; // Make a copy of this all lower-case version of the event we wish to process.
    this->ptr_lcEvent = (char *)this->whatThisEventIsAsString_lc.c_str();
    if (this->db1__) cout << "The event " << this->whatThisEventIsAsString << ", when transformed to all lower case is " <<  \
        this->whatThisEventIsAsString_lc << ". The size of whatThisEventIsAsString_lc is " << \
        sizeof(this->whatThisEventIsAsString_lc) << endl;
    
    
#define SUNRISE 0
#define SUNSET  1
#define NOONMIN 2
#define SPRING  3
#define SUMMER  4
#define AUTUMN  5
#define WINTER  6
#define ERA     7
#define GAST    8
#define LAST    9
    
    if ( 0 == this->whatThisEventIsAsString_lc.compare("era") ) {
        this->whatKindOfProcessing = ERA; //Set the processing table index so we can jump to the relavent processing table;
        
    } else if (0 == (this->whatThisEventIsAsString_lc.compare("sr")) || (0 == this->whatThisEventIsAsString_lc.compare("sunrise"))) {
        this->whatKindOfProcessing = SUNRISE;
        
    } else if ((0 == this->whatThisEventIsAsString_lc.compare("ss")) || (0 == this->whatThisEventIsAsString_lc.compare("sunset"))) {
        this->whatKindOfProcessing = SUNSET;
        
    } else if ((0 == this->whatThisEventIsAsString_lc.compare("noonmin")) || (0 == this->whatThisEventIsAsString_lc.compare("noon"))) {
        this->whatKindOfProcessing = NOONMIN;
        
    } else if (0 == this->whatThisEventIsAsString_lc.compare("spring")) {
        this->whatKindOfProcessing = SPRING;
        
    } else if (0 == this->whatThisEventIsAsString_lc.compare("summer")) {
        this->whatKindOfProcessing = SUMMER;
        
    } else if ((0 == this->whatThisEventIsAsString_lc.compare("autumn")) || (0 == this->whatThisEventIsAsString_lc.compare("fall") ) ) {
        this->whatKindOfProcessing = AUTUMN;
        
    } else if (0 == this->whatThisEventIsAsString_lc.compare("winter")) {
        this->whatKindOfProcessing = WINTER;
        
    } else if (0 == this->whatThisEventIsAsString_lc.compare("gast")) {
        this->whatKindOfProcessing = GAST;
        
    } else if (0 == this->whatThisEventIsAsString_lc.compare("last")) {
        this->whatKindOfProcessing = LAST;
    } else {
        cerr << "The parameter associated with the -E switch, " << this->whatThisEventIsAsString << ", does not match " << \
        "ERA, or SUNRISE, or SR, or SS, or SUNSET, or NOONMIN, or SPRING, or SUMMER, or AUTUMN/FALL, or WINTER, " << \
        "or GAST,  or LAST.\nWe're teminating with exit code " << BAD_E_COMMAND_LINE_SWITCH << " due to unknown " << \
        "processing directive supplied via the -E command line switch " << endl;
        exit (BAD_E_COMMAND_LINE_SWITCH); 
    }
    return((int)this->clpt[_EVENT_].hasAssociatedParameter);
}
int Pclp::process_SiteID(void) {
    this->ptrToSiteID = (char *)*(this->my_argv + NEXTCOMMANDLINEPARAMETER + this->k);
    this->Constants::ptrWhatThisSiteIdIs =  *(this->my_argv + NEXTCOMMANDLINEPARAMETER + this->k);
    if (this->db1__) cout << "SiteID: processing command line switch " << this->theKeywordWeFound << \
        " and associated parameter's value of " << this->ptrToSiteID << \
        ". See LocalWeather database's tbl_site_names for a fuller definitoin of SiteID."  << endl;
    return((int)this->clpt[_EVENTVALUE_].hasAssociatedParameter);
}
int Pclp::process_UserName(void) {
    this->ptrUserName = (char *)*(this->my_argv + NEXTCOMMANDLINEPARAMETER + this->k);
    if (this->db1__) cout << "USERNAME: processing command line switch/keyword " << this->theKeywordWeFound << \
        " which is associated with the parameter " << this->ptrUserName << endl;
    return((int)this->clpt[_EVENTVALUE_].hasAssociatedParameter);
}
int Pclp::process_EventValue(void) {
    this->ptrWhatThisEventDefinitionIs = (char *)*(this->my_argv + NEXTCOMMANDLINEPARAMETER + this->k);
    Constants::ptrWhatThisEventDefinitionIs = *(this->my_argv + NEXTCOMMANDLINEPARAMETER + this->k);
    if (this->db1__) cout << "EVENTVALUE: processing command line switch " << this->theKeywordWeFound << \
        " and associated parameter's value of " << this->ptrWhatThisEventDefinitionIs << \
        ". This event value is the value which this program will find, through interpolation, when the event, introduced by " << \
        "the -E command line switch, assumes this value." << endl;
    this->eventParameter = atof(this->ptrWhatThisEventDefinitionIs);  // -e will be like 90.5833… when  -E is SUNRISE or SUNSET
    //Turn this value positive so we can tell that it has been modified.
    return((int)this->clpt[_EVENTVALUE_].hasAssociatedParameter);
}

int Pclp::process_Port(void) {
    //Override default port number [default=5436];
    this->ptrPortName = (char *)*(this->my_argv + NEXTCOMMANDLINEPARAMETER + this->k);
    this->dbc->portId = this->ptrPortName;
    if (this->db1__) cout << "PORT: " << " processing command line switch " << this->theKeywordWeFound << \
        " and associated parameter's value of " << this->dbc->portId << endl;
    return((int)this->clpt[_PORT_].hasAssociatedParameter);
}

int Pclp::process_Host( void  ) {
    //Override database's default host name [default=localhost];
    this->ptrHostName = (char *)*(this->my_argv + NEXTCOMMANDLINEPARAMETER + this->k);
    this->dbc->hostName = this->ptrHostName;
    if (this->db1__) cout << "HOST: " << " processing command line switch " << this->theKeywordWeFound << \
        " and associated parameter's value of " << this->dbc->hostName << endl;
    return((int)this->clpt[_HOST_].hasAssociatedParameter);
}
int Pclp::process_Database( void ) {
    //Override default database name [default=LocalWeather];
    this->ptrDatabaseName = (char *)*(this->my_argv + NEXTCOMMANDLINEPARAMETER + this->k);
    this->dbc->dbname = this->ptrDatabaseName;
    Constants::ptrDatabaseName = *(this->my_argv + NEXTCOMMANDLINEPARAMETER + this->k);
    if (this->db1__) cout << "DATABASE: " << " processing command line switch " << this->theKeywordWeFound << \
        " and associated parameter's value of " << this->dbc->dbname << endl;
    return((int)this->clpt[_DATABASE_].hasAssociatedParameter);
}
int Pclp::process_DB1( void ) {
    this->db1__ = 1;
    if (this->db1__) cout << "DB1: " << " processing command line switch " << this->theKeywordWeFound << \
        ", which has no associated parameter"  << endl;
    return((int)this->clpt[_DB1_].hasAssociatedParameter);
}
int Pclp::process_DB2( void ) {
    this->db2__ = 1;
    if (this->db1__) cout << "DB2: " << " processing command line switch " << this->theKeywordWeFound << \
        ", which has no associated parameter"  << endl;
    return((int)this->clpt[_DB2_].hasAssociatedParameter);
}
int Pclp::process_DB3( void ) {
    this->db3__ = 1;
    if (this->db1__) cout << "DB3: " << " processing command line switch " << this->theKeywordWeFound << \
        ", which has no associated parameter"  << endl;
    return((int)this->clpt[_DB3_].hasAssociatedParameter);
}
int Pclp::process_DB4( void ) {
    this->db4__ = 1;
    if (this->db1__) cout << "DB4: " << " processing command line switch " << this->theKeywordWeFound << \
        ", which has no associated parameter"  << endl;
    return((int)this->clpt[_DB4_].hasAssociatedParameter);
}
int Pclp::process_DB5( void  ) {
    this->db5__ = 1;
    if (this->db5__) cout << "DB5: " << " processing command line switch " << this->theKeywordWeFound << \
        ", which has no associated parameter"  << endl;
    return((int)this->clpt[_DB5_].hasAssociatedParameter);
}
int Pclp::process_Year(void) {
    ptrWhatThisSomeYearIs = (char *)*(my_argv + NEXTCOMMANDLINEPARAMETER + this->k);
    Constants::ptrWhatThisSomeYearIs = *(my_argv + NEXTCOMMANDLINEPARAMETER + this->k);
    if (this->db1__) cout << "SOMEYEAR: "<< " processing command line switch " << this->theKeywordWeFound << \
        " and the associated parameter's value of " << ptrWhatThisSomeYearIs << endl;
    return((int)this->clpt[_YEAR_].hasAssociatedParameter);
}
int Pclp::process_Season(void) {
    ptrWhatThisSeasonIs = (char *)*(my_argv + NEXTCOMMANDLINEPARAMETER + this->k);
    Constants::ptrWhatThisSeasonIs = *(my_argv + NEXTCOMMANDLINEPARAMETER + this->k);
    if (this->db1__) cout << "SEASON: "<< " processing command line switch " << this->theKeywordWeFound << \
        " and the associated parameter's value of " << ptrWhatThisSomeYearIs << endl;
     return((int)this->clpt[_SEASON_].hasAssociatedParameter);
}
int Pclp::process_SomeDate( void ) {
    ptrWhatThisSomeDateIs = (char *)*(my_argv + NEXTCOMMANDLINEPARAMETER + this->k);
    Constants::ptrWhatThisSomeDateIs = *(my_argv + NEXTCOMMANDLINEPARAMETER + this->k);
    if (this->db1__) cout << "SOMEDATE: "<< " processing command line switch " << this->theKeywordWeFound << \
        " and the associated parameter's value of " << ptrWhatThisSomeDateIs << endl;
    //Turn this value positive so we can tell that it has been modified.
    return((int)this->clpt[_SOMEDATE_].hasAssociatedParameter);
}
int Pclp::process_BeforeDataPoints( void ) {
    this->ptrWhatThisBeforeDataPointsIs = (char *)*(this->my_argv + NEXTCOMMANDLINEPARAMETER + this->k);
    Constants::ptrWhatThisBeforeDataPointsIs = *(this->my_argv + NEXTCOMMANDLINEPARAMETER + this->k); //Save a copy of the keyword's parameter as it appears on \
    the command line as a pointer to c-string. Saved in private area.
    if (this->db1__) cout << "BEFOREDATAPOINTS: " << " processing command line switch " << this->theKeywordWeFound << \
        " and the associated parameter's value of " << this->ptrWhatThisBeforeDataPointsIs << endl;
    //Turn this value positive so we can tell that it has been modified.
    return((int)this->clpt[_BEFOREDATAPOINTS_].hasAssociatedParameter);
}
int Pclp::process_AfterDataPoints( void ) {
    this->ptrWhatThisAfterDataPointsIs = (char *)*(this->my_argv + NEXTCOMMANDLINEPARAMETER + this->k);
    Constants::ptrWhatThisAfterDataPointsIs = *(this->my_argv + NEXTCOMMANDLINEPARAMETER + this->k); //Save a copy of the keyword's parameter as it appears on \
    the command line as a pointer to c-string. Saved in private area.
    if (this->db1__) cout << "AFTERDATAPOINTS: " << " processing command line switch " << this->theKeywordWeFound << \
        " and associated parameter's value of " << this->ptrWhatThisAfterDataPointsIs << endl;
    //Turn this value positive so we can tell that it has been modified.
    return((int)this->clpt[_AFTERDATAPOINTS_].hasAssociatedParameter);
}
int Pclp::process_File( void ) {
    //Override the default name of the output file \
    [Default: const char *outputFileName = "/Users/cjc/sql/testingFabricateSQL3.sql";]
    //Turn this value positive, indicating it has been modified and default overridden
    this->outputFileName =(char *)*(this->my_argv + NEXTCOMMANDLINEPARAMETER + this->k);
    if (this->db1__) cout << "FILE: " << " processing command line switch " << this->theKeywordWeFound << \
        " and associated parameter's value of " << this->outputFileName << endl;
    return((int)this->clpt[_FILE_].hasAssociatedParameter);
}
int Pclp::process_NoSQL( void ) {
    this->executeTheSQL = false;
    if (this->db1__) cout << "NOSQL: " << " processing command line switch " << this->theKeywordWeFound << \
        ", which has no associated parameter"  << endl;
    return((int)this->clpt[_NOSQL_].hasAssociatedParameter);
}
int Pclp::process_Postamble(void) {
    //[Default: const char *ptrWhatThisPreambleIs = "BEGIN; DECLARE srnoonssportal CURSOR FOR "]
    //Turn this value positive, indicating it has been modified meaning default override
    this->ptrWhatThisPostambleIs = (char *)*(this->my_argv + NEXTCOMMANDLINEPARAMETER + this->k); //Override the default preamble
    if (this->db1__) cout << "POSTAMBLE: " << " processing command line switch " << this->theKeywordWeFound << \
        " and associated parameter's value of " << ptrWhatThisPostambleIs << endl;
    return((int)this->clpt[_POSTAMBLE_].hasAssociatedParameter);
}
int Pclp::process_MaxPrecision(void) {
    //Override default displayed maximum precision of displayed parameters of type double [default: 15 characters]
    //Turn this value positive, indicating it has been modified meaning default override
    this->DESIRED_MAX_PRECISION = atoi(*(this->my_argv + NEXTCOMMANDLINEPARAMETER + this->k)); //Override default value [default=15].
    if (this->db1__) cout << "MAXPRECISION: " << " processing coPclp::mmand line switch " << this->theKeywordWeFound << \
        " and associated parameter's value of " << DESIRED_MAX_PRECISION << endl;
    return((int)this->clpt[_MAXPRECISION_].hasAssociatedParameter);
}
int Pclp::process_Preamble(void) {
    //[Default: const char *ptrWhatThisPreambleIs = "BEGIN; DECLARE srnoonssportal CURSOR FOR "]
    //Turn this value positive, indicating it has been modified meaning default override
    this->ptrWhatThisPreambleIs = (char *)*(this->my_argv + NEXTCOMMANDLINEPARAMETER + this->k); //Override the default preamble
    if (this->db1__) cout << "PREAMBLE: " << " processing command line switch " << this->theKeywordWeFound << \
        " and associated parameter's value of " << this->ptrWhatThisPreambleIs << endl;
    return((int)this->clpt[_PREAMBLE_].hasAssociatedParameter);
}
int Pclp::process_Modulo(void) {
    this->modulo_lines_perPage = atoi(*(my_argv + NEXTCOMMANDLINEPARAMETER +  this->k));
    if ( this->db1__) cout << "MODULO: " << " processing command line switch " <<  this->theKeywordWeFound << \
        " and associated value " << modulo_lines_perPage  << endl;
    return((int) this->clpt[_MODULO_].hasAssociatedParameter);
}
int Pclp::process_Field(void) {
    this->ptrWhatThisFieldIs = (char *)*( this->my_argv + NEXTCOMMANDLINEPARAMETER +  this->k);
    Constants::ptrWhatThisFieldIs = *( this->my_argv + NEXTCOMMANDLINEPARAMETER +  this->k);
    if ( this->db1__) cout << "FIELD: " << " processing command line switch " <<  this->theKeywordWeFound  << \
        " and associated parameter's value of " <<  this->ptrWhatThisFieldIs << endl;
    return((int) this->clpt[_FIELD_].hasAssociatedParameter);
}

int Pclp::process_SunRiseDefinition(void) {
    this->ptrWhatThis_SR_DefIs = (char *)*( this->my_argv + NEXTCOMMANDLINEPARAMETER +  this->k);
    Constants::ptrWhatThis_SR_DefIs = *( this->my_argv + NEXTCOMMANDLINEPARAMETER +  this->k);
    if ( this->db1__) cout << "SUNRISEDEFINITION: " << " processing command line switch " <<  this->theKeywordWeFound  << \
        " and associated parameter's value " <<  this->ptrWhatThis_SR_DefIs << endl;
    //Turn this value positive so we can tell that it has been modified.
    return((int) this->clpt[_SUNRISEDEFINITION_].hasAssociatedParameter);
}

int Pclp::process_SunSetDefinition(void) {
    this->ptrWhatThis_SS_DefIs = (char *)*( this->my_argv + NEXTCOMMANDLINEPARAMETER +  this->k);
    Constants::ptrWhatThis_SS_DefIs = *( this->my_argv + NEXTCOMMANDLINEPARAMETER +  this->k);
    if ( this->db1__) cout << "SUNSETDEFINITION: " << " processing command line switch " <<  this->theKeywordWeFound  << \
        " and associated parameter's value " <<  this->ptrWhatThis_SS_DefIs << endl;
    //Turn this value positive so we can tell that it has been modified.
    return((int) this->clpt[_SUNSETDEFINITION_].hasAssociatedParameter);
}

int Pclp::process_Help(void) {
    if ( this->db1__) cout << "HELP: " << " processing command line switch " <<  this->theKeywordWeFound << \
        " and associated parameter's value of: NULL"  << endl;
    Pclp::helpMsg();
    exit ( EXITCODE_HELPMESSAGE );
    //    return((int)this->clpt[_HELP_].hasAssociatedParameter);
}
//Here we present the following wrapper functions necessary because the Xcode compiler doesn't seem to know how to handle \
“pointers-to-member-function” but does know how to handle plain-old pointers-to-function
int processSiteID(Pclp& p) {                      //00
    return(p.process_SiteID());
}
int processSomeDate(Pclp& p) {                    //01
    return(p.process_SomeDate());
}
int processBeforeDataPoints(Pclp& p) {
    return(p.process_BeforeDataPoints());         //02
}
int processAfterDataPoints(Pclp& p) {
    return(p.process_AfterDataPoints());          //03
}
int processField(Pclp& p) {
    return(p.process_Field());                    //04
}
int processEvent(Pclp& p) {
    return(p.process_Event());                    //05
}
int processEventValue(Pclp& p) {
    return(p.process_EventValue());               //06
}
int processSunRiseDefinition(Pclp& p) {
    return(p.process_SunRiseDefinition());        //07
}
int processDB1(Pclp& p) {
    return(p.process_DB1());                      //08
}
int processDB2(Pclp& p) {
    return(p.process_DB2());                      //09
}
int processDB3(Pclp& p) {
    return(p.process_DB3());                      //10
}
int processDB4(Pclp& p) {
    return(p.process_DB4());                      //11
}
int processDB5(Pclp& p) {
    return(p.process_DB5());                      //12
}
int processModulo(Pclp& p) {
    return(p.process_Modulo());                   //13
}
int processPreamble(Pclp& p) {
    return(p.process_Preamble());                 //14
}
int processPostamble(Pclp& p) {
    return(p.process_Postamble());                //15
}
int processMaxPrecision(Pclp& p) {
    return(p.process_MaxPrecision());             //16
}
int processNoSQL(Pclp& p) {
    return(p.process_NoSQL());                    //17
}
int processFile(Pclp& p) {
    return(p.process_File());                     //18
}
int processPort(Pclp& p) {
    return(p.process_Port());                     //19
}
int processHost(Pclp& p) {
    return(p.process_Host());                     //20
}
int processDatabase(Pclp& p) {
    return(p.process_Database());                 //21
}
int processUserName(Pclp& p) {
    return(p.process_UserName());                 //22
}
int processHelp(Pclp& p) {
    return(p.process_Help());                     //23
}
int processSunSetDefinition(Pclp& p) {
    return(p.process_SunSetDefinition());         //24
}
int processYear(Pclp& p) {
    return(p.process_Year());                     //25
}
int processSeason(Pclp& p) {
    return(p.process_Season());                   //26
}
////////////////////////// Pclp DESTRUCTOR /////////////////////////////////////////////////////////////////////////////////////////
Pclp::~Pclp(void) {
    cout << "Inside the class Pclp (for processing table) destructor. " << "\nIt looks like there were no malloc'd/calloc'd " << \
    "\nMemories, or objects to free/delete before exiting. Puka, Puka." << endl;
}
////////////////////////// Pclp CONSTRUCTOR ////////////////////////////////////////////////////////////////////////////////////////
//To supress the error message:  "Constructor for 'Pclp' must explicitly initialize the base class 'Constants' which does not have \
a default constructor". This means that the derived class, i.e., Pclp, firs calls the constructor for class named Constants.
//Must Do this ------------------------>VVVVVVVVVVVVVVVVVVVVVVV
Pclp::Pclp(int argc, const char **argv) : Constants (argc, argv){
    ;
    cout << "Inside the class Pclp (for processing table) constructor." << endl;

    ptr_nameOfThisProgram = *(argv+0);
    my_argc = argc;
    my_argv = argv;
    ptrWhatThisSiteIdIs__ = ptrWhatThisSiteIdIs;
    ptrWhatThisSomeDateIs__ = ptrWhatThisSomeDateIs;                     //Should point to base class' ptrWhatThisSomeDateIs
    ptrWhatThisBeforeDataPointsIs__ = ptrWhatThisBeforeDataPointsIs;     //Should point to base class' ptrWhatThisBeforeDataPointsIs
    ptrWhatThisAfterDataPointsIs__ = ptrWhatThisAfterDataPointsIs;       //Should point to base class' ptrWhatThisAfterDataPointsIs
    ptrWhatThisPostambleIs__ = ptrWhatThisPostambleIs;                   //Should point to base class' ptrWhatThisPostambleIs
    ptrWhatThisDefIs__ = ptrWhatThisDefIs;
    ptrWhatThisPreambleIs__ = ptrWhatThisPreambleIs;                     //Should point to base class' ptrWhatThisPreambleIs
    ptrWhatThisFieldIs__ =  ptrWhatThisFieldIs;                          //Should point to base class' ptrWhatThisFieldIs
    ptrWhatThisEventIs__ = ptrWhatThisEventIs;                           //Should point to base class' ptrWhatThisEventIs
    ptrWhatThisEventDefinitionIs__ = ptrWhatThisEventDefinitionIs;       //Should point to base class' ptrWhatThisEventDefinitionIs
    ptrWhatThis_SR_DefIs__ = ptrWhatThis_SR_DefIs;                       //Should point to base class' ptrWhatThis_SR_DefIs
    ptrWhatThis_SS_DefIs__ = ptrWhatThis_SS_DefIs;                       //Should point to base class' ptrWhatThis_SS_DefIs
    executeTheSQL = true; //Set to the default value. Member function, process_CommandLineParameters  will reset this value if so \
    directed by the parameter-less commmand line keyword/switch.
    DESIRED_MAX_PRECISION = 15;
    /*
     #define _SITEID_            0                                        // 00
     #define _SOMEDATE_          1 + _SITEID_                             // 01
     #define _BEFOREDATAPOINTS_  1 + _SOMEDATE_                           // 02
     #define _AFTERDATAPOINTS_   1 + _BEFOREDATAPOINTS_                   // 03
     #define _FIELD_             1 + _AFTERDATAPOINTS_                    // 04
     #define _EVENT_             1 + _FIELD_                              // 05
     #define _EVENTVALUE_        1 + _EVENT_                              // 06
     #define _SUNRISEDEFINITION_ 1 + _EVENTVALUE_                         // 07
     #define _DB1_               1 + _SUNRISEDEFINITION_                  // 08
     #define _DB2_               1 + _DB1_                                // 09
     #define _DB3_               1 + _DB2_                                // 10
     #define _DB4_               1 + _DB3_                                // 11
     #define _DB5_               1 + _DB4_                                // 12
     #define _MODULO_            1 + _DB5_                                // 13
     #define _PREAMBLE_          1 + _MODULO_                             // 14
     #define _POSTAMBLE_         1 + _PREAMBLE_                           // 15
     #define _MAXPRECISION_      1 + _POSTAMBLE_                          // 16
     #define _NOSQL_             1 + _MAXPRECISION_                       // 17
     #define _FILE_              1 + _NOSQL_                              // 18
     #define _PORT_              1 + _FILE_                               // 19
     #define _HOST_              1 + _PORT_                               // 20
     #define _DATABASE_          1 + _HOST_                               // 21
     #define _USERNAME_          1 + _DATABASE_                           // 22
     #define _HELP_              1 + _USERNAME_                           // 23
     #define _SUNSETDEFINITION_  1 + _HELP_                               // 24
     #define _YEAR_              1 + _SUNSETDEFINITION_                   // 25
     #define _SEASON_            1 + _YEAR_                               // 26
     */
    //Populate each entry of the command line parameter table (clpt) with the pointer to the function that processes the \
    command line keyword and, if necessary, the keywork's parameter.
    
    arrayOfPointerToFunction[_SITEID_]  = &processSiteID;
    clpt[_SITEID_].ptpf             = &Pclp::process_SiteID;                       //00
    arrayOfPointerToFunction[_SOMEDATE_]  = &processSomeDate;
    clpt[_SOMEDATE_].ptpf           = &Pclp::process_SomeDate;                     //01
    arrayOfPointerToFunction[_BEFOREDATAPOINTS_] = &processBeforeDataPoints;
    clpt[_BEFOREDATAPOINTS_].ptpf   = &Pclp::process_BeforeDataPoints;             //02
    arrayOfPointerToFunction[_AFTERDATAPOINTS_] = &processAfterDataPoints;
    clpt[_AFTERDATAPOINTS_].ptpf    = &Pclp::process_AfterDataPoints;              //03
    arrayOfPointerToFunction[_FIELD_] = &processField;
    clpt[_FIELD_].ptpf              = &Pclp::process_Field;                        //04
    arrayOfPointerToFunction[_EVENT_] = &processEvent;
    clpt[_EVENT_].ptpf              = &Pclp::process_Event;                        //05
    arrayOfPointerToFunction[_EVENTVALUE_] = &processEventValue;
    clpt[ _EVENTVALUE_ ].ptpf       = &Pclp::process_EventValue;                   //06
    arrayOfPointerToFunction[ _SUNRISEDEFINITION_] = &processSunRiseDefinition;
    clpt[ _SUNRISEDEFINITION_].ptpf = &Pclp::process_SunRiseDefinition;            //07
    arrayOfPointerToFunction[_DB1_] = &processDB1;
    clpt[ _DB1_ ].ptpf              = &Pclp::process_DB1;                          //08
    arrayOfPointerToFunction[_DB2_] = &processDB2;
    clpt[ _DB2_ ].ptpf              = &Pclp::process_DB2;                          //09
    arrayOfPointerToFunction[_DB3_] = &processDB3;
    clpt[ _DB3_ ].ptpf              = &Pclp::process_DB3;                          //10
    arrayOfPointerToFunction[_DB4_] = &processDB4;
    clpt[ _DB4_ ].ptpf              = &Pclp::process_DB4;                          //11
    arrayOfPointerToFunction[_DB5_] = &processDB5;
    clpt[ _DB5_ ].ptpf              = &Pclp::process_DB5;                          //12
    arrayOfPointerToFunction[_MODULO_] = &processModulo;
    clpt[ _MODULO_ ].ptpf           = &Pclp::process_Modulo;                       //13
    arrayOfPointerToFunction[_PREAMBLE_] = &processPreamble;
    clpt[ _PREAMBLE_ ].ptpf         = &Pclp::process_Preamble;                     //14
    arrayOfPointerToFunction[_POSTAMBLE_] = &processPostamble;
    clpt[ _POSTAMBLE_ ].ptpf        = &Pclp::process_Postamble;                    //15
    arrayOfPointerToFunction[_MAXPRECISION_] = &processMaxPrecision;
    clpt[ _MAXPRECISION_ ].ptpf     = &Pclp::process_MaxPrecision;                 //16
    arrayOfPointerToFunction[_NOSQL_] = &processNoSQL;
    clpt[ _NOSQL_ ].ptpf            = &Pclp::process_NoSQL;                        //17
    arrayOfPointerToFunction[_FILE_] = &processFile;
    clpt[ _FILE_ ].ptpf             = &Pclp::process_File;                         //18
    arrayOfPointerToFunction[_PORT_] = &processPort;
    clpt[ _PORT_ ].ptpf             = &Pclp::process_Port;                         //19
    arrayOfPointerToFunction[_HOST_] = &processHost;
    clpt[ _HOST_ ].ptpf             = &Pclp::process_Host;                         //20
    arrayOfPointerToFunction[_DATABASE_] = &processDatabase;
    clpt[ _DATABASE_ ].ptpf         = &Pclp::process_Database;                     //21
    arrayOfPointerToFunction[_USERNAME_] = &processUserName;
    clpt[ _USERNAME_ ].ptpf         = &Pclp::process_UserName;                     //22
    arrayOfPointerToFunction[_HELP_] = &processHelp;
    clpt[ _HELP_ ].ptpf             = &Pclp::process_Help;                         //23
    arrayOfPointerToFunction[_SUNSETDEFINITION_] = &processSunSetDefinition;
    clpt[ _SUNSETDEFINITION_ ].ptpf = &Pclp::process_SunSetDefinition; //            24  Added on 2021-02-10T09:21:56
    arrayOfPointerToFunction[_YEAR_] = &processYear;
    clpt[ _YEAR_ ].ptpf = &Pclp::process_Year;                         //            25   Added on 2021-02-28T17:58:29
    arrayOfPointerToFunction[_SEASON_] = &processSeason;
    clpt[ _SEASON_ ].ptpf = &Pclp::process_Season;                     //            26    Added on 2021-02-28T18:00:41
//                                                clpt is an array with 27 entries.
}
//===============================================================================================================================
