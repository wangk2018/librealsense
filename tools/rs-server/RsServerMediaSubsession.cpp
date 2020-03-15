// License: Apache 2.0. See LICENSE file in root directory.
// Copyright(c) 2017 Intel Corporation. All Rights Reserved.

#include "RsServerMediaSubsession.h"
#include "RsServerMediaSession.h"
#include "RsSimpleRTPSink.h"

#define CAPACITY 100

RsServerMediaSubsession *RsServerMediaSubsession::createNew(UsageEnvironment &t_env, rs2::video_stream_profile &t_videoStreamProfile, std::shared_ptr<RsDevice> rsDevice)
{
  return new RsServerMediaSubsession(t_env, t_videoStreamProfile, rsDevice);
}

RsServerMediaSubsession ::RsServerMediaSubsession(UsageEnvironment &env, rs2::video_stream_profile &t_videoStreamProfile, std::shared_ptr<RsDevice> device)
    : OnDemandServerMediaSubsession(env, false), m_videoStreamProfile(t_videoStreamProfile)
{
  //envir() << "RsServerMediaSubsession constructor" <<this << "\n";
  m_frameQueue = rs2::frame_queue(CAPACITY, true);
  m_rsDevice=device;
}

RsServerMediaSubsession::~RsServerMediaSubsession()
{
  //envir() << "RsServerMediaSubsession destructor" <<this << "\n";
  //TODO:: free the queue
}

rs2::frame_queue &RsServerMediaSubsession::getFrameQueue()
{
  return m_frameQueue;
}

rs2::video_stream_profile RsServerMediaSubsession::getStreamProfile()
{
  return m_videoStreamProfile;
}

FramedSource *RsServerMediaSubsession::createNewStreamSource(unsigned /*t_clientSessionId*/, unsigned &t_estBitrate)
{
  t_estBitrate = 20000;
  return RsDeviceSource::createNew(envir(), m_videoStreamProfile, m_frameQueue);
}

RTPSink *RsServerMediaSubsession ::createNewRTPSink(Groupsock *t_rtpGroupsock,
                                                    unsigned char t_rtpPayloadTypeIfDynamic,
                                                    FramedSource * /*t_inputSource*/)
{
  return RsSimpleRTPSink::createNew(envir(), t_rtpGroupsock, t_rtpPayloadTypeIfDynamic, 90000, "X" , "Y" , m_videoStreamProfile, m_rsDevice); //TODO: to rename X and Y
}
