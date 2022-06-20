//
//  doDeclare_srnoonssportal_CursorFor.cpp
//  certain_Astro_Parameters
//
//  Created by Clifford Campo on 2/21/21.
//  Copyright © 2021 CliffordCampo. All rights reserved.
//

#include "doDeclare_srnoonssportal_CursorFor.hpp"
int doDeclare_srnoonssportal_CursorFor(Constants& c) {
    c.dbc->res = PQexec(c.dbc->conn, c.ptr_cbb_results);
    if (PQresultStatus(c.dbc->res) != PGRES_COMMAND_OK) {
        std::cerr << "SQL command \"" << std::endl;
        std::cerr << c.ptr_cbb_results;
        std::cerr << "\"  failed because "  << PQerrorMessage(c.dbc->conn) << std::endl;
        PQclear(c.dbc->res); c.dbc->res=nullptr;  //Clear SQL command in cbb_results that DID NOT work
        return UNABLE_TO_PQexec_DECLARE_PORTAL_CURSOR;
    }
    return 0;
}
