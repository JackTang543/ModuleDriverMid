#include "sBSP_I2C.h"
#include "sBSP_GPIO.h"

#include "sHMI_Debug.h"

//sBSP_I2C.c
//Sightseer's BSP I2C interface
//v1.0  2024.04.16inHNIP9607



//I2C1是否检查发送/接收错误
#define I2C1_CHECK_ERROR
//I2C1的轮询超时时间
#define I2C1_POT_MS    (100)
//I2C是否使用中断处理,默认使用轮询处理,此宏不影响Seq操作
//#define I2C1_USE_IT


uint8_t I2C1M_RxCpltFlag;
uint8_t I2C1M_TxCpltFlag;

I2C_HandleTypeDef hi2c1;

/**
  * @brief  I2C1的初始化
  *
  * @param  ClkSpdHz I2C时钟频率
  *
  * @return 如果正常返回0,其他为异常值
  */
int8_t sBSP_I2C1_Init(uint32_t ClkSpdHz){
    hi2c1.Instance = I2C1;
    hi2c1.Init.ClockSpeed = ClkSpdHz;
    hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
    //从模式地址,主模式不需要
    hi2c1.Init.OwnAddress1 = 0;
    //地址长度,从模式的设置
    hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    //双地址模式,从模式的设置
    hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    //第二个地址
    hi2c1.Init.OwnAddress2 = 0;
    //广播模式,如果地址为0x00,那么所有设备都会响应(若可用)
    hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    //时钟延展,从模式功能,作用:发送:你太快啦,等我一下
    hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;

    #ifdef I2C1_CHECK_ERROR
    if(HAL_I2C_Init(&hi2c1) == HAL_ERROR){return -1;}
    #else
    HAL_I2C_Init(&hi2c1);
    #endif 

    return 0;
}

void sBSP_I2C1_DeInit(){

}

/**
  * @brief  I2C1主机发送一个字节
  *
  * @param  DevAddr 设备地址
  * @param  data 要发送的数据
  *
  * @return 如果正常返回0,其他为异常值
  */
int8_t sBSP_I2C1M_SendByte(uint16_t DevAddr,uint8_t data){
    I2C1M_TxCpltFlag = 0;

    #ifdef I2C1_CHECK_ERROR
    #ifdef I2C1_USE_IT
    if(HAL_I2C_Master_Transmit_IT(&hi2c1,DevAddr,&data,1,I2C1_POT_MS) != HAL_OK) {
        return -1;
    }
    #else
    if(HAL_I2C_Master_Transmit(&hi2c1,DevAddr,&data,1,I2C1_POT_MS) != HAL_OK) {
        return -1;
    }
    #endif
    #else
    #ifdef I2C1_USE_IT
    HAL_I2C_Master_Transmit_IT(&hi2c1,DevAddr,&data,1,I2C1_POT_MS);
    #else
    HAL_I2C_Master_Transmit(&hi2c1,DevAddr,&data,1,I2C1_POT_MS);
    #endif
    #endif

    return 0;
}

/**
  * @brief  I2C1主机发送多个字节
  *
  * @param  DevAddr 设备地址
  * @param  pData 数据指针
  * @param  length 数据长度
  *
  * @return 如果正常返回0,其他为异常值
  */
int8_t sBSP_I2C1M_SendBytes(uint16_t DevAddr,uint8_t* pData,uint16_t length){
    I2C1M_TxCpltFlag = 0;
    
    #ifdef I2C1_CHECK_ERROR
    #ifdef I2C1_USE_IT
    if(HAL_I2C_Master_Transmit_IT(&hi2c1,DevAddr,pData,length,I2C1_POT_MS) != HAL_OK) {
        return -1;
    }
    #else
    if(HAL_I2C_Master_Transmit(&hi2c1,DevAddr,pData,length,I2C1_POT_MS) != HAL_OK) {
        return -1;
    }
    #endif
    #else
    #ifdef I2C1_USE_IT
    HAL_I2C_Master_Transmit_IT(&hi2c1,DevAddr,pData,length,I2C1_POT_MS);
    #else
    HAL_I2C_Master_Transmit(&hi2c1,DevAddr,pData,length,I2C1_POT_MS);
    #endif
    #endif

    return 0;
}

/**
  * @brief  I2C1主机接收一个字节
  *
  * @param  DevAddr 设备地址
  *
  * @return 返回的数据
  */
uint8_t sBSP_I2C1M_ReadByte(uint16_t DevAddr){
    I2C1M_RxCpltFlag = 0;

    uint8_t ret = 0;

    #ifdef I2C1_USE_IT
    HAL_I2C_Master_Receive_IT(&hi2c1,DevAddr,&ret,1,I2C1_POT_MS);
    #else
    HAL_I2C_Master_Receive(&hi2c1,DevAddr,&ret,1,I2C1_POT_MS);
    #endif

    return ret;
}

/**
  * @brief  I2C1主机接收多个字节
  *
  * @param  DevAddr 设备地址
  * @param  pData   数据指针
  * @param  length  数据长度
  *
  * @return 如果正常返回0,其他为异常值
  */
int8_t sBSP_I2C1M_ReadBytes(uint16_t DevAddr,uint8_t* pData,uint16_t length){
    I2C1M_RxCpltFlag = 0;

    #ifdef I2C1_CHECK_ERROR
    #ifdef I2C1_USE_IT
    if(HAL_I2C_Master_Receive_IT(&hi2c1,DevAddr,pData,length,I2C1_POT_MS) != HAL_OK) {
        return -1;
    }
    #else
    if(HAL_I2C_Master_Receive(&hi2c1,DevAddr,pData,length,I2C1_POT_MS) != HAL_OK) {
        return -1;
    }
    #endif
    #else
    #ifdef I2C1_USE_IT
    HAL_I2C_Master_Receive_IT(&hi2c1,DevAddr,pData,length,I2C1_POT_MS);
    #else
    HAL_I2C_Master_Receive(&hi2c1,DevAddr,pData,length,I2C1_POT_MS);
    #endif
    #endif

    return 0;
}

/**
  * @brief  I2C1主机顺序操作接收一个字节
  *
  * @param  DevAddr 设备地址
  * @param  XferOptions 操作,参考HAL库I2C @ref I2C_XferOptions_definition
  * @param  pData 接收到的数据
  *
  * @return 如果正常返回0,其他为异常值
  */
int8_t sBSP_I2C1M_SeqReadByte(uint16_t DevAddr,uint8_t* pData,uint32_t XferOptions){
    I2C1M_RxCpltFlag = 0;

    #ifdef I2C1_CHECK_ERROR
    if(HAL_I2C_Master_Seq_Receive_IT(&hi2c1,DevAddr,pData,1,XferOptions) != HAL_OK) {
        return -1;
    }
    #else
    HAL_I2C_Master_Seq_Receive_IT(&hi2c1,DevAddr,pData,1,XferOptions);
    #endif

    return 0;
}

/**
  * @brief  I2C1主机顺序操作接收多个字节
  *
  * @param  DevAddr 设备地址
  * @param  pData   数据指针
  * @param  length  数据长度
  * @param  XferOptions 操作,参考HAL库I2C @ref I2C_XferOptions_definition
  *
  * @return 如果正常返回0,其他为异常值
  */
int8_t sBSP_I2C1M_SeqReadBytes(uint16_t DevAddr,uint8_t* pData,uint16_t length,uint32_t XferOptions){
    I2C1M_RxCpltFlag = 0;

    #ifdef I2C1_CHECK_ERROR
    if(HAL_I2C_Master_Seq_Receive_IT(&hi2c1,DevAddr,pData,length,XferOptions) != HAL_OK) {
        return -1;
    }
    #else
    HAL_I2C_Master_Seq_Receive_IT(&hi2c1,DevAddr,pData,length,XferOptions);
    #endif

    return 0;
}

/**
  * @brief  I2C1主机顺序操作发送一个字节
  *
  * @param  DevAddr 设备地址
  * @param  XferOptions 操作,参考HAL库I2C @ref I2C_XferOptions_definition
  *
  * @return 如果正常返回0,其他为异常值
  */
int8_t sBSP_I2C1M_SeqSendByte(uint16_t DevAddr,uint8_t data,uint32_t XferOptions){
    I2C1M_TxCpltFlag = 0;

    #ifdef I2C1_CHECK_ERROR
    if(HAL_I2C_Master_Seq_Transmit_IT(&hi2c1,DevAddr,&data,1,XferOptions) != HAL_OK) {
        return -1;
    }
    #else
    HAL_I2C_Master_Seq_Transmit_IT(&hi2c1,DevAddr,&data,1,XferOptions);
    #endif

    return 0;
}

/**
  * @brief  I2C1主机顺序操作发送多个字节
  *
  * @param  DevAddr 设备地址
  * @param  pData   数据指针
  * @param  length  数据长度
  * @param  XferOptions 操作,参考HAL库I2C @ref I2C_XferOptions_definition
  *
  * @return 如果正常返回0,其他为异常值
  */
int8_t sBSP_I2C1M_SeqSendBytes(uint16_t DevAddr,uint8_t* pData,uint16_t length,uint32_t XferOptions){
    I2C1M_TxCpltFlag = 0;

    #ifdef I2C1_CHECK_ERROR
    if(HAL_I2C_Master_Seq_Transmit_IT(&hi2c1,DevAddr,pData,length,XferOptions) != HAL_OK) {
        return -1;
    }
    #else
    HAL_I2C_Master_Seq_Transmit_IT(&hi2c1,DevAddr,pData,length,XferOptions);
    #endif

    return 0;
}




inline int8_t sBSP_I2C1M_IsTxCplt(){
    return I2C1M_TxCpltFlag;
}

inline int8_t sBSP_I2C1M_IsRxCplt(){
    return I2C1M_RxCpltFlag;
}

inline int8_t sBSP_I2C1M_IsCplt(){
    return I2C1M_TxCpltFlag | I2C1M_RxCpltFlag;
}


void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c){
    if(hi2c->Instance == I2C1){
        I2C1M_TxCpltFlag = 1;
    }
}

void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c){
    if(hi2c->Instance == I2C1){
        I2C1M_RxCpltFlag = 1;
    }
}

void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c){
    if(hi2c->Instance == I2C1){
        sHMI_Debug_Printf("sBSP_I2C:ERROR\n");
    }
}

//重写HAL的MSP(MCU Support Package)Init
void HAL_I2C_MspInit(I2C_HandleTypeDef *hi2c){
    if(hi2c->Instance == I2C1){
        //PB7 -> I2C1.SDA
        //PB6 -> I2C1.SCL
        __GPIOB_CLK_ENABLE();
        GPIO_InitTypeDef GPIO_InitStruct = {0};
        GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
        GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(GPIOB,&GPIO_InitStruct);

        __I2C1_CLK_ENABLE();

        HAL_NVIC_SetPriority(I2C1_EV_IRQn, 1, 0);
        HAL_NVIC_EnableIRQ(I2C1_EV_IRQn);
        HAL_NVIC_SetPriority(I2C1_ER_IRQn, 1, 0);
        HAL_NVIC_EnableIRQ(I2C1_ER_IRQn);
    }
}

void HAL_I2C_MspDeInit(I2C_HandleTypeDef *hi2c){

}





