//
//  doBegin.cpp
//  certain_Astro_Parameters
//
//  Created by Clifford Campo on 2/21/21.
//  Copyright © 2021 CliffordCampo. All rights reserved.
//

#include "doBegin.hpp"
int doBegin(Constants& c) {
c.dbc->res = PQexec(c.dbc->conn, "BEGIN");
if (PQresultStatus(c.dbc->res) != PGRES_COMMAND_OK) {
    std::cerr << """BEGIN""  "  << " failed because " << PQerrorMessage(c.dbc->conn) << std::endl;
    PQclear(c.dbc->res); c.dbc->res=nullptr;  //Clear Begin command that DID NOT work
    return UNABLE_TO_PERFORM_SQL_BEGIN; 
    } else {
        PQclear(c.dbc->res); c.dbc->res=nullptr;  //Clear Begin command that DID work
    }
}
