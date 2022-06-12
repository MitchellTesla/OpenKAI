/*
 * Module.h
 *
 *  Created on: Nov 22, 2016
 *      Author: root
 */

#ifndef OpenKAI_src_Config_Module_H_
#define OpenKAI_src_Config_Module_H_

#include "../Base/common.h"
#include "../Base/BASE.h"
#include "../Script/Kiss.h"

#include "../Arithmetic/Destimator.h"
#include "../UI/_Console.h"

#ifdef WITH_ACTUATOR
#include "../Actuator/_ArduServo.h"
#include "../Actuator/_ActuatorSync.h"
#include "../Actuator/_OrientalMotor.h"
#include "../Actuator/_HYMCU_RS485.h"
#include "../Actuator/Articulated/_S6H4D.h"
#include "../Actuator/_StepperGripper.h"
#include "../Actuator/Motor/_ZLAC8015.h"
#include "../Actuator/Motor/_ZLAC8015D.h"
#ifdef USE_INNFOS
#include "../Actuator/_InnfosGluon.h"
#endif
#ifdef USE_DYNAMIXEL
#include "../Actuator/Articulated/_LabArm.h"
#endif
#ifdef USE_XARM
#include "../Actuator/Articulated/_xArm.h"
#endif
#endif

#ifdef WITH_ARDUPILOT
#include "../Autopilot/ArduPilot/_AP_base.h"
#include "../Autopilot/ArduPilot/_AP_actuator.h"
#include "../Autopilot/ArduPilot/_AP_distLidar.h"
#include "../Autopilot/ArduPilot/_AP_goto.h"
#include "../Autopilot/ArduPilot/_AP_gcs.h"
#include "../Autopilot/ArduPilot/_AP_mission.h"
#include "../Autopilot/ArduPilot/_AP_posCtrl.h"
#include "../Autopilot/ArduPilot/_AP_RTH.h"
#include "../Autopilot/ArduPilot/_AP_relay.h"
#include "../Autopilot/ArduPilot/_AP_servo.h"
#include "../Autopilot/ArduPilot/_AP_takeoff.h"
#include "../Autopilot/ArduPilot/_AP_link.h"
#include "../Autopilot/ArduPilot/_AProver_drive.h"
#ifdef USE_OPENCV
#include "../Autopilot/ArduPilot/_AP_avoid.h"
#include "../Autopilot/ArduPilot/_AP_depthVision.h"
#include "../Autopilot/ArduPilot/_AP_land.h"
#include "../Autopilot/ArduPilot/_AP_follow.h"
#include "../Autopilot/ArduPilot/_AP_video.h"
#include "../Autopilot/ArduPilot/_APcopter_photo.h"
#ifdef WITH_APP_ROBOTARM
#include "../Autopilot/ArduPilot/_AProver_picking.h"
#endif
#ifdef WITH_NAVIGATION
#ifdef USE_REALSENSE
#include "../Autopilot/ArduPilot/_AP_GPS.h"
#endif
#endif
#endif
#endif

#ifdef WITH_AUTOPILOT_DRIVE
#include "../Autopilot/Drive/_Drive.h"
#endif

#ifdef WITH_APP_MULTI3DSCAN
#endif

#ifdef WITH_APP_BENCHROVER
#include "../Application/BenchRover/_AProver_BR.h"
#include "../Application/BenchRover/_AProver_BRfollow.h"
#endif

#ifdef WITH_APP_CAMCALIB
#include "../Application/CamCalib/_CamCalib.h"
#endif

#ifdef WITH_APP_DEPTHCAM
#include "../Application/DepthCam/_DepthCam.h"
#endif

#ifdef WITH_APP_DRONEBOX
#include "../Application/DroneBox/_DroneBox.h"
#include "../Application/DroneBox/_DroneBoxJSON.h"
#include "../Application/DroneBox/_AP_droneBoxJSON.h"
#endif

#ifdef WITH_APP_KUPHENOROVER
#include "../Application/KUphenoRover/_AProver_KU.h"
#include "../Application/KUphenoRover/_AProver_KUfollowTag.h"
#endif

#ifdef WITH_APP_MEASUREMENT && USE_OPENCV
#include "../Application/Measurement/_ARmeasure.h"
#include "../Application/Measurement/_ARmeasureVertex.h"
#include "../Application/Measurement/_ARmeasureFree.h"
#include "../Application/Measurement/_ARmeasureCalibCam.h"
#include "../Application/Measurement/_ARmeasureCalibDofs.h"
#include "../Application/Measurement/_RaspiWSbattery.h"
#endif

#ifdef WITH_APP_3DSCAN && USE_OPEN3D
#include "../Application/3Dscan/_3DScanCalibCam.h"
#include "../Application/3Dscan/_3DScanCalibOfs.h"
#include "../Application/3Dscan/_PCscan.h"
#include "../Application/3Dscan/_PCcalib.h"
#endif

#ifdef WITH_APP_ROBOTARM
#include "../Application/RobotArm/_Sequencer.h"
#ifdef USE_OPENCV
#include "../Application/RobotArm/_PickingArm.h"
#endif
#endif

#ifdef WITH_APP_ROVER
#include "../Application/Rover/_PWMrover.h"
#include "../Application/Rover/_RCrover.h"
#endif

#ifdef WITH_APP_SURVEILLANCE && USE_OPENCV
#include "../Application/Surveillance/_ANR.h"
#include "../Application/Surveillance/_GDcam.h"
#endif

#ifdef WITH_APP_TAGROVER
#include "../Application/TagRover/_AProver_tag.h"
#include "../Application/TagRover/_AProver_followTag.h"
#endif

#ifdef WITH_APP_UTPHENOROVER
#include "../Application/UTphenoRover/_AProver_UT.h"
#include "../Application/UTphenoRover/_UTprArmL.h"
#include "../Application/UTphenoRover/_AProver_UTfollowTag.h"
#endif

#ifdef WITH_COMPUTE
#ifdef USE_OPENCL
#include "../Compute/OpenCL/clBase.h"
#endif
#endif

#ifdef WITH_CONTROL
#include "../Control/PID.h"
#endif

#ifdef WITH_DATA
#include "../Data/Image/_GDimgUploader.h"
#include "../Data/Image/_BBoxCutOut.h"
#include "../Data/Image/_CutOut.h"
#include "../Data/Video/_FrameCutOut.h"
#endif

#ifdef WITH_DETECTOR
#ifdef USE_OPENCV
#include "../Detector/_Lane.h"
#include "../Detector/_DNNdetect.h"
#include "../Detector/_DNNclassifier.h"
#include "../Detector/_DNNtext.h"
#include "../Detector/_DepthSegment.h"
#include "../Detector/_IRLock.h"
#include "../Detector/_Line.h"
#include "../Detector/_OpenPose.h"
#include "../Detector/_HandKey.h"
#include "../Detector/_SlideWindow.h"
#include "../Detector/_Thermal.h"
#ifdef USE_OPENCV_CONTRIB
#include "../Detector/_ArUco.h"
#include "../Detector/_MotionDetector.h"
#include "../Tracker/_SingleTracker.h"
#endif
#ifdef USE_CUDA
#include "../Detector/_Bullseye.h"
#include "../Detector/_Cascade.h"
#endif
#ifdef USE_OCR
#include "../Detector/OCR.h"
#endif
#ifdef USE_OPENALPR
#include "../Detector/_OpenALPR.h"
#endif
#ifdef USE_CHILITAGS
#include "../Detector/_Chilitags.h"
#endif
#endif
#endif

#ifdef WITH_DNN
#ifdef USE_OPENCV
#ifdef USE_JETSON_INFERENCE
#include "../DNN/JetsonInference/_ImageNet.h"
#include "../DNN/JetsonInference/_DetectNet.h"
#endif
#ifdef USE_CAFFE
#include "../DNN/Caffe/_Caffe.h"
#include "../Regression/_CaffeRegressionTrain.h"
#include "../Regression/_CaffeRegressionInf.h"
#endif
#ifdef USE_DARKNET
#include "../DNN/Darknet/_YOLO.h"
#endif
#endif
#endif

#ifdef WITH_FILTER
#endif

#ifdef WITH_GCS
#endif

#ifdef WITH_IO
#include "../IO/_File.h"
#include "../IO/_TCPserver.h"
#include "../IO/_TCPclient.h"
#include "../IO/_SerialPort.h"
#include "../IO/_UDP.h"
#include "../IO/_WebSocket.h"
#endif

#ifdef WITH_NAVIGATION
#include "../Navigation/_GPS.h"
#include "../Navigation/_RTCM3.h"
#ifdef USE_REALSENSE
#include "../Navigation/_RStracking.h"
#endif
#endif

#ifdef WITH_NET
#include "../Net/_Curl.h"
#endif

#ifdef WITH_3D
#ifdef USE_OPEN3D
#include "../3D/_GeometryViewer.h"
#include "../3D/Mesh/_MeshStream.h"
#include "../3D/PointCloud/_PCstream.h"
#include "../3D/PointCloud/_PCframe.h"
#include "../3D/PointCloud/_PClattice.h"
#include "../3D/PointCloud/_PCfile.h"
#include "../3D/PointCloud/_PCmerge.h"
#include "../3D/PointCloud/_PCsend.h"
#include "../3D/PointCloud/_PCrecv.h"
#include "../3D/PointCloud/PCfilter/_PCtransform.h"
#include "../3D/PointCloud/PCfilter/_PCcrop.h"
#include "../3D/PointCloud/PCfilter/_PCremove.h"
#include "../3D/PointCloud/PCfilter/_PCdownSample.h"
#include "../3D/PointCloud/PCregistration/_PCregistCol.h"
#include "../3D/PointCloud/PCregistration/_PCregistICP.h"
#include "../3D/PointCloud/PCregistration/_PCregistGlobal.h"
#ifdef USE_REALSENSE
#include "../3D/PointCloud/_PCrs.h"
#endif
#ifdef USE_HPS3D
#include "../3D/PointCloud/_HPS3D.h"
#endif
#endif
#endif

#ifdef WITH_PROTOCOL
#include "../Protocol/_Canbus.h"
#include "../Protocol/_Mavlink.h"
#include "../Protocol/_MOAB.h"
#include "../Protocol/_PWMio.h"
#include "../Protocol/_ProtocolBase.h"
#include "../Protocol/_Modbus.h"
#include "../Protocol/_SBus.h"
#endif

// #ifdef WITH_SCIENCE
// #include "../Science/_Solver.h"
// #include "../Science/_SolverBase.h"
// #ifdef USE_MATHGL
// #include "../Science/_FourierSeries.h"
// #endif
// #endif

#ifdef WITH_SENSOR
#include "../Sensor/_LeddarVu.h"
#include "../Sensor/_TOFsense.h"
#include "../Sensor/_BenewakeTF.h"
#ifdef USE_LIVOX
#include "../Sensor/Livox/_Livox.h"
#include "../Sensor/Livox/LivoxLidar.h"
#endif
#endif

#ifdef WITH_SLAM
#ifdef USE_OPENCV
#ifdef USE_ORB_SLAM
#include "../SLAM/_ORB_SLAM.h"
#endif
#endif
#endif

#ifdef WITH_STATE
#include "../State/_StateControl.h"
#endif

#ifdef WITH_UI
#ifdef USE_OPENCV
#include "../UI/_GstOutput.h"
#include "../UI/_WindowCV.h"
#endif
#ifdef USE_GUI
#include "../UI/_WindowGUI.h"
#endif
#endif

#ifdef WITH_UNIVERSE
#include "../Universe/_Object.h"
#include "../Universe/_Universe.h"
#endif

#ifdef WITH_VISION
#ifdef USE_OPENCV
#include "../Vision/_Camera.h"
#include "../Vision/_VideoFile.h"
#include "../Vision/_ImgFile.h"
#include "../Vision/_GStreamer.h"
#include "../Vision/_Raspivid.h"
#include "../Vision/_GPhoto.h"
#include "../Vision/ImgFilter/_Morphology.h"
#include "../Vision/ImgFilter/_Mask.h"
#include "../Vision/ImgFilter/_Threshold.h"
#include "../Vision/ImgFilter/_Contrast.h"
#include "../Vision/ImgFilter/_Crop.h"
#include "../Vision/ImgFilter/_Depth2Gray.h"
#include "../Vision/ImgFilter/_Erode.h"
#include "../Vision/ImgFilter/_Grayscale.h"
#include "../Vision/ImgFilter/_HistEqualize.h"
#include "../Vision/ImgFilter/_Invert.h"
#include "../Vision/ImgFilter/_InRange.h"
#include "../Vision/ImgFilter/_Resize.h"
#include "../Vision/ImgFilter/_Remap.h"
#include "../Vision/ImgFilter/_Rotate.h"
#ifdef USE_CUDA
#include "../Vision/_DenseFlow.h"
#endif
#ifdef USE_MYNTEYE
#include "../Vision/_Mynteye.h"
#endif
#ifdef USE_REALSENSE
#include "../Vision/_RealSense.h"
#include "../Vision/ImgFilter/_RSdepth.h"
#endif
#ifdef USE_PYLON
#include "../Vision/_Pylon.h"
#endif
#endif
#endif


#define ADD_MODULE(x)             \
	if (pK->m_class == #x)        \
	{                             \
		return createInst<x>(pK); \
	}

namespace kai
{

	class Module
	{
	public:
		Module();
		virtual ~Module();
		BASE *createInstance(Kiss *pK);

	private:
		template <typename T>
		BASE *createInst(Kiss *pKiss);
	};

}

#endif
