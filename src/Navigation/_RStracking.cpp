/*
 * _RStracking.cpp
 *
 *  Created on: Oct 29, 2019
 *      Author: yankai
 */

#ifdef USE_REALSENSE
#include "_RStracking.h"

namespace kai
{

	_RStracking::_RStracking()
	{
	}

	_RStracking::~_RStracking()
	{
		close();
	}

	bool _RStracking::init(void *pKiss)
	{
		IF_F(!this->_NavBase::init(pKiss));
		Kiss *pK = (Kiss *)pKiss;

		return true;
	}

	bool _RStracking::open(void)
	{
		IF_T(m_bReady);

		try
		{
			if (!m_rsSN.empty())
				m_rsConfig.enable_device(m_rsSN);

			m_rsConfig.enable_stream(RS2_STREAM_POSE, RS2_FORMAT_6DOF);
			m_rsProfile = m_rsPipe.start(m_rsConfig);
			rs2::device dev = m_rsProfile.get_device();
			LOG_I("Device Name:" + string(dev.get_info(RS2_CAMERA_INFO_NAME)));
			LOG_I("Firmware Version:" + string(dev.get_info(RS2_CAMERA_INFO_FIRMWARE_VERSION)));
			LOG_I("Serial Number:" + string(dev.get_info(RS2_CAMERA_INFO_SERIAL_NUMBER)));
			LOG_I("Product Id:" + string(dev.get_info(RS2_CAMERA_INFO_PRODUCT_ID)));

			rs2::frameset rsFrameset = m_rsPipe.wait_for_frames();
		}
		catch (const rs2::camera_disconnected_error &e)
		{
			LOG_E("Realsense tracking disconnected");
			return false;
		}
		catch (const rs2::recoverable_error &e)
		{
			LOG_E("Realsense tracking open failed");
			return false;
		}
		catch (const rs2::error &e)
		{
			LOG_E("Realsense tracking error");
			return false;
		}
		catch (const std::exception &e)
		{
			LOG_E("Realsense tracking exception");
			return false;
		}

		m_bReady = true;
		m_bReset = false;
		return true;
	}

	void _RStracking::close(void)
	{
		IF_(!m_bReady);
		m_rsPipe.stop();
		m_bReady = false;
	}

	void _RStracking::hardwareReset(void)
	{
		rs2::device dev = m_rsProfile.get_device();
		dev.hardware_reset();
	}

	bool _RStracking::start(void)
	{
		NULL_F(m_pT);
		return m_pT->start(getUpdate, this);
	}

	void _RStracking::update(void)
	{
		while (m_pT->bRun())
		{
			if (!m_bReady)
			{
				if (!open())
				{
					LOG_E("Cannot open RealSense tracking");
					hardwareReset();
					m_pT->sleepT(SEC_2_USEC);
					continue;
				}

				resetAll();
			}

			if (m_bReset)
			{
				close();
				m_bReady = false;
				continue;
			}

			m_pT->autoFPSfrom();

			if (!updateRS())
			{
				hardwareReset();
				m_pT->sleepT(SEC_2_USEC);
				m_bReady = false;
			}

			m_pT->autoFPSto();
		}
	}

	bool _RStracking::updateRS(void)
	{
		try
		{
			auto frames = m_rsPipe.wait_for_frames();
			auto f = frames.first_or_default(RS2_STREAM_POSE);
			auto pose = f.as<rs2::pose_frame>().get_pose_data();

			*m_vT.v(m_vAxisIdx.x) = pose.translation.x;
			*m_vT.v(m_vAxisIdx.y) = pose.translation.y;
			*m_vT.v(m_vAxisIdx.z) = pose.translation.z;

			m_vQ.x = pose.rotation.x;
			m_vQ.y = pose.rotation.y;
			m_vQ.z = pose.rotation.z;
			m_vQ.w = pose.rotation.w;

			m_mR = Eigen::Quaternionf(
					   m_vQ.w,
					   m_vQ.x,
					   m_vQ.y,
					   m_vQ.z)
					   .toRotationMatrix();

			Matrix4f mT = Matrix4f::Identity();
			mT.block(0, 0, 3, 3) = m_mR;
			mT(0, 3) = m_vT.x;
			mT(1, 3) = m_vT.y;
			mT(2, 3) = m_vT.z;
			m_mT = mT;

			//TODO: calc with mR
			// float w = m_vQ.w;
			// float x = -m_vQ.z;
			// float y = m_vQ.x;
			// float z = -m_vQ.y;
			// const float ovP = 180.0 / OK_PI;
			// *m_vR.v(m_vAxisIdx.x) = -asin(2.0 * (x * z - w * y)) * ovP;								  //pitch
			// *m_vR.v(m_vAxisIdx.y) = atan2(2.0 * (w * x + y * z), w * w - x * x - y * y + z * z) * ovP; //roll
			// *m_vR.v(m_vAxisIdx.z) = atan2(2.0 * (w * z + x * y), w * w + x * x - y * y - z * z) * ovP; //yaw

			m_confidence = pose.tracker_confidence;
		}
		catch (const rs2::camera_disconnected_error &e)
		{
			LOG_E("Realsense tracking disconnected");
			return false;
		}
		catch (const rs2::recoverable_error &e)
		{
			LOG_E("Realsense tracking open failed");
			return false;
		}
		catch (const rs2::error &e)
		{
			LOG_E("Realsense tracking error");
			return false;
		}
		catch (const std::exception &e)
		{
			LOG_E("Realsense exception");
			return false;
		}

		return true;
	}

}
#endif
