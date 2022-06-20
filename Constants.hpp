//
//  Constants.hpp
//  certain_Astro_Parameters
//
//  Created by Clifford Campo on 2/21/21.
//  Copyright © 2021 CliffordCampo. All rights reserved.
//

#ifndef Constants_h
#define Constants_h
//
//  Constants.hpp
//  astroParameter_SRSS
//
//  Created by Clifford Campo on 2/17/21.
//  Copyright © 2021 CliffordCampo. All rights reserved.
//

#ifndef extractKeySiteParameters_hpp
#define extractKeySiteParameters_hpp
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <getopt.h>
#include <map>
#include <iostream>
#include <sstream>
#include <string>
#include <gsl/gsl_chebyshev.h>
//#include <internal/libpq-int.h> //pg_result is defined here as a struct
// #include <libpq-fe.h>
#include "/usr/local/pgsql/pgsql1302/include/libpq-fe.h"
#include <fabricateSQL/fabricateSQLr3.h>
//#include "/usr/local/include/databaseOps/databaseOps.h"
#include <databaseOps/databaseOps.h>
#include <algorithm>
// #include "Pclp.hpp"
using namespace std;

//#include "testingFabricateSQL3.hpp"
/*
 Table "public.tbl_sun_loc_site"
 Column     |            Type             |                           Modifiers                           | Storage | Stats target |                              Description
 ---------------+-----------------------------+---------------------------------------------------------------+---------+--------------+-----------------------------------------------------------------------
 id             | integer                     | not null default nextval('tbl_sun_loc_site_id_seq'::regclass) | plain   |              |
 lt             | timestamp without time zone | not null                                                      | plain   |              |
 topocentricra  | double precision            |                                                               | plain   |              |
 topocentricdec | double precision            |                                                               | plain   |              |
 topocentricdis | double precision            |                                                               | plain   |              |
 azimuth        | double precision            |                                                               | plain   |              |
 zenithdistance | double precision            |                                                               | plain   |              |
 geocentricra   | double precision            |                                                               | plain   |              |
 geocentricdec  | double precision            |                                                               | plain   |              |
 geocentricdis  | double precision            |                                                               | plain   |              |
 jdutc          | double precision            | not null                                                      | plain   |              |
 jdlocal        | double precision            |                                                               | plain   |              |
 siteid         | smallint                    | not null                                                      | plain   |              | See table tbl_site_names for the integer values unique to each site.
 era            | double precision            |                                                               | plain   |              |
 last           | double precision            |                                                               | plain   |              |
 gast           | double precision            |                                                               | plain   |              |
 vcelx          | double precision            |                                                               | plain   |              |
 vcely          | double precision            |                                                               | plain   |              |
 vcelz          | double precision            |                                                               | plain   |              |
 events         | event                       |                                                               | plain   |              |
 Indexes:
 "tbl_sun_loc_site_siteid_jdutck_pk" PRIMARY KEY, btree (siteid, jdutc)
 Foreign-key constraints:
 "tbl_sun_loc_site_fk" FOREIGN KEY (siteid) REFERENCES tbl_site_names(siteid)
 */
#define NEXTCOMMANDLINEPARAMETER 1
#define BAD_doInterpolation_PARAMETER -14

//
//Used in otherFunctions.cpp
#define ID 0
#define LT 1
#define TOPOCENTRICRA 2
#define TOPOCENTRICDEC 3
#define TOPOCENTRICDIS 4
#define AZIMUTH 5
#define ZENITHDISTANCE 6
#define GEOCENTRICRA 7
#define GEOCENTRICDEC 8
#define GEOCENTRICDIS 9
#define JDUTC 10
#define JDLOCAL 11
#define SITEID 12
#define ERA_ 13
#define LAST_ 14
#define GAST_ 15
#define VCELX 16
#define VCELY 17
#define VCELZ 18
#define EVENTS 19
#define MAX_COLUMNS_IN_tbl_sun_loc_site 20
#define DOSETUP   0 //Selects one of the two second dimensions of the array of pointers to function. The functions of this \
first of two of the second dimensions setup the parameters for performing fabricateSQLr3.dylib.
#define DOPROCESS 1 //Selects second of the two second dimensions of the array of pointers to function. The functions of this \
second of two of the second dimensions setup the parameters for performing
#define THE_SET_OF_DO_FUNCTIONS DOSETUP
#define THE_SET_OF_PROCESS_FUNCTIONS DOPROCESS
#define SOMETHINGREASONABLYLARGE 24 //Used for scanning 2D Arrays to make sure we don't loop forever searching for the Array's TERM
#define NUMBEROFCOLUMNSIN2DARRAY 2
enum fieldIDs {fid=ID, flt=LT, ftopocentricra=TOPOCENTRICRA, ftopocentricdec=TOPOCENTRICDEC, ftopocentricdis=TOPOCENTRICDIS, \
    fazimuth=AZIMUTH, fzenithdistance=ZENITHDISTANCE,  fgeocentricra=GEOCENTRICRA, fgeocentricdec=GEOCENTRICDEC, \
    fgeocentricdis=GEOCENTRICDIS, fjdutc=JDUTC, fjdlocal=JDLOCAL, fsiteid=SITEID, fera=ERA_, flast=LAST_, fgast=GAST_, \
    fvcelx=VCELX, fvcely=VCELY, fvcelz=VCELZ, fevents=EVENTS};
typedef struct timings {
    unsigned long constructorTime;  //Time for fabricateSQLr3's time to execute the constructor
    unsigned long fabricateTime;    //Time for fabricateSQLr3's time to execute the fabrication of the SQL
    unsigned long deleteTime;       //Time for fabricateSQLr3's time to execute the destructor
    unsigned long totalTime;        //Total execution time; is the sum of the above three times.
} timings;

//
//                                                             0000000000111111111
//                                                             0133456789012345678
#define NUMBER_OF_CHARACTERS_DESCRIBING_ISO_8601_DATETIME 19 //YYYY-MM-DD hh:mm:ss => 19 characters
//For now, keep the definition of sunrise and sunset as the same zenithdistance value
#define ZENITHDISTANCE_SR_OR_SS "90.58333333333"
#define ZENITHDISTANCE_SR ZENITHDISTANCE_SR_OR_SS  // There's always the option of setting this via compile directive.
#define ZENITHDISTANCE_SS ZENITHDISTANCE_SR_OR_SS  // There's always the option of setting this via compile directive.

#ifdef ALTERNATETYPEDEF
typedef struct {
    double        dbl_value;
    int  index;
    int  unused;
} myParameters;
#else
typedef double myParameters;
#endif
typedef struct {
    int columnIndexNumber;
    int modulousNumber;    //Usually 24 (e.g., last[rolls over daily], gast[daily], geocentricra[yearly], topocentricra[yearly]) \
    or 360 (e.g., era[rolls over daily], azimuth[daily] )
    double achievedValaue; //The value for which we're looking for the column under investingation to achieve
} pertinantDoProcessParameters;
typedef struct {
    char dochebyshev[16] = {'\0'}; // Contains DOCHEBYSHEV [11 characters + 5 unused. Initialized to all zeros.
    int loopCounter;
    int rc;
    double *di;  //points to desired independent variable [Just one value, no array corresponding to fi]
    myParameters *aai; //points to array of independent variables [array of size = o]
    myParameters *aad; //points to array of associated dependent variables [array of size = o]
    double *fd;  // points to dependent variable, found by chebyshev interpolation [Just one value, no array]
    double workIndependentLower;
    double workInDependentUpper;
    double workDependentLower;
    double workDependentUpper;
    double workTrialIndependent;
    double workTrialDependent;
    double workResult;
    double a;
    double b;
    size_t o;   // Number of elements in arrays aai and aad
} do_Chebyshev;
typedef struct {                //Represents one row of data returned from tbl_sun_loc_site
    int id;                     //00 -- 0 must be unique
    char lt[20];                //01 -- 4 must be unique and monotonically increasing. In ISO 8601 format: YYYY-MM-DD HH:MM:SS
    double topocentricra;       //02 -- 24
    double topocentricdec;      //03 -- 32
    double topocentricdis;      //04 -- 40
    double azimuth;             //05 -- 48
    double zenithdistance;      //06 -- 56
    double geocentricra;        //07 -- 64
    double geocentricdec;       //08 -- 72
    double geocentricdis;       //09 -- 80
    double jdutc;               //10 -- 88
    double jdlocal;             //11 -- 96
    unsigned short int siteid;  //12 -- 98Always the same value for each set of one day's data. See tbl_site_names of database \
    LocalWeather for the possible siteid's and associated geospatial parameters associated with each siteid.
    double era;                 //13 -- 106
    double last;                //14 -- 114
    double gast;                //15 -- 122
    double vcelx;               //16 -- 130
    double vcely;               //17 -- 138
    double vcelz;               //18 -- 146
    char   events[16];          //19 -- 154  Usually null. Can take on the following values:
    /*  LocalWeather=# select *, length(enumlabel) FROM pg_enum  ORDER BY length;
     enumtypid | enumsortorder | enumlabel | length
     ----------+---------------+-----------+--------
         24591 |             7 | Fall      |      4
         24591 |            14 | ERA0      |      4
         24591 |             2 | Noon      |      4
         24591 |            16 | LAST0     |      5
         24591 |            15 | GAST0     |      5
         24591 |             9 | Winter    |      6
         24591 |             5 | SunSet    |      6
         24591 |             6 | Summer    |      6
         24591 |             8 | Autumn    |      6
         24591 |            10 | Spring    |      6
         24591 |            12 | EndDST    |      6
         24591 |             3 | Noon180   |      7
         24591 |             1 | SunRise   |      7
         24591 |             4 | NoonMin   |      7
         24591 |            13 | resetERA  |      8
         24591 |            11 | StartDST  |      8
     */
} namesOfColumns ;
typedef struct {
    char **fa;
    char *myTemplate;
    unsigned int  templateSize;
    unsigned int  numberOfTokenReplacementPairs;
} fabstruct;
typedef struct sqlArray {
    char *psql;
    unsigned long ulsql;
} sqlArray;
typedef struct {
    const char *token;
    const char **replacement;
} arrayEntry;
class Constants { // Constants IS the BASE CLASS
private:
    unsigned long I_M_HERE;
public:
    char INSERT_INTO_BUFFER[1000] = {'\0'};
    char cbb_results[2000] =  {'\0'};
    int the_argc;
    const char **the_argv;
    // DO NOT TOUCH *titles or RISK throwing titles out of alignment!
    const char *titles = " id              lt         topocentricra   topocentricdec topocentricdis   azimuth     \
    zenithdistance  geocentricra    geocentricdec  geocentricdis       jdutc             jdlocal     siteid       era          \
    last             gast         vcelx           vcely           vcelz   events";
    
    char *ptr_cbb_results = &cbb_results[0];
    int oprc = 0;
    int optionIndex = 0;
    char *name;
    int val;
    int flag;
    int has_arg;
    int j=1;
    int i=0;
    int k=0;
    int db1__ = 0;
    int db2__ = 0;
    int db3__ = 0;
    int db4__ = 0;
    int db5__ = 0;
    int rc;
    int er;
    int DESIRED_MAX_PRECISION = 15;  //Default Max Precision
    //  The next seven pointers point to calloc'd memory that we will need to free if these values are other than nullptr. ---+
    namesOfColumns *allFetchedData = nullptr; //Will point to memory calloc'd by function getTuples.                          |
    Dbc *dbc                      = nullptr;  //Dbc is defined in include file:  /usr/local/include/databaseOps/databaseOps.h |
    myParameters *ptrArrayDependent     = nullptr;  //                                                                        |
    myParameters *ptrArrayIndependent   = nullptr;  //                                                                        |
    do_Chebyshev *dc              = nullptr;        //                                                                        |
    timings *ptrPerformanceBuffer = nullptr;        //                                                                        |
    namesOfColumns * _Nullable ptrAllResultingDependentVaraiables = nullptr;        //                                        |
    //  The above seven pointers point to calloc'd memory that we will need to free if these values are other than nullptr.   +
    size_t outBufSize;
    ConnStatusType connStatus;
    int charactersWrittenToFile=0;
    int numberOf2Dentries=0;
    int pbSize=0;
    typedef int (*ptf)(Constants &, void *);
    const char *outputFileName = "/Users/cjc/sql/testingFabricateSQL3.sql";
    FILE *fp = nullptr;
    string whatThisEventIsAsString;
    string whatThisEventIsAsString_lc; //Lowercase equivalent of above string.
    const char *ptrNullPostamble = ";";
    const char *ptrSS = "SS";
    int optc;
    char theSiteID[3] = {'0'}; //Max of 2 characters
    const char *ptrToSiteID = theSiteID;
    const char theDate[12] = {'0'}; //The date we're looking for in YYYY-MM-DD Format: 10 characters + 0x'00' c-string terminator character
    const char *ptrToDate = &theDate[0];
    const char *ptrCloseEnd = "CLOSE srnoonssportal; END;";
    const char *ptrCLOSE = "CLOSE srnoonssportal";
    const char *ptrEND = "END";
    
    const char *ptrDefaultConnectionString = "dbname=LocalWeather host=localhost user=cjc port=5436";
    const char *ptrConstructedConnectionString; //This connection string is constructed in real-time
    const double * _Nullable ptrIndependentVariable = nullptr;
    const double * _Nullable ptrDependentVariable = nullptr;
    double resultingDependentVariable; //Interpolated dependent variable
    
    /* ==W A R N I N G======W A R N I N G=======W A R N I N G===========W I L L    R O B I N S O N==================================== \
     Through much trial and error, I discovered that the order of the targets of these pointers MUST be in the same order as appearying \
     in the 2Dimentional arrays of pointers to pointers!! THIS IS FUCKING CRITICAL!!!!!! I wasted days and days tracking down this \
     problem!!!!!!!!!!! ptrThisSRDef shall appear first, followed by ptrWhatThisSunRiseDefIs, followed by ptrThisPreamble, ... ending \
     with ptrWhatThisTermIs. Failure to observe this ordering will result in BAD THINGS HAPPENING: like encountering TERM, which ends \
     fabricateSQLr3.dylib processing, prior to having processed all Token/Replacement pairs. */
    
    //const char *ptrThisSRDef = "SRD";
    //const char *ptrWhatThisSunRiseDefIs = "90.58333333";
    const char * _Nullable ptrC = ", ";
    const char * _Nullable ptrUserName = nullptr;
    const char * _Nullable ptrDatabaseName = nullptr;
    const char * _Nullable ptrHostName = nullptr;
    const char * _Nullable ptrPortName = nullptr;
    const char * _Nullable ptrThisDef = "ZDDEF";
    const char * _Nullable ptrWhatThisDefIs  = ZENITHDISTANCE_SR_OR_SS; //Default zenith distance defining sun rise or sun set.
    const char * _Nullable ptrThisPreamble = "PREAMBLE";
    const char * _Nullable ptrWhatThisPreambleIs = "DECLARE srnoonssportal CURSOR FOR ";  //Default Preamble
    const char * _Nullable ptrThisSiteid = "SITEID";
    const char * _Nullable ptrWhatThisSiteIdIs;   //There is NO Default Siteid
    const char *ptrThisSomeDate = "SOMEDATE";
    const char *ptrWhatThisSomeDateIs = ""; //Default Date [i.e., no default date]
    const char *ptrThisBeforeDataPoints = "BEFOREDATAPOINTS";
    const char *ptrWhatThisBeforeDataPointsIs = "4";
    const char *ptrThisAfterDataPoints = "AFTERDATAPOINTS";
    const char *ptrWhatThisAfterDataPointsIs = "4";    //Default: the same as the default value associated with before data points
    const char *ptrThisEvent = "SRorSS";
    const char *ptrWhatThisEventIs = "";               //There is no default Event
    const char *ptrThisField = "FIELD";
    const char *ptrWhatThisFieldIs = "zenithdistance"; // = "zenithdistance"; //Default Field
    const char *ptrThisPostamble = "POSTAMBLE";
    const char *ptrWhatThisPostambleIs = ";";       //Default postamble id is a c-string-terminating NULL character.
    const char *ptrWhatThisEventDefinitionIs = ZENITHDISTANCE_SR_OR_SS;
    const char *ptrWhatThis_SR_DefIs = ZENITHDISTANCE_SR;
    const char *ptrWhatThis_SS_DefIs = ZENITHDISTANCE_SS;
    const char *ptrWhatThisGeocentricraSeasonIs = "6"; //Default is Summer.
    // MAKE SURE TERM appears LAST before we go off and declare/define 2 dimensional (2D) arrays of pointers to pointers to characters.
    const char *ptrThisTerm = "TERM";
    const char *ptrWhatThisTermIs = "TERM";
    // =============================================================================================================================
    //siteid |  sitename  | latitude | longitude | altitude |  hostid   | portid | timezone | solarradiation
    //-------+------------+----------+-----------+----------+-----------+--------+----------+----------------
    //0      | KPAPAOLI10 |  40.0461 |    -75.48 |      504 | localhost | 5436   |       -5 | t
    
    const char *getSiteParameters = "SELECT siteid, sitename, latitude, longitude, altitude, hostid, portid, timezone, solarradiation FROM tbl_site_names WHERE siteid=";
    // =============================================================================================================================
    //NEED TO replace Tokens:
    //FIELD with era, or gast, or last
    //ZDEF have *ptrWhatThisDefIs = "0" rather than = "90.58333333"
    //SITEID with the siteid supplied via command line
    //SOMEDATE with the date supplied via command line
    int preambleLength = (int)sizeof(*ptrWhatThisPreambleIs);
    int postableLength = (int)sizeof(*ptrWhatThisPostambleIs);
#ifdef ORIGINALERA0
    const char *sqlTemplateERA0 = "PREAMBLE SELECT * FROM tbl_sun_loc_site \
    WHERE lt::DATE='SOMEDATE' AND siteid=SITEID AND id BETWEEN \
    -BEFOREDATAPOINTS + \
    ( SELECT id FROM \
    (SELECT *,abs(FIELD-ZDDEF)  FROM tbl_sun_loc_site WHERE lt::DATE='SOMEDATE' AND siteid=SITEID AND events IS NULL AND id > \
    (SELECT avg(id) FROM tbl_sun_loc_site WHERE lt::DATE='SOMEDATE' AND siteid=SITEID AND events IS NULL)) a  WHERE abs(FIELD-ZDDEF) = \
    (SELECT min(abs(FIELD-ZDDEF))  FROM tbl_sun_loc_site WHERE lt::DATE='SOMEDATE' AND siteid=SITEID AND events IS NULL AND id > \
    (SELECT avg(id) FROM tbl_sun_loc_site where lt::DATE='SOMEDATE' AND siteid=SITEID AND events IS NULL))  ) \
    AND \
    +AFTERDATAPOINTS + \
    ( SELECT id FROM \
    (SELECT *,abs(FIELD-ZDDEF)  FROM tbl_sun_loc_site WHERE lt::DATE='SOMEDATE' AND siteid=SITEID AND events IS NULL AND id > \
    (SELECT avg(id) FROM tbl_sun_loc_site WHERE lt::DATE='SOMEDATE' AND siteid=SITEID AND events IS NULL)) a  WHERE abs(FIELD-ZDDEF) = \
    (SELECT min(abs(FIELD-ZDDEF))  FROM tbl_sun_loc_site WHERE lt::DATE='SOMEDATE' AND siteid=SITEID AND events IS NULL AND id > \
    (SELECT avg(id) FROM tbl_sun_loc_site WHERE lt::DATE='SOMEDATE' AND siteid=SITEID AND events IS NULL))   );POSTAMBLE;;";
//This is the newest, and best, version of sqlTemplateERA0. Previous, original, version rtained for historical reference only.
    const char *sqlTemplateERA0 ="PREAMBLE SELECT * from tbl_sun_loc_site where events IS NULL AND id BETWEEN -BEFOREDATAPOINTS + \
    ( SELECT id  FROM tbl_sun_loc_site  WHERE FIELD = (SELECT min(FIELD)  FROM tbl_sun_loc_site WHERE lt::DATE='SOMEDATE' AND \
    siteid=SITEID AND events IS NULL) AND siteid=SITEID)  AND +AFTERDATAPOINTS + ( SELECT id  FROM tbl_sun_loc_site  WHERE FIELD = \
    (SELECT min(FIELD) FROM tbl_sun_loc_site WHERE lt::DATE='SOMEDATE' AND siteid=SITEID AND events IS NULL) AND siteid = SITEID) \
    ORDER BY jdlocal ;POSTAMBLE";
#endif
    //Define a 2-dimensional array of pointers to characters used to construct the above SQL for determining sun set
#ifdef ORIGINALERA0
    const char **sunTemplateERA0[9][2] = {   \
        {&ptrThisDef,              &ptrWhatThisDefIs},    \
        {&ptrThisPreamble,         &ptrWhatThisPreambleIs},     \
        {&ptrThisSiteid,           &ptrWhatThisSiteIdIs},       \
        {&ptrThisSomeDate,         &ptrWhatThisSomeDateIs},   \
        {&ptrThisBeforeDataPoints, &ptrWhatThisBeforeDataPointsIs}, \
        {&ptrThisAfterDataPoints,  &ptrWhatThisAfterDataPointsIs},  \
        {&ptrThisField,            &ptrWhatThisFieldIs}, \
        {&ptrThisPostamble,        &ptrWhatThisPostambleIs}, \
        {&ptrThisTerm,             &ptrWhatThisTermIs}};
// Newer Version of **sunTemplateERA0[8][2]
    const char **sunTemplateERA0[8][2] = {                          \
        {&ptrThisPreamble,         &ptrWhatThisPreambleIs},         \
        {&ptrThisSiteid,           &ptrWhatThisSiteIdIs},           \
        {&ptrThisSomeDate,         &ptrWhatThisSomeDateIs},         \
        {&ptrThisBeforeDataPoints, &ptrWhatThisBeforeDataPointsIs}, \
        {&ptrThisAfterDataPoints,  &ptrWhatThisAfterDataPointsIs},  \
        {&ptrThisField,            &ptrWhatThisFieldIs},            \
        {&ptrThisPostamble,        &ptrWhatThisPostambleIs},        \
        {&ptrThisTerm,             &ptrWhatThisTermIs}};
#endif
    unsigned long sizeOfTemplateERA0 = sizeof(sqlTemplateERA0); //Get the number of characters, including the string-terminating '\0' character.
    const char *ptrSQLTemplateERA0; // specified by Constants' constructor = &sqlTemplateERA0[0];
    // ===================================================================================================Example of Template Reuse
    //NEED TO replace Tokens:
    //Token FIELD: have ptrWhatThisFieldIs =  "gast" (or era, or last)
    //Token ZDDEF: have *ptrWhatThisDefIs = "0" rather than = "90.58333333"
    //SITEID with the siteid supplied via command line
    //SOMEDATE with the date supplied via command line
    



    // ===================================================================================================Example of Template Reuse
    //NEED TO replace Tokens:
    //FIELD with last (or era, or gast)
    //ZDDEF have *ptrWhatThisDefIs = "0" rather than = "90.5833333"
    //SITEID with the siteid supplied via command line
    //SOMEDATE with the date supplied via command line
    const double cd360= 360;
    const double cd24 =  24;
    const double cd6  =   6;
    const double cd12 =  12;
    const double cd18 =  18;
    const double zd = atof(ZENITHDISTANCE_SR_OR_SS);   //90.583333333;  //zenithdistance defining sunrise or sunset
    double eventParameter;  // points to the desired event parameter; usually either: 360 [e.g. finding when era is \
    zero or 360%360]; 24 [ e.g. finding when gast, or last is zero or 24%24. Could also be used for determining spring's arrival \
    when geocentricra = 24%24 or topocentricra = 24%24 ];
    const char *sqlTemplateSS = "PREAMBLE SELECT * FROM tbl_sun_loc_site \
    WHERE lt::DATE='SOMEDATE' AND siteid=SITEID AND id BETWEEN \
    -BEFOREDATAPOINTS + ( SELECT id FROM (SELECT *,abs(FIELD - ZDDEF) \
    FROM tbl_sun_loc_site WHERE lt::DATE='SOMEDATE' AND siteid=SITEID AND events IS NULL AND id SRorSS (SELECT avg(id) FROM tbl_sun_loc_site \
    WHERE lt::DATE='SOMEDATE' AND siteid=SITEID AND events IS NULL)) a WHERE abs(FIELD - ZDDEF) = \
    (SELECT min(abs(FIELD - ZDDEF)) FROM tbl_sun_loc_site WHERE lt::DATE='SOMEDATE' AND siteid=SITEID AND events IS NULL AND id SRorSS \
    (SELECT avg(id) FROM tbl_sun_loc_site where lt::DATE='SOMEDATE' AND siteid=SITEID AND events IS NULL))  ) \
    AND     \
    +AFTERDATAPOINTS + ( SELECT id FROM (SELECT *,abs(FIELD - ZDDEF) \
    FROM tbl_sun_loc_site WHERE lt::DATE='SOMEDATE' AND siteid=SITEID AND events IS NULL AND id SRorSS (SELECT avg(id) FROM tbl_sun_loc_site \
    WHERE lt::DATE='SOMEDATE' AND siteid=SITEID AND events IS NULL)) a WHERE abs(FIELD - ZDDEF) = \
    (SELECT min(abs(FIELD - ZDDEF)) FROM tbl_sun_loc_site WHERE lt::DATE='SOMEDATE' AND siteid=SITEID AND events IS NULL AND id SRorSS \
    (SELECT avg(id) FROM tbl_sun_loc_site WHERE lt::DATE='SOMEDATE' AND siteid=SITEID AND events IS NULL))) ORDER BY id;POSTAMBLE;;";
    //Define a 2-dimensional array of pointers to characters used to construct the above SQL for determining sun set
    const char **sunSetArray[10][2] = {   \
        {&ptrThisDef,              &ptrWhatThisDefIs},    \
        {&ptrThisPreamble,         &ptrWhatThisPreambleIs},     \
        {&ptrThisSiteid,           &ptrWhatThisSiteIdIs},       \
        {&ptrThisSomeDate,         &ptrWhatThisSomeDateIs},   \
        {&ptrThisBeforeDataPoints, &ptrWhatThisBeforeDataPointsIs}, \
        {&ptrThisAfterDataPoints,  &ptrWhatThisAfterDataPointsIs},  \
        {&ptrThisEvent,            &ptrSS}, \
        {&ptrThisField,            &ptrWhatThisFieldIs}, \
        {&ptrThisPostamble,        &ptrWhatThisPostambleIs}, \
        {&ptrThisTerm,             &ptrWhatThisTermIs}};
    unsigned long sizeOfTemplateSS = sizeof(sqlTemplateSS); //Get the number of characters, including the string-terminating '\0' character.
    const char *ptrSQLTemplateSS = &sqlTemplateSS[0];
    const char *sqlTemplateNOON =
    "PREAMBLE\nSELECT * FROM tbl_sun_loc_site WHERE lt::DATE='SOMEDATE' AND siteid=SITEID AND events IS NULL AND id BETWEEN \n\
    -BEFOREDATAPOINTS + (SELECT id FROM tbl_sun_loc_site WHERE lt::DATE='SOMEDATE' AND siteid=SITEID AND events IS NULL AND FIELD = \
    (SELECT min(FIELD) FROM tbl_sun_loc_site WHERE lt::DATE='SOMEDATE' AND siteid=SITEID AND events IS NULL) ) \n \
    AND \n \
    +AFTERDATAPOINTS + (SELECT id FROM tbl_sun_loc_site WHERE lt::DATE='SOMEDATE' AND siteid=SITEID AND events IS NULL AND FIELD = \
    (SELECT min(FIELD) FROM tbl_sun_loc_site WHERE lt::DATE='SOMEDATE' AND siteid=SITEID AND events IS NULL)) ORDER BY id;POSTAMBLE;;";
    //From the above sqlTemplateNOON[], note how the SQL comments need to be bracketed between `\n` characters to delineate the \
    sql comment from the REAL SQL
    unsigned long sizeOfTemplateNOON = sizeof(sqlTemplateNOON); //Get the number of characters, including the string-terminating '\0' character.
    const char *ptrSQLTemplateNOON = &sqlTemplateNOON[0];
    //Define a 2-dimensional array of pointers to characters used to construct the above SQL for determining sun set
    const char **noonArray[8][2] = { \
        {&ptrThisPreamble,         &ptrWhatThisPreambleIs},  \
        {&ptrThisSiteid,           &ptrWhatThisSiteIdIs},      \
        {&ptrThisSomeDate,         &ptrWhatThisSomeDateIs},  \
        {&ptrThisField,            &ptrWhatThisFieldIs}, \
        {&ptrThisBeforeDataPoints, &ptrWhatThisBeforeDataPointsIs},  \
        {&ptrThisAfterDataPoints,  &ptrWhatThisAfterDataPointsIs},     \
        {&ptrThisPostamble,        &ptrWhatThisPostambleIs},  \
        {&ptrThisTerm,             &ptrWhatThisTermIs}};
    
    const char *sqlTemplateSR = "PREAMBLE\nSELECT * FROM tbl_sun_loc_site \
    WHERE lt::DATE='SOMEDATE' AND siteid=SITEID AND id BETWEEN \n \
    -BEFOREDATAPOINTS + ( SELECT id FROM (SELECT *,abs(FIELD-ZDDEF) \
    FROM tbl_sun_loc_site WHERE lt::DATE='SOMEDATE' AND siteid=SITEID AND events IS NULL AND id SRorSS (SELECT avg(id) FROM tbl_sun_loc_site \
    WHERE lt::DATE='SOMEDATE' AND siteid=SITEID AND events IS NULL)) a WHERE abs(FIELD - ZDDEF) = \
    (SELECT min(abs(FIELD - ZDDEF)) FROM tbl_sun_loc_site WHERE lt::DATE='SOMEDATE' AND siteid=SITEID AND events IS NULL AND id SRorSS \
    (SELECT avg(id) FROM tbl_sun_loc_site where lt::DATE='SOMEDATE' AND siteid=SITEID AND events IS NULL))  ) \n \
    AND \n \
    +AFTERDATAPOINTS + ( SELECT id FROM (SELECT *,abs(FIELD - ZDDEF) \
    FROM tbl_sun_loc_site WHERE lt::DATE='SOMEDATE' AND siteid=SITEID AND events IS NULL AND id SRorSS (SELECT avg(id) FROM tbl_sun_loc_site \
    WHERE lt::DATE='SOMEDATE' AND siteid=SITEID AND events IS NULL)) a WHERE abs(FIELD - ZDDEF) = \
    (SELECT min(abs(FIELD - ZDDEF)) FROM tbl_sun_loc_site WHERE lt::DATE='SOMEDATE' AND siteid=SITEID AND events IS NULL AND id SRorSS \
    (SELECT avg(id) FROM tbl_sun_loc_site WHERE lt::DATE='SOMEDATE' AND siteid=SITEID AND events IS NULL))   ) \
    ORDER BY id ASC;POSTAMBLE;;";
    unsigned long sizeOfTemplateSR = sizeof(*sqlTemplateSR); //Get the number of characters, including the string-terminating '\0' character.
    const char *ptrSQLTemplateSR = sqlTemplateSR;
    const char **sunRiseArray[10][2] = {  /* The order of the targets of thse pointers MUST be in the same order as appearying \
                                           here!! THIS IS CRITICAL!!!!!! I wasted days tracking down this problem!!!!!!!!!!! ptrThisZDDEFef shall appear first, followed by \
                                           ptrWhatThisSunRiseDefIs, followed by ptrThisPreamble, ... ending with ptrWhatThisTermIs. Failure to observe this ordering will \
                                           result in BAD THINGS HAPPENING: like encountering TERM, which ends fabricateSQLr3.dylib processing, prior to having processed \
                                           all Token/Replacement pairs. */ \
        {&ptrThisDef, &ptrWhatThisDefIs}, \
        {&ptrThisPreamble,&ptrWhatThisPreambleIs}, \
        {&ptrThisSiteid,&ptrWhatThisSiteIdIs}, \
        {&ptrThisSomeDate,&ptrWhatThisSomeDateIs}, \
        {&ptrThisBeforeDataPoints, &ptrWhatThisBeforeDataPointsIs}, \
        {&ptrThisAfterDataPoints,&ptrWhatThisAfterDataPointsIs}, \
        {&ptrThisEvent, &ptrWhatThisEventIs}, \
        {&ptrThisField,&ptrWhatThisFieldIs}, \
        {&ptrThisPostamble, &ptrWhatThisPostambleIs}, \
        {&ptrThisTerm, &ptrWhatThisTermIs} \
    }; //
    int sizeOf_sunRiseArray = sizeof(sunRiseArray);
    const char ***sra = &sunRiseArray[0][0];
    int   numberOfPairs[3] = {10, 8, 10};
    const char  ***fabricate2DArrays[4] = { \
        &sunRiseArray[0][0], \
        &noonArray[0][0], \
        &sunSetArray[0][0], \
        0
    }; //Used by test program to loop through the three, 2-dimensional, arrays that will be handed-off to fabricateSQLr3.dylib \
    for testing this dylib.
    
    //fabstruct ptr_fs = fs[0];
    
    const char ***threeArrays[3] = {&sunRiseArray[0][0],  &noonArray[0][0], &sunSetArray[0][0]};
    arrayEntry anotherSunRiseArray[10] = { \
        {"PREAMBLE",         &ptrWhatThisPreambleIs}, \
        {"ZDDEF",            &ptrWhatThisDefIs}, \
        {"SITEID",           &ptrWhatThisSiteIdIs}, \
        {"DATE",             &ptrWhatThisSomeDateIs}, \
        {"BEFOREDATAPOINTS", &ptrWhatThisBeforeDataPointsIs}, \
        {"AFTERDATAPOINTS",  &ptrWhatThisAfterDataPointsIs}, \
        {"SRorSS",           &ptrWhatThisEventIs}, \
        {"FIELD",            &ptrWhatThisFieldIs}, \
        {"POSTAMBLE",        &ptrWhatThisPostambleIs}, \
        {"TERM",             &ptrWhatThisTermIs}};
    arrayEntry *ptr_anotherSunRiseArray = &anotherSunRiseArray[0];
    int sizeOfDbc = sizeof(Dbc);   // Dbc is defined in include file:  /usr/local/include/databaseOps/databaseOps.h
    int modulo;
    int columnIndexNumber;
    const int modulo_lines_perPage = 47;
    int numberOfColumns=2;
    int numberOfRows;
    unsigned int numberOfArrayRows;
    // char *cbb_results;
//    Note that sqlTemplateERA0, sqlTemplateGAST0, and sqlTemplateLAST0 are all the same.
//    Note too, that sunTemplateERA0, sumTemplateGAST0, and sunTemplateLAST0 are all the same.
//    PERHAPS we can get away with just one to serve all three: 
//    sqlTemplateERA0_GAST0_LAST0, and another like,
//    sunTemplateERA0_GAST0_LAST0
    
// 4SEASONS    4SEASONS    4SEASONS    4SEASONS    4SEASONS    4SEASONS    4SEASONS    4SEASONS    4SEASONS    4SEASONS    4SEASONS \
    The following worked in fabricateSQL3r3 on 2022-06-11
    const char *ptrEndCap;
    //   const char *ptrThisSomeYear = "YEAR";
    //    const char *ptrWhatThisSomeYearIs ="date_part('Year', now())"; //Defaults to current year.
    // NOT USED    const char *ptrThisSeason = "SEASON";
    const char *sqlTemplate4Seasons = "PREAMBLE SELECT *, date_part('Hour', lt) FROM tbl_sun_loc_site WHERE id \
    BETWEEN \
    - BEFOREDATAPOINTS + \
    (SELECT id FROM tbl_sun_loc_site WHERE events IS NULL AND siteid = SITEID AND abs(FIELD - ZDDEF) = \
    (SELECT  min(abs(FIELD - ZDDEF))  FROM tbl_sun_loc_site WHERE events IS NULL AND siteid=SITEID AND \
    date_part('Year',lt) = YEAR ) AND events IS NULL) \
    AND \
    + AFTERDATAPOINTS + \
    (SELECT id FROM tbl_sun_loc_site WHERE events is NULL AND siteid = SITEID AND abs(FIELD - ZDDEF) = \
    (SELECT  min(abs(FIELD - ZDDEF))  FROM tbl_sun_loc_site WHERE events IS NULL AND siteid=SITEID AND \
    date_part('Year',lt) = YEAR ) AND events IS NULL) \
    AND events IS NULL ORDER BY jdlocal ASC;POSTAMBLE";
    
    const char **sunTemplate4Seasons[9][2] = {                       /* 9 rows of 2 columns per row */   \
        {&ptrThisDef,              &ptrWhatThisGeocentricraSeasonIs},  /* [0][0] [0][1] */   \
        {&ptrThisPreamble,         &ptrWhatThisPreambleIs},            /* [1][0] [1][1] */   \
        {&ptrThisSiteid,           &ptrWhatThisSiteIdIs},              /* [2][0] [2][1] */   \
        {&ptrThisSomeYear,         &ptrWhatThisSomeYearIs},            /* [2][0] [2][1] */   \
        {&ptrThisBeforeDataPoints, &ptrWhatThisBeforeDataPointsIs},    /* [4][0] [4][1] */   \
        {&ptrThisAfterDataPoints,  &ptrWhatThisAfterDataPointsIs},     /* [5][0] [5][1] */   \
        {&ptrThisField,            &ptrWhatThisFieldIs},               /* [6][0] [6][1] */   \
        {&ptrThisPostamble,        &ptrWhatThisPostambleIs},           /* [7][0] [7][1] */   \
        {&ptrThisTerm,             &ptrWhatThisTermIs}};               /* [8][0] [8][1] */
    // 4SEASONS    4SEASONS    4SEASONS    4SEASONS    4SEASONS    4SEASONS    4SEASONS    4SEASONS    4SEASONS    4SEASONS    4SEASONS
    
    
    
//  ERA    ERA     ERA     ERA     ERA     ERA     ERA     ERA     ERA     ERA     ERA     ERA     ERA     ERA     ERA     ERA
    const char *sqlTemplateERA0 ="PREAMBLE SELECT * from tbl_sun_loc_site where events IS NULL AND id BETWEEN -BEFOREDATAPOINTS + \
    ( SELECT id  FROM tbl_sun_loc_site  WHERE FIELD = (SELECT min(FIELD)  FROM tbl_sun_loc_site WHERE lt::DATE='SOMEDATE' AND \
    siteid=SITEID AND events IS NULL) AND siteid=SITEID)  AND +AFTERDATAPOINTS + ( SELECT id  FROM tbl_sun_loc_site  WHERE FIELD = \
    (SELECT min(FIELD) FROM tbl_sun_loc_site WHERE lt::DATE='SOMEDATE' AND siteid=SITEID AND events IS NULL) AND siteid = SITEID) \
    ORDER BY jdlocal ;POSTAMBLE";
    const char **sunTemplateERA0[8][2] = {                          \
        {&ptrThisPreamble,         &ptrWhatThisPreambleIs},        \
        {&ptrThisSiteid,           &ptrWhatThisSiteIdIs},           \
        {&ptrThisSomeDate,         &ptrWhatThisSomeDateIs},         \
        {&ptrThisBeforeDataPoints, &ptrWhatThisBeforeDataPointsIs}, \
        {&ptrThisAfterDataPoints,  &ptrWhatThisAfterDataPointsIs},  \
        {&ptrThisField,            &ptrWhatThisFieldIs},            \
        {&ptrThisPostamble,        &ptrWhatThisPostambleIs},        \
        {&ptrThisTerm,             &ptrWhatThisTermIs}};
//  ERA    ERA     ERA     ERA     ERA     ERA     ERA     ERA     ERA     ERA     ERA     ERA     ERA     ERA     ERA     ERA
// ----    ----    ----    ----    ----    ----    ----    ----    ----    ----    ----    ----    ----    ----    ----    ----
// GAST    GAST    GAST    GAST    GAST    GAST    GAST    GAST    GAST    GAST    GAST    GAST    GAST    GAST    GAST    GAST
    const char *ptr1DTemplateGAST0 ="PREAMBLE SELECT * from tbl_sun_loc_site where events IS NULL AND id BETWEEN -BEFOREDATAPOINTS + \
    ( SELECT id  FROM tbl_sun_loc_site  WHERE FIELD = (SELECT min(FIELD)  FROM tbl_sun_loc_site WHERE lt::DATE='SOMEDATE' AND \
    siteid=SITEID AND events IS NULL) AND siteid=SITEID)  AND +AFTERDATAPOINTS + ( SELECT id  FROM tbl_sun_loc_site  WHERE FIELD = \
    (SELECT min(FIELD) FROM tbl_sun_loc_site WHERE lt::DATE='SOMEDATE' AND siteid=SITEID AND events IS NULL) AND siteid = SITEID) \
    ORDER BY jdlocal ;POSTAMBLE";
    //Define a 2-dimensional array of pointers to characters used to construct the above SQL for determining gast
    const char **ptr2DTemplateGAST0[8][2] = {     \
        {&ptrThisPreamble,         &ptrWhatThisPreambleIs},    \
        {&ptrThisSiteid,           &ptrWhatThisSiteIdIs},       \
        {&ptrThisSomeDate,         &ptrWhatThisSomeDateIs},   \
        {&ptrThisBeforeDataPoints, &ptrWhatThisBeforeDataPointsIs}, \
        {&ptrThisAfterDataPoints,  &ptrWhatThisAfterDataPointsIs},  \
        {&ptrThisField,            &ptrWhatThisFieldIs}, \
        {&ptrThisPostamble,        &ptrWhatThisPostambleIs}, \
        {&ptrThisTerm,             &ptrWhatThisTermIs}}; 
// GAST    GAST    GAST    GAST    GAST    GAST    GAST    GAST    GAST    GAST    GAST    GAST    GAST    GAST    GAST    GAST
// ----    ----    ----    ----    ----    ----    ----    ----    ----    ----    ----    ----    ----    ----    ----    ----
// LAST    LAST    LAST    LAST    LAST    LAST    LAST    LAST    LAST    LAST    LAST    LAST    LAST    LAST    LAST    LAST
    const char *sqlTemplateLAST0 ="PREAMBLE SELECT * from tbl_sun_loc_site where events IS NULL AND id BETWEEN -BEFOREDATAPOINTS + \
    ( SELECT id  FROM tbl_sun_loc_site  WHERE FIELD = (SELECT min(FIELD)  FROM tbl_sun_loc_site WHERE lt::DATE='SOMEDATE' AND \
    siteid=SITEID AND events IS NULL) AND siteid=SITEID)  AND +AFTERDATAPOINTS + ( SELECT id  FROM tbl_sun_loc_site  WHERE FIELD = \
    (SELECT min(FIELD) FROM tbl_sun_loc_site WHERE lt::DATE='SOMEDATE' AND siteid=SITEID AND events IS NULL) AND siteid = SITEID) \
    ORDER BY jdlocal ;POSTAMBLE";
    //Define a 2-dimensional array of pointers to characters used to construct the above SQL for determining gast
    const char **sunTemplateLAST0[8][2] = {     \
        {&ptrThisPreamble,         &ptrWhatThisPreambleIs},     \
        {&ptrThisSiteid,           &ptrWhatThisSiteIdIs},       \
        {&ptrThisSomeDate,         &ptrWhatThisSomeDateIs},   \
        {&ptrThisBeforeDataPoints, &ptrWhatThisBeforeDataPointsIs}, \
        {&ptrThisAfterDataPoints,  &ptrWhatThisAfterDataPointsIs},  \
        {&ptrThisField,            &ptrWhatThisFieldIs}, \
        {&ptrThisPostamble,        &ptrWhatThisPostambleIs}, \
        {&ptrThisTerm,             &ptrWhatThisTermIs}};
// LAST    LAST    LAST    LAST    LAST    LAST    LAST    LAST    LAST    LAST    LAST    LAST    LAST    LAST    LAST    LAST
// 4SEASONS    4SEASONS    4SEASONS    4SEASONS    4SEASONS    4SEASONS    4SEASONS    4SEASONS    4SEASONS    4SEASONS    4SEASONS

    const char *ptrThisSomeYear = "YEAR";
    const char *ptrWhatThisSomeYearIs ="date_part('Year', now())"; //Defaults to current year.
    const char *ptrThisSeason = "SEASON";
    const char *ptrWhatThisSeasonIs = "?"; // "0" => Spring, "6" => Summer, "12" => Autumn/Fall, "18" => Winter
    //2022-06-08T12:55:29 this version worked when plugged into psql  ***COMMENTED OUT THIS DUPLICATE ON 2022-06-11T10:04:12***  \
    const char *sqlTemplate4Seasons = "PREAMBLE SELECT *, date_part('Hour', lt) FROM tbl_sun_loc_site WHERE id \
    BETWEEN \
    - BEFOREDATAPOINTS + \
    (SELECT id FROM tbl_sun_loc_site WHERE events IS NULL AND siteid = SITEID AND abs(FIELD - SEASON) = \
    (SELECT  min(abs(FIELD - SEASON))  FROM tbl_sun_loc_site WHERE events IS NULL AND siteid=SITEID AND \
    date_part('Year',lt) = YEAR ) AND events IS NULL) \
    AND \
    + AFTERDATAPOINTS + \
    (SELECT id FROM tbl_sun_loc_site WHERE events is NULL AND siteid = SITEID AND abs(FIELD - SEASON) = \
    (SELECT  min(abs(FIELD - SEASON))  FROM tbl_sun_loc_site WHERE events IS NULL AND siteid=SITEID AND \
    date_part('Year',lt) = YEAR ) AND events IS NULL) \
    AND events IS NULL ORDER BY jdlocal ASC;POSTAMBLE";
//2022-06-08T12:53:24 What follows is the previous version of sqlTemplate4Seasons, modified to sqlTemplate_4_Seasons not to \
    conflict with existing code in otherFunctions.cpp. This can be eliminate when we're finished testing \
    const char *sqlTemplate_4_Seasons = "PREAMBLE SELECT * FROM tbl_sun_loc_site WHERE id \
    BETWEEN \
    - BEFOREDATAPOINTS + \
    (SELECT id FROM tbl_sun_loc_site WHERE events IS NULL AND siteid = SITEID  AND \
        FIELD - SEASON = (SELECT  min(abs(FIELD-SEASON))  FROM tbl_sun_loc_site WHERE events IS NULL AND siteid=SITEID AND \
        date_part('Year',lt)=YEAR) AND events IS NULL) \
    AND \
    + AFTERDATAPOINTS + \
    (SELECT id FROM tbl_sun_loc_site WHERE events is NULL AND siteid = SITEID  AND \
        FIELD - SEASON = (SELECT  min(abs(FIELD-SEASON))  FROM tbl_sun_loc_site WHERE events IS NULL AND siteid=SITEID AND \
        date_part('Year',lt) = YEAR) AND events IS NULL) \
    AND events IS NULL ORDER BY jdlocal;POSTAMBLE"; \
    const char **sunTemplate4SEASONS[9][2] = {     \
        {&ptrThisPreamble,         &ptrWhatThisPreambleIs},     \
        {&ptrThisSeason,           &ptrWhatThisSeasonIs},  \
        {&ptrThisSiteid,           &ptrWhatThisSiteIdIs},       \
        {&ptrThisSomeYear,         &ptrWhatThisSomeYearIs},   \
        {&ptrThisBeforeDataPoints, &ptrWhatThisBeforeDataPointsIs}, \
        {&ptrThisAfterDataPoints,  &ptrWhatThisAfterDataPointsIs},  \
        {&ptrThisField,            &ptrWhatThisFieldIs}, \
        {&ptrThisPostamble,        &ptrWhatThisPostambleIs}, \
        {&ptrThisTerm,             &ptrWhatThisTermIs}};
// 4SEASONS    4SEASONS    4SEASONS    4SEASONS    4SEASONS    4SEASONS    4SEASONS    4SEASONS    4SEASONS    4SEASONS    4SEASONS
//  SUNRISE    SUNRISE    SUNRISE    SUNRISE    SUNRISE    SUNRISE    SUNRISE    SUNRISE    SUNRISE    SUNRISE    SUNRISE    SUNRISE
    const char *sqlTemplateSunRise = "PREAMBLE \
    SELECT *, date_part('Hour', lt) FROM tbl_sun_loc_site WHERE id \
    BETWEEN \
    - :BEFOREDATAPOINTS + \
    (SELECT id FROM tbl_sun_loc_site WHERE events IS NULL AND siteid = :SITEID  AND \
    abs(:FIELD - :SEASON) = (SELECT  min(abs(:FIELD-:SEASON))  FROM tbl_sun_loc_site WHERE events IS NULL AND siteid=:SITEID AND \
    lt::DATE = :'DATE' AND date_part('Hour', lt) < 12) AND events IS NULL) \
    AND \
    + :AFTERDATAPOINTS + \
    (SELECT id FROM tbl_sun_loc_site WHERE events is NULL AND siteid = :SITEID  AND abs(:FIELD -:SEASON) = \
    (SELECT  min(abs(:FIELD-:SEASON))  FROM tbl_sun_loc_site WHERE events IS NULL AND siteid=:SITEID AND lt::DATE = :'DATE' \
    AND date_part('Hour', lt)  < 12) AND events IS NULL) \
    AND events IS NULL ORDER BY jdlocal ASC;POSTAMBLE";
    const char **sunTemplateSunRise[9][2] = {     \
        {&ptrThisPreamble,         &ptrWhatThisPreambleIs},     \
        {&ptrThisSiteid,           &ptrWhatThisSiteIdIs},       \
        {&ptrThisBeforeDataPoints, &ptrWhatThisBeforeDataPointsIs}, \
        {&ptrThisAfterDataPoints,  &ptrWhatThisAfterDataPointsIs},  \
        {&ptrThisField,            &ptrWhatThisFieldIs}, /* Should be zenithdistance  */ \
        {&ptrThisSeason,           &ptrWhatThisSeasonIs}, /* Should be 90.5833…333 which is the definition of sunrise  */ \
        {&ptrThisPostamble,        &ptrWhatThisPostambleIs}, \
        {&ptrThisTerm,             &ptrWhatThisTermIs}};
//  SUNRISE    SUNRISE    SUNRISE    SUNRISE    SUNRISE    SUNRISE    SUNRISE    SUNRISE    SUNRISE    SUNRISE    SUNRISE    SUNRISE
//  SUNSET     SUNSET     SUNSET     SUNSET     SUNSET     SUNSET     SUNSET     SUNSET     SUNSET     SUNSET     SUNSET     SUNSET
    const char *sqlTemplateSunSet = "PREAMBLE \
    SELECT *, date_part('Hour', lt) FROM tbl_sun_loc_site WHERE id \
    BETWEEN \
    - :BEFOREDATAPOINTS + \
    (SELECT id FROM tbl_sun_loc_site WHERE events IS NULL AND siteid = :SITEID  AND abs(:FIELD - :SEASON) = \
    (SELECT  min(abs(:FIELD-:SEASON))  FROM tbl_sun_loc_site WHERE events IS NULL AND siteid=:SITEID AND \
    lt::DATE = :'DATE' AND date_part('Hour', lt) > 12) AND events IS NULL) \
    AND \
    + :AFTERDATAPOINTS + \
    (SELECT id FROM tbl_sun_loc_site WHERE events is NULL AND siteid = :SITEID  AND abs(:FIELD -:SEASON) = \
    (SELECT  min(abs(:FIELD-:SEASON))  FROM tbl_sun_loc_site WHERE events IS NULL AND siteid=:SITEID AND \
    lt::DATE = :'DATE' AND date_part('Hour', lt) > 12) AND events IS NULL) \
    AND events IS NULL ORDER BY jdlocal ASC;POSTAMBLE";
    const char **sunTemplateSunSet[9][2] = {     \
        {&ptrThisPreamble,         &ptrWhatThisPreambleIs},     \
        {&ptrThisSiteid,           &ptrWhatThisSiteIdIs},       \
        {&ptrThisBeforeDataPoints, &ptrWhatThisBeforeDataPointsIs}, \
        {&ptrThisAfterDataPoints,  &ptrWhatThisAfterDataPointsIs},  \
        {&ptrThisField,            &ptrWhatThisFieldIs}, /* Should be zenithdistance  */ \
        {&ptrThisSeason,           &ptrWhatThisSeasonIs}, /* Should be 90.5833…333 which is the definition of sunset   */ \
        {&ptrThisPostamble,        &ptrWhatThisPostambleIs}, \
        {&ptrThisTerm,             &ptrWhatThisTermIs}};
//  SUNSET     SUNSET     SUNSET     SUNSET     SUNSET     SUNSET     SUNSET     SUNSET     SUNSET     SUNSET     SUNSET     SUNSET
    // ******************************************************************************************************************************* \
    Note: fabricateSQL, the dylib under test, is found at: \
    /Users/cjc/Library/Developer/Xcode/DerivedData/Build/Products/Debug/libfabricateSQL.dylib \
    This program can load the above dynamic library (dylib) which this program tests, then before compiling: \
    a) place `/Users/cjc/Library/Developer/Xcode/DerivedData/Build/Products/Debug/libfabricateSQL.dylib` in \
    "Dynamic Library Install Name"; \
    b) Place `-lfabricateSQL` in "Other Linker Flags". \
    *******************************************************************************************************************************
/*  It looks like we don't need this getopt data structures any more since we have our own way of processing command line switches
 in a more c++ manner.
          LONG NAME       ARGUMENT REQUIREMENT FLAG  SHORTNAME
    const char *theArguments = "S:D:b:a:F:E:r:s:12345hnf:P:o:e:M:p:H:d:U:";
    struct option long_options[25] = {
        {"siteid",             required_argument, NULL, 'S'},
        {"somedate",           required_argument, NULL, 'D'},
        {"beforedatapoints",   required_argument, NULL, 'b'},
        {"afterdatapoints",    required_argument, NULL, 'a'},
        {"field",              required_argument, NULL, 'F'},
        {"event",              required_argument, NULL, 'E'},
        {"eventValue",         required_argument, NULL, 'e'},
        {"sunrisedefinition",  required_argument, NULL, 'r'},
        {"db1",                no_argument,       NULL, '1'},
        {"db2",                no_argument,       NULL, '2'},
        {"db3",                no_argument,       NULL, '3'},
        {"db4",                no_argument,       NULL, '4'},
        {"db5",                no_argument,       NULL, '5'},
        {"modulo",             optional_argument, NULL, 'm'},
        {"preamble",           optional_argument, NULL, 'P'},
        {"postamble",          optional_argument, NULL, 'o'},
        {"maxprecision",       optional_argument, NULL, 'M'},
        {"noSQL",              no_argument,       NULL, 'n'},
        {"file",               optional_argument, NULL, 'f'},
        {"port",               optional_argument, NULL, 'p'},
        {"host",               optional_argument, NULL, 'H'},
        {"database",           optional_argument, NULL, 'd'},
        {"username",           optional_argument, NULL, 'U'},
        {"help",               no_argument,       NULL, 'h'},
        {NULL,                 0,                 NULL,   0}
    };
 */
    // field index numbers of LocalWeather database's table named tbl_sun_loc_site:
 
    int oport = -1;
    int ohost = -2;
    int odatabase = -3;
    int ousername = -4;
    
    int option_index = 0;
    int eventAsIndexNumber;

    enum indicies { sunrise=0, sunset = 1, noonmin =2, spring = 3, summer = 4, autumn = 5, winter = 6, era = 7, gast = era + 1, \
        last = gast + 1   };
    multimap<string, int> dict  {                  // A dictionary of the 9 events with each event having a variety of spellings.
        {"sr",      sunrise},
        {"ss",      sunset},
        {"noonmin", noonmin},
        {"spring",  spring},
        {"summer",  summer},
        {"autumn",  autumn},
        {"winter",  winter},
        {"era",     era},
        {"fall",    autumn},
        {"sunrise", sunrise},
        {"sunset",  sunset},
        {"noon",    noonmin},
        {"n",       noonmin},
        {"last",    last},
        {"gast",    gast},
        {"g",       gast},
        {"l",       last},
        {"e",       era},
        {"w",       winter},
        {"f",       autumn},
        {"g",       spring},
        {"s",       summer},
        {"a",       autumn}
    };
    //These #defines reflect the order in which the particular entry appears in the array of type eP.
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
#define FALL    AUTUMN
    enum entryOrder { _sunrise=SUNRISE, _sunset=SUNSET, _noonmin=NOONMIN, _spring=SPRING, _summer=SUMMER,
    _autumn=AUTUMN, _winter=WINTER, _era=ERA, _gast=GAST, _last=LAST} entryOrder;
    typedef struct eP { // Map of myEvents array of pertinant processing parameters
//Used in Constants::Constants Constructor

        string eventName;    //In common lower case                                                0
        unsigned int modulous;     // if zero then assume no modulous                                    1
        double eventDefinition; //Event definition                                                 2
//                                                  0000000000111111
//                                                  0123456789012345
#define HASMODULOUS                               0b0000000000000001
#define HAS_EVENT_DEFINITION                      0b0000000000000010
#define FIELD_ZENITHDISTANCE                      0b0000000010000000
#define FIELD_ZENITHDISTANCE_MINIMUM              0b0000000001000000
#define FIELD_LAST                                0b0000000000100000
#define SEASON_SPRING_GEOCENTRICRA                0b0100000000000000
#define ROLLOVER_DAILY                            0b0010000000000000
#define ROLLOVER_YEARLY                           0b0001000000000000
        uint16_t flags;                                                 //                          3
        uint8_t fieldID;                                               //                          4
        uint8_t  index;                                                 //                          5
        ptf      processingFunction;  //we really can't define pointers-to-function at compile time because the addresses \
        where these functions reside in memory have NOT yet been established at compile time, however we can supply an index \
        number, based upon the enum named tbl_sun_loc_site_field_id, above. Pointers to function need to be defined by the \
        constructor.
    } eP;
    enum tbl_sun_loc_site_field_id {id_ = 0, lt_ = id_ + 1, topocentricra_ = lt_ + 1, topocentricdec_ = 1 + topocentricra_, \
        topocentricdis_ = 1 + topocentricdec_, azimuth_ = 1 + topocentricdec_, zenithdistance_ = 1 + topocentricdec_, \
        geocentricra_ = 1 + zenithdistance_, geocentricdec_ = 1 + geocentricra_, geocentricdis_ = 1 + geocentricdec_, \
        jdutc_ = 1 + geocentricdis_, jdlocal_ = 1 + jdutc_, siteid_ = 1 + jdlocal_, era_ = 1 + jdlocal_, last_ = 1 + era_, \
        gast_ = 1 + last_, vcelx_= 1 + gast_, vcely_ = 1 + vcelx_, vcelz_ = 1 + vcely_, events_ = 1 + vcelz_} tbl_sun_loc_site_field_id;
//These events are in the order specified by enum indicies, above.
    string events_lc[1+last] = {"sunrise", "sunset", "noonmin", "spring", "summer", "autumn", "winter", "era", "gast", "last" };
    eP myEvents[10] = {    /* Use the events_lc parameter to search pick-out the desired entry. We set the sixth, right-most,
                            parameter to nullptr because at the time of compilation the address of the processing function is not
                            yet known.
            0       1           2                  3                                                  4           5          6        */
    {"sunrise", 0, atof(ZENITHDISTANCE_SR),HAS_EVENT_DEFINITION,                               zenithdistance_,_sunrise,  nullptr}, /*0*/ \
    {"sunsets", 0, atof(ZENITHDISTANCE_SS),HAS_EVENT_DEFINITION,                               zenithdistance_,_sunset,  nullptr}, /*1*/ \
    {"noonmin", 0,  0,                FIELD_ZENITHDISTANCE_MINIMUM,                       zenithdistance_,     _noonmin,  nullptr}, /*2*/ \
    {"spring",  24, 0,                ROLLOVER_YEARLY + HASMODULOUS +  HAS_EVENT_DEFINITION,  geocentricra_,   _spring,  nullptr}, /*3*/ \
    {"summer",  24, 6,                ROLLOVER_YEARLY + HASMODULOUS +  HAS_EVENT_DEFINITION,  geocentricra_,   _summer,  nullptr}, /*4*/ \
    {"autumn",  24, 12,               ROLLOVER_YEARLY + HASMODULOUS +  HAS_EVENT_DEFINITION,  geocentricra_,   _autumn,  nullptr}, /*5*/ \
    {"winter",  24, 18,               ROLLOVER_YEARLY + HASMODULOUS +  HAS_EVENT_DEFINITION,geocentricra_,     _winter,  nullptr}, /*6*/ \
    {"era",     360, 0,               HASMODULOUS +  HAS_EVENT_DEFINITION,                    era_,            _era,  nullptr}, /*7*/ \
    {"gast",    24,  0,               ROLLOVER_DAILY + HASMODULOUS +  HAS_EVENT_DEFINITION,   gast_,           _gast,  nullptr}, /*8*/ \
    {"last",    24,  0,               ROLLOVER_DAILY + HASMODULOUS +  HAS_EVENT_DEFINITION,   last_,           _last,  nullptr } /*9*/ \
    };
    eP *ptr_eP = myEvents;
    int size_of_pg_result;
    size_t position;
    string test = "ToLoweR";
    string testLower;
    char *ptrWorkChar;
    double workDouble;
    int workInt;
    char lastCharacter;
    char *work0;
    char *work1;
    const string insertIntoStart = "INSERT INTO tbl_sun_loc_site (lt, topocentricra, topocentricdec, topocentricdis, azimuth, \
    zenithdistance, geocentricra, geocentricdec, geocentricdis, jdutc, jdlocal, siteid, era, last, gast, vcelx, vcely, vcelz, events) \
    VALUES('";
    const string insertIntoClose = "');";
    string insertIntoClose_non_const = "');";
    int lowerIndex=0;
    int upperIndex=0;
    char ** _Nullable some2Darray = nullptr;

    const char * _Nullable some1Darray;
    int some2DArraySize=0;
    int some1DArraySize=0;
    char * _Nullable lastSemiColon=nullptr;
 //   PGconn *conn;
 //   PGresult *res;
    string buf; // NOT a c-string buffer!!
    string workbuf[MAX_COLUMNS_IN_tbl_sun_loc_site];  //NOTE WELL the #define MAX_COLUMNS_IN_tbl_sun_loc_site
    double resultJDLOCAL;
    double ongoingResult;
//    bool reverseSort = false;  //Set to true whenever we do processSR
    myParameters * _Nullable ptrTemp ;
    myParameters *ptrDependentUpper;
    myParameters *ptrDependentLower;
    myParameters *ptrIndependentUpper;
    myParameters *ptrIndependentLower;
    myParameters *bhigher;
    myParameters *blower;
    int numberOfLoops = 0;
    int numberOfEntries = 0;
    int numberOf_BS_Entries = 0;
    int * _Nullable bs_rc=0;
    double a;  //a is used in bracket_search
    double b;  //b is used in  bracket_search
    double delta;
    const char * _Nullable  ptrToGregorian;
    const char *iI = &INSERT_INTO_BUFFER[0];
    bool executeTheSQL = true; // Default to execute the SQL this astroParameter executable generates.
    const char *ptrConnectionString;
    int ptrBracketSearch;
    std::ostringstream connectionStringBuf;
    string intermediateConnectionString;
    gsl_function F;
    gsl_cheb_series *cs;
    ptf ptrToFunction[10][2];
    //Constructor and Destructor declarations.
    Constants(void);
    Constants(int, const char **);
    ~Constants(void);
 //   int bracket_search (const void *, const void *);
    void doFabricateConnectionString(void);
    void reallocateArrays(Constants &, bool doInterchange = false);
    void freeCallocdMemory(Constants& v); // Added 2022-06-18T09:47:58 as common processing for the various processXYZ functions
};
int bracket_search (const void *, const void *);

#endif /* extractKeySiteParameters_hpp */
#endif /* Constants_h */

