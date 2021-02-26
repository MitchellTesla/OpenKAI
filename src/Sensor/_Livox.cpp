/*
 *  Created on: Aug 21, 2019
 *      Author: yankai
 */
#include "_Livox.h"

#ifdef USE_LIVOX

namespace kai
{

// For callback use
_Livox* g_Livox = NULL;

_Livox::_Livox()
{
    m_bOpen = false;

    auto_connect_mode_ = true;
    whitelist_count_   = 0;
    is_initialized_    = false;

    lidar_count_       = 0;
    memset ( broadcast_code_whitelist_, 0, sizeof ( broadcast_code_whitelist_ ) );

    memset ( lidars_, 0, sizeof ( lidars_ ) );
    for ( uint32_t i=0; i<kMaxLidarCount; i++ )
    {
        lidars_[i].handle = kMaxLidarCount;
        /** Unallocated state */
        lidars_[i].connect_state = kConnectStateOff;
    }

}

_Livox::~_Livox()
{
}

bool _Livox::init ( void* pKiss )
{
    IF_F ( !this->_ModuleBase::init ( pKiss ) );
    Kiss* pK = ( Kiss* ) pKiss;

    pK->a("vBroadcastCode", &m_vBroadcastCode );

    return true;
}

bool _Livox::open ( void )
{
    if ( is_initialized_ )
    {
        printf ( "LiDAR data source is already inited!\n" );
        return true;
    }

    if ( !Init() )
    {
        Uninit();
        printf ( "Livox-SDK init fail!\n" );
        return false;
    }

    LivoxSdkVersion _sdkversion;
    GetLivoxSdkVersion ( &_sdkversion );
    printf ( "Livox SDK version %d.%d.%d\n", _sdkversion.major, _sdkversion.minor, _sdkversion.patch );

    SetBroadcastCallback ( _Livox::OnDeviceBroadcast );
    SetDeviceStateUpdateCallback ( _Livox::OnDeviceChange );

    /** Add commandline input broadcast code */
    for ( auto input_str : m_vBroadcastCode )
    {
        _Livox::AddBroadcastCodeToWhitelist ( input_str.c_str() );
    }

    /** Add local broadcast code */
    _Livox::AddLocalBroadcastCode();

    if ( whitelist_count_ )
    {
        _Livox::DisableAutoConnectMode();
        printf ( "Disable auto connect mode!\n" );

        printf ( "List all broadcast code in whiltelist:\n" );
        for ( uint32_t i=0; i<whitelist_count_; i++ )
        {
            printf ( "%s\n", broadcast_code_whitelist_[i] );
        }
    }
    else
    {
        _Livox::EnableAutoConnectMode();
        printf ( "No broadcast code was added to whitelist, swith to automatic connection mode!\n" );
    }

    /** Start livox sdk to receive lidar data */
    if ( !Start() )
    {
        Uninit();
        printf ( "Livox-SDK init fail!\n" );
        return -1;
    }

    /** Add here, only for callback use */
    if ( !g_Livox )
    {
        g_Livox = this;
    }
    is_initialized_= true;
    printf ( "Livox-SDK init success!\n" );

    return true;
}

void _Livox::close ( void )
{
    if ( !is_initialized_ )
    {
        printf ( "LiDAR data source is not exit" );
        return;
    }

    Uninit();
    printf ( "Livox SDK Deinit completely!\n" );
}

bool _Livox::start ( void )
{
    NULL_F ( m_pT );
    return m_pT->start ( getUpdate, this );
}

void _Livox::update ( void )
{
    while ( m_pT->bRun() )
    {
        if ( !m_bOpen )
        {
            if ( !open() )
            {
                m_pT->sleepT ( USEC_1SEC );
                continue;
            }
        }

        m_pT->autoFPSfrom();

        updateLidar();

        m_pT->autoFPSto();
    }
}

bool _Livox::updateLidar ( void )
{
    return true;
}

void _Livox::draw ( void )
{
    this->_ModuleBase::draw();
    string msg;

}

// Static function in _Livox for callback or event process

/** Receiving point cloud data from Livox LiDAR. */
void _Livox::GetLidarDataCb ( uint8_t handle, LivoxEthPacket *data,
                              uint32_t data_num, void *client_data )
{
    using namespace std;

    _Livox* lidar_this = static_cast<_Livox *> ( client_data );
    LivoxEthPacket* eth_packet = data;

    if ( !data || !data_num || ( handle >= kMaxLidarCount ) )
    {
        return;
    }

    if ( eth_packet )
    {
        lidar_this->data_recveive_count_[handle] ++;
        if ( lidar_this->data_recveive_count_[handle] % 100 == 0 )
        {
            printf ( "receive packet count %d %d\n", handle, lidar_this->data_recveive_count_[handle] );

            /** Parsing the timestamp and the point cloud data. */
            uint64_t cur_timestamp = * ( ( uint64_t * ) ( data->timestamp ) );
            if ( data ->data_type == kCartesian )
            {
                LivoxRawPoint *p_point_data = ( LivoxRawPoint * ) data->data;
            }
            else if ( data ->data_type == kSpherical )
            {
                LivoxSpherPoint *p_point_data = ( LivoxSpherPoint * ) data->data;
            }
            else if ( data ->data_type == kExtendCartesian )
            {
                LivoxExtendRawPoint *p_point_data = ( LivoxExtendRawPoint * ) data->data;
            }
            else if ( data ->data_type == kExtendSpherical )
            {
                LivoxExtendSpherPoint *p_point_data = ( LivoxExtendSpherPoint * ) data->data;
            }
            else if ( data ->data_type == kDualExtendCartesian )
            {
                LivoxDualExtendRawPoint *p_point_data = ( LivoxDualExtendRawPoint * ) data->data;
            }
            else if ( data ->data_type == kDualExtendSpherical )
            {
                LivoxDualExtendSpherPoint *p_point_data = ( LivoxDualExtendSpherPoint * ) data->data;
            }
            else if ( data ->data_type == kImu )
            {
                LivoxImuPoint *p_point_data = ( LivoxImuPoint * ) data->data;
            }
            else if ( data ->data_type == kTripleExtendCartesian )
            {
                LivoxTripleExtendRawPoint *p_point_data = ( LivoxTripleExtendRawPoint * ) data->data;
            }
            else if ( data ->data_type == kTripleExtendSpherical )
            {
                LivoxTripleExtendSpherPoint *p_point_data = ( LivoxTripleExtendSpherPoint * ) data->data;
            }
        }
    }
}

void _Livox::OnDeviceBroadcast ( const BroadcastDeviceInfo *info )
{
    if ( info == nullptr )
    {
        return;
    }

    if ( info->dev_type == kDeviceTypeHub )
    {
        printf ( "In lidar mode, couldn't connect a hub : %s\n", info->broadcast_code );
        return;
    }

    if ( g_Livox->IsAutoConnectMode() )
    {
        printf ( "In automatic connection mode, will connect %s\n", info->broadcast_code );
    }
    else
    {
        if ( !g_Livox->FindInWhitelist ( info->broadcast_code ) )
        {
            printf ( "Not in the whitelist, please add %s to if want to connect!\n",\
                     info->broadcast_code );
            return;
        }
    }

    livox_status result = kStatusFailure;
    uint8_t handle = 0;
    result = AddLidarToConnect ( info->broadcast_code, &handle );
    if ( result == kStatusSuccess && handle < kMaxLidarCount )
    {
        SetDataCallback ( handle, _Livox::GetLidarDataCb, ( void * ) g_Livox );
        LidarDevice* p_lidar = & ( g_Livox->lidars_[handle] );
        p_lidar->handle = handle;
        p_lidar->connect_state = kConnectStateOff;
        p_lidar->config.enable_fan = true;
        p_lidar->config.return_mode = kStrongestReturn;
        p_lidar->config.coordinate = kCoordinateCartesian;
        p_lidar->config.imu_rate = kImuFreq200Hz;
    }
    else
    {
        printf ( "Add lidar to connect is failed : %d %d \n", result, handle );
    }
}

/** Callback function of changing of device state. */
void _Livox::OnDeviceChange ( const DeviceInfo *info, DeviceEvent type )
{
    if ( info == nullptr )
    {
        return;
    }

    uint8_t handle = info->handle;
    if ( handle >= kMaxLidarCount )
    {
        return;
    }

    LidarDevice* p_lidar = & ( g_Livox->lidars_[handle] );
    if ( type == kEventConnect )
    {
        QueryDeviceInformation ( handle, DeviceInformationCb, g_Livox );
        if ( p_lidar->connect_state == kConnectStateOff )
        {
            p_lidar->connect_state = kConnectStateOn;
            p_lidar->info = *info;
        }
        printf ( "[WARNING] Lidar sn: [%s] Connect!!!\n", info->broadcast_code );
    }
    else if ( type == kEventDisconnect )
    {
        p_lidar->connect_state = kConnectStateOff;
        printf ( "[WARNING] Lidar sn: [%s] Disconnect!!!\n", info->broadcast_code );
    }
    else if ( type == kEventStateChange )
    {
        p_lidar->info = *info;
        printf ( "[WARNING] Lidar sn: [%s] StateChange!!!\n", info->broadcast_code );
    }

    if ( p_lidar->connect_state == kConnectStateOn )
    {
        printf ( "Device Working State %d\n", p_lidar->info.state );
        if ( p_lidar->info.state == kLidarStateInit )
        {
            printf ( "Device State Change Progress %u\n", p_lidar->info.status.progress );
        }
        else
        {
            printf ( "Device State Error Code 0X%08x\n", p_lidar->info.status.status_code.error_code );
        }
        printf ( "Device feature %d\n", p_lidar->info.feature );
        SetErrorMessageCallback ( handle, _Livox::LidarErrorStatusCb );

        /** Config lidar parameter */
        if ( p_lidar->info.state == kLidarStateNormal )
        {
            if ( p_lidar->config.coordinate != 0 )
            {
                SetSphericalCoordinate ( handle, _Livox::SetCoordinateCb, g_Livox );
            }
            else
            {
                SetCartesianCoordinate ( handle, _Livox::SetCoordinateCb, g_Livox );
            }
            p_lidar->config.set_bits |= kConfigCoordinate;

            if ( kDeviceTypeLidarMid40 != info->type )
            {
                LidarSetPointCloudReturnMode ( handle, ( PointCloudReturnMode ) ( p_lidar->config.return_mode ),\
                                               _Livox::SetPointCloudReturnModeCb, g_Livox );
                p_lidar->config.set_bits |= kConfigReturnMode;
            }

            if ( kDeviceTypeLidarMid40 != info->type && kDeviceTypeLidarMid70 != info->type )
            {
                LidarSetImuPushFrequency ( handle, ( ImuFreq ) ( p_lidar->config.imu_rate ),\
                                           _Livox::SetImuRatePushFrequencyCb, g_Livox );
                p_lidar->config.set_bits |= kConfigImuRate;
            }

            p_lidar->connect_state = kConnectStateConfig;
        }
    }
}

/** Query the firmware version of Livox LiDAR. */
void _Livox::DeviceInformationCb ( livox_status status, uint8_t handle, \
                                   DeviceInformationResponse *ack, void *client_data )
{
    if ( status != kStatusSuccess )
    {
        printf ( "Device Query Informations Failed : %d\n", status );
    }
    if ( ack )
    {
        printf ( "firm ver: %d.%d.%d.%d\n",
                 ack->firmware_version[0],
                 ack->firmware_version[1],
                 ack->firmware_version[2],
                 ack->firmware_version[3] );
    }
}

/** Callback function of Lidar error message. */
void _Livox::LidarErrorStatusCb ( livox_status status, uint8_t handle, ErrorMessage *message )
{
    static uint32_t error_message_count = 0;
    if ( message != NULL )
    {
        ++error_message_count;
        if ( 0 == ( error_message_count % 100 ) )
        {
            printf ( "handle: %u\n", handle );
            printf ( "temp_status : %u\n", message->lidar_error_code.temp_status );
            printf ( "volt_status : %u\n", message->lidar_error_code.volt_status );
            printf ( "motor_status : %u\n", message->lidar_error_code.motor_status );
            printf ( "dirty_warn : %u\n", message->lidar_error_code.dirty_warn );
            printf ( "firmware_err : %u\n", message->lidar_error_code.firmware_err );
            printf ( "pps_status : %u\n", message->lidar_error_code.device_status );
            printf ( "fan_status : %u\n", message->lidar_error_code.fan_status );
            printf ( "self_heating : %u\n", message->lidar_error_code.self_heating );
            printf ( "ptp_status : %u\n", message->lidar_error_code.ptp_status );
            printf ( "time_sync_status : %u\n", message->lidar_error_code.time_sync_status );
            printf ( "system_status : %u\n", message->lidar_error_code.system_status );
        }
    }
}

void _Livox::ControlFanCb ( livox_status status, uint8_t handle, \
                            uint8_t response, void *client_data )
{

}

void _Livox::SetPointCloudReturnModeCb ( livox_status status, uint8_t handle, \
        uint8_t response, void *client_data )
{
    _Livox* lds_lidar = static_cast<_Livox *> ( client_data );

    if ( handle >= kMaxLidarCount )
    {
        return;
    }
    LidarDevice* p_lidar = & ( lds_lidar->lidars_[handle] );

    if ( status == kStatusSuccess )
    {
        p_lidar->config.set_bits &= ~ ( ( uint32_t ) ( kConfigReturnMode ) );
        printf ( "Set return mode success!\n" );

        if ( !p_lidar->config.set_bits )
        {
            LidarStartSampling ( handle, _Livox::StartSampleCb, lds_lidar );
            p_lidar->connect_state = kConnectStateSampling;
        }
    }
    else
    {
        LidarSetPointCloudReturnMode ( handle, ( PointCloudReturnMode ) ( p_lidar->config.return_mode ),\
                                       _Livox::SetPointCloudReturnModeCb, lds_lidar );
        printf ( "Set return mode fail, try again!\n" );
    }
}

void _Livox::SetCoordinateCb ( livox_status status, uint8_t handle, \
                               uint8_t response, void *client_data )
{
    _Livox* lds_lidar = static_cast<_Livox *> ( client_data );

    if ( handle >= kMaxLidarCount )
    {
        return;
    }
    LidarDevice* p_lidar = & ( lds_lidar->lidars_[handle] );

    if ( status == kStatusSuccess )
    {
        p_lidar->config.set_bits &= ~ ( ( uint32_t ) ( kConfigCoordinate ) );
        printf ( "Set coordinate success!\n" );

        if ( !p_lidar->config.set_bits )
        {
            LidarStartSampling ( handle, _Livox::StartSampleCb, lds_lidar );
            p_lidar->connect_state = kConnectStateSampling;
        }
    }
    else
    {
        if ( p_lidar->config.coordinate != 0 )
        {
            SetSphericalCoordinate ( handle, _Livox::SetCoordinateCb, lds_lidar );
        }
        else
        {
            SetCartesianCoordinate ( handle, _Livox::SetCoordinateCb, lds_lidar );
        }

        printf ( "Set coordinate fail, try again!\n" );
    }
}

void _Livox::SetImuRatePushFrequencyCb ( livox_status status, uint8_t handle, \
        uint8_t response, void *client_data )
{
    _Livox* lds_lidar = static_cast<_Livox *> ( client_data );

    if ( handle >= kMaxLidarCount )
    {
        return;
    }
    LidarDevice* p_lidar = & ( lds_lidar->lidars_[handle] );

    if ( status == kStatusSuccess )
    {
        p_lidar->config.set_bits &= ~ ( ( uint32_t ) ( kConfigImuRate ) );
        printf ( "Set imu rate success!\n" );

        if ( !p_lidar->config.set_bits )
        {
            LidarStartSampling ( handle, _Livox::StartSampleCb, lds_lidar );
            p_lidar->connect_state = kConnectStateSampling;
        }

    }
    else
    {
        LidarSetImuPushFrequency ( handle, ( ImuFreq ) ( p_lidar->config.imu_rate ),\
                                   _Livox::SetImuRatePushFrequencyCb, lds_lidar );
        printf ( "Set imu rate fail, try again!\n" );
    }
}

/** Callback function of starting sampling. */
void _Livox::StartSampleCb ( livox_status status, uint8_t handle, \
                             uint8_t response, void *client_data )
{
    _Livox* lds_lidar = static_cast<_Livox *> ( client_data );

    if ( handle >= kMaxLidarCount )
    {
        return;
    }

    LidarDevice* p_lidar = & ( lds_lidar->lidars_[handle] );
    if ( status == kStatusSuccess )
    {
        if ( response != 0 )
        {
            p_lidar->connect_state = kConnectStateOn;
            printf ( "Lidar start sample fail : state[%d] handle[%d] res[%d]\n", \
                     status, handle, response );
        }
        else
        {
            printf ( "Lidar start sample success\n" );
        }
    }
    else if ( status == kStatusTimeout )
    {
        p_lidar->connect_state = kConnectStateOn;
        printf ( "Lidar start sample timeout : state[%d] handle[%d] res[%d]\n", \
                 status, handle, response );
    }
}

/** Callback function of stopping sampling. */
void _Livox::StopSampleCb ( livox_status status, uint8_t handle, \
                            uint8_t response, void *client_data )
{
}

/** Add broadcast code to whitelist */
int _Livox::AddBroadcastCodeToWhitelist ( const char* bd_code )
{
    if ( !bd_code || ( strlen ( bd_code ) > kBroadcastCodeSize ) || \
            ( whitelist_count_ >= kMaxLidarCount ) )
    {
        return -1;
    }

    if ( _Livox::FindInWhitelist ( bd_code ) )
    {
        printf ( "%s is alrealy exist!\n", bd_code );
        return -1;
    }

    strcpy ( broadcast_code_whitelist_[whitelist_count_], bd_code );
    ++whitelist_count_;

    return 0;
}

void _Livox::AddLocalBroadcastCode ( void )
{
    for ( size_t i=0; i<sizeof ( m_vBroadcastCode ) /sizeof ( intptr_t ); ++i )
    {
        std::string invalid_bd = "000000000";
        printf ( "Local broadcast code : %s\n", m_vBroadcastCode[i].c_str() );
        if ( ( kBroadcastCodeSize == m_vBroadcastCode[i].length() + 1 ) && \
                ( nullptr == strstr ( m_vBroadcastCode[i].c_str(), invalid_bd.c_str() ) ) )
        {
            _Livox::AddBroadcastCodeToWhitelist ( m_vBroadcastCode[i].c_str() );
        }
        else
        {
            printf ( "Invalid local broadcast code : %s\n", m_vBroadcastCode[i].c_str() );
        }
    }
}

bool _Livox::FindInWhitelist ( const char* bd_code )
{
    if ( !bd_code )
    {
        return false;
    }

    for ( uint32_t i=0; i<whitelist_count_; i++ )
    {
        if ( strncmp ( bd_code, broadcast_code_whitelist_[i], kBroadcastCodeSize ) == 0 )
        {
            return true;
        }
    }

    return false;
}

}
#endif

