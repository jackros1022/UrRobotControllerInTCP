//
// Created by lhc on 17-4-10.
// Copyright (c) 2017 Wuhan Collaborative Robot Technology Co.,Ltd. All rights reserved.
//

#ifndef PROJECT_FORCEGUIDECONTROLLER_H
#define PROJECT_FORCEGUIDECONTROLLER_H

#include <mutex>
#include <thread>
#include <cobotsys_abstract_controller.h>
#include <cobotsys_abstract_arm_robot_realtime_driver.h>
#include <cobotsys_abstract_force_sensor.h>
#include <cobotsys_abstract_kinematic_solver.h>
#include <cobotsys_abstract_forcecontrol_solver.h>
#include <QObject>
#include <QString>

using namespace cobotsys;

class ForceGuideController : public QObject, public AbstractController, public ArmRobotRealTimeStatusObserver, public ForceSensorStreamObserver {
	Q_OBJECT
public:
	ForceGuideController();
	virtual ~ForceGuideController();

	virtual bool setup(const QString& configFilePath);
	virtual bool start();
	virtual void pause();
	virtual void stop();

public:
	virtual void onForceSensorConnect();
	virtual void onForceSensorDisconnect();
	virtual void onForceSensorDataStreamUpdate(const std::shared_ptr<cobotsys::Wrench>& ptrWrench);

public:
	virtual void onArmRobotConnect();
	virtual void onArmRobotDisconnect();
	virtual void onArmRobotStatusUpdate(const ArmRobotStatusPtr& ptrRobotStatus);

protected:
	bool createRobot();
	bool createForceSensor();
	bool createKinematicSolver();
	bool createForceControlSolver();

	void startRobot();
	void stopRobot();

	void startForceSensor();
	void stopForceSensor();

	void guideControlThread();


protected:
	bool m_bcontrolStart;
	bool m_exit;

	QString m_robotFactory;
	QString m_robotType;
	QString m_robotConfig;
	std::shared_ptr<AbstractArmRobotRealTimeDriver> m_ptrRobot;
	bool m_bRobotConnect;

	QString m_sensorFactory;
	QString m_sensorType;
	QString m_sensorConfig;
	std::shared_ptr<AbstractForceSensor> m_ptrSensor;
	bool m_bSensorConnect;

	QString m_solverFactory;
	QString m_solverType;
	QString m_solverConfig;
	std::shared_ptr<AbstractForceControlSolver> m_ptrForceControlSolver;

	QString m_kinematicFactory;
	QString m_kinematicType;
	QString m_kinematicConfig;
	std::shared_ptr<AbstractKinematicSolver> m_ptrKinematicSolver;

	std::vector<double> m_deltaValue;

	std::mutex m_mutex;
	std::thread m_controlThread;
	
	int m_joint_num;
	std::vector<double> m_curQ;
	cobotsys::Wrench m_wrenchData;

	//bool m_firstMove;
	bool m_posReady;
	bool m_sensorReady;
	bool m_setVoltage;
};


#endif //PROJECT_FORCEGUIDECONTROLLER_H
