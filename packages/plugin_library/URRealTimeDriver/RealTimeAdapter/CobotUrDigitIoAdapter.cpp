//
// Created by 潘绪洋 on 17-4-18.
// Copyright (c) 2017 Wuhan Collaborative Robot Technology Co.,Ltd. All rights reserved.
//

#include <cobotsys_logger.h>
#include "CobotUrDigitIoAdapter.h"

CobotUrDigitIoAdapter::CobotUrDigitIoAdapter() {
    m_realTimeCommCtrl = nullptr;
    m_isInput = false;
    m_isOutput = false;
    m_inputIoStatus = 0;
    m_outputIoStatus = 0;
}

CobotUrDigitIoAdapter::~CobotUrDigitIoAdapter() {
}

bool CobotUrDigitIoAdapter::setup(const QString& configFilePath) {
    return true;
}

void CobotUrDigitIoAdapter::setIo(DigitIoPorts ioPorts, DigitIoStatus ioStatus) {
    if (m_isOutput) {
        if (ioPorts & DigitIoPort::Port_0) setDigitOut(0, ioStatus == DigitIoStatus::Set);
        if (ioPorts & DigitIoPort::Port_1) setDigitOut(1, ioStatus == DigitIoStatus::Set);
        if (ioPorts & DigitIoPort::Port_2) setDigitOut(2, ioStatus == DigitIoStatus::Set);
        if (ioPorts & DigitIoPort::Port_3) setDigitOut(3, ioStatus == DigitIoStatus::Set);
        if (ioPorts & DigitIoPort::Port_4) setDigitOut(4, ioStatus == DigitIoStatus::Set);
        if (ioPorts & DigitIoPort::Port_5) setDigitOut(5, ioStatus == DigitIoStatus::Set);
        if (ioPorts & DigitIoPort::Port_6) setDigitOut(6, ioStatus == DigitIoStatus::Set);
        if (ioPorts & DigitIoPort::Port_7) setDigitOut(7, ioStatus == DigitIoStatus::Set);
    }
}

DigitIoStatus CobotUrDigitIoAdapter::getIoStatus(DigitIoPort ioPort) {
    if (m_isInput) {
        if (m_inputIoStatus & (int) ioPort) {
            return DigitIoStatus::Set;
        }
    }
    return DigitIoStatus::Reset;
}

bool CobotUrDigitIoAdapter::isDigitInput() const {
    return m_isInput;
}

bool CobotUrDigitIoAdapter::isDigitOutput() const {
    return m_isOutput;
}

void CobotUrDigitIoAdapter::debugIoStatus() {
    if (isDigitInput() && m_debugIoLastStatus != m_inputIoStatus) {
#ifdef DEBUG
        COBOT_LOG.message("Input") << std::hex << setw(8) << m_inputIoStatus;
#endif // DEBUG

        m_debugIoLastStatus = m_inputIoStatus;
    }
    if (isDigitOutput() && m_debugIoLastStatus != m_outputIoStatus) {
#ifdef DEBUG
        COBOT_LOG.message("Output") << std::hex << setw(8) << m_outputIoStatus;
#endif // DEBUG

        m_debugIoLastStatus = m_outputIoStatus;
    }
}

void CobotUrDigitIoAdapter::setDigitOut(int portIndex, bool b) {
    char buf[256] = {0};
    if (portIndex < 8) {
        double ver = 0;
        if (m_realTimeCommCtrl)
            ver = m_realTimeCommCtrl->ur->getRobotState()->getVersion();
        if (ver < 2) {
            sprintf(buf, "sec setOut():\n\tset_digital_out(%d, %s)\nend\n", portIndex,
                    b ? "True" : "False");
        } else {
            sprintf(buf, "sec setOut():\n\tset_standard_digital_out(%d, %s)\nend\n",
                    portIndex, b ? "True" : "False");
        }

        if (m_realTimeCommCtrl) {
            m_realTimeCommCtrl->addCommandToQueue(buf);
        }
    }
}

bool CobotUrDigitIoAdapter::setToolVoltage(double v) {
    char buf[256];
    int voltage = (int) v;
    sprintf(buf, "sec setOut():\n\tset_tool_voltage(%d)\nend\n", voltage);
    if (m_realTimeCommCtrl) {
        m_realTimeCommCtrl->addCommandToQueue(buf);
        return true;
    }
    return false;
}

void CobotUrDigitIoAdapter::setUrRealTimeCtrl(CobotUrRealTimeCommCtrl* realTimeCommCtrl) {
    m_realTimeCommCtrl = realTimeCommCtrl;
    if (m_realTimeCommCtrl == nullptr) {
        m_inputIoStatus = 0;
        m_outputIoStatus = 0;
    }
}

bool CobotUrDigitIoAdapter::isOpened() const {
    return (m_realTimeCommCtrl != nullptr);
}

