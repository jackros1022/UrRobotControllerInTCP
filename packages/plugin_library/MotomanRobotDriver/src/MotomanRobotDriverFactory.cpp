//
// Created by 杨帆 on 17-4-10.
// Copyright (c) 2017 Wuhan Collaborative Robot Technology Co.,Ltd. All rights reserved.
//
#include <cobotsys_abstract_object_factory.h>
#include "MotomanRealTimeDriver.h"
#include <extra2.h>
#include <cobotsys_abstract_factory_macro.h>
//TODO FINISH
COBOTSYS_FACTORY_BEGIN(MotomanRobotDriverFactory)
        COBOTSYS_FACTORY_EXPORT(MotomanRealTimeDriver)
COBOTSYS_FACTORY_END(MotomanRobotDriverFactory, "1.0")
