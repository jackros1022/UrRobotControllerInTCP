//
// Created by 潘绪洋 on 17-3-27.
// Copyright (c) 2017 Wuhan Collaborative Robot Technology Co.,Ltd. All rights reserved.
//


#include <extra2.h>
#include <cobotsys_global_object_factory.h>
#include "ArmRobotManipulator.h"
#include <QDebug>
#include <QtWidgets/QFileDialog>
#include <cobotsys_file_finder.h>

class AutoCtx {
protected:
    bool m_sign;
public:
    AutoCtx(bool& b) : m_sign(b){ m_sign = true; }

    ~AutoCtx(){ m_sign = false; }
};
#define Q_SLOT_REDUCE(flag) if ((flag)) {return;} AutoCtx autoctx(flag);


ArmRobotManipulator::ArmRobotManipulator(){
    m_initUIData = 0;
    ui.setupUi(this);

    m_noHandleChange = false;

    m_sliders.push_back(ui.slider_1);
    m_sliders.push_back(ui.slider_2);
    m_sliders.push_back(ui.slider_3);
    m_sliders.push_back(ui.slider_4);
    m_sliders.push_back(ui.slider_5);
    m_sliders.push_back(ui.slider_6);

    m_target.push_back(ui.dsb_target_1);
    m_target.push_back(ui.dsb_target_2);
    m_target.push_back(ui.dsb_target_3);
    m_target.push_back(ui.dsb_target_4);
    m_target.push_back(ui.dsb_target_5);
    m_target.push_back(ui.dsb_target_6);

    m_actual.push_back(ui.dsb_real_1);
    m_actual.push_back(ui.dsb_real_2);
    m_actual.push_back(ui.dsb_real_3);
    m_actual.push_back(ui.dsb_real_4);
    m_actual.push_back(ui.dsb_real_5);
    m_actual.push_back(ui.dsb_real_6);

    for (auto& slider : m_sliders) {
        connect(slider, &QSlider::valueChanged, [=](int){ this->handleSliderChange(); });
    }

    for (auto& iter : m_target) {
        connect(iter, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
                [=](double){ handleTargetChange(); });
    }

    connect(ui.btnCreate, &QPushButton::released, this, &ArmRobotManipulator::createRobot);
    connect(ui.btnStart, &QPushButton::released, this, &ArmRobotManipulator::startRobot);
    connect(ui.btnStop, &QPushButton::released, this, &ArmRobotManipulator::stopRobot);
    connect(this, &ArmRobotManipulator::updateActualQ, this, &ArmRobotManipulator::onActualQUpdate);

    connect(ui.btnRecTarget, &QPushButton::released, this, &ArmRobotManipulator::onRecTarget);
    connect(ui.btnGoTarget, &QPushButton::released, this, &ArmRobotManipulator::onGoTarget);

    ui.btnStart->setEnabled(false);
    ui.btnStop->setEnabled(false);
}

ArmRobotManipulator::~ArmRobotManipulator(){
}

bool ArmRobotManipulator::setup(const QString& configFilePath){
    QJsonObject json;
    if (loadJson(json, configFilePath)) {
        m_joint_num = json["joint_num"].toDouble(6);
        m_defaultRobotInfo.clear();
        m_defaultRobotInfo << json["default_robot"].toObject()["factory"].toString();
        m_defaultRobotInfo << json["default_robot"].toObject()["type"].toString();

        int jmin = json["joint_min"].toInt(-180);
        int jmax = json["joint_max"].toInt(180);
        for (int i = 0; i < (int) m_sliders.size(); i++) {
            m_sliders[i]->setRange(jmin, jmax);
        }


        qDebug() << m_defaultRobotInfo;

        setupCreationList();
        return true;
    }
    return false;
}

void ArmRobotManipulator::handleSliderChange(){
    Q_SLOT_REDUCE(m_noHandleChange);

    for (size_t i = 0; i < m_sliders.size(); i++) {
        auto slider = m_sliders[i];
        auto dsbTgt = m_target[i];
        auto dsbAct = m_actual[i];
        if (slider->value() != (int) dsbTgt->value()) {
            dsbTgt->setValue(slider->value());
        }
    }

    updateTargetQ();
}

void ArmRobotManipulator::handleTargetChange(){
    Q_SLOT_REDUCE(m_noHandleChange);

    for (size_t i = 0; i < m_target.size(); i++) {
        auto slider = m_sliders[i];
        auto dsbTgt = m_target[i];
        auto dsbAct = m_actual[i];

        if ((int) dsbTgt->value() != slider->value()) {
            slider->setValue((int) dsbTgt->value());
            dsbTgt->setValue(slider->value());
            return;
        }
    }
}

void ArmRobotManipulator::createRobot(){
    if (!GlobalObjectFactory::instance()) return;
    if (ui.cboRobotType->count() == 0)
        return;

    QString robotConfig = QFileDialog::getOpenFileName(this,
                                                       tr("Get Robot Config JSON file ..."),
                                                       QString(FileFinder::getPreDefPath().c_str()),
                                                       tr("JSON files (*.JSON *.json)"));
    if (robotConfig.isEmpty())
        return;

    QStringList obj_info = ui.cboRobotType->currentData().toStringList();
    QString factory = obj_info.front();
    QString typen = obj_info.back();

    auto obj = GlobalObjectFactory::instance()->createObject(factory, typen);
    m_ptrRobot = std::dynamic_pointer_cast<AbstractArmRobotRealTimeDriver>(obj);
    if (m_ptrRobot) {
        auto ob = std::dynamic_pointer_cast<ArmRobotRealTimeStatusObserver>(shared_from_this());
        m_ptrRobot->attach(ob);
        if (m_ptrRobot->setup(robotConfig)) {
            COBOT_LOG.notice() << "Create and setup success";
        } else {
            m_ptrRobot.reset();
        }
    }

    ui.btnStart->setEnabled(true);
    ui.btnStop->setEnabled(true);
}

void ArmRobotManipulator::setupCreationList(){
    if (!GlobalObjectFactory::instance()) return;

    bool foundDefault = false;
    QString defIdxText;

    ui.cboRobotType->clear();
    auto factory_names = GlobalObjectFactory::instance()->getFactoryNames();
    for (auto& name : factory_names) {
        auto types = GlobalObjectFactory::instance()->getFactorySupportedNames(name);

        for (auto& type : types) {
            auto obj = GlobalObjectFactory::instance()->createObject(name, type);
            auto robot = std::dynamic_pointer_cast<AbstractArmRobotRealTimeDriver>(obj);
            if (robot) {
                QStringList data;
                QString text;
                text = QString("%1 - %2").arg(name.c_str()).arg(type.c_str());
                data << name.c_str();
                data << type.c_str();
                ui.cboRobotType->addItem(text, data);
                if (data == m_defaultRobotInfo) {
                    foundDefault = true;
                    defIdxText = text;
                }
            }
        }
    }

    if (foundDefault) {
        auto defaultIndex = ui.cboRobotType->findText(defIdxText);
        ui.cboRobotType->setCurrentIndex(defaultIndex);
        COBOT_LOG.info() << "Default ROBOT: " << defaultIndex;
    }
}

void ArmRobotManipulator::startRobot(){
    if (m_ptrRobot) {
        m_initUIData = 5;
        if (m_ptrRobot->start()) {
            COBOT_LOG.info() << "Robot Start Success";
        }
    }
}

void ArmRobotManipulator::stopRobot(){
    if (m_ptrRobot) {
        m_ptrRobot->stop();
    }
}

void ArmRobotManipulator::onArmRobotConnect(){
}

void ArmRobotManipulator::onArmRobotDisconnect(){
}

void ArmRobotManipulator::onArmRobotStatusUpdate(const ArmRobotStatusPtr& ptrRobotStatus){
    auto q_actual_size = ptrRobotStatus->q_actual.size();
    if (q_actual_size > 6)
        q_actual_size = 6;

    m_mutex.lock();
    m_actualValue.resize(q_actual_size);
    for (size_t i = 0; i < q_actual_size; i++) {
        m_actualValue[i] = ptrRobotStatus->q_actual[i];
    }
    m_mutex.unlock();

    Q_EMIT updateActualQ();
}

void ArmRobotManipulator::closeEvent(QCloseEvent* event){
    if (m_ptrRobot) {
        m_ptrRobot->stop();
    }
    QWidget::closeEvent(event);
}

void ArmRobotManipulator::onActualQUpdate(){
    std::vector<double> tmpq;
    m_mutex.lock();
    tmpq = m_actualValue;
    m_mutex.unlock();

    for (size_t i = 0; i < tmpq.size(); i++) {
        m_actual[i]->setValue(tmpq[i] / CV_PI * 180);
    }

    if (m_initUIData > 0) {
        auto log = COBOT_LOG.info();
        log << "Real: ";
        for (size_t i = 0; i < tmpq.size(); i++) {
            m_target[i]->setValue(tmpq[i] / CV_PI * 180);
            log << tmpq[i] / CV_PI * 180 << ". ";
        }
        m_initUIData--;
    }
}

void ArmRobotManipulator::updateTargetQ(){
    if (m_initUIData > 0) {
        return;
    }

    std::vector<double> target_q;
    if (m_ptrRobot) {
        target_q.resize(m_target.size());
        for (int i = 0; i < (int) m_target.size(); i++) {
            target_q[i] = m_target[i]->value() / 180 * CV_PI;
        }
        m_ptrRobot->move(target_q);
    }
}

void ArmRobotManipulator::onRecTarget(){
    m_recTarget.resize(m_target.size());
    for (int i = 0; i < (int) m_recTarget.size(); i++) {
        m_recTarget[i] = m_target[i]->value();
    }
}

void ArmRobotManipulator::onGoTarget(){
    if (m_recTarget.size() == m_target.size()) {
        for (int i = 0; i < (int) m_recTarget.size(); i++) {
            m_target[i]->setValue(m_recTarget[i]);
        }
    }
}