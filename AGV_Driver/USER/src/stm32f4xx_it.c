/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/stm32f4xx_it.c 
  * @author  MCD Application Team
  * @version V1.4.0
  * @date    04-August-2014
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_it.h"
#include "Timer.h" 
#include "Usart1.h"
#include "Power.h"
#include "BMS.h"
#include "Adc.h"
#include "stm32f4xx.h"
#include "Rfid.h"
/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
	
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
	  if (CoreDebug->DHCSR & 1) {  //check C_DEBUGEN == 1 -> Debugger Connected  
      __breakpoint(0);  // halt program execution here         
  }
  while (1)
  {
		
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
	//内存异常
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
//void SVC_Handler(void)
//{
//}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
//void PendSV_Handler(void)
//{
//}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
//void SysTick_Handler(void)
//{
// 
//}

/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 

/* 以下为添加中断处理函数代码 */

/* 定时器1-10 */
void TIM2_IRQHandler(void)
{
  if((TIM2->SR & TIM_SR_UIF) != RESET && (TIM2->DIER & TIM_DIER_UIE) != RESET)
  {
     TIM2->SR = (uint16_t)~TIM_SR_UIF;
//     Timer_IQR();
  }
}

void TIM3_IRQHandler(void)
{
  if((TIM3->SR & TIM_SR_UIF) != RESET && (TIM3->DIER & TIM_DIER_UIE) != RESET)
  {
     TIM3->SR = (uint16_t)~TIM_SR_UIF;
     Timer_IQR();
  }
}
/* 外部中断 */
void EXTI2_IRQHandler(void)
{
	if((EXTI->PR & EXTI_Line2) != (uint32_t)RESET)
	{
		 //IR_Ctrl_1();
		 EXTI->PR = EXTI_Line2;
	}

}

void EXTI3_IRQHandler(void)
{

	if((EXTI->PR & EXTI_Line3) != (uint32_t)RESET)
	{
		//IR_Ctrl_2();
		 EXTI->PR = EXTI_Line3;
	}
}

void EXTI4_IRQHandler(void)
{
  if((EXTI->PR & EXTI_Line4) != (uint32_t)RESET)
	{
		//IR_Ctrl_3();
		 EXTI->PR = EXTI_Line4;
	}

}

void USART1_IRQHandler(void)
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		Uart1_ReceiveIQR((uint8_t)USART1->DR);
	   USART_ClearFlag(USART1, USART_IT_RXNE);
	}
	
	if(USART_GetITStatus(USART1, USART_IT_TXE) != RESET)
	{
//		Uart1_TansmitIQR((uint8_t)USART1->DR);//预留
	   USART_ClearFlag(USART1, USART_IT_TXE);
	}

}

void USART2_IRQHandler(void)
{
		if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
//		Uart2_ReceiveIQR((uint8_t)USART2->DR);
	   USART_ClearFlag(USART2, USART_IT_RXNE);
	}

		if(USART_GetITStatus(USART2, USART_IT_TXE) != RESET)
	{
//		Uart2_TansmitIQR((uint8_t)USART2->DR);//预留
	   USART_ClearFlag(USART2, USART_IT_TXE);
	}
}
void USART3_IRQHandler(void)
{
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{
	   Uart3_ReceiveIQR((uint8_t)USART3->DR);
	   USART_ClearFlag(USART3, USART_IT_RXNE);
	}

		if(USART_GetITStatus(USART3, USART_IT_TXE) != RESET)
	{
//		Uart2_TansmitIQR((uint8_t)USART2->DR);//预留
	   USART_ClearFlag(USART2, USART_IT_TXE);
	}
}
void UART4_IRQHandler(void)
{

  	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)
	{
		USART_ClearFlag(UART4, USART_IT_RXNE);
		Uart4_ReceiveIQR((uint8_t)UART4->DR);//预留 
	}

		if(USART_GetITStatus(UART4, USART_IT_TXE) != RESET)
	{
//		Uart4_TansmitIQR((uint8_t)UART4->DR);//预留
	   USART_ClearFlag(UART4, USART_IT_TXE);
	}
	
}

void USART6_IRQHandler(void)
{
		if(USART_GetITStatus(USART6, USART_IT_RXNE) != RESET)
	{
	   Uart6_ReceiveIQR((uint8_t)USART6->DR);//预留
	   USART_ClearFlag(USART6, USART_IT_RXNE);
	}

		if(USART_GetITStatus(USART6, USART_IT_TXE) != RESET)
	{
//		Uart6_TansmitIQR((uint8_t)USART6->DR);//预留
	   USART_ClearFlag(USART6, USART_IT_TXE);
	}
	
}

void DMA2_Stream0_IRQHandler(void)
{
  if(DMA_GetITStatus(DMA2_Stream0,DMA_IT_TCIF0) !=RESET)
  {
	  DMA_ClearFlag(DMA2_Stream0,DMA_IT_TCIF0);
      DMA_IT_Ctrl();//ADC数据处理
  }
}

void ADC_IRQHandler(void)
{
  if(ADC_GetFlagStatus(ADC3,ADC_FLAG_EOC)!= RESET)
  {
	 ADC_ClearFlag(ADC3,ADC_FLAG_EOC);
     ADC_IT_Ctrl();
  }

}
	






/* End */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
