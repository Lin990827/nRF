#include "user_ble.h"
#include "app_error.h"
#include "app_scheduler.h"
#include "ble_bas.h"
#include "ble_conn_params.h"
#include "ble_gap.h"
#include "ble_gatt.h"
#include "ble_hrs.h"
#include "led.h"
#include "nrf_ble_gatt.h"
#include "nrf_ble_qwr.h"
#include "nrf_sdh.h"
#include "nrf_sdh_ble.h"
#include "peer_manager.h"
#include "peer_manager_handler.h"
#include <string.h>
#define NRF_LOG_MODULE_NAME use_ble
#define NRF_LOG_LEVEL       4 /* 0:Off 1:Error 2:Warning 3:Info 4:Debug */
#define NRF_LOG_INFO_COLOR  4 /* 0:Default 1:Black 2:Red 3:Green 4:Yellow 5:Blue 6:Magenta 7:Cyan 8:White */
#define NRF_LOG_DEBUG_COLOR 8 /* 0:Default 1:Black 2:Red 3:Green 4:Yellow 5:Blue 6:Magenta 7:Cyan 8:White */
#include "nrf_log.h"
NRF_LOG_MODULE_REGISTER();

NRF_BLE_GATT_DEF(ble_gatt); /* 定义一个 GATT 实例 */
NRF_BLE_QWR_DEF(queued_write); /* 定义一个队列写实例 */
BLE_BAS_DEF(ble_battery); /* 定义一个电池服务实例 */
BLE_HRS_DEF(ble_heart_rate); /* 定义一个心率服务实例 */

static uint16_t           conn_handle = BLE_CONN_HANDLE_INVALID; /* 当前连接句柄 */
static uint8_t            adv_handle  = BLE_GAP_ADV_SET_HANDLE_NOT_SET; /* 广播句柄 */
static ble_gap_adv_data_t enc_adv_data; /* 编码广播数据 */
/* clang-format off */
static uint8_t adv_data[] = {0x03, BLE_GAP_AD_TYPE_APPEARANCE, 0x00, 0x00,
							 0x02, BLE_GAP_AD_TYPE_FLAGS, 0x06,
							 0x03, BLE_GAP_AD_TYPE_16BIT_SERVICE_UUID_COMPLETE, 0x0A, 0x18,
							 0x10, BLE_GAP_AD_TYPE_COMPLETE_LOCAL_NAME, 0x4E, 0x6F, 0x72, 0x64, 0x69, 0x63, 0x5F, 0x54, 0x65, 0x6D, 0x70, 0x6C, 0x61, 0x74, 0x65}; /* 广播数据 */
/* clang-format on */

static void ble_event_handler(ble_evt_t const *p_ble_evt, void *p_context); /* BLE 事件处理 */
static void ble_qwr_error_handler(uint32_t nrf_error); /* BLE 队列写错误处理 */
static void ble_connect_param_event_handler(ble_conn_params_evt_t *p_evt); /* BLE 连接参数事件处理 */
static void ble_connect_param_error_handler(uint32_t nrf_error); /* BLE 连接参数错误处理 */
static void peer_manger_event_handler(pm_evt_t const *p_event); /* 对等管理器处理 */

/**@brief BLE 协议栈初始化函数
 *
 * @param[in]  None
 *
 * @retval     None
 *
 * @note None
 * @attention None
 */
void ble_stack_init(void)
{
    uint32_t ram_start = 0; /* 协议栈 RAM 起始地址 */

    APP_ERROR_CHECK(nrf_sdh_enable_request()); /* BLE 协议栈回复使能应答，主要是配置协议栈时钟 */
    APP_ERROR_CHECK(nrf_sdh_ble_default_cfg_set(BLE_CONNECTION_CONFIG, &ram_start)); /* 获取 BLE 协议栈起始地址，默认连接配置 */
    APP_ERROR_CHECK(nrf_sdh_ble_enable(&ram_start)); /* 使能 BLE 协议栈 */

    NRF_SDH_BLE_OBSERVER(m_ble_observer, BLE_EVENT_HANDLER_PRIORITY, ble_event_handler, NULL); /* 注册 BLE 观察者事件处理 */

    NRF_LOG_INFO("BLE stack initialization end.");
}

/**@brief BLE 事件处理函数
 *
 * @param[in]  *p_ble_evt - 事件类型指针
 * @param[in]  *p_context - 未使用的指针
 *
 * @retval     None
 *
 * @note 包含以下几种事件类型：COMMON、GAP、GATT Client、GATT Server、L2CAP
 * @attention None
 */
static void ble_event_handler(ble_evt_t const *p_ble_evt, void *p_context)
{
    /* 判断 BLE 事件类型 */
    switch (p_ble_evt->header.evt_id) {
        case BLE_GAP_EVT_CONNECTED: /* 连接 */
            conn_handle = p_ble_evt->evt.gap_evt.conn_handle; /* 保存 GAP 连接句柄 */
            APP_ERROR_CHECK(nrf_ble_qwr_conn_handle_assign(&queued_write, conn_handle)); /* 将连接句柄分配给队列写 */
            led_close(LED2);
            led_open(LED3);
            NRF_LOG_INFO("Connected to peer.");
            break;
        case BLE_GAP_EVT_DISCONNECTED: /* 断开连接 */
            conn_handle = BLE_CONN_HANDLE_INVALID; /* 连接句柄无效 */
            start_advertis(); /* 开启广播 */
            led_open(LED2);
            led_close(LED3);
            NRF_LOG_INFO("Disconnected from peer.");
            break;
        case BLE_GAP_EVT_PHY_UPDATE_REQUEST: /* PHY更新请求*/
        {
            ble_gap_phys_t const phy = {
                .rx_phys = BLE_GAP_PHY_AUTO,
                .tx_phys = BLE_GAP_PHY_AUTO,
            }; /* PHY 指针 */

            APP_ERROR_CHECK(sd_ble_gap_phy_update(p_ble_evt->evt.gap_evt.conn_handle, &phy)); /* 发起或响应 PHY 更新请求 */
            NRF_LOG_INFO("PHY Update Request.");
        } break;
        case BLE_GAP_EVT_SEC_PARAMS_REQUEST: /* 请求提供安全参数 */
            APP_ERROR_CHECK(sd_ble_gap_sec_params_reply(conn_handle, BLE_GAP_SEC_STATUS_PAIRING_NOT_SUPP, NULL, NULL)); /* 回复 GAP 安全参数 */
            NRF_LOG_INFO("Request to provide security parameters.");
            break;
        case BLE_GATTS_EVT_SYS_ATTR_MISSING: /* 等待持久的系统属性访问*/
            APP_ERROR_CHECK(sd_ble_gatts_sys_attr_set(conn_handle, NULL, 0, 0)); /* 更新持续的系统属性信息 */
            NRF_LOG_INFO("A persistent system attribute access is pending.");
            break;
        case BLE_GATTC_EVT_TIMEOUT: /* 通用属性客户端超时*/
            APP_ERROR_CHECK(sd_ble_gap_disconnect(p_ble_evt->evt.gattc_evt.conn_handle, BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION)); /* 断开连接 */
            NRF_LOG_INFO("GATT Client Timeout.");
            break;
        case BLE_GATTS_EVT_TIMEOUT: /* 通用属性服务端超时 */
            APP_ERROR_CHECK(sd_ble_gap_disconnect(p_ble_evt->evt.gattc_evt.conn_handle, BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION)); /* 断开连接 */
            NRF_LOG_INFO("GATT Server Timeout.");
            break;
        default:
            NRF_LOG_INFO("BLE EVENT ID:%d", p_ble_evt->header.evt_id);
            break;
    }
}

/**@brief GAP（通用访问配置文件）初始化函数
 *
 * @param[in]  None
 *
 * @retval     None
 *
 * @note 函数功能：	1.配置设备名称，生成设备图标
 * 					2.配置 GAP 的连接参数
 * @attention None
 */
void gap_init(void)
{
    ble_gap_conn_params_t   gap_connect_param; /* GAP 连接参数 */
    ble_gap_conn_sec_mode_t gap_connect_sec; /* GAP 连接安全模式 */

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&gap_connect_sec); /* 设置安全模式不需要保护，打开连接 */
    APP_ERROR_CHECK(sd_ble_gap_device_name_set(&gap_connect_sec, (const uint8_t *)BLE_DEVICE_NAME, strlen(BLE_DEVICE_NAME))); /* 设置 GAP 设备名称 */
    APP_ERROR_CHECK(sd_ble_gap_appearance_set(BLE_APPEARANCE_GENERIC_COMPUTER)); /* 设置 GAP 外观值：通用计算机 */

    memset(&gap_connect_param, 0, sizeof(gap_connect_param));
    gap_connect_param.min_conn_interval = MIN_CONNECTION_INTERVAL; /* 最小连接间隔，以1.25毫秒为单位 */
    gap_connect_param.max_conn_interval = MAX_CONNECTION_INTERVAL; /* 最大连接间隔，以1.25毫秒为单位 */
    gap_connect_param.slave_latency     = SLAVE_LATENCY; /* 从机连接事件数的延迟时间 */
    gap_connect_param.conn_sup_timeout  = CONNECT_SUP_TIMEOUT; /* 连接监控超时时间，以1.25毫秒为单位 */
    APP_ERROR_CHECK(sd_ble_gap_ppcp_set(&gap_connect_param)); /* 设置 GAP 首选连接参数 */

    NRF_LOG_INFO("GAP initialization end.");
}

/**@brief GATT（通用属性配置文件）初始化函数
 *
 * @param[in]  None
 *
 * @retval     None
 *
 * @note None
 * @attention None
 */
void gatt_init(void)
{
    APP_ERROR_CHECK(nrf_ble_gatt_init(&ble_gatt, NULL)); /* 初始化 GATT */

    NRF_LOG_INFO("GATT initialization end.");
}

/**@brief 广播初始化函数
 *
 * @param[in]  None
 *
 * @retval     None
 *
 * @note None
 * @attention None
 */
void advertis_init(void)
{
    ble_gap_adv_params_t adv_param; /* 广播参数 */
    ble_gap_addr_t       peer_addr; /* 白名单地址 */

    enc_adv_data.adv_data.p_data      = adv_data; /* 广播数据缓冲区指针 */
    enc_adv_data.adv_data.len         = sizeof(adv_data); /* 广播数据大小 */
    enc_adv_data.scan_rsp_data.p_data = NULL; /* 广播响应数据缓冲区指针 */
    enc_adv_data.scan_rsp_data.len    = 0; /* 广播响应数据大小 */

    memset(&peer_addr, 0, sizeof(peer_addr));
    peer_addr.addr_id_peer = 1; /* 指示地址从可解析专用地址解析（当对等方使用隐私时） */
    peer_addr.addr_type    = BLE_GAP_ADDR_TYPE_RANDOM_STATIC; /* 随机静态(身份)地址 */
    peer_addr.addr[0]      = 0x00; /* 白名单地址 */
    peer_addr.addr[1]      = 0x00;
    peer_addr.addr[2]      = 0x00;
    peer_addr.addr[3]      = 0x00;
    peer_addr.addr[4]      = 0x00;
    peer_addr.addr[5]      = 0x00;

    memset(&adv_param, 0, sizeof(adv_param));
    adv_param.properties.type             = BLE_GAP_ADV_TYPE_CONNECTABLE_SCANNABLE_UNDIRECTED; /* 广播类型：可连接和可扫描的非定向广告事件 */
    adv_param.properties.anonymous        = 0; /* 从所有PDU中不省略广告商的地址 */
    adv_param.properties.include_tx_power = 0; /* 此功能在此软件设备上不受支持 */
    adv_param.p_peer_addr                 = &peer_addr; /* 已知对等方的地址 */
    adv_param.p_peer_addr                 = NULL; /* 已知对等方的地址 */
    adv_param.interval                    = ADVERTIS_INTERVAL; /* 广播间隔（单位625us） */
    adv_param.duration                    = BLE_GAP_ADV_TIMEOUT_GENERAL_UNLIMITED; /* 广播持续时间（单位：10ms） */
    adv_param.max_adv_evts                = 0; /* 停止广播前发送的最大广播事件 */
    adv_param.channel_mask[0]             = 0x00; /* 广播通道索引（使用所有通道） */
    adv_param.channel_mask[1]             = 0x00;
    adv_param.channel_mask[2]             = 0x00;
    adv_param.channel_mask[3]             = 0x00;
    adv_param.channel_mask[4]             = 0x00;
    adv_param.filter_policy               = BLE_GAP_ADV_FP_ANY; /* 广播过滤策略：允许任何设备扫描请求和连接请求 */
    adv_param.primary_phy                 = BLE_GAP_PHY_1MBPS; /* 发送主发布通道报文的 PHY：1 Mbps */
    adv_param.secondary_phy               = BLE_GAP_PHY_1MBPS; /* 发送第二发布通道报文的 PHY：1 Mbps */
    adv_param.set_id                      = 0; /* 广告集标识符将该广告集与由本设备和其他设备传输的其他广告集区分开来 */
    adv_param.scan_req_notification       = 0; /* 为广告集禁用扫描请求通知 */
    APP_ERROR_CHECK(sd_ble_gap_adv_set_configure(&adv_handle, &enc_adv_data, &adv_param)); /* 广播配置 */

    NRF_LOG_INFO("Advertis initialization end.");
}

/**@brief 服务初始化函数
 *
 * @param[in]  None
 *
 * @retval     None
 *
 * @note None
 * @attention None
 */
void service_init(void)
{
    nrf_ble_qwr_init_t qwr_init; /* 队列写初始化结构体 */

    memset(&qwr_init, 0, sizeof(qwr_init));
    qwr_init.error_handler = ble_qwr_error_handler; /* 队列写错误处理 */
    APP_ERROR_CHECK(nrf_ble_qwr_init(&queued_write, &qwr_init)); /* 初始化队列写模块 */

    NRF_LOG_INFO("Service initialization end.");
}

/**@brief BLE 队列写错误处理函数
 *
 * @param[in]  nrf_error - 错误代码
 *
 * @retval     None
 *
 * @note None
 * @attention None
 */
static void ble_qwr_error_handler(uint32_t nrf_error)
{
    APP_ERROR_HANDLER(nrf_error);
}

/**@brief 连接参数初始化函数
 *
 * @param[in]  None
 *
 * @retval     None
 *
 * @note None
 * @attention None
 */
void connect_param_init(void)
{
    ble_conn_params_init_t connect_params; /* 连接参数 */

    memset(&connect_params, 0, sizeof(connect_params));
    connect_params.p_conn_params                  = NULL; /* 从主机获取连接参数 */
    connect_params.first_conn_params_update_delay = FIRST_CONN_PARAMS_UPDATE_DELAY; /* 初始化事件(连接或开始通知)到第一次调用sd_ble_gap_conn_param_update的时间 */
    connect_params.next_conn_params_update_delay  = NEXT_CONN_PARAMS_UPDATE_DELAY; /* 第一次之后调用sd_ble_gap_conn_param_update之间的时间间隔(以时钟周期数为单位) */
    connect_params.max_conn_params_update_count   = MAX_CONN_PARAMS_UPDATE_COUNT; /* 放弃协商前的尝试次数 */
    connect_params.start_on_notify_cccd_handle    = BLE_GATT_HANDLE_INVALID; /* 无效的CCCD句柄 */
    connect_params.disconnect_on_fail             = false; /* 更新参数失败不会断开连接 */
    connect_params.evt_handler                    = ble_connect_param_event_handler; /* 连接参数事件处理 */
    connect_params.error_handler                  = ble_connect_param_error_handler; /* 连接参数错误处理 */

    APP_ERROR_CHECK(ble_conn_params_init(&connect_params));
}

/**@brief BLE 连接参数事件处理函数
 *
 * @param[in]  *p_evt - 连接参数事件指针
 *
 * @retval     None
 *
 * @note None
 * @attention None
 */
static void ble_connect_param_event_handler(ble_conn_params_evt_t *p_evt)
{
    /* 判断事件类型 */
    switch (p_evt->evt_type) {
        case BLE_CONN_PARAMS_EVT_FAILED: /* 协商失败 */
            APP_ERROR_CHECK(sd_ble_gap_disconnect(conn_handle, BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION)); /* 断开连接 */
            break;
        case BLE_CONN_PARAMS_EVT_SUCCEEDED: /* 协商成功 */
            break;
    }
}

/**@brief BLE 连接参数错误处理函数
 *
 * @param[in]  nrf_error - 错误代码
 *
 * @retval     None
 *
 * @note None
 * @attention None
 */
static void ble_connect_param_error_handler(uint32_t nrf_error)
{
    APP_ERROR_HANDLER(nrf_error);
}

/**@brief 对等管理器初始化函数
 *
 * @param[in]  None
 *
 * @retval     None
 *
 * @note None
 * @attention None
 */
void peer_manager_init(void)
{
    ble_gap_sec_params_t gap_sec_param; /* GAP 安全参数 */

    APP_ERROR_CHECK(pm_init()); /* 初始化对等管理器 */

    memset(&gap_sec_param, 0, sizeof(gap_sec_param));
    gap_sec_param.bond           = 1; /* 执行键 */
    gap_sec_param.mitm           = 0; /* 不启用中场保护 */
    gap_sec_param.lesc           = 0; /* 不启用 LE 安全连接配对 */
    gap_sec_param.keypress       = 0; /* 生成按键通知 */
    gap_sec_param.io_caps        = BLE_GAP_IO_CAPS_NONE; /* 没有 IO 输入输出功能 */
    gap_sec_param.oob            = 0; /* 带外数据不可用 */
    gap_sec_param.min_key_size   = 7; /* 最小加密密钥大小，以字节为单位 */
    gap_sec_param.max_key_size   = 16; /* 最大加密密钥大小，以字节为单位 */
    gap_sec_param.kdist_own.enc  = 1;
    gap_sec_param.kdist_own.id   = 1;
    gap_sec_param.kdist_peer.enc = 1;
    gap_sec_param.kdist_peer.id  = 1;
    APP_ERROR_CHECK(pm_sec_params_set(&gap_sec_param)); /* 设置配对过程的配对和键合参数 */
    APP_ERROR_CHECK(pm_register(peer_manger_event_handler)); /* 注册对等管理器处理函数 */
}

/**@brief 对等管理器处理函数
 *
 * @param[in]  p_event - 对等管理器事件指针
 *
 * @retval     None
 *
 * @note None
 * @attention None
 */
static void peer_manger_event_handler(pm_evt_t const *p_event)
{
    pm_handler_on_pm_evt(p_event); /* 对等管理器调用 */
    pm_handler_disconnect_on_sec_failure(p_event); /* 对等管理器断开 */
    pm_handler_flash_clean(p_event); /* 对等管理器维护功能 */
    switch (p_event->evt_id) {
        case PM_EVT_PEERS_DELETE_SUCCEEDED: /* 调用完成 */
            start_advertis();
            break;
        default:
            break;
    }
}

/**@brief 开启广播函数
 *
 * @param[in]  None
 *
 * @retval     None
 *
 * @note None
 * @attention None
 */
void start_advertis(void)
{
    APP_ERROR_CHECK(sd_ble_gap_adv_start(adv_handle, BLE_CONNECTION_CONFIG)); /* 开启广播（GAP 可发现、可连接模式、广播过程） */
}

/**@brief 停止广播函数
 *
 * @param[in]  None
 *
 * @retval     None
 *
 * @note None
 * @attention None
 */
void stop_advertis(void)
{
    APP_ERROR_CHECK(sd_ble_gap_adv_stop(adv_handle)); /* 停止广播 */
}
