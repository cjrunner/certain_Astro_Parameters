//
//  doFetchAll.cpp
//  certain_Astro_Parameters
//
//  Created by Clifford Campo on 2/21/21.
//  Copyright © 2021 CliffordCampo. All rights reserved.
//

#include "doFetchAll.hpp"
// If SQL in cbb_buffer worked, DO NOT do a PQclear just yet or else we'll destroy the data returned by this SQL before we're \
able to do a FETCH ALL IN someportal
int doFetchAll(Constants& c) {
    c.dbc->res = PQexec(c.dbc->conn, "FETCH ALL IN srnoonssportal");
    if (PQresultStatus(c.dbc->res) != PGRES_TUPLES_OK) {
        std::cerr << "SQL command """ << "FETCH ALL IN srnoonssportal" << """  failed because "  << PQerrorMessage(c.dbc->conn) << std::endl;
        PQclear(c.dbc->res); c.dbc->res=nullptr; //Clear FETCH ALL command that ***DID NOT*** work
        return FETCH_ALL_SQL_COMMAND_FAILED; 
    }
    return 0;
}
