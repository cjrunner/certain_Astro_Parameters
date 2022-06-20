//
//  goExecuteSomeSQL.hpp
//  certain_Astro_Parameters
//
//  Created by Clifford Campo on 2/21/21.
//  Copyright © 2021 CliffordCampo. All rights reserved.
//

#ifndef goExecuteSomeSQL_hpp
#define goExecuteSomeSQL_hpp

#include "Constants.hpp"
#define BADBEGIN          -(2+10)
#define BADFETCHALL       -(3+10)
#define BADCLOSE          -(4+10)
#define BADEND            -(5+10)
#define BADGENERALEXECUTE -(6+10)
int goExecuteSomeSQL(Constants& c, const char *theSQL, int expectedResult, int failureReturnCode, bool=true, bool=false);
#endif /* goExecuteSomeSQL_hpp */
