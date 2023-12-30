
/*Ԥ��������*/
#include <reg52.h> 		  //������Ƭ���Ĵ�����ͷ�ļ�
#define uchar unsigned char
#define uint unsigned int

//�����ĸ�����ܵı�������ֻ����P2_3)
sbit P2_0 = P2^0;
sbit P2_1 = P2^1;
sbit P2_2 = P2^2;
sbit P2_3 = P2^3;

/*һ�����飬����������ܵļ�����ʾ״̬ */
uchar code table[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,
                        0xf8,0x80,0x90,0xf7,0xff}; //0 1 2 3 4 5 6 7 8 9 _ ��

uchar dat,num,count0;
uchar  count0 = 0;
uchar  flag =0;

//void delay_ms(unsigned int xms)  //����ʱ���ṩ����xms��ʵ��ֵ��
//{
//	unsigned int i,j;   //�����޷������ͱ���i��j
//	for(i=0; i<xms; i++)  //����û��";"����һ�������ѭ���塣����xms��ʱx���롣
//	    for(j=0; j<110; j++);  //�����";"����ʾ���for����ѭ�����ǿ���䡣
//}

/*��ʱ��0�жϳ�ʼ������*/
 void Time_init(void)
 {	 	
 	TMOD = 0x21;					// ��ʱ��������ʽ��Ϊ1	
    TH0 = 0x4c;  					// ���ö�ʱ��0��ʼֵ����������ΪFC18h��Լ1�룩����������һ��д�ģ���Ҳû��ע��ʱ��ʱ��Բ���  
    TL0 = 0x00;  					// ���ö�ʱ��0��8λ��ʼֵ		     
	ET0=1;							//������ʱ��0�ж�
	TR0=1;							//������ʱ��0
 }

/*���ڳ�ʼ������*/
 void sbuf_init(void)
 {	
 	TMOD=0x21;		   //�ö�ʱ�����ô��ڲ�����
	TH1=0xfd;
	TL1=0xfd;
	TR1=1;
 	EA=1;
 	REN=1;        					//���ڳ�ʼ��
	SM0=0;							//������ʽ0
	SM1=1;
 	ES=1;
}

 /*������*/
void main()
{	

	sbuf_init();					//���ڳ�ʼ��
	Time_init();					//��ʱ�жϳ�ʼ��
	while(1)
	{	
		 if(flag == 1)					//���ֶ�ʱ�жϣ�
		 {
		 	flag = 0;
			switch(dat)    				//�жϴ��ڽ��յ�����
				{
					case 49:P0=table[1];
							P2_3 = 0;break; //���ĸ��������ʾΪ����1
					case 50:P0=table[2];
							P2_3 = 0;break; //���ĸ��������ʾΪ����2
					case 51:P0=table[3];
							P2_3 = 0;break; //���ĸ��������ʾΪ����3
					case 52:P0=table[4];
							P2_3 = 0;break; //���ĸ��������ʾΪ����4
					default:P0=table[8];
							P2_3 = 0;break;  		//���ĸ��������ʾΪ����8
				}
				 ES =1;
		 }
					
	 }
}


/*��ʱ��0�жϷ������*/
void t0_time(void) interrupt 1		
{
	TH0 = 0x4c;  					// ���¼��ض�ʱ����ʼֵ  
    TL0 = 0x00;  					// ���¼��ض�ʱ����8λ��ʼֵ  
    count0++;	 					//���㶨ʱ���������
	
	if(count0==40)			
	{	ES =1;
		flag = 1;
		count0=0;
//		P0=table[2];	 			//������ܵĵײ���
//   		P2_1 = 0;		
//   		delay_ms(500);
//   		P2_1 = 1;		
	} 
}

/*�����жϷ������*/
void ser(void) interrupt 4		 
{
	RI=0;							//�����жϱ�־�������н��յ�һ���ֽڵ����ݽ���ʱ��RI�ᱻӲ���Զ���λ
	dat=SBUF;						//��������SBUF����������������ݽ��ա�
	//while(!TI)
	// 	TI = 0;							//��������ʽ�У���Ӧ�жϺ�TI������������㡣
	//flag = 1;	
	SBUF=dat;			   
	ES=0;
}

