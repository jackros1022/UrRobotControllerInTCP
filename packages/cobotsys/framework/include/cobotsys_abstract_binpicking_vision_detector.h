//
// Created by 潘绪洋 on 17-4-5.
// Copyright (c) 2017 Wuhan Collaborative Robot Technology Co.,Ltd. All rights reserved.
//

#ifndef COBOTSYS_COBOTSYS_ABSTRACT_BINPICKING_VISION_ALGORITHM_H
#define COBOTSYS_COBOTSYS_ABSTRACT_BINPICKING_VISION_ALGORITHM_H

#include "cobotsys_abstract_object.h"
#include "cobotsys_data_types.h"
#include <opencv2/opencv.hpp>

namespace cobotsys {
namespace binpicking {

class AbstractBinpickingVisionDetector : public AbstractObject {
public:
    AbstractBinpickingVisionDetector();
    virtual ~AbstractBinpickingVisionDetector();

    /**
     *
     * @param images
     * @return true 表示成功的找到可检取目标
     */
    virtual bool processVisionImage(const std::vector<VisionInputImage>& images) = 0;

    virtual bool getPickObjects(std::vector<BinObjGrabPose>& result) const = 0;
};
}
}


#endif //COBOTSYS_COBOTSYS_ABSTRACT_BINPICKING_VISION_ALGORITHM_H
