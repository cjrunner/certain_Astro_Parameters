//
//  Pclp.hpp
//  certain_Astro_Parameters
//
//  Created by Clifford Campo on 2/21/21.
//  Copyright © 2021 CliffordCampo. All rights reserved.
//
//
//  processCommandLineParameters.hpp
//  astroParameter
//
//  Created by Clifford Campo on 12/6/20.
//  Copyright © 2020 CliffordCampo. All rights reserved.
//
#ifndef Pclp_h
#define Pclp_h


#ifndef processCommandLineParameters_hpp
#define processCommandLineParameters_hpp
#include "helpMsg.hpp"
#include <map>
#define _SITEID_            0                                // 00
#define _SOMEDATE_          1                                // 01
#define _BEFOREDATAPOINTS_  2                                // 02
#define _AFTERDATAPOINTS_   3                                // 03
#define _FIELD_             4                                // 04
#define _EVENT_             5                                // 05
#define _EVENTVALUE_        6                                // 06
#define _SUNRISEDEFINITION_ 7                                // 07
#define _DB1_               8                                // 08
#define _DB2_               9                                // 09
#define _DB3_               10                               // 10
#define _DB4_               11                               // 11
#define _DB5_               12                               // 12
#define _MODULO_            13                               // 13
#define _PREAMBLE_          14                               // 14
#define _POSTAMBLE_         15                               // 15
#define _MAXPRECISION_      16                               // 16
#define _NOSQL_             17                               // 17
#define _FILE_              18                               // 18
#define _PORT_              19                               // 19
#define _HOST_              20                               // 20
#define _DATABASE_          21                               // 21
#define _USERNAME_          22                               // 22
#define _HELP_              23                               // 23
#define _SUNSETDEFINITION_  24                               // 24
#define _YEAR_              25                               // 25
#define _SEASON_            26                               // 26
#define EXITCODE_HELPMESSAGE 1
#define EXIT_ILLEGAL_EVENT_CHARACTER  -(1+20)
#define BAD_E_COMMAND_LINE_SWITCH     -(2+20)
#include "Constants.hpp"
using namespace std;
// now a member function int processCommandLineParamaters(Constants &, int, const char **);
//        +--------------------Derived Class
//        |      +-------------Indicates presence of class derivation list
//        |      |           +-Base Class
//        V      V           V
class   Pclp     : public Constants { 
    typedef int (*PTF)(Pclp&); //This is just a plain-old-pointer-to_function type definition (aka, typedef)
    typedef int (Pclp::*Ptfcl )(void); //A non-static member function of class Pclp (Pointer to member function whereas pointer to \
    function would be `typedef int (*Ptfcl )(void);`) Note that the const “decoration” will only point to member functions \
    decorated with const.
    typedef struct {
        int8_t thekeyID;
#define HASNOT 1
#define HAS    2
        int8_t hasAssociatedParameter; // 1 => no associated parameter; 2 => has associated parameter
        Ptfcl  ptpf; //Point to processing function (i.e., ptpf) of type Ptfcl
    } pt_;
    
private:
    const char *ptr_nameOfThisProgram;
    const char *ptrThisBeforeDataPoints;
    const char *ptrThisAfterDataPoints;
public:
    Ptfcl  ptr_DB2;
    PTF  arrayOfPointerToFunction[27];
    PTF  myptToFunction;
    char eventInLowerCase[16];
    char eventInLowerCaseBuffer[16];
    int i;
    int j;
    int k;
    int my_argc; //Contains the number of command line parameters, including the name of this executable.
    const char **my_argv; //Pointer to an array of pointers to character arrays. Thesse character arrays represent the command \
    line's kewords and the parameter, if any,of each command line keyword. Each keyword may have zero or 1 parameter.
    const int8_t CL_SITEID=0;
    int8_t CL_SOMEDATE=1;
    int8_t CL_BEFOREDATAPOINTS=2;
    int8_t CL_AFTERDATAPOINTS=3;
    
    int rc=0;
    
    string whatThisEventIsAsString;
    string whatThisEventIsAsString_lc;
    string str_eventInLowerCase; //An object which is a copy of the above, whatThisEventIsAsString_lc.
    int whatKindOfProcessing;
    char *ptr_lcEvent;
    double eventParameter;
    //Our own internal pointers to command line arguments, which are all of type c-string.
    char *ptrPortName;
    char *ptrHostName;
    char *ptrDatabaseName;
    const char *ptrWhatThisSomeYearIs__;
    const char *ptrWhatThisSomeDateIs__;             //Should point to base class' ptrWhatThisSomeDateIs
    const char *ptrWhatThisBeforeDataPointsIs__;     //Should point to base class' ptrWhatThisBeforeDataPointsIs
    const char *ptrWhatThisAfterDataPointsIs__;      //Should point to base class' ptrWhatThisAfterDataPointsIs
    char *outputFileName;
    bool executeTheSQL;
    int  DESIRED_MAX_PRECISION;
    const char *ptrWhatThisPostambleIs__;       //Should point to base class' ptrWhatThisPostambleIs
    const char *ptrWhatThisDefIs__;
    const char *ptrWhatThisPreambleIs__;        //Should point to base class' ptrWhatThisPreambleIs
    int  modulo_lines_perPage;
    const char *ptrWhatThisFieldIs__;           //Should point to base class' ptrWhatThisFieldIs
    char *ptr_eventInLowerCaseBuffer;
    const char *ptrWhatThisEventIs__;           //Should point to base class' ptrWhatThisEventIs
    const char *ptrWhatThisEventDefinitionIs__; //Should point to base class' ptrWhatThisEventDefinitionIs
    const char *ptrWhatThis_SR_DefIs__;         //Should point to base class' ptrWhatThis_SR_DefIs
    const char *ptrWhatThis_SS_DefIs__;         //Should point to base class' ptrWhatThis_SS_DefIs
    /////////////////////////////////////////////
    const char   * _Nullable  ptrWhatThisSiteIdIs__ ;        //Should point to base class' ptrWhatThisSiteIdIs
    multimap<string, int> commandLineArguments {
        {"--siteid", _SITEID_},
        {"-S", _SITEID_},
        {"--somedate", _SOMEDATE_},
        {"-D", _SOMEDATE_},
        {"--beforedatapoints", _BEFOREDATAPOINTS_},
        {"-b", _BEFOREDATAPOINTS_},
        {"--afterdatapoints", _AFTERDATAPOINTS_ },
        {"-a", _AFTERDATAPOINTS_ },
        {"--field", _FIELD_},
        {"-F", _FIELD_},
        {"--event", _EVENT_},
        {"-E", _EVENT_},
        {"--eventValue", _EVENTVALUE_},
        {"-e", _EVENTVALUE_},
        {"--sunrisedefinition", _SUNRISEDEFINITION_},
        {"-r", _SUNRISEDEFINITION_},
        {"--db1", _DB1_},
        {"-1", _DB1_},
        {"--db2", _DB2_},
        {"-2", _DB2_},
        {"--db3", _DB3_},
        {"-3", _DB3_},
        {"--db4", _DB4_},
        {"-4", _DB4_},
        {"--db5", _DB5_},
        {"-5", _DB5_},
        {"--modulo", _MODULO_},
        {"-m", _MODULO_},
        {"--preamble", _PREAMBLE_},
        {"-P", _PREAMBLE_},
        {"--postamble", _POSTAMBLE_},
        {"-o", _POSTAMBLE_},
        {"--maxprecision", _MAXPRECISION_},
        {"-M", _MAXPRECISION_},
        {"--noSQL", _NOSQL_},
        {"-n", _NOSQL_},
        {"--file", _FILE_},
        {"-f", _FILE_},
        {"--port", _PORT_},
        {"-p", _PORT_},
        {"--host", _HOST_},
        {"-H", _HOST_},
        {"-Y", _YEAR_},
        {"--year", _YEAR_},
        {"-s", _SEASON_},
        {"--season", _SEASON_},
        {"--database", _DATABASE_},
        {"-d", _DATABASE_},
        {"--username", _USERNAME_},
        {"-U", _USERNAME_},
        {"--help", _HELP_},
        {"-h", _HELP_},
        {"-?", _HELP_}
    };
    
    
    string theKeywordWeFound = "";
    int theAssociatedValue = 0;
    char work_c;
    /////////////////////////////////////////////
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
     */
    //    ptf* processHelp;
    pt_ clpt[27] =  { // At the present time (2021-02-10T08:51:45) there are NO spare slots
        {_SITEID_, HAS,           },  //00
        {_SOMEDATE_, HAS,         },  //01
        {_BEFOREDATAPOINTS_, HAS, },  //02
        {_AFTERDATAPOINTS_, HAS,  },  //03
        {_FIELD_, HAS,            },  //04
        {_EVENT_, HAS,            },  //05
        {_EVENTVALUE_, HAS,       },  //06
        {_SUNRISEDEFINITION_, HAS,},  //07
        {_DB1_, HASNOT,           },  //08
        {_DB2_, HASNOT,           },  //09
        {_DB3_, HASNOT,           },  //10
        {_DB4_, HASNOT,           },  //11
        {_DB5_, HASNOT,           },  //12
        {_MODULO_, HAS,           },  //13
        {_PREAMBLE_, HAS,         },  //14
        {_POSTAMBLE_, HAS,        },  //15
        {_MAXPRECISION_, HAS,     },  //16
        {_NOSQL_, HASNOT,         },  //17
        {_FILE_, HAS,             },  //18
        {_PORT_, HAS,             },  //19
        {_HOST_, HAS,             },  //20
        {_DATABASE_, HAS,         },  //21
        {_USERNAME_, HAS,         },  //22
        {_HELP_, HASNOT,          },  //23
        {_SUNSETDEFINITION_, HAS, },  //24
        {_YEAR_, HAS,             },  //25      Added 2022-05-28T09:48:56
        {_SEASON_, HAS,           }   //26      Added 2022-05-28T09:49:58
    };
    
    pt_ *ptr_processingTable = clpt;
    /////////////////////////////////////////////
    int process_CommandLineParamaters(Pclp&);
    //    Pclp(void); No longer used. If we declare this function then the linker goes looking for it.
    Pclp(int, const char **); //Constructor takes in the `int argc` and `const char **argv` command line definition paramters
    ~Pclp(void);
    int process_SiteID(void);
    int process_SomeDate(void );
    int process_BeforeDataPoints(void);
    int process_AfterDataPoints(void);
    int process_Field(void);
    int process_Event( void );
    int process_EventValue( void );
    int process_SunRiseDefinition(void);
    int process_DB1( void );
    int process_DB2( );
    int process_DB3( void );
    int process_DB4( void );
    int process_DB5( void );
    int process_Modulo(void);
    int process_Preamble(void);
    int process_Postamble( void );
    int process_MaxPrecision( void );
    int process_NoSQL( void );
    int process_File( void );
    int process_Port( void );
    int process_Host( void );
    int process_Database(void);
    int process_UserName(void);
    int process_Help(void);
    int process_SunSetDefinition(void);
    int process_Year(void );
    int process_Season(void );
    
    ////////////////////////////////////////////////////////////////////////
    
    void helpMsg(void);
};

extern int processSiteID(Pclp&);                               //00
extern int processSomeDate(Pclp&);                             //01
extern int processBeforeDataPoints(Pclp&);                     //02
extern int processAfterDataPoints(Pclp&);                      //03
extern int processField( Pclp&);                               //04
extern int processEvent( Pclp&);                               //05
extern int processEventValue( Pclp&);                          //06
extern int processSunRiseDefinition( Pclp&);                   //07
extern int processDB1( Pclp&);                                 //08
extern int processDB2( Pclp&);                                 //09
extern int processDB3( Pclp&);                                 //10
extern int processDB4( Pclp&);                                 //11
extern int processDB5( Pclp&);                                 //13
extern int processModulo( Pclp&);                              //14
extern int processPreamble( Pclp&);                            //15
extern int processPostamble( Pclp&);                           //16
extern int processMaxPrecision( Pclp&);                        //17
extern int processNoSQL( Pclp&);                               //18
extern int processFile( Pclp&);                                //19
extern int processPort( Pclp&);                                //20
extern int processHost( Pclp&);                                //21
extern int processDatabase( Pclp&);                            //22
extern int processUserName( Pclp&);                            //23
extern int processHelp( Pclp&);                                //24
extern int processSunSetDefinition( Pclp&);                    //25
extern int processYear(void );                                 //26
extern int processSeason(void );                               //27

#endif /* processCommandLineParameters_hpp */
#endif /* Pclp_h */
