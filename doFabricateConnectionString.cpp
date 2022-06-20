//
//  doFabricateConnectionString.cpp
//  certain_Astro_Parameters
//
//  Created by Clifford Campo on 2/21/21.
//  Copyright © 2021 CliffordCampo. All rights reserved.
//
//
//  doDefaultNames.cpp
//  astroParameter
//  copied from:
//  Created by Clifford Campo on 12/9/20.
//  Copyright © 2020 CliffordCampo. All rights reserved.
//
#include "Constants.hpp" 
void Constants::doFabricateConnectionString(void) {
    //Any parameters that have not been specified by command line switches get the default values.
    workbuf[0] = string(dbc->dbname);
    workbuf[1] = string(dbc->userId);
    workbuf[2] = string(dbc->hostName);
    workbuf[3] = string(dbc->portId);
/*
     c.dbc->connectionStringBuf << "dbname=" << string(c.dbc->dbname) << " user=" << string(c.dbc->userId) << \
     " host=" << string(c.dbc->hostName) << " port=" << string(c.dbc->portId); //First, build a c++-based string-stream from a \
     bunch of C arrays of characters.
*/
    intermediateConnectionString = "dbname=" + string(dbc->dbname) + " user=" + string(dbc->userId) + " host=" + \
    string(dbc->hostName) + " port=" + string(dbc->portId);
    //    c.intermediateConnectionString = c.dbc->connectionStringBuf.str(); // Now convert this string steam into a c++ string.
    ptrConstructedConnectionString = intermediateConnectionString.c_str(); //Finally, convert this c++ string back into a single \
    “C array of characters“. NOTE: there doesn't seem to be a way of going from string-stream buffer directly to a \
    c-character array. NB: We depend upon file ~/.pgpass to supply the appropriate database password.
}
