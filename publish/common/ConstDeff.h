#ifndef DEF_CONST_DEFF_HEADER
#define DEF_CONST_DEFF_HEADER


#define DEF_NICK_NAME_LEN	20		//�û��ǳ�
#define DEF_CITY_NAME_LEN	64		//����������󳤶�
#define DEF_IDIOGRAPH_LEN	100		//����ǩ����󳤶�
#define DEF_COUNTRY_NAME_LEN  64	//����������󳤶�
#define DEF_PROVINCE_NAME_LEN 64	//ʡ��������󳤶�
#define DEF_EMPLOYMENT_NAME_LEN 64	//ְҵ������󳤶�
#define DEF_EMAIL_NAME_LEN	64		//email ��ַ��󳤶�
#define DEF_SELF_DESCRIBE_LEN 256	//����˵��

#define DEF_ROOM_NAME_LEN	40		//����������󳤶�
#define DEF_ROOM_PWD_LEN	32		//����������󳤶�
#define DEF_ROOM_TOPIC_LEN  255		//����������󳤶�
#define DEF_ROOM_WELCOME_WORD_LEN 255	//���件ӭ����󳤶�

#define DEF_STATION_NAME_LEN	32	//Сվ������󳤶�(ˮ��3.0���)

#define	DEF_CATEGORY_NAME_LEN			80	//������೤��
#define	DEF_CATEGORY_DESCRIP_LEN		100	//���������������
#define	DEF_PLUGIN_NAME_LEN				128	//���������Ƴ���

#define DEF_IP_ADDR_LEN		15		//ip��ַ
#define DEF_VERSION_LEN		7		//�ַ�����ʾ�İ汾��
#define DEF_MD5_PWD_LEN		32		//md5 ���볤��


#define DEF_HTML_URL_LEN	128		//url��ַ����
#define DEF_GENERAL_DATA_LEN 2048	//ͨ����Ϣbuffer����
#define DEF_REASON_BUF_LEN	 255	//�����������
#define DEF_MAX_DESCRIBE_LEN 512	//������󳤶�


#define DEF_SESSION_ENCRY_KEY_LEN 64	//����ͨ�ż�����Կ����
#define DEF_RSA_ENCRY_KEY_LEN	512		//rsa��Կ����
#define DEF_MAX_PACK_LEN		2048	//�������󳤶�	

#define DEF_FAS_ENCRY_KEY_LEN 64   //������Ϣ����������֤������

//���ǹ�˾���յ��ɱң���������˺ţ�һ�����ڼ����н�������̻���
#define	DEF_SHARE_GIFT_USER_ID			9900000000LL			//�̻�����ϵͳ�����˺�

#define	DEF_REBROADER_LOWBOUNDD			9900000000LL			//ת��Աid��Сֵ
#define	DEF_REBROADER_UPBOUND			9999999999LL			//ת��Աid���ֵ

#define	DEF_VISITER_ID_BOUND			10000000000	            //�ο�id����Сֵ

// ������ID (ˮ��3.0����)
#define DEF_UNKNOWN_USER_ID             90000000LL

//ʹ����ʵ�����û�ID���ϴ���Сֵ�������������Ƶ�û�ID
#define DEF_VIRTUAL_USER_ID_BASE		1000000000000			//�������Ƶ�û�ID����Сֵ

#define DEF_OME_NAME_LEN      64		//Oem ����
#define DEF_ALIAS_NAME_LEN    40		//Oem ����

#define  DEF_UNINSTASLL_REASON_LEN	255  // ж�����ɳ���
#define  DEF_UNINSTASLL_CONTACT_LEN	128  // ��ϵ��ʽ����
#define  DEF_MAX_SEARCH_RESULT	10		 //�ͻ���������������û���Ϣ�����ķ�������

//ˮ��3.4����
#define  DEF_SEND_VERIFICATION_CODE_RQ_ID	15  //������֤�������ʶ
#define  DEF_VERIFY_RESULT_ID	16    	//������֤����֤�����ʶ
#define  MAX_GOODS_KINDS_CNT 120			//Ⱥˢ���������������

//��������Ŀ���
#define  DEF_MAX_USER_COUNT  100			//���η��͸��Ʒѵ��������

//���ݿ���ر仯����
enum DB_CHANGE_TYPE
{
	enum_Chang_Type_Add = 1,
	enum_Chang_Type_Remove = 2,
	enum_Chang_Type_Modify = 3
};

//����������
enum ENUM_SERVER_TYPE
{	
	enum_svr_type_cas = 1,					//��ͨ�����ҽ��������
	enum_svr_type_acs = 21,					//�Ʒѷ�����(��������ʹ��)
	enum_svr_type_cqs,						//���������ѯ������
	enum_svr_type_cgs,						//���������ط�����
	enum_svr_type_rtp_proxy,				//rtp����ת��������
	enum_svr_type_ncs,						//NAT��������
	enum_svr_type_dns,						//����������
	enum_svr_type_login,					//��¼������
	enum_svr_type_proxy,					//����ת��������(��),��Ҫ���ͻ���ʹ��
	enum_svr_type_moniter,					//��ط�����
	enum_svr_type_rbs,						//ת��������
	enum_svr_type_acs_proxy,				//�ƷѴ��������(���ͻ��˽���ʹ��)
    enum_svr_type_phone_moniter,			//��Ƶ�绰��ط�����
	enum_svr_type_res,						//��Դ���ط�����
	enum_svr_type_res_proxy,				//��Դ���ش��������(���ͻ��˽���ʹ��),�����޸�Ϊ�����������,�������/��Ϣ֪ͨ����/ԭ��Դ���ش������������,�޸�ʱ��2010.12.20
	enum_svr_type_uts,                      //��½ͳ�Ʒ�����
	enum_svr_type_grs,                      //��Ϸ���������
	enum_svr_type_gvs,                      //��Ϸ��Ƶ������
	enum_svr_type_wrs,						//Web������������
	enum_svr_type_webfms,					//ҳ���adobe flash������
	enum_svr_type_webbroad,					//ҳ��˷�����Ϣ�㲥������
	enum_svr_type_cms,						//����ý�������
	enum_svr_type_moniter_proxy,			//��ش��������
	enum_svr_type_web_user_key,             //WEB�û���֤������
	enum_svr_type_fas,						//���������
	enum_svr_type_mns,						//��Ϣ֪ͨ������
	enum_svr_type_room_proxy,				//��������������46
	enum_svr_type_web_wgk,					//Web���ط�������47
	enum_svr_type_web_wqs,					//WebĿ¼��������48
	enum_svr_type_tcp_idc,					//������TCP�ٶȲ��Է�������49
	enum_svr_type_srs,					    //���ǹ�ϵ������ 50
	enum_svr_type_cms_rtp_proxy,			//CMS RTP���������,�ṩ˫��ת������ 51
	enum_svr_type_chatdcs = 52,				//�����������ռ�������
	enum_svr_type_subcgs = 53,				//��������GK
	enum_svr_type_active_server = 54,		//�齱�������
	enum_svr_type_cas_moniter = 55,			//�ı���ط�����
	enum_svr_type_cms_moniter = 56,			//��Ƶ��ط�����
	enum_svr_type_vote = 57,				//ͶƱ������
	enum_svr_type_rtmp_gk = 58,				//RtmpGK������
	enum_svr_type_master_umrs = 59,			//���û���״̬���������
	enum_svr_type_slave_umrs = 60,			//���û���״̬���������
	enum_svr_type_idc_speed = 61,			//�������ٷ�����
	enum_svr_type_web_moniter = 62,			//Web��ط�����
	enum_svr_type_cqs_lvs = 63,				//cqs��Ӧ��Lvs������
	enum_svr_type_roompxy_lvs = 64,			//��������������Ӧ��Lvs
	enum_svr_type_warning = 65,              //�澯������
	enum_srv_type_warning_lvs = 66,          //�澯��������Ӧ��LVS
	enum_svr_type_room_query_svr  = 67,      //�����ѯ������
	enum_svr_type_msg_center_svr = 68,       //��Ϣ���ķ�����
	enum_svr_type_hbs_slave = 69 ,			 //�Ķ�������slave ����ҵվ��  69
	enum_svr_type_hbs_master = 70 ,			 //�Ķ�������master ����ҵվ�� 70


	enum_svr_type_chat_gas_min = 100,		//��������Ϸgasid��ʼ
	enum_svr_type_chat_gas_max = 199,		//��������Ϸgasid���ֵ

	enum_svr_type_game_gas_min = 200,		//������Ϸgasid��ʼ
	enum_svr_type_game_gas_max = 299,		//������Ϸgasid���ֵ

	enum_svr_type_game_acs = 301,				//������Ϸ�Ʒ�
	enum_svr_type_game_acs_proxy,			//������Ϸ�ƷѴ���

	enum_svr_type_ims_slave = 1000,			//��IMServer������
	enum_svr_type_ims_master,				//��IMServer������
	enum_svr_type_improxy,					//IMProxy������
	enum_svr_type_igs,						//IM���ط�����	
	enum_svr_type_ftp_proxy,				//�ļ��������ת����������Ŀǰ��IMʹ��
	enum_svr_type_iag_slave,				//��IMAPIGateWay��������API���أ����ⲿ�ṩ���ѹ�ϵ��ѯ����ϢͶ�ݡ��ȹ��ܡ����ⲿϵͳ��IMϵͳ���ϵĽӿ�
	enum_svr_type_iag_master,				//��IMAPIGateWay������
	enum_svr_type_imcs,						//IM���÷�����

	enum_svr_type_forward_manager = 1050,	//ת�����������
	enum_svr_type_forward_server,			//ת��������

	enum_svr_type_nd_manager = 1200,		//NetDiskManager������
	enum_svr_type_nd_node_server_slave,		//NetDisk �ڵ�ӷ�����
	enum_svr_type_nd_node_server_master,	//NetDisk �ڵ���������
	enum_svr_type_nd_up_proxy_master,		//NetDisk �ϴ�������������
	enum_svr_type_nd_up_proxy_slave,		//NetDisk �ϴ�����������
	enum_svr_type_nd_down_proxy_master,		//NetDisk ���ش�����������
	enum_svr_type_nd_down_proxy_slave,		//NetDisk ���ش���������

	enum_svr_type_group_gs = 1300,			//Ⱥ�����������GroupServer,ԭenum_svr_type_group_gcs 
	enum_svr_type_group_gproxy,				//Ⱥ�����������GroupProxyServer,ԭenum_svr_type_group_gqs
	enum_svr_type_group_ggk,				//ԭȺGK��GroupGateKeeper
	enum_svr_type_group_gus,				//Ⱥ�û�������� GroupUserServer,ԭenum_svr_type_group_gcs_proxy
	enum_svr_type_group_search,				//����������
	enum_svr_type_group_user_db,			//Ⱥ�û����ݷ�����
	enum_svr_type_group_db,					//Ⱥ���ݷ�����
	enum_srv_type_group_id_manager,			//Ⱥ�Ź�����

	enum_svr_type_login_gks = 1400,			//��¼ϵͳ���ط�������LoginGateKeeperSever
	enum_svr_type_login_proxy,				//��¼�����������LoginProxyServer
	enum_svr_type_login_verify,				//��¼��֤��������LoginVerifyServer
	enum_svr_type_login_uis,				//�û���Ϣ��������UserInfoServer
	enum_svr_type_login_uts,				//�û���Ϊͳ�Ʒ����� UserStatisticsServer
	enum_svr_type_login_tps,				//����������������	ThirdPartyServer
	enum_svr_type_login_ifs,				//�ӿڷ�����	LoginInterfaceServer
	enum_svr_type_login_uss,				//�û�״̬������ UserStateSever


	enum_svr_type_login_web_gss=1430,		//��Ů�����WEB����
	enum_svr_type_login_web_passport=1431,	//WEBע���¼���passport����
	enum_svr_type_login_web_www0=1432,		//WEB��ת����
	enum_svr_type_login_web_cookise=1433,	//��Cookise����

	enum_svr_type_login_backup_verify=1450,	//���ݵ�¼��֤������
	enum_svr_type_login_backup_uis,			//�����û���Ϣ������

	enum_svr_type_master_ups = 1500,		//�������·����� ups��update server
	enum_svr_type_slave_ups,				//�Ӽ����·����� ups��update server

	enum_svr_type_psys_gk = 1600,						//������ϢϢϵͳGK
	enum_svr_type_psys_proxy = 1601,					//������ϢϢϵͳ���������
	enum_svr_type_psys_user_server = 1602,				//������ϢϢϵͳ������,�û�������
	enum_svr_type_psys_acs_access_proxy = 1603,			//������ϢϢϵͳ������,�Ʒѷ��ʴ���
	enum_svr_type_psys_dcgw = 1604,						//����������������
	enum_svr_type_rsys_dcgw = 1605,						//����������������
	enum_svr_type_rsys_lbcs = 1606,						//LBCS ������
	enum_svr_type_rsys_rls = 1607,						//����ҵ�������
	enum_svr_type_rsys_rlsp = 1608,						//����ҵ�����������
	enum_svr_type_rsys_udc = 1609,						//�û����ݻ��������
	enum_svr_type_rsys_lrdc = 1610,						//�������ݻ��������
	enum_svr_type_rsys_cmsp = 1611,						//CMS ���� ���������
	enum_svr_type_psys_ldcgw = 1612,					//�û����ݻ�������
	enum_svr_type_rsys_ldcgw = 1613,					//�������ݻ�������
	enum_svr_type_rsys_es = 1614,						//���¼�������
	enum_svr_type_rsys_les = 1615,						//Ҷ�¼�������
	enum_svr_type_rsys_httpproxy = 1616,				//http���������
	enum_svr_type_rsys_rlrs = 1617,						//����״̬���������
	enum_svr_type_rsys_mdcs = 1618,						//�ⲿ���ݴ��������
	enum_svr_type_rsys_data_report = 1619,				//�����ϱ�������
	enum_svr_type_rsys_idc_select = 1620,				//IDCѡ����Է�����
	enum_svr_type_rsys_idc_select_proxy = 1621,			//IDCѡ����Դ��������
	enum_svr_type_rsys_consume_dynamic_order = 1622,	//�û����Ѷ�̬���а������
	enum_svr_type_rsys_gift_effect = 1623,				//������Ч������
	enum_svr_type_rsys_cmsp_up = 1624,					//CMS �ϴ� ���������
	enum_svr_type_rsys_login_access_proxy = 1625,		//��¼ϵͳ���ʴ��������
	enum_svr_type_rsys_http_cgi_access_proxy = 1626,	//HTTP-CGI�ӿڷ��ʴ���
	enum_svr_type_rsys_tvwall_logic = 1627,				//����ǽ�߼�������
	enum_svr_type_rsys_tvwall_queue = 1628,				//����ǽ���з�����
	enum_svr_type_rsys_tvwall_stream = 1629,			//����ǽת��������
	enum_svr_type_rsys_qqav_proxy = 1630,				//QQAV���������

	enum_svr_type_rsys_rds_for_user_list = 1650,		//�ַ���������-�û��б�
	enum_svr_type_rsys_srds_for_user_list = 1651,		//�ַ��ӷ�����-�û��б�
	enum_svr_type_rsys_lrds_for_user_list = 1652,		//�ַ�Ҷ������-�û��б�
	enum_svr_type_rsys_rds_for_msg = 1653,				//�ַ���������-��Ϣ
	enum_svr_type_rsys_srds_for_msg = 1654,				//�ַ��ӷ�����-��Ϣ
	enum_svr_type_rsys_lrds_for_msg = 1655,				//�ַ�Ҷ������-��Ϣ
	enum_svr_type_rsys_rds_for_media = 1656,			//�ַ���������-ý��
	enum_svr_type_rsys_srds_for_media = 1657,			//�ַ��ӷ�����-ý��
	enum_svr_type_rsys_lrds_for_media = 1658,			//�ַ�Ҷ������-ý��

	enum_svr_type_linux_cs_proxy = 1700,				//linux-ͨ��cs���������
	enum_svr_type_linux_msg_push_proxy = 1701,		//linux-��Ϣ���ʹ���

	enum_svr_type_medal_manager = 1730,				//ѫ�¹��������
	enum_svr_type_medal_user = 1731,				//ѫ���û�������
	enum_svr_type_medal_user_flag = 1732,			//ѫ���û���Ƿ�����
	enum_svr_type_medal_data_agent = 1733,			//ѫ�����ݴ��������

	enum_svr_type_login_id_db_operator = 1751,			//��½ϵͳ�ع�-�ʺ����ݿ����������
	enum_svr_type_login_id_top_cache = 1752,				//��½ϵͳ�ع�-һ������
	enum_svr_type_login_id_sub_cache = 1753,				//��½ϵͳ�ع�-��������
	enum_svr_type_login_id_verify_logic = 1754,			//��½ϵͳ�ع�-��֤�߼�������
	enum_svr_type_login_id_verify_proxy = 1755,			//��½ϵͳ�ع�-��֤����
};

//��������
enum ENUM_ISP_TYPE
{
	enum_isp_ChinaTeleCom,	//�й�����
	enum_isp_ChinaNetCom,	//�й���ͨ	
	enum_isp_other,		//����
};

// Сվ��Ʒ��ʶ����������������վʹ�õ�OEM ID �ڰ�������ʹ��ͬһ���ֶΣ�������Ҫ����֮������ظ�
// OEM ID����ο� VersionConfig.h �� enum_All_OemID
enum ENUM_MERCURY_PRODUCT
{
	ENUM_MERCURY_PRODUCT_KELE = 1,              //����
	ENUM_MERCURY_PRODUCT_QIXI = 2,              //��ϲ
	ENUM_MERCURY_PRODUCT_YESE = 3,              //ҹɫ
	ENUM_MERCURY_PRODUCT_QIJI = 4,              //�漣
	ENUM_MERCURY_PRODUCT_JUXING = 5,            //����
	ENUM_MERCURY_PRODUCT_PINGGUO = 6,			//ƻ��
	ENUM_MERCURY_PRODUCT_CAIHONG = 11,          //�ʺ�
	ENUM_MERCURY_PRODUCT_SHENHUA = 12,			//��
	ENUM_MERCURY_PRODUCT_HONGXIU = 13,			//����
};

//Сվ��Ʒ��Ĭ�϶˿ڣ��ͻ���ͨ�����������õ�IP��ַ������Ĭ�϶˿ڿ������ӵ���ӵķ���������
enum ENUM_MERCURY_PRODUCT_COMM_PORT
{
	ENUM_MERCURY_PRODUCT_COMM_PORT_KELE = 2108,				//���֡�����:unicom.kele55.com
	ENUM_MERCURY_PRODUCT_COMM_PORT_QIXI = 2109,				//��ϲ	����:unicom.qixi55.com
	ENUM_MERCURY_PRODUCT_COMM_PORT_CAIHONG = 2110,			//�ʺ�	����:unicom.caihong55.com
	ENUM_MERCURY_PRODUCT_COMM_PORT_QIJI = 2111,				//�漣	����:unicom.qiji55.com
	ENUM_MERCURY_PRODUCT_COMM_PORT_JUXING = 2112,			//����	����:unicom.juxing55.com
	ENUM_MERCURY_PRODUCT_COMM_PORT_PINGUO = 2113,			//ƻ��	����:unicom.pinguo55.com
	ENUM_MERCURY_PRODUCT_COMM_PORT_SHENHUA = 2114,			//��	����:unicom.shenhua55.com
	ENUM_MERCURY_PRODUCT_COMM_PORT_HONGXIU = 2115,			//����	����:unicom.hongxiu55.com
};


//�豸����
#ifndef DEF_AUDIO_DEVICE
#define DEF_AUDIO_DEVICE 0	 //�����豸
#define DEF_VIDEO_DEVICE 1	 //��Ƶ�豸
#endif
#endif
