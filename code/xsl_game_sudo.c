/**
  ******************************************************************************
  * @file    xsl_game_sudo.c
  * @author  徐松亮 许红宁(5387603@qq.com)
  * @version V1.0.0
  * @date    2018/11/01
  ******************************************************************************
  * @attention
  * 1,解决多层次数独
  * 2,计算最大可能结果数量
  * 3,数独出题,并遍历最大数量
  * 待解决
  * 1,数独出题
  * 2,一共有多少数独题
  * GNU General Public License (GPL)
  *
  * <h2><center>&copy; COPYRIGHT 2017 XSLXHN</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdint.h>
// 用于计算耗时统计
#include <time.h>
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define XSLGAMESUDO_SUDOKU_LEVEL 9
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static uint8_t XslGameSudo_SudoBuf[XSLGAMESUDO_SUDOKU_LEVEL][XSLGAMESUDO_SUDOKU_LEVEL] = {
	8, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 3, 6, 0, 0, 0, 0, 0,
	0, 7, 0, 0, 9, 0, 2, 0, 0,
	0, 5, 0, 0, 0, 7, 0, 0, 0,
	0, 0, 0, 0, 4, 5, 7, 0, 0,
	0, 0, 0, 1, 0, 0, 0, 3, 0,
	0, 0, 1, 0, 0, 0, 0, 6, 8,
	0, 0, 8, 5, 0, 0, 0, 1, 0,
	0, 9, 0, 0, 0, 0, 4, 0, 0};
/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/**
 * @brief   格式化打印
 * @note    打印指定行列的数组
 * @param   *pbuf	---	指针
 * 			row		---	行
 * 			col		---	列
 * @return  null
 */
static void XslGameSudo_FormatPrint(uint8_t *pbuf, uint8_t row, uint8_t col)
{
	uint8_t i, j;

	for (i = 0; i < row; i++)
	{
		for (j = 0; j < col; j++)
		{
			printf("%2d", pbuf[i * row + j]);
			if (j == (col - 1))
				printf("\n");
		}
	}
}
/**
 * @brief   寻找下一个未填充的单元
 * @note    寻找下一个未填充的单元
 * @param   arr
 * 			startrow
 * 			*row
 * 			*col
 * @return  0-没有空单元	1-有空单元
 */
static uint8_t XslGameSudo_findNextEmpty(uint8_t arr[XSLGAMESUDO_SUDOKU_LEVEL][XSLGAMESUDO_SUDOKU_LEVEL], int startrow, uint8_t *row, uint8_t *col)
{
	uint8_t i, j;

	for (i = startrow; i < XSLGAMESUDO_SUDOKU_LEVEL; i++)
	{
		for (j = 0; j < XSLGAMESUDO_SUDOKU_LEVEL; j++)
		{
			if (arr[i][j] == 0)
			{
				*row = i;
				*col = j;
				return 1;
			}
		}
	}
	return 0;
}

/**
 * @brief   寻找下一个未填充的单元
 * @note    寻找下一个未填充的单元
 * @param   arr
 * 			row
 * 			col
 * @return  0-未填写	1-填写
 */
int do_resolve(uint8_t arr[XSLGAMESUDO_SUDOKU_LEVEL][XSLGAMESUDO_SUDOKU_LEVEL], uint8_t row, uint8_t col)
{
	uint8_t i = 0, j = 0, n = 0;
	uint8_t next_row = 0, next_col = 0;
	while (1)
	{
	next_num:
		// 填充失败判断-->尝试填充1-9都失败
		++n;
		if (n >= (XSLGAMESUDO_SUDOKU_LEVEL + 1))
		{
			break;
		}
		// 填充违规判断--1-->判断行重复
		for (j = 0; j < XSLGAMESUDO_SUDOKU_LEVEL; j++)
		{
			if (arr[row][j] == n)
			{
				goto next_num;
			}
		}

		// 填充违规判断--2-->判断列重复
		for (i = 0; i < XSLGAMESUDO_SUDOKU_LEVEL; i++)
		{
			if (arr[i][col] == n)
			{
				goto next_num;
			}
		}

		// 填充违规判断--3-->判断所在小九宫格重复
		uint8_t x = (row / 3) * 3;
		uint8_t y = (col / 3) * 3;
		for (i = x; i < x + 3; i++)
		{
			for (j = y; j < y + 3; j++)
			{
				if (arr[i][j] == n)
				{
					goto next_num;
				}
			}
		}

		//填充确认-->可以填充
		arr[row][col] = n;
		//----------------------------------------------------
		//如果9宫格已填满，完成，这里不考虑有多解的情况
		if (!find_next_empty(arr, row, &next_row, &next_col))
		{
			//将数据记录下来
			//????????????????????????
			//????????????????????????
			return 1;
		}
		//否则继续填下一个未填充的格子
		if (!do_resolve(arr, next_row, next_col))
		{
			arr[row][col] = 0;
			continue;
		}
		else
		{
			return 1;
		}
	}
	// 失败
	return 0;
}

/**
 * @brief   解析数独
 * @note    解析行列都为COUNT_SUDOKU_LEVEL的数独题
 * @param   a
 * @return  0-未填写	1-填写
 */
uint8_t XslGameSudo_Processor(uint8_t a[XSLGAMESUDO_SUDOKU_LEVEL][XSLGAMESUDO_SUDOKU_LEVEL])
{
	uint8_t row, col;
	uint8_t res;
	//找到还有没有可能性
	find_next_empty(a, 0, &row, &col);
	//解决问题
	res = do_resolve(a, row, col);
	//条件判定
	if (res == 0)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}
/*
uint8_t XslGameSudo_Processor1(pro)
{
	if(递归终止条件)
	{
		T	<-	P的结果
		return T;
	}
	//
}
*/



/**
 * @brief   main函数
 * @note    主函数入口
 * @param   null
 * @return  null
 */
void main(void)
{
	uint8_t res;
	uint32_t time1, time2;
	// 启动数独测试
	time1 = GetTickCount();
	res = XslGameSudo_Processor(XslGameSudo_SudoBuf);
	time2 = GetTickCount();
	// 打印结果
	XslGameSudo_FormatPrint((uint8_t *)&XslGameSudo_SudoBuf[0][0], XSLGAMESUDO_SUDOKU_LEVEL, XSLGAMESUDO_SUDOKU_LEVEL);
	// 打印耗时
	printf("Time(ms):%ld\n", time2 - time1);
	// 打印计算成功或失败
	if (res == 1)
	{
		printf("Sudoku OK");
	}
	else
	{
		printf("Sudoku ERR");
	}
	// 定住页面,否则程序结束直接闪退,延时10秒自动退出
	time1 = time2 = 0;
	time1 = GetTickCount();
	while (((time2 - time1) < 10000)||(time2==0))
	{
		time2 = GetTickCount();
	}
}
