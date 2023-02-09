#ifndef __AT24CXX_H__
#define __AT24CXX_H__

#include "sys.h"

/*=================================================
	AT24CXX选型
=================================================*/

#define AT24C01						(128)
#define AT24C02						(256)
#define AT24C04						(512)
#define AT24C08						(1024)
#define AT24C16						(2048)
#define AT24C32						(4096)
#define AT24C64						(8192)
#define AT24C128					(16384)
#define AT24C256					(32768)
#define EE_TYPE						AT24C02
#define EE_SIZE						EE_TYPE

#define AT24C02_PAGE_SIZE			(8)
#define AT24C04_PAGE_SIZE			(16)
#define AT24C08_PAGE_SIZE			(16)
#define AT24C16_PAGE_SIZE			(16)
#define AT24C32_PAGE_SIZE			(32)
#define AT24C64_PAGE_SIZE			(32)
#define EE_PAGE_SIZE				AT24C02_PAGE_SIZE

#define EE_ADDR						(0XA0)
#define EE_WRITE					(0XA0)
#define EE_READ						(0XA1)

/*=================================================
	AT24CXX的BUFF
=================================================*/

extern uint8_t EE_Write_Buffer[EE_SIZE];
extern uint8_t EE_Read_Buffer[EE_SIZE];

/*=================================================
	AT24CXX函数状态
=================================================*/

typedef enum{
	AT24CXX_OK = 0,
	AT24CXX_ERROR = 1
}at24cxx_err_t;

/*=================================================
	AT24CXX相关函数
=================================================*/

at24cxx_err_t AT24CXX_Init(void);

uint8_t AT24CXX_Read_OneByte(uint16_t MemAddress);
at24cxx_err_t AT24CXX_Read_MultiByte(uint16_t MemAddress, uint8_t *pData, uint16_t Size);
at24cxx_err_t AT24CXX_Write_OneByte(uint16_t MemAddress, uint8_t write_byte);
at24cxx_err_t AT24CXX_Write_MultiByte(uint16_t MemAddress, uint8_t *pData, uint16_t Size);

at24cxx_err_t AT24CXX_Print_Read_Buffer(void);

#endif /* __AT24CXX_H__ */
