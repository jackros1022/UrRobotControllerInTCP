//
// Created by 于天水 on 17-4-12.
//

#include <extra2.h>
#include <cobotsys_abstract_factory_macro.h>
#include "BaslerCamera.h"

COBOTSYS_FACTORY_BEGIN(BaslerCameraFactory)
        COBOTSYS_FACTORY_EXPORT(BaslerCamera)
COBOTSYS_FACTORY_END(BaslerCameraFactory, "1.0")