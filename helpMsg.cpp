//
//  helpMsg.cpp
//  certain_Astro_Parameters
//
//  Created by Clifford Campo on 2/21/21.
//  Copyright © 2021 CliffordCampo. All rights reserved.
//
//
//  helpMsg.cpp
//  astroParameter
//  copied from:
//  Created by Clifford Campo on 11/23/20.
//  Copyright © 2020 CliffordCampo. All rights reserved.
//

#include "Pclp.hpp"
// P R O T O T Y P E    P R O T O T Y P E    P R O T O T Y P E    P R O T O T Y P E    P R O T O T Y P E    P R O T O T Y P E
//void fabricateSQL(int numberOfTokensEqualToNumberOfReplacements, /* number of tokens (and replacements) to process */ \
//                  char **tokens, /*  an array of pointers to characters forming the tokens */ \
//                  char **replacements, /* an array of pointers to characters forming the replacemnet strings */ \
//                  char *input_c_string_template_buffer, /* points to the buffer containing the template */ \
//                  char *resulting_c_StringBuffer, /* points to the buffer contining the resulting sql */ \
//                  int sizeOf_resulting_c_StringBuffer, /* size of the previous buffer */  \
//                  int debugFlag /* output debug messages when set to >0 [TRUE] */     );

void Pclp::helpMsg(void) {
    printf("The purpose of this program, named %s,\n", ptr_nameOfThisProgram);
    printf("is to drive the testing of the dylib named libfabricateSQLr3. The parameters driving this test are as follows:\n");
    printf("\n--siteid or -S: defines the siteid as defined in database LocalWeather, on any working port, \
               \ntable named tbl_site_names [Default: No Default]\n");
    printf("\n--somedate or -D: defines the date of interest. MANDATORY. Must be in YYYY-MM-DD format\n");
    printf ("\n--beforedatapoints or -b: defines the number of tuples (rows) prior to the data of interest [Default: No Default]\n");
    printf("\n--afterdatapoints or -a: defines the number of tuples (rows) aftr the data of interest [Default: No Default]\n");
    printf("\n--field or -F: Specifies the field name. Field names are as described by LocalWeather database's table, \
               \nnamed tbl_sun_loc_site. Most fields (table columns) are of double precision data type. [Default: No Default]\n");
    printf("\n--event or -E: Specifies the event for which fabricateSQLr3 will create SQL to extract the data from tbl_sun_loc_site \
           \nto characterize the event of interest. Typical events are SunRise (specified by -E SR), SunSet (specified by -E SS), \
           \nnoon (specified by -E NoonMin), or Earth Rotation Angle = 0 (Specified by -E ERA) \
           \nOther events for future implementation are: \
           \n\tgast = 0 (-E gast); \
           \n\tlast = 0 (-E last); \
           \n\tarrival of Spring (-E Spring); \
           \n\tarrival of Summer (-E Summer); \
           \n\tarrival of Fall or Autumn (-E Fall or -E Autumn), arrival of Winter (-E Winter)\n");
    printf("\n--eventValue or -e: Value which we require the --event/-E to assume\n");
    printf("\n--sunrisedefiningion or -r: specifies the zenithdistance when sunrise occurs. [Default: %sº]\n", ptrWhatThisDefIs );
    printf("\n--sunsetdefiningion or -t: specifies the zenithdistance when sunset occurs. [Default: %sº]. Most unusual to be \
           \ndifferent from sunrisedefinition\n", ptrWhatThisDefIs);
    printf("\n--db1 or -1: Debug mode 1: provides display of processing command line switches and associated parameters.\n" );
    printf("\n--db2 or -2: Debug mode 2: Debug the parameter substitution, from values obtained \
           \nfrom the command line, into the 2D arrays, known as sunSetArray, sunRiseArray, and noonArray, which contain the \
           \nToken/Replacement pairs driving the processing of the libfabricateSQLr3.dylib\n");
    printf("\n--db3 or -3: View development of parameters that are sent to the libfabricateSQLr3.dylib.\n");
    printf("\n--db4 or -4: debug fabricateSQL3.dylib itself so this dylib displays its intermediate results as its processing \
           \nproceeds\n");
    printf("\n--db5 or -5: display messages returned by destructors describing necessary clean-up activity. \n");
    printf("\n--modulo or -m: incases of multiple row output from SQL execution, specifies the number of rows output before column \
           \nheader/titles are output. Default value is %d. A value of 1 results in output of just the first \
           \nheader/title line. A value of 0 supresses all title/header output.\n", modulo_lines_perPage);
    printf("\n--preamble or -P: override the default preamble; default preamble looks like: “%s”\n", ptrWhatThisPreambleIs);
    printf("\n--postamble or -o: override the default postamble; default preamble looks like: “%s”\n", ptrWhatThisPostambleIs);
    printf("\n--maxprecision or -M: override the maximum precision for displaying type double parameters [default: 15 digits].\n");
    printf("\n--noSQL or -n: The presence of this flag on the command line indicates that we will NOT execute the SQL generated by \
           \nthis executable, named %s\n", ptr_nameOfThisProgram );
    printf("\n--port or -p: The port associated with this database [default=5436]\n");
    printf("\n--host or -H: The host assocated with this database [default=localhost]\n");
    printf("-database or -d: The name of this database [default=LocalWeather]\n");
    printf("\n--Year or -Y: Used in seasonal processing. Specifies the year in which we assess the date and time of the arrival \
           \nsummer, or fall or winter, or spring. Requires specifying the --season or -s parameter, too. For the --season or -s switch: \
           \n\tto specify spring, then -s 0;\n\tto specify summer, then -s 6; \n\tto specify fall/autumn, then -s 12; \
           \n\tto specity winter, then -s 18");
    printf("\n--season or -s: Takes on an integer value to specify the season. See --Year, above, for guidance on providing \
           \nappropriate paramter for --season or -s keywords. When specifying season, we assume the field parameter will be \
           \ngeocentricra, however by providing the --field, or -f,  parameter with the parameter of topocentricra, equally valid \
           \nseason arrival times can be determined, too." );
    printf("\n--username or -U: The name of the database's user [default=cjc]\n");
}
