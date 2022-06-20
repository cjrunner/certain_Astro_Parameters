//
//  goExecuteSomeSQL.cpp
//  certain_Astro_Parameters
//
//  Created by Clifford Campo on 2/21/21.
//  Copyright © 2021 CliffordCampo. All rights reserved.
//

#include "goExecuteSomeSQL.hpp"
/*
 The following comes right out of libpq-fe.h and describes the status codes resulting from PQexec
 typedef enum
 {
 PGRES_EMPclpY_QUERY = 0,    *0  empty query string was executed *
 PGRES_COMMAND_OK,           *1  a query command that doesn't return anything was executed properly by the backend *
 PGRES_TUPLES_OK,            *2  a query command that returns tuples was executed properly by the backend, PGresult contains the \
                                 result tuples *
 PGRES_COPY_OUT,             *3 Copy Out data transfer in progress *
 PGRES_COPY_IN,              *4 Copy In data transfer in progress *
 PGRES_BAD_RESPONSE,         *5 an unexpected response was recvd from the backend *
 PGRES_NONFATAL_ERROR,       *6 notice or warning message *
 PGRES_FATAL_ERROR,          *7 query failed *
 PGRES_COPY_BOTH,            *8 Copy In/Out data transfer in progress *
 PGRES_SINGLE_TUPLE          *9 single tuple from larger resultset *
 } ExecStatusType;
 */
int goExecuteSomeSQL(Constants& c, const char *theSQL, int expectedResult, int failureReturnCode, bool doPQclear, bool returnsData) {
    c.dbc->res = PQexec(c.dbc->conn, theSQL);
    if (PQresultStatus(c.dbc->res) != expectedResult) {  //After doing FETCH ALL IN srnoonssportal, the expected return code from  \
        PQresultStatus is PGRES_TUPLES_OK
        std::cerr << "SQL command\n" << theSQL  << "\nfailed given error message: " << PQerrorMessage(c.dbc->conn) << std::endl;
        PQclear(c.dbc->res); c.dbc->res=nullptr;  //Clear Begin command that DID NOT work
        return failureReturnCode;
    } else if (returnsData) {
//        Come Here If We Are Expecting Data to be Returned, such as with a “FETCH ALL …” SQL Operation.
        c.numberOfColumns = PQnfields(c.dbc->res);
        c.numberOfRows = PQntuples(c.dbc->res);
        if (c.numberOfColumns == 0 || c.numberOfRows == 0) {
            cerr << "Within function goExecuteSomeSQL: " << \
            "Either no rows were returned and/or no columns were returned from FETCH ALL operation! number of columns = " \
            << c.numberOfColumns << ", and the number of rows = " << c.numberOfRows << endl;
            c.rc = -(30+2); //Set Negative, bad, return code.
            return (c.rc); //Quit
        } else {
            c.ptrArrayDependent = (myParameters *)calloc(sizeof(myParameters), c.numberOfRows); //At First jdlocal will be in \
            ArrayDependent Because we want to determine the exact time a certain Field reaches a certain number (e.g. When does \
            the zenithdistance field reach 90.5833…3º which is the definition of SunRise or SunSet. After we get this time we will \
            move the jdlocal data from ArrayDependent to Array Independent to determine the values that the other fields had at \
            this particular jdlocal time.
            c.ptrArrayIndependent = (myParameters *)calloc(sizeof(myParameters), c.numberOfRows); // At first ArrayIndependent will \
            be filled with sufficient number of data points for era, or, gast, or last, or zenithdistance [for determining time of \
            sunrise, sunset, or noon], or geocentricra [for determining the time summer arrives (geocentricra=6º), \
            fall arrives (geocentricra = 12º), or winter arrives (geocentricra = 18º), or spring arrives (geocentricra = 0º). \
            Either era, gast, last, zenithdistance [sunrise, sunset, noon], or geocentricra [summer, fall, winter, spring] will be \
            in arrayIndepented because we want to find the specific time, jdlocal, when era=360, gast = 24 hours, last = 24 hours, \
            zenithdistance=90.5833… [sunrise or sunset], zenithdistance is a minimum value [noon], geocentricra is 6º [summer], \
            12º [fall/autumn], 18º [winter], or 0º [spring].  For example: When does ERA = 0º? To accomplish this we set the \
            independent variable (ERA) to zero and then provide doChebyshev, for performing Chebyshev Polynomial Interpolation, \
            with the jdlocal values surrounding the location where what we believe the sought-after ERA value lies.
            
// ---------> We will leave it to the various processXX routines to fill these Dependent and Independent Arrays <-----------------
        }
    } else {
        std::cout << "SQL command:\n" << theSQL << \
                 "\nseems to have worked because PQresultStatus returned in an expected result of " << expectedResult << \
                 ". Will now perform a PQclear(c.dbc->res) to clean-up." << endl;
        if (doPQclear) {
            PQclear(c.dbc->res); //Clear *theSQL command that DID work
            c.dbc->res = nullptr; //Indicates PQclear executed
        } //End of if
    } //End of if-then-else
    return 0;
}
