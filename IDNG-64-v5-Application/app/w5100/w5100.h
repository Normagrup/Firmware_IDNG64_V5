/*
@file		w5100.h
*/

#ifndef	_W5100_H_
#define	_W5100_H_


#define MR __DEF_IINCHIP_MAP_BASE__
#define IDM_OR ((__DEF_IINCHIP_MAP_BASE__ + 0x00))
#define IDM_AR0 ((__DEF_IINCHIP_MAP_BASE__ + 0x01))
#define IDM_AR1 ((__DEF_IINCHIP_MAP_BASE__ + 0x02))
#define IDM_DR ((__DEF_IINCHIP_MAP_BASE__ + 0x03))


/**
 @brief Gateway IP Register address
 */
#define GAR0				(COMMON_BASE + 0x0001)
/**
 @brief Subnet mask Register address
 */
#define SUBR0			(COMMON_BASE + 0x0005)
/**
 @brief Source MAC Register address
 */
#define SHAR0				(COMMON_BASE + 0x0009)
/**
 @brief Source IP Register address
 */
#define SIPR0				(COMMON_BASE + 0x000F)
/**
 @brief Interrupt Register
 */
#define IR					(COMMON_BASE + 0x0015)
/**
 @brief Interrupt mask register
 */
#define IMR					(COMMON_BASE + 0x0016)
/**
 @brief Timeout register address( 1 is 100us )
 */
#define RTR0				(COMMON_BASE + 0x0017)
/**
 @brief Retry count reigster
 */
#define RCR						(COMMON_BASE + 0x0019)
/**
 @brief Receive memory size reigster
 */
#define RMSR			(COMMON_BASE + 0x001A)
/**
 @brief Transmit memory size reigster
 */
#define TMSR			(COMMON_BASE + 0x001B)
/**
 @brief Authentication type register address in PPPoE mode
 */
#define PATR0					(COMMON_BASE + 0x001C)
//#define PPPALGO (COMMON_BASE + 0x001D)
#define PTIMER (COMMON_BASE + 0x0028)
#define PMAGIC (COMMON_BASE + 0x0029)

/**
 @brief Unreachable IP register address in UDP mode
 */
#define UIPR0				(COMMON_BASE + 0x002A)
/**
 @brief Unreachable Port register address in UDP mode
 */
#define UPORT0			(COMMON_BASE + 0x002E)

/**
 @brief unsigned char  register
*/
#define CH_BASE (COMMON_BASE + 0x0400)
/**
 @brief	size of each channel register map
 */
#define CH_SIZE		0x0100
/**
 @brief unsigned char  Mode register
 */
#define Sn_MR(ch)		(CH_BASE + ch * CH_SIZE + 0x0000)
/**
 @brief channel Sn_CR register
 */
#define Sn_CR(ch)				(CH_BASE + ch * CH_SIZE + 0x0001)
/**
 @brief channel interrupt register
 */
#define Sn_IR(ch)			(CH_BASE + ch * CH_SIZE + 0x0002)
/**
 @brief channel status register
 */
#define Sn_SR(ch)			(CH_BASE + ch * CH_SIZE + 0x0003)
/**
 @brief source port register
 */
#define Sn_PORT0(ch)		(CH_BASE + ch * CH_SIZE + 0x0004)
/**
 @brief Peer MAC register address
 */
#define Sn_DHAR0(ch)			(CH_BASE + ch * CH_SIZE + 0x0006)
/**
 @brief Peer IP register address
 */
#define Sn_DIPR0(ch)			(CH_BASE + ch * CH_SIZE + 0x000C)
/**
 @brief Peer port register address
 */
#define Sn_DPORT0(ch)		(CH_BASE + ch * CH_SIZE + 0x0010)
/**
 @brief Maximum Segment Size(Sn_MSSR0) register address
 */
#define Sn_MSSR0(ch)					(CH_BASE + ch * CH_SIZE + 0x0012)
/**
 @brief Protocol of IP Header field register in IP raw mode
 */
#define Sn_PROTO(ch)			(CH_BASE + ch * CH_SIZE + 0x0014)

/** 
 @brief IP Type of Service(TOS) Register 
 */
#define Sn_TOS(ch)						(CH_BASE + ch * CH_SIZE + 0x0015)
/**
 @brief IP Time to live(TTL) Register 
 */
#define Sn_TTL(ch)						(CH_BASE + ch * CH_SIZE + 0x0016)

/**
 @brief Transmit free memory size register
 */
#define Sn_TX_FSR0(ch)	(CH_BASE + ch * CH_SIZE + 0x0020)
/**
 @brief Transmit memory read pointer register address
 */
#define Sn_TX_RD0(ch)			(CH_BASE + ch * CH_SIZE + 0x0022)
/**
 @brief Transmit memory write pointer register address
 */
#define Sn_TX_WR0(ch)			(CH_BASE + ch * CH_SIZE + 0x0024)
/**
 @brief Received data size register
 */
#define Sn_RX_RSR0(ch)	(CH_BASE + ch * CH_SIZE + 0x0026)
/**
 @brief Read point of Receive memory
 */
#define Sn_RX_RD0(ch)			(CH_BASE + ch * CH_SIZE + 0x0028)
/**
 @brief Write point of Receive memory
 */
#define Sn_RX_WR0(ch)			(CH_BASE + ch * CH_SIZE + 0x002A)



/* MODE register values */
#define MR_RST			0x80 /**< reset */
#define MR_PB			0x10 /**< ping block */
#define MR_PPPOE		0x08 /**< enable pppoe */
#define MR_LB  		0x04 /**< little or big endian selector in indirect mode */
#define MR_AI			0x02 /**< auto-increment in indirect mode */
#define MR_IND			0x01 /**< enable indirect mode */

/* IR register values */
#define IR_CONFLICT	0x80 /**< check ip confict */
#define IR_UNREACH	0x40 /**< get the destination unreachable message in UDP sending */
#define IR_PPPoE		0x20 /**< get the PPPoE close message */
#define IR_SOCK(ch)	(0x01 << ch) /**< check unsigned char  interrupt */

/* Sn_MR values */
#define Sn_MR_CLOSE		0x00		/**< unused unsigned char  */
#define Sn_MR_TCP		0x01		/**< TCP */
#define Sn_MR_UDP		0x02		/**< UDP */
#define Sn_MR_IPRAW	0x03		/**< IP LAYER RAW SOCK */
#define Sn_MR_MACRAW	0x04		/**< MAC LAYER RAW SOCK */
#define Sn_MR_PPPOE		0x05		/**< PPPoE */
#define Sn_MR_ND		0x20		/**< No Delayed Ack(TCP) flag */
#define Sn_MR_MULTI		0x80		/**< support multicating */


/* Sn_CR values */
#define Sn_CR_OPEN		0x01		/**< initialize or open unsigned char  */
#define Sn_CR_LISTEN		0x02		/**< wait connection request in tcp mode(Server mode) */
#define Sn_CR_CONNECT	0x04		/**< send connection request in tcp mode(Client mode) */
#define Sn_CR_DISCON		0x08		/**< send closing reqeuset in tcp mode */
#define Sn_CR_CLOSE		0x10		/**< close unsigned char  */
#define Sn_CR_SEND		0x20		/**< updata txbuf pointer, send data */
#define Sn_CR_SEND_MAC	0x21		/**< send data with MAC address, so without ARP process */
#define Sn_CR_SEND_KEEP	0x22		/**<  send keep alive message */
#define Sn_CR_RECV		0x40		/**< update rxbuf pointer, recv data */

#ifdef __DEF_IINCHIP_PPP__
	#define Sn_CR_PCON				0x23		 
	#define Sn_CR_PDISCON			0x24		 
	#define Sn_CR_PCR					0x25		 
	#define Sn_CR_PCN					0x26		
	#define Sn_CR_PCJ					0x27		
#endif

/* Sn_IR values */
#ifdef __DEF_IINCHIP_PPP__
	#define Sn_IR_PRECV			0x80		
	#define Sn_IR_PFAIL			0x40		
	#define Sn_IR_PNEXT			0x20		
#endif
#define Sn_IR_SEND_OK			0x10		/**< complete sending */
#define Sn_IR_TIMEOUT			0x08		/**< assert timeout */
#define Sn_IR_RECV				0x04		/**< receiving data */
#define Sn_IR_DISCON				0x02		/**< closed unsigned char  */
#define Sn_IR_CON					0x01		/**< established connection */

/* Sn_SR values */
#define SOCK_CLOSED				0x00		/**< closed */
#define SOCK_INIT 				0x13		/**< init state */
#define SOCK_LISTEN				0x14		/**< listen state */
#define SOCK_SYNSENT	   		0x15		/**< connection state */
#define SOCK_SYNRECV		   	0x16		/**< connection state */
#define SOCK_ESTABLISHED		0x17		/**< success to connect */
#define SOCK_FIN_WAIT			0x18		/**< closing state */
#define SOCK_CLOSING		   	0x1A		/**< closing state */
#define SOCK_TIME_WAIT			0x1B		/**< closing state */
#define SOCK_CLOSE_WAIT			0x1C		/**< closing state */
#define SOCK_LAST_ACK			0x1D		/**< closing state */
#define SOCK_UDP				   0x22		/**< udp unsigned char  */
#define SOCK_IPRAW			   0x32		/**< ip raw mode unsigned char  */
#define SOCK_MACRAW			   0x42		/**< mac raw mode unsigned char  */
#define SOCK_PPPOE				0x5F		/**< pppoe unsigned char  */

/* IP PROTOCOL */
#define IPPROTO_IP              0           /**< Dummy for IP */
#define IPPROTO_ICMP            1           /**< Control message protocol */
#define IPPROTO_IGMP            2           /**< Internet group management protocol */
#define IPPROTO_GGP             3           /**< Gateway^2 (deprecated) */
#define IPPROTO_TCP             6           /**< TCP */
#define IPPROTO_PUP             12          /**< PUP */
#define IPPROTO_UDP             17          /**< UDP */
#define IPPROTO_IDP             22          /**< XNS idp */
#define IPPROTO_ND              77          /**< UNOFFICIAL net disk protocol */
#define IPPROTO_RAW             255         /**< Raw IP packet */


/*********************************************************
* iinchip access function
*********************************************************/
extern unsigned char IINCHIP_READ(unsigned int addr);
extern unsigned char IINCHIP_WRITE(unsigned int addr,unsigned char data);
extern unsigned int wiz_read_buf(unsigned int addr, unsigned char * buf,unsigned int len);
extern unsigned int wiz_write_buf(unsigned int addr,unsigned char * buf,unsigned int len);

extern void iinchip_init(void); // reset iinchip
extern void sysinit(unsigned char tx_size, unsigned char rx_size); // setting tx/rx buf size
extern unsigned char getISR(unsigned char s);
extern void putISR(unsigned char s, unsigned char val);
extern unsigned int getIINCHIP_RxMAX(unsigned char s);
extern unsigned int getIINCHIP_TxMAX(unsigned char s);
extern unsigned int getIINCHIP_RxMASK(unsigned char s);
extern unsigned int getIINCHIP_TxMASK(unsigned char s);
extern unsigned int getIINCHIP_RxBASE(unsigned char s);
extern unsigned int getIINCHIP_TxBASE(unsigned char s);
extern void setGAR(unsigned char * addr); // set gateway address
extern void setSUBR(unsigned char * addr); // set subnet mask address
extern void setSHAR(unsigned char * addr); // set local MAC address
extern void setSIPR(unsigned char * addr); // set local IP address
extern void setRTR(unsigned int timeout); // set retry duration for data transmission, connection, closing ...
extern void setRCR(unsigned char retry); // set retry count (above the value, assert timeout interrupt)
extern void setIMR(unsigned char mask); // set interrupt mask. 
extern void getGAR(unsigned char * addr);
extern void getSUBR(unsigned char * addr);
extern void getSHAR(unsigned char * addr);
extern void getSIPR(unsigned char * addr);
extern unsigned char getIR( void );
extern void setSn_MSS(unsigned char  s, unsigned int Sn_MSSR0); // set maximum segment size
extern void setSn_PROTO(unsigned char  s, unsigned char proto); // set IP Protocol value using IP-Raw mode
extern unsigned char getSn_IR(unsigned char  s); // get unsigned char  interrupt status
extern unsigned char getSn_SR(unsigned char  s); // get unsigned char  status
extern unsigned int getSn_TX_FSR(unsigned char  s); // get unsigned char  TX free buf size
extern unsigned int getSn_RX_RSR(unsigned char  s); // get unsigned char  RX recv buf size
extern void setSn_DHAR(unsigned char  s, unsigned char * addr);
extern void setSn_DIPR(unsigned char  s, unsigned char * addr);
extern void setSn_DPORT(unsigned char  s, unsigned char * addr);
extern void getSn_DHAR(unsigned char  s, unsigned char * addr);
extern void getSn_DIPR(unsigned char  s, unsigned char * addr);
extern void getSn_DPORT(unsigned char  s, unsigned char * addr);
extern void setSn_TTL(unsigned char  s, unsigned char ttl);
extern void setMR(unsigned char val);


void clearSUBR (void);
void applySUBR (void);


#ifdef __DEF_IINCHIP_PPP__
extern unsigned char pppinit(unsigned char *id, unsigned char idlen, unsigned char *passwd, unsigned char passwdlen);
extern unsigned char pppterm(unsigned char *mac,unsigned char *sessionid);
#endif

extern void send_data_processing(unsigned char  s, unsigned char *data, unsigned int len);
extern void recv_data_processing(unsigned char  s, unsigned char *data, unsigned int len);
extern void read_data(unsigned char  s, volatile unsigned char * src, volatile unsigned char * dst, unsigned int len);
extern void write_data(unsigned char  s, volatile unsigned char * src, volatile unsigned char * dst, unsigned int len);

#endif
