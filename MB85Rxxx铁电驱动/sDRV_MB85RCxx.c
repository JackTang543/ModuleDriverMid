#include "sDRV_MB85RCxx.h"
#include "sBSP_I2C.h"

//sDRV_MB85RCxx.c
//Sightseer's MB85RCxx铁电驱动
//
//v1.0 2024.04.16 inHNIP9607
//目前只支持MB85RC16


//目前只实现了RC16
#define MB85RC16
#define MB85RC16_BYTES    (2048)
//#define MB85RC64
//#define MB85RC256



//铁电的地址
#define MB85RCxx_ADDR       (0x50 << 1)

//获取11位地址高3位
#define __GET_ADDR_UPP3BIT(__ADDR__)        (__ADDR__ >> 8)
//获取11位地址的低8位
#define __GET_ADDR_LOW8BIT(__ADDR__)        (__ADDR__ & (0xFF))


static inline void delay(uint16_t ms){
    HAL_Delay(ms);
}

/**
  * @brief  初始化铁电
  *
  * @param  无
  *
  * @return 返回0表示正常
  */
int8_t sDRV_MB85RCxx_Init(){
    //数据手册上写可以支持1MHz的速度,实测没问题
    //10KHz~1MHz
    if(sBSP_I2C1_Init(10000) != 0){
        return -1;
    }
    return 0;
}

/**
  * @brief  写一个字节
  *
  * @param  addr 写入的地址
  * @param  byte 数据
  *
  * @return 返回0表示正常
  */
int8_t sDRV_MB85RCxx_WriteByte(uint16_t addr,uint8_t byte){
    uint8_t tmp[] = {__GET_ADDR_LOW8BIT(addr),byte};
    sBSP_I2C1M_SendBytes(MB85RCxx_ADDR | __GET_ADDR_UPP3BIT(addr),tmp,2);
    return 0;
}

/**
  * @brief  写多个字节
  *
  * @param  addr   起始地址
  * @param  pData  数据指针
  * @param  length 数据长度
  *
  * @return 返回0表示正常
  */
int8_t sDRV_MB85RCxx_WriteBytes(uint16_t addr,uint8_t* pData,uint16_t length){
    sBSP_I2C1M_SeqSendByte(MB85RCxx_ADDR | __GET_ADDR_UPP3BIT(addr),__GET_ADDR_LOW8BIT(addr),I2C_FIRST_FRAME);
    delay(1);
    sBSP_I2C1M_SeqSendBytes(MB85RCxx_ADDR | __GET_ADDR_UPP3BIT(addr),pData,length,I2C_LAST_FRAME);
    delay(length);
    return 0;
}

/**
  * @brief  读一个字节
  *
  * @param  addr   地址
  *
  * @return 读到的数据
  */
uint8_t sDRV_MB85RCxx_ReadByte(uint16_t addr){
    uint8_t tmp = 0;
    sBSP_I2C1M_SeqSendByte(MB85RCxx_ADDR | __GET_ADDR_UPP3BIT(addr),__GET_ADDR_LOW8BIT(addr),I2C_FIRST_FRAME);
    delay(2);
    sBSP_I2C1M_SeqReadByte(MB85RCxx_ADDR | __GET_ADDR_UPP3BIT(addr),&tmp,I2C_LAST_FRAME);
    delay(2);
    return tmp;
}

/**
  * @brief  读多个字节
  *
  * @param  addr   起始地址
  * @param  pData  数据指针
  * @param  length 数据长度
  *
  * @return 返回0表示正常
  */
int8_t sDRV_MB85RCxx_ReadBytes(uint16_t addr,uint8_t* pData,uint16_t length){
    sBSP_I2C1M_SeqSendByte(MB85RCxx_ADDR | __GET_ADDR_UPP3BIT(addr),__GET_ADDR_LOW8BIT(addr),I2C_FIRST_FRAME);
    delay(2);
    sBSP_I2C1M_SeqReadBytes(MB85RCxx_ADDR | __GET_ADDR_UPP3BIT(addr),pData,length,I2C_LAST_FRAME);
    delay(length);
    return 0;
}

/**
  * @brief  格式化铁电
  *
  * @param  all_val : 格式化的值(让所有值都等于)
  * @return 返回0表示正常
  */
int8_t sDRV_MB85RCxx_Format(uint8_t all_val){
    for(uint32_t i = 0;i < MB85RC16_BYTES;i++){
        sDRV_MB85RCxx_WriteByte(i,all_val);
    }
    return 0;
}

