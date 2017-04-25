//
// Created by 潘绪洋 on 17-4-24.
// Copyright (c) 2017 Wuhan Collaborative Robot Technology Co.,Ltd. All rights reserved.
//

#ifndef COBOTSYS_PHYSICALDISTRIBUTIONCONTROLLER_H
#define COBOTSYS_PHYSICALDISTRIBUTIONCONTROLLER_H

#include <cobotsys_abstract_controller.h>
#include <cobotsys_global_object_factory.h>
#include <cobotsys_abstract_arm_robot_realtime_driver.h>
#include <cobotsys_abstract_kinematic_solver.h>
#include <cobotsys_abstract_arm_robot_move_driver.h>
#include <mutex>
#include <cobotsys_abstract_camera.h>
#include <cobotsys_abstract_binpicking_vision_detector.h>
#include <cobotsys_abstract_binpicking_picker.h>
#include <cobotsys_abstract_binpicking_placer.h>
#include <condition_variable>

#include "JsonServer.h"
#include "MatViewer.h"
#include <thread>
#include <MatMerger.h>
#include <QVBoxLayout>

using namespace cobotsys;
using namespace cobotsys::binpicking;

class PhysicalDistributionController : public AbstractControllerWidget,
                                       public ArmRobotRealTimeStatusObserver,
                                       public CameraStreamObserver {
Q_OBJECT
public:
    PhysicalDistributionController();
    virtual ~PhysicalDistributionController();

    virtual bool setup(const QString& configFilePath);

    virtual bool start();
    virtual void pause();
    virtual void stop();

protected:
    void mainLoop();
    void setupUi();

protected:
    virtual void onArmRobotConnect();
    virtual void onArmRobotDisconnect();
    virtual void onArmRobotStatusUpdate(const ArmRobotStatusPtr& ptrRobotStatus);

    virtual void onCameraStreamUpdate(const CameraFrame& cameraFrame, AbstractCamera* camera);

protected:
    bool _setupInternalObjects(ObjectGroup& objectGroup);
    void _debugImages();

protected:
    shared_ptr<AbstractArmRobotRealTimeDriver> m_ptrRobot;
    shared_ptr<AbstractKinematicSolver> m_ptrKinematicSolver;
    shared_ptr<AbstractArmRobotMoveDriver> m_ptrRobotXyz;
    shared_ptr<AbstractCamera> m_ptrCameraMaster;
    shared_ptr<AbstractBinpickingVisionDetector> m_ptrDetector;
    shared_ptr<AbstractBinpickingPicker> m_ptrPicker;
    shared_ptr<AbstractBinpickingPlacer> m_ptrPlacer;

    std::mutex m_mutex;
    bool m_robotConnected;

    JsonServer* m_jsonServer;

    std::thread m_mainTaskThread;
    bool m_loop;

    std::condition_variable m_cond;
    std::vector<VisionInputImage> m_images;
    bool m_imageUpdated;

    int m_numImageCaptured;

    MatViewer* m_matViewer;

};


#endif //COBOTSYS_PHYSICALDISTRIBUTIONCONTROLLER_H