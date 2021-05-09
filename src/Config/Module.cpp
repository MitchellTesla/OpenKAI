/*
 * Module.cpp
 *
 *  Created on: Nov 22, 2016
 *      Author: yankai
 */

#include "Module.h"

namespace kai
{

	Module::Module()
	{
	}

	Module::~Module()
	{
	}

	BASE *Module::createInstance(Kiss *pK)
	{
		IF_N(!pK);

		ADD_MODULE(_Console);
		ADD_MODULE(_Canbus);
		ADD_MODULE(_GPS);
		ADD_MODULE(_LeddarVu);
		ADD_MODULE(_TOFsense);
		ADD_MODULE(_Mavlink);
		ADD_MODULE(_StateControl);
		ADD_MODULE(_MOAB);
		ADD_MODULE(_Modbus);
		ADD_MODULE(_Object);
		ADD_MODULE(PID);
		ADD_MODULE(_Drive);
		ADD_MODULE(Destimator);
		ADD_MODULE(_ProtocolBase);
		ADD_MODULE(_SerialPort);
		ADD_MODULE(_TCPserver);
		ADD_MODULE(_TCPclient);
		ADD_MODULE(_UDP);
		ADD_MODULE(_Universe);
		ADD_MODULE(_WebSocket);

		ADD_MODULE(_ArduServo);
		ADD_MODULE(_ActuatorSync);
		ADD_MODULE(_Sequencer);
		ADD_MODULE(_OrientalMotor);
		ADD_MODULE(_DRV8825_RS485);
		ADD_MODULE(_S6H4D);
		ADD_MODULE(_StepperGripper);
		ADD_MODULE(_ZLAC8015);

		ADD_MODULE(_AP_base);
		ADD_MODULE(_AP_distLidar);
		ADD_MODULE(_AP_actuator);
		ADD_MODULE(_AP_mission);
		ADD_MODULE(_AP_posCtrl);
		ADD_MODULE(_AP_RTH);
		ADD_MODULE(_AP_relay);
		ADD_MODULE(_AP_servo);
		ADD_MODULE(_AP_takeoff);
		ADD_MODULE(_AP_goto);
		ADD_MODULE(_AP_gcs);
		ADD_MODULE(_AP_link);
		ADD_MODULE(_AProver_drive);

		ADD_MODULE(_AProver_UT);
		ADD_MODULE(_UTprArmL);
		ADD_MODULE(_AProver_UTfollowTag);
		ADD_MODULE(_AProver_KU);
		ADD_MODULE(_AProver_KUfollowTag);
		ADD_MODULE(_AProver_BR);
		ADD_MODULE(_AProver_BRfollow);
		ADD_MODULE(_AProver_tag);
		ADD_MODULE(_AProver_followTag);

		ADD_MODULE(_DroneBox);
		ADD_MODULE(_DroneBoxJSON);
		ADD_MODULE(_AP_droneBoxJSON);

#ifdef USE_OPENCL
		ADD_MODULE(clBase);
#endif

#ifdef USE_OPENCV
		ADD_MODULE(_ANR);
		ADD_MODULE(_Camera);
		ADD_MODULE(_CamCalib);
		ADD_MODULE(_Contrast);
		ADD_MODULE(_Crop);
		ADD_MODULE(_DNNclassifier);
		ADD_MODULE(_DNNdetect);
		ADD_MODULE(_DNNtext);
		ADD_MODULE(_Depth2Gray);
		ADD_MODULE(_DepthSegment);
		ADD_MODULE(_Erode);
		ADD_MODULE(_GPhoto);
		ADD_MODULE(_GStreamer);
		ADD_MODULE(_Grayscale);
		ADD_MODULE(_GDcam);
		ADD_MODULE(_HistEqualize);
		ADD_MODULE(_Invert);
		ADD_MODULE(_IRLock);
		ADD_MODULE(_InRange);
		ADD_MODULE(_ImgFile);
		ADD_MODULE(_Lane);
		ADD_MODULE(_Line);
		ADD_MODULE(_Mask);
		ADD_MODULE(_Morphology);
		ADD_MODULE(_OpenPose);
		ADD_MODULE(_HandKey);
		ADD_MODULE(_Raspivid);
		ADD_MODULE(_Resize);
		ADD_MODULE(_Remap);
		ADD_MODULE(_Rotate);
		ADD_MODULE(_RTCM3);
		ADD_MODULE(_SlideWindow);
		ADD_MODULE(_Thermal);
		ADD_MODULE(_Threshold);
		ADD_MODULE(_VideoFile);
		ADD_MODULE(_WindowCV);

		ADD_MODULE(_PickingArm);

		ADD_MODULE(_GDimgUploader);
		ADD_MODULE(_BBoxCutOut);
		ADD_MODULE(_CutOut);
		ADD_MODULE(_FrameCutOut);

		ADD_MODULE(_AP_avoid);
		ADD_MODULE(_AP_depthVision);
		ADD_MODULE(_AP_land);
		ADD_MODULE(_AP_follow);
		ADD_MODULE(_APcopter_photo);
		ADD_MODULE(_AProver_picking);

#ifdef USE_OPENCV_CONTRIB
		ADD_MODULE(_ArUco);
		ADD_MODULE(_MotionDetector);
		ADD_MODULE(_SingleTracker);
#endif

#ifdef USE_CUDA
		ADD_MODULE(_Bullseye);
		ADD_MODULE(_Cascade);
		ADD_MODULE(_DenseFlow);
#endif

#ifdef USE_REALSENSE
		ADD_MODULE(_AP_GPS);
		ADD_MODULE(_RealSense);
#endif

#ifdef USE_MYNTEYE
		ADD_MODULE(_Mynteye);
#endif

#ifdef USE_JETSON_INFERENCE
		//	ADD_MODULE(_ImageNet);
		ADD_MODULE(_DetectNet);
#endif

#ifdef USE_INNFOS
		ADD_MODULE(_InnfosGluon);
#endif

#ifdef USE_OCR
		ADD_MODULE(OCR);
#endif

#ifdef USE_OPENALPR
		ADD_MODULE(_OpenALPR);
#endif

#ifdef USE_CAFFE
		ADD_MODULE(_Caffe);
		ADD_MODULE(_CaffeRegressionTrain);
		ADD_MODULE(_CaffeRegressionInf);
#endif

#ifdef USE_DARKNET
		ADD_MODULE(_YOLO);
#endif

#ifdef USE_PYLON
		ADD_MODULE(_Pylon);
#endif

#ifdef USE_CHILITAGS
		ADD_MODULE(_Chilitags);
#endif

#ifdef USE_ORB_SLAM
		ADD_MODULE(_ORB_SLAM);
#endif

#endif //USE_OPENCV

#ifdef USE_OPEN3D
		ADD_MODULE(_PCstream);
		ADD_MODULE(_PCframe);
		ADD_MODULE(_PClattice);
		ADD_MODULE(_PCfile);
		ADD_MODULE(_PCmerge);
		ADD_MODULE(_PCviewer);
		ADD_MODULE(_PCsend);
		ADD_MODULE(_PCrecv);
		ADD_MODULE(_PCtransform);
		ADD_MODULE(_PCcrop);
		ADD_MODULE(_PCremove);
		ADD_MODULE(_PCdownSample);
		ADD_MODULE(_PCregistCol);
		ADD_MODULE(_PCregistICP);
		ADD_MODULE(_PCregistGlobal);
		ADD_MODULE(_PCscan);
		ADD_MODULE(_PCcalib);
#ifdef USE_REALSENSE
		ADD_MODULE(_PCrs);
#endif
#endif

#ifdef USE_HPS3D
		ADD_MODULE(_HPS3D);
#endif

#ifdef USE_REALSENSE
		ADD_MODULE(_RStracking);
#endif

#ifdef USE_LIVOX
		ADD_MODULE(_Livox);
		ADD_MODULE(LivoxLidar);
#endif

#ifdef USE_DYNAMIXEL
#endif

#ifdef USE_XARM
		ADD_MODULE(_xArm);
#endif

#ifdef USE_GUI
		ADD_MODULE(_WindowGUI);
#endif

		return NULL;
	}

	template <typename T>
	BASE *Module::createInst(Kiss *pKiss)
	{
		IF_N(!pKiss);

		T *pInst = new T();
		return pInst;
	}

}
