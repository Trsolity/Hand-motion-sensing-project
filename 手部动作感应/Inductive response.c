
/*预处理命令*/
#include <reg52.h> 		  //包含单片机寄存器的头文件
#define uchar unsigned char
#define uint unsigned int

//定义四个数码管的变量（我只用了P2_3)
sbit P2_0 = P2^0;
sbit P2_1 = P2^1;
sbit P2_2 = P2^2;
sbit P2_3 = P2^3;

/*一个数组，包含了数码管的几种显示状态 */
uchar code table[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,
                        0xf8,0x80,0x90,0xf7,0xff}; //0 1 2 3 4 5 6 7 8 9 _ 空

uchar dat,num,count0;
uchar  count0 = 0;
uchar  flag =0;

//void delay_ms(unsigned int xms)  //调用时需提供参数xms的实际值。
//{
//	unsigned int i,j;   //定义无符号整型变量i、j
//	for(i=0; i<xms; i++)  //后面没带";"，下一条语句是循环体。参数xms延时x毫秒。
//	    for(j=0; j<110; j++);  //后面带";"，表示这个for语句的循环体是空语句。
//}

/*定时器0中断初始化函数*/
 void Time_init(void)
 {	 	
 	TMOD = 0x21;					// 定时器工作方式都为1	
    TH0 = 0x4c;  					// 设置定时器0初始值，这里设置为FC18h（约1秒）（这是文心一言写的，我也没关注定时的时间对不对  
    TL0 = 0x00;  					// 设置定时器0低8位初始值		     
	ET0=1;							//开启定时器0中断
	TR0=1;							//启动定时器0
 }

/*串口初始化函数*/
 void sbuf_init(void)
 {	
 	TMOD=0x21;		   //用定时器设置串口波特率
	TH1=0xfd;
	TL1=0xfd;
	TR1=1;
 	EA=1;
 	REN=1;        					//串口初始化
	SM0=0;							//工作方式0
	SM1=1;
 	ES=1;
}

 /*主函数*/
void main()
{	

	sbuf_init();					//串口初始化
	Time_init();					//定时中断初始化
	while(1)
	{	
		 if(flag == 1)					//出现定时中断，
		 {
		 	flag = 0;
			switch(dat)    				//判断串口接收的数据
				{
					case 49:P0=table[1];
							P2_3 = 0;break; //第四个数码管显示为数字1
					case 50:P0=table[2];
							P2_3 = 0;break; //第四个数码管显示为数字2
					case 51:P0=table[3];
							P2_3 = 0;break; //第四个数码管显示为数字3
					case 52:P0=table[4];
							P2_3 = 0;break; //第四个数码管显示为数字4
					default:P0=table[8];
							P2_3 = 0;break;  		//第四个数码管显示为数字8
				}
				 ES =1;
		 }
					
	 }
}


/*定时器0中断服务程序*/
void t0_time(void) interrupt 1		
{
	TH0 = 0x4c;  					// 重新加载定时器初始值  
    TL0 = 0x00;  					// 重新加载定时器低8位初始值  
    count0++;	 					//计算定时器溢出次数
	
	if(count0==40)			
	{	ES =1;
		flag = 1;
		count0=0;
//		P0=table[2];	 			//让数码管的底部亮
//   		P2_1 = 0;		
//   		delay_ms(500);
//   		P2_1 = 1;		
	} 
}

/*串口中断服务程序*/
void ser(void) interrupt 4		 
{
	RI=0;							//接收中断标志。当串行接收到一个字节的数据结束时，RI会被硬件自动置位
	dat=SBUF;						//接收数据SBUF，即将计算机的数据接收。
	//while(!TI)
	// 	TI = 0;							//在其他方式中，响应中断后TI必须用软件清零。
	//flag = 1;	
	SBUF=dat;			   
	ES=0;
}

