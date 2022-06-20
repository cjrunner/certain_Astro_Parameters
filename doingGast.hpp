//
//  doingGast.hpp
//  certain_Astro_Parameters
//
//  Created by Clifford Campo on 2/21/21.
//  Copyright © 2021 CliffordCampo. All rights reserved.
//
//
//  doingGast.hpp
//  astroParameter_SRSS
//
//  Created by Clifford Campo on 2/14/21.
//  Copyright © 2021 CliffordCampo. All rights reserved.
//

#ifndef doingGast_hpp
#define doingGast_hpp
#include "Pclp.hpp"
#include "Constants.hpp"
#include <stdio.h>
class DoingGast {
    typedef int (DoingGast::*ptfdg)(Constants&, bool);
private:
    bool debugFlag;
public:
    ptfdg ptr_doingGast;
    const int gastModulous=24;
    int rc;
    //The following pointers should be filled in by the command line processing routine.
    const char *ptrThisPreamble = "PREAMBLE";
    const char *ptrWhatThisPreambleIs = "DECLARE srnoonssportal CURSOR FOR ";  //Default Preamble;
    const char *ptrThisSiteid = "SITEID";
    const char *ptrWhatThisSiteIdIs; //No Default parameter, siteid must be supplied.
    const char *ptrThisSomeDate = "SOMEDATE";
    const char *ptrWhatThisSomeDateIs;//No Default parameter, date must be supplied in ISO 8601, YYYY-MM-DD, format.
    const char *ptrThisBeforeDataPoints = "BEFOREDATAPOINTS";
    const char *ptrWhatThisBeforeDataPointsIs = "4";
    const char *ptrThisAfterDataPoints = "AFTERDATAPOINTS";
    const char *ptrWhatThisAfterDataPointsIs = "4";
    const char *ptrThisField = "FIELD";
    const char *ptrWhatThisFieldIs = "gast";
    const char *ptrThisPostamble = "POSTAMBLE";
    const char *ptrWhatThisPostambleIs = ";"; //Just to make sure we have a c-string-terminating null character.
    const char *ptrThisTerm = "TERM";
    const char *ptrWhatThisTermIs = "TERM";
    const char *ptrWhatThisEventIs;
    const char *ptrWhatThisEventDefinitionIs;
    
    const char *sqlTemplateGAST0 ="PREAMBLE SELECT * from tbl_sun_loc_site where events IS NULL AND id BETWEEN -BEFOREDATAPOINTS + \
    ( SELECT id  FROM tbl_sun_loc_site  WHERE FIELD = (SELECT min(FIELD)  FROM tbl_sun_loc_site WHERE lt::DATE='SOMEDATE' AND \
    siteid=SITEID AND events IS NULL) AND siteid=SITEID)  AND +AFTERDATAPOINTS + ( SELECT id  FROM tbl_sun_loc_site  WHERE FIELD = \
    (SELECT min(FIELD) FROM tbl_sun_loc_site WHERE lt::DATE='SOMEDATE' AND siteid=SITEID AND events IS NULL) AND siteid = SITEID) \
    ORDER BY jdlocal ;POSTAMBLE";
    
//Define a 2-dimensional array of pointers to characters used to construct the above SQL for determining gast
    int some2DArraySize=8;
    const char **sunTemplateGAST0[8][2] = {     \
        {&ptrThisPreamble,         &ptrWhatThisPreambleIs},     \
        {&ptrThisSiteid,           &ptrWhatThisSiteIdIs},       \
        {&ptrThisSomeDate,         &ptrWhatThisSomeDateIs},   \
        {&ptrThisBeforeDataPoints, &ptrWhatThisBeforeDataPointsIs}, \
        {&ptrThisAfterDataPoints,  &ptrWhatThisAfterDataPointsIs},  \
        {&ptrThisField,            &ptrWhatThisFieldIs}, \
        {&ptrThisPostamble,        &ptrWhatThisPostambleIs}, \
        {&ptrThisTerm,             &ptrWhatThisTermIs}};
    const char ***some2Darray = sunTemplateGAST0[0];
    const char *some1Darray = &sqlTemplateGAST0[0];
    int some1DArraySize=0;
    char *lastSemiColon=nullptr;
    DoingGast(bool=0);
    DoingGast(Constants& c, bool=0);
    ~DoingGast(void);
    int doingGAST(Constants&, bool);
    int processingGAST( void );
};
#endif /* doingGast_hpp */

