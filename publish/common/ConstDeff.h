#ifndef DEF_CONST_DEFF_HEADER
#define DEF_CONST_DEFF_HEADER


#define DEF_NICK_NAME_LEN	20		//用户昵称
#define DEF_CITY_NAME_LEN	64		//城市名称最大长度
#define DEF_IDIOGRAPH_LEN	100		//个人签名最大长度
#define DEF_COUNTRY_NAME_LEN  64	//国家名称最大长度
#define DEF_PROVINCE_NAME_LEN 64	//省份名称最大长度
#define DEF_EMPLOYMENT_NAME_LEN 64	//职业描述最大长度
#define DEF_EMAIL_NAME_LEN	64		//email 地址最大长度
#define DEF_SELF_DESCRIBE_LEN 256	//个人说明

#define DEF_ROOM_NAME_LEN	40		//房间名称最大长度
#define DEF_ROOM_PWD_LEN	32		//房间密码最大长度
#define DEF_ROOM_TOPIC_LEN  255		//房间主题最大长度
#define DEF_ROOM_WELCOME_WORD_LEN 255	//房间欢迎词最大长度

#define DEF_STATION_NAME_LEN	32	//小站名称最大长度(水星3.0添加)

#define	DEF_CATEGORY_NAME_LEN			80	//房间分类长度
#define	DEF_CATEGORY_DESCRIP_LEN		100	//房间分类描述长度
#define	DEF_PLUGIN_NAME_LEN				128	//房间插件名称长度

#define DEF_IP_ADDR_LEN		15		//ip地址
#define DEF_VERSION_LEN		7		//字符串表示的版本号
#define DEF_MD5_PWD_LEN		32		//md5 密码长度


#define DEF_HTML_URL_LEN	128		//url地址长度
#define DEF_GENERAL_DATA_LEN 2048	//通用信息buffer长度
#define DEF_REASON_BUF_LEN	 255	//结果描述长度
#define DEF_MAX_DESCRIBE_LEN 512	//描述最大长度


#define DEF_SESSION_ENCRY_KEY_LEN 64	//网络通信加密密钥长度
#define DEF_RSA_ENCRY_KEY_LEN	512		//rsa密钥长度
#define DEF_MAX_PACK_LEN		2048	//网络包最大长度	

#define DEF_FAS_ENCRY_KEY_LEN 64   //家族信息服务器的认证串长度

//凡是公司回收的呱币，都存入此账号，一般用于集体中奖礼物，如烟花。
#define	DEF_SHARE_GIFT_USER_ID			9900000000LL			//烟花礼物系统回收账号

#define	DEF_REBROADER_LOWBOUNDD			9900000000LL			//转播员id最小值
#define	DEF_REBROADER_UPBOUND			9999999999LL			//转播员id最大值

#define	DEF_VISITER_ID_BOUND			10000000000	            //游客id的最小值

// 神秘人ID (水星3.0增加)
#define DEF_UNKNOWN_USER_ID             90000000LL

//使用真实上麦用户ID加上此最小值，构成虚拟大视频用户ID
#define DEF_VIRTUAL_USER_ID_BASE		1000000000000			//虚拟大视频用户ID的最小值

#define DEF_OME_NAME_LEN      64		//Oem 名称
#define DEF_ALIAS_NAME_LEN    40		//Oem 别名

#define  DEF_UNINSTASLL_REASON_LEN	255  // 卸载理由长度
#define  DEF_UNINSTASLL_CONTACT_LEN	128  // 联系方式长度
#define  DEF_MAX_SEARCH_RESULT	10		 //客户端向服务器搜索用户信息，最大的返回数量

//水星3.4增加
#define  DEF_SEND_VERIFICATION_CODE_RQ_ID	15  //发送验证码请求标识
#define  DEF_VERIFY_RESULT_ID	16    	//发送验证码验证结果标识
#define  MAX_GOODS_KINDS_CNT 120			//群刷礼物最大礼物上限

//公积金项目添加
#define  DEF_MAX_USER_COUNT  100			//单次发送给计费的最大人数

//数据库相关变化类型
enum DB_CHANGE_TYPE
{
	enum_Chang_Type_Add = 1,
	enum_Chang_Type_Remove = 2,
	enum_Chang_Type_Modify = 3
};

//服务器类型
enum ENUM_SERVER_TYPE
{	
	enum_svr_type_cas = 1,					//普通聊天室接入服务器
	enum_svr_type_acs = 21,					//计费服务器(服务器间使用)
	enum_svr_type_cqs,						//聊天大厅查询服务器
	enum_svr_type_cgs,						//聊天室网守服务器
	enum_svr_type_rtp_proxy,				//rtp代理转发服务器
	enum_svr_type_ncs,						//NAT检测服务器
	enum_svr_type_dns,						//域名服务器
	enum_svr_type_login,					//登录服务器
	enum_svr_type_proxy,					//代理转发服务器(新),主要被客户端使用
	enum_svr_type_moniter,					//监控服务器
	enum_svr_type_rbs,						//转播服务器
	enum_svr_type_acs_proxy,				//计费代理服务器(供客户端接入使用)
    enum_svr_type_phone_moniter,			//视频电话监控服务器
	enum_svr_type_res,						//资源下载服务器
	enum_svr_type_res_proxy,				//资源下载代理服务器(供客户端接入使用),现在修改为基础服务代理,家族代理/消息通知代理/原资源下载代理都用这个类型,修改时间2010.12.20
	enum_svr_type_uts,                      //登陆统计服务器
	enum_svr_type_grs,                      //游戏房间服务器
	enum_svr_type_gvs,                      //游戏视频服务器
	enum_svr_type_wrs,						//Web房间接入服务器
	enum_svr_type_webfms,					//页面端adobe flash服务器
	enum_svr_type_webbroad,					//页面端房间信息广播服务器
	enum_svr_type_cms,						//房间媒体服务器
	enum_svr_type_moniter_proxy,			//监控代理服务器
	enum_svr_type_web_user_key,             //WEB用户认证服务器
	enum_svr_type_fas,						//家族服务器
	enum_svr_type_mns,						//消息通知服务器
	enum_svr_type_room_proxy,				//房间代理服务器，46
	enum_svr_type_web_wgk,					//Web网守服务器，47
	enum_svr_type_web_wqs,					//Web目录服务器，48
	enum_svr_type_tcp_idc,					//各机房TCP速度测试服务器，49
	enum_svr_type_srs,					    //明星关系服务器 50
	enum_svr_type_cms_rtp_proxy,			//CMS RTP代理服务器,提供双线转发服务 51
	enum_svr_type_chatdcs = 52,				//聊天室数据收集服务器
	enum_svr_type_subcgs = 53,				//聊天室子GK
	enum_svr_type_active_server = 54,		//抽奖活动服务器
	enum_svr_type_cas_moniter = 55,			//文本监控服务器
	enum_svr_type_cms_moniter = 56,			//视频监控服务器
	enum_svr_type_vote = 57,				//投票服务器
	enum_svr_type_rtmp_gk = 58,				//RtmpGK服务器
	enum_svr_type_master_umrs = 59,			//主用户麦状态缓存服务器
	enum_svr_type_slave_umrs = 60,			//从用户麦状态缓存服务器
	enum_svr_type_idc_speed = 61,			//机房测速服务器
	enum_svr_type_web_moniter = 62,			//Web监控服务器
	enum_svr_type_cqs_lvs = 63,				//cqs对应的Lvs服务器
	enum_svr_type_roompxy_lvs = 64,			//房间代理服务器对应的Lvs
	enum_svr_type_warning = 65,              //告警服务器
	enum_srv_type_warning_lvs = 66,          //告警服务器对应的LVS
	enum_svr_type_room_query_svr  = 67,      //房间查询服务器
	enum_svr_type_msg_center_svr = 68,       //消息中心服务器
	enum_svr_type_hbs_slave = 69 ,			 //心动服务器slave （商业站）  69
	enum_svr_type_hbs_master = 70 ,			 //心动服务器master （商业站） 70


	enum_svr_type_chat_gas_min = 100,		//房间内游戏gasid起始
	enum_svr_type_chat_gas_max = 199,		//房间内游戏gasid最大值

	enum_svr_type_game_gas_min = 200,		//休闲游戏gasid起始
	enum_svr_type_game_gas_max = 299,		//休闲游戏gasid最大值

	enum_svr_type_game_acs = 301,				//休闲游戏计费
	enum_svr_type_game_acs_proxy,			//休闲游戏计费代理

	enum_svr_type_ims_slave = 1000,			//从IMServer服务器
	enum_svr_type_ims_master,				//主IMServer服务器
	enum_svr_type_improxy,					//IMProxy服务器
	enum_svr_type_igs,						//IM网守服务器	
	enum_svr_type_ftp_proxy,				//文件传输代理转发服务器，目前仅IM使用
	enum_svr_type_iag_slave,				//从IMAPIGateWay服务器。API网关，向外部提供好友关系查询、消息投递、等功能。是外部系统与IM系统整合的接口
	enum_svr_type_iag_master,				//主IMAPIGateWay服务器
	enum_svr_type_imcs,						//IM配置服务器

	enum_svr_type_forward_manager = 1050,	//转发管理服务器
	enum_svr_type_forward_server,			//转发服务器

	enum_svr_type_nd_manager = 1200,		//NetDiskManager服务器
	enum_svr_type_nd_node_server_slave,		//NetDisk 节点从服务器
	enum_svr_type_nd_node_server_master,	//NetDisk 节点主服务器
	enum_svr_type_nd_up_proxy_master,		//NetDisk 上传代理主服务器
	enum_svr_type_nd_up_proxy_slave,		//NetDisk 上传代理辅服务器
	enum_svr_type_nd_down_proxy_master,		//NetDisk 下载代理主服务器
	enum_svr_type_nd_down_proxy_slave,		//NetDisk 下载代理辅服务器

	enum_svr_type_group_gs = 1300,			//群聊天服务器，GroupServer,原enum_svr_type_group_gcs 
	enum_svr_type_group_gproxy,				//群代理服务器，GroupProxyServer,原enum_svr_type_group_gqs
	enum_svr_type_group_ggk,				//原群GK，GroupGateKeeper
	enum_svr_type_group_gus,				//群用户组服务器 GroupUserServer,原enum_svr_type_group_gcs_proxy
	enum_svr_type_group_search,				//搜索服务器
	enum_svr_type_group_user_db,			//群用户数据服务器
	enum_svr_type_group_db,					//群数据服务器
	enum_srv_type_group_id_manager,			//群号管理器

	enum_svr_type_login_gks = 1400,			//登录系统网守服务器　LoginGateKeeperSever
	enum_svr_type_login_proxy,				//登录代理服务器　LoginProxyServer
	enum_svr_type_login_verify,				//登录验证服务器　LoginVerifyServer
	enum_svr_type_login_uis,				//用户信息服务器　UserInfoServer
	enum_svr_type_login_uts,				//用户行为统计服务器 UserStatisticsServer
	enum_svr_type_login_tps,				//第三方交互服务器	ThirdPartyServer
	enum_svr_type_login_ifs,				//接口服务器	LoginInterfaceServer
	enum_svr_type_login_uss,				//用户状态服务器 UserStateSever


	enum_svr_type_login_web_gss=1430,		//美女秀管理WEB服务
	enum_svr_type_login_web_passport=1431,	//WEB注册登录相关passport服务
	enum_svr_type_login_web_www0=1432,		//WEB跳转服务
	enum_svr_type_login_web_cookise=1433,	//种Cookise服务

	enum_svr_type_login_backup_verify=1450,	//备份登录验证服务器
	enum_svr_type_login_backup_uis,			//备份用户信息服务器

	enum_svr_type_master_ups = 1500,		//主检查更新服务器 ups：update server
	enum_svr_type_slave_ups,				//从检查更新服务器 ups：update server

	enum_svr_type_psys_gk = 1600,						//个人信息息系统GK
	enum_svr_type_psys_proxy = 1601,					//个人信息息系统代理服务器
	enum_svr_type_psys_user_server = 1602,				//个人信息息系统服务器,用户服务器
	enum_svr_type_psys_acs_access_proxy = 1603,			//个人信息息系统服务器,计费访问代理
	enum_svr_type_psys_dcgw = 1604,						//个人数据中心网关
	enum_svr_type_rsys_dcgw = 1605,						//房间数据中心网关
	enum_svr_type_rsys_lbcs = 1606,						//LBCS 服务器
	enum_svr_type_rsys_rls = 1607,						//房间业务服务器
	enum_svr_type_rsys_rlsp = 1608,						//房间业务服务器代理
	enum_svr_type_rsys_udc = 1609,						//用户数据缓存服务器
	enum_svr_type_rsys_lrdc = 1610,						//房间数据缓存服务器
	enum_svr_type_rsys_cmsp = 1611,						//CMS 下载 代理服务器
	enum_svr_type_psys_ldcgw = 1612,					//用户数据缓存网关
	enum_svr_type_rsys_ldcgw = 1613,					//房间数据缓存网关
	enum_svr_type_rsys_es = 1614,						//根事件服务器
	enum_svr_type_rsys_les = 1615,						//叶事件服务器
	enum_svr_type_rsys_httpproxy = 1616,				//http代理服务器
	enum_svr_type_rsys_rlrs = 1617,						//房间状态检验服务器
	enum_svr_type_rsys_mdcs = 1618,						//外部数据处理服务器
	enum_svr_type_rsys_data_report = 1619,				//数据上报服务器
	enum_svr_type_rsys_idc_select = 1620,				//IDC选择策略服务器
	enum_svr_type_rsys_idc_select_proxy = 1621,			//IDC选择策略代理服务器
	enum_svr_type_rsys_consume_dynamic_order = 1622,	//用户消费动态排行榜服务器
	enum_svr_type_rsys_gift_effect = 1623,				//礼物特效服务器
	enum_svr_type_rsys_cmsp_up = 1624,					//CMS 上传 代理服务器
	enum_svr_type_rsys_login_access_proxy = 1625,		//登录系统访问代理服务器
	enum_svr_type_rsys_http_cgi_access_proxy = 1626,	//HTTP-CGI接口访问代理
	enum_svr_type_rsys_tvwall_logic = 1627,				//电视墙逻辑服务器
	enum_svr_type_rsys_tvwall_queue = 1628,				//电视墙队列服务器
	enum_svr_type_rsys_tvwall_stream = 1629,			//电视墙转流服务器
	enum_svr_type_rsys_qqav_proxy = 1630,				//QQAV代理服务器

	enum_svr_type_rsys_rds_for_user_list = 1650,		//分发根服务器-用户列表
	enum_svr_type_rsys_srds_for_user_list = 1651,		//分发子服务器-用户列表
	enum_svr_type_rsys_lrds_for_user_list = 1652,		//分发叶服务器-用户列表
	enum_svr_type_rsys_rds_for_msg = 1653,				//分发根服务器-消息
	enum_svr_type_rsys_srds_for_msg = 1654,				//分发子服务器-消息
	enum_svr_type_rsys_lrds_for_msg = 1655,				//分发叶服务器-消息
	enum_svr_type_rsys_rds_for_media = 1656,			//分发根服务器-媒体
	enum_svr_type_rsys_srds_for_media = 1657,			//分发子服务器-媒体
	enum_svr_type_rsys_lrds_for_media = 1658,			//分发叶服务器-媒体

	enum_svr_type_linux_cs_proxy = 1700,				//linux-通用cs代理服务器
	enum_svr_type_linux_msg_push_proxy = 1701,		//linux-消息推送代理

	enum_svr_type_medal_manager = 1730,				//勋章管理服务器
	enum_svr_type_medal_user = 1731,				//勋章用户服务器
	enum_svr_type_medal_user_flag = 1732,			//勋章用户标记服务器
	enum_svr_type_medal_data_agent = 1733,			//勋章数据代理服务器

	enum_svr_type_login_id_db_operator = 1751,			//登陆系统重构-帐号数据库操作服务器
	enum_svr_type_login_id_top_cache = 1752,				//登陆系统重构-一级缓存
	enum_svr_type_login_id_sub_cache = 1753,				//登陆系统重构-二级缓存
	enum_svr_type_login_id_verify_logic = 1754,			//登陆系统重构-验证逻辑服务器
	enum_svr_type_login_id_verify_proxy = 1755,			//登陆系统重构-验证代理
};

//网络类型
enum ENUM_ISP_TYPE
{
	enum_isp_ChinaTeleCom,	//中国电信
	enum_isp_ChinaNetCom,	//中国网通	
	enum_isp_other,		//其他
};

// 小站产品标识，这个定义和呱呱主站使用的OEM ID 在包定义中使用同一个字段，所以需要两者之间避免重复
// OEM ID定义参考 VersionConfig.h 中 enum_All_OemID
enum ENUM_MERCURY_PRODUCT
{
	ENUM_MERCURY_PRODUCT_KELE = 1,              //可乐
	ENUM_MERCURY_PRODUCT_QIXI = 2,              //七喜
	ENUM_MERCURY_PRODUCT_YESE = 3,              //夜色
	ENUM_MERCURY_PRODUCT_QIJI = 4,              //奇迹
	ENUM_MERCURY_PRODUCT_JUXING = 5,            //巨星
	ENUM_MERCURY_PRODUCT_PINGGUO = 6,			//苹果
	ENUM_MERCURY_PRODUCT_CAIHONG = 11,          //彩虹
	ENUM_MERCURY_PRODUCT_SHENHUA = 12,			//神话
	ENUM_MERCURY_PRODUCT_HONGXIU = 13,			//红袖
};

//小站产品的默认端口，客户端通过域名解析得到IP地址，加上默认端口可以连接到这接的服务器程序
enum ENUM_MERCURY_PRODUCT_COMM_PORT
{
	ENUM_MERCURY_PRODUCT_COMM_PORT_KELE = 2108,				//可乐　域名:unicom.kele55.com
	ENUM_MERCURY_PRODUCT_COMM_PORT_QIXI = 2109,				//七喜	域名:unicom.qixi55.com
	ENUM_MERCURY_PRODUCT_COMM_PORT_CAIHONG = 2110,			//彩虹	域名:unicom.caihong55.com
	ENUM_MERCURY_PRODUCT_COMM_PORT_QIJI = 2111,				//奇迹	域名:unicom.qiji55.com
	ENUM_MERCURY_PRODUCT_COMM_PORT_JUXING = 2112,			//巨星	域名:unicom.juxing55.com
	ENUM_MERCURY_PRODUCT_COMM_PORT_PINGUO = 2113,			//苹果	域名:unicom.pinguo55.com
	ENUM_MERCURY_PRODUCT_COMM_PORT_SHENHUA = 2114,			//神话	域名:unicom.shenhua55.com
	ENUM_MERCURY_PRODUCT_COMM_PORT_HONGXIU = 2115,			//红袖	域名:unicom.hongxiu55.com
};


//设备类型
#ifndef DEF_AUDIO_DEVICE
#define DEF_AUDIO_DEVICE 0	 //语音设备
#define DEF_VIDEO_DEVICE 1	 //视频设备
#endif
#endif
