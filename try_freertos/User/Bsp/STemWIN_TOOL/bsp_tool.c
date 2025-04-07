#include "GUI.h"
#include "DIALOG.h"

#include "bsp_tool.h"
#include "./Bsp/FATFS_TASK/FatFs_test.h"
#include "./Bsp/usart1/bsp_usart.h"
#include "Win_LoginDLG.h"
#include <stddef.h>
#include <string.h>
#include "stdio.h"
#include "math.h"
/* FreeRTOSͷ�ļ� */
#include "FreeRTOS.h"
#include "task.h"

#define EPS 1e-7

extern WM_HWIN Show_prev,Show_next;
extern int flag_login,flag_keyboard;

extern struct Monitor_Value 		 MONV;
extern struct Disposition_Value  DISPV;
extern struct Connect_Value 		 CONTV;

/*ICONVIEW ͼƬʵ��*/
char *_acbuffer = NULL;
GUI_BITMAP ac0;
GUI_BITMAP* ac;
GUI_LOGPALETTE palette;
UINT    Icon_f_num;
FATFS   Icon_fs;								/* FatFs�ļ�ϵͳ���� */
FIL     Icon_file;							/* file objects */
FRESULT Icon_result; 
DIR     Icon_dir;

	GUI_BITMAP Source;
GUI_BITMAP ac_buff[BITMAP_NUM];
int bmp_i=0;
GUI_BITMAP Logo[3];

WM_HMEM hMem;
GUI_MEMDEV_Handle hBMP;

/**
  * @brief ʹ���ڴ��豸����BMPͼƬ����
  * @note wu
  * @param sFilename����Ҫ���ص�ͼƬ��
  * @retval ��
  */
	
void LoadBMP_UsingMEMDEV(char *sFilename, GUI_BITMAP* ic)
{
	
  /* �����ٽ�� */
	taskENTER_CRITICAL();
	

//	if (flag_login == 2 || flag_keyboard == 2) {//Show_prev != Show_next
//    for (int i = 0; i < BITMAP_NUM; i++) {
//        if (*ac_buff[i].pData) {
//					GUI_ALLOC_Free(*ac_buff[i].pData);
//            memset(&ac_buff[i], 0, sizeof(GUI_BITMAP));
//        }
//    }
//    memset(ac_buff, 0, sizeof(ac_buff));
//   bmp_i=0;
//}
	
//  memset(&ic, 0, sizeof(GUI_BITMAP));
	
		//���ⲿSPI Flash�����ļ�ϵͳ���ļ�ϵͳ����ʱ���SPI�豸��ʼ��
	Icon_result = f_mount(&Icon_fs,"0:",1);
	
	/* ��ͼƬ */
	Icon_result = f_open(&Icon_file, sFilename, FA_READ);// FA_OPEN_EXISTING | FA_READ | FA_OPEN_ALWAYS
	if ((Icon_result != FR_OK))
	{
		printf("�ļ���ʧ�ܣ�\r\n");
		_acbuffer[0]='\0';
	}
//	printf("ICON�ļ��򿪣�\r\n");
	/* ����һ�鶯̬�ڴ�ռ� */
	hMem = GUI_ALLOC_AllocZero(Icon_file.fsize);
	/* ת����̬�ڴ�ľ��Ϊָ�� */
	
	_acbuffer = GUI_ALLOC_h2p(hMem);

	/* ��ȡͼƬ���ݵ���̬�ڴ��� */
	Icon_result = f_read(&Icon_file, _acbuffer, Icon_file.fsize, &Icon_f_num);
	if(Icon_result != FR_OK)
	{
		printf("�ļ���ȡʧ�ܣ�\r\n");
//		printf("%c",*sFilename);
	}
	/* ��ȡ��Ϲر��ļ� */
	f_close(&Icon_file);
	

	
		/* ����ʹ���ļ�ϵͳ��ȡ�������ļ�ϵͳ */
	f_mount(NULL,"0:",1);
	
	
	
	/* �����ڴ��豸 */
//	hBMP = GUI_MEMDEV_CreateEx(0, 0,                        /* ��ʼ���� */
//													 GUI_BMP_GetXSize(_acbuffer), /* x����ߴ� */
//													 GUI_BMP_GetYSize(_acbuffer),/* y����ߴ� */
//	                         GUI_MEMDEV_HASTRANS);
	/* ѡ���ڴ��豸 */
//	GUI_MEMDEV_Select(hBMP);
	/* ����BMP���ڴ��豸�� */
//	GUI_BMP_Draw(_acbuffer, 0, 0);
	
	

	GUI_CreateBitmapFromStream(ac,&palette,_acbuffer);
	ic=ac;
	
	memset(&ac, 0, sizeof(GUI_BITMAP));  // ��ֹ����
	memset(_acbuffer, 0, sizeof(*_acbuffer));  // ��ֹ����
//	ICONVIEW_AddBitmapItem(hItem,&ac_buff[bmp_i],"");
	
//	GUI_Delay(50);
	bmp_i++;
	
	/* �л��ڴ��豸��LCD */
//	GUI_MEMDEV_Select(0);
//	GUI_MEMDEV_CopyToLCD(hBMP);
	/* �ͷ��ڴ� */
//		free(_acbuffer);
	GUI_ALLOC_Free(hMem);

/* �˳��ٽ�� */
	taskEXIT_CRITICAL();

}

char* Text_Edit_Set(char* str1,char* str2)
{
	strcat(str1,str2);
	return str1;
	
}

/*2D���߻���*/


/* ���������� */
    GUI_POINT TrianglePointy[] = {
        {  8,  0 },
        {  0, 15 },
        { 16,  15 },
    };
		
		 /* ���������� */
    GUI_POINT TrianglePointx[] = {
        {  0,  0 },
        {  0, 16 },
        { 15,  8 },
    };

// ����������ת��Ϊ��Ļ����
 void DataToScreen(int x, int y, int* sx, int* sy) {
    // X��ת��
    *sx = MARGIN_LEFT + (x - X_MIN) * (CHART_WIDTH - MARGIN_LEFT - MARGIN_RIGHT) / (X_MAX - X_MIN);
    
    // Y��ת����ע��emWin����ϵY�������ӣ�
    *sy = ( CHART_HEIGHT - MARGIN_BOTTOM) 
          - (y - Y_MIN) * (CHART_HEIGHT - MARGIN_TOP - MARGIN_BOTTOM) / (Y_MAX - Y_MIN);
	 /**sx = MARGIN_LEFT + (x - X_MIN) * (CHART_WIDTH - MARGIN_LEFT - MARGIN_RIGHT) / (X_MAX - X_MIN);
    
    // Y��ת����ע��emWin����ϵY�������ӣ�
    *sy = (MARGIN_TOP + CHART_HEIGHT - MARGIN_BOTTOM) 
          - (y - Y_MIN) * (CHART_HEIGHT - MARGIN_TOP - MARGIN_BOTTOM) / (Y_MAX - Y_MIN);*/
}
 
 void DataToScreenf(float x, float y, float* sx, float* sy) {
    // X��ת��
    *sx = MARGIN_LEFT + (x - X_MIN) * (CHART_WIDTH - MARGIN_LEFT - MARGIN_RIGHT) / (X_MAX - X_MIN);
    
    // Y��ת����ע��emWin����ϵY�������ӣ�
    *sy = ( CHART_HEIGHT - MARGIN_BOTTOM) 
          - (y - Y_MIN) * (CHART_HEIGHT - MARGIN_TOP - MARGIN_BOTTOM) / (Y_MAX - Y_MIN);
	 /**sx = MARGIN_LEFT + (x - X_MIN) * (CHART_WIDTH - MARGIN_LEFT - MARGIN_RIGHT) / (X_MAX - X_MIN);
    
    // Y��ת����ע��emWin����ϵY�������ӣ�
    *sy = (MARGIN_TOP + CHART_HEIGHT - MARGIN_BOTTOM) 
          - (y - Y_MIN) * (CHART_HEIGHT - MARGIN_TOP - MARGIN_BOTTOM) / (Y_MAX - Y_MIN);*/
}

 void DataToScreenl(float x, float y, float* sx, float* sy) {
    // X��ת��
    *sx = MARGIN_LEFT + (x - X_MIN) * (CHART_WIDTH - MARGIN_LEFT - MARGIN_RIGHT) / (X_MAX - X_MIN);
    
    // Y��ת����ע��emWin����ϵY�������ӣ�
    *sy = ( CHART_HEIGHT - MARGIN_BOTTOM) 
          - (y - Y_MIN) * (CHART_HEIGHT - MARGIN_TOP - MARGIN_BOTTOM) / (Y_MAX - Y_MIN);
	 /**sx = MARGIN_LEFT + (x - X_MIN) * (CHART_WIDTH - MARGIN_LEFT - MARGIN_RIGHT) / (X_MAX - X_MIN);
    
    // Y��ת����ע��emWin����ϵY�������ӣ�
    *sy = (MARGIN_TOP + CHART_HEIGHT - MARGIN_BOTTOM) 
          - (y - Y_MIN) * (CHART_HEIGHT - MARGIN_TOP - MARGIN_BOTTOM) / (Y_MAX - Y_MIN);*/
}

 void DrawAxes(void) {
	 
	 
//	  ArrowConfig cfg = {
//        .length = 12,             // ��ͷ����12����
//        .angle  = 3.14/6,         // 30�ȼн�
//        .fill   = 1               // ����ͷ
//		};
    // ����X��
    GUI_DrawLine(MARGIN_LEFT, CHART_HEIGHT - MARGIN_BOTTOM,
                CHART_WIDTH - MARGIN_RIGHT, CHART_HEIGHT - MARGIN_BOTTOM);
	 
		GUI_FillPolygon(TrianglePointx, 3, CHART_WIDTH - MARGIN_RIGHT*2+15, CHART_HEIGHT - MARGIN_BOTTOM*2+11);
		
//	 GUI_DrawArrow(MARGIN_LEFT, CHART_HEIGHT - MARGIN_BOTTOM,
//                CHART_WIDTH - MARGIN_RIGHT, CHART_HEIGHT - MARGIN_BOTTOM, cfg);
    // ����Y��
    GUI_DrawLine(MARGIN_LEFT,MARGIN_TOP+40,
                MARGIN_LEFT, CHART_HEIGHT - MARGIN_BOTTOM);
	  GUI_FillPolygon(TrianglePointy, 3,MARGIN_LEFT-8,MARGIN_TOP+25);

    // ������ǩ
    GUI_SetFont(&GUI_Font16B_ASCII);
    // X������
    GUI_DispStringAt("Deg",270,200 );
    // Y�����ƣ���ֱ��ʾ��Ҫ���⴦��
    GUI_DispStringAt("N.m", 43, 50);
}

 void DrawTicks(int XBlock,int YBlock) {
    char buf[10];
    int sx, sy;
	  float sxf,syf;
    int num=0;
    // X��̶�
    for (int x = X_MIN; x <= X_MAX; x+=X_MAX/XBlock) {
        DataToScreen(x, Y_MIN, &sx, &sy);
        // ���ƿ̶���
        GUI_DrawLine(sx, sy, sx, sy );
        // ��ʾ�̶�ֵ
        sprintf(buf, "%d", x);
        GUI_DispStringAt(buf, sx-2 , sy +3);//sx-10
			num++;
			if(num==XBlock){break;}
    }
		num=0;
    // Y��̶�
    for (float y = Y_MIN; y <= Y_MAX; y += Y_MAX/YBlock) {//y += 20
        DataToScreenf(X_MIN, y, &sxf, &syf);
        // ���ƿ̶���
        GUI_DrawLine(sxf , syf, sxf, syf);
        // ��ʾ�̶�ֵ
        sprintf(buf, "%.2f", y);
			if(y==0)
				{
        GUI_DispStringAt(buf, sxf - 29, syf -5);
			  }else{
				GUI_DispStringAt(buf, sxf - 29, syf);
				}
			num++;
			if(num==YBlock){break;}
    }
}
 

 void DrawCurve(DataPoint points[],int num_points) {
    float prev_sx, prev_sy;
	 
    
	 
    for (int i = 0; i < num_points; i++) {
        float sxf, syf;
        DataToScreenf(points[i].x, points[i].y, &sxf, &syf);
        
        if (i > 0) {
					WM_SelectWindow(MONV.Monitor);
						GUI_SetPenSize(2);
		    GUI_SetColor(GUI_BLACK);
				GUI_SetLineStyle(GUI_LS_SOLID);
            // ����ǰһ�㵽��ǰ��
            GUI_AA_DrawLine(prev_sx, prev_sy, sxf, syf);
					
        }
        prev_sx = sxf;
        prev_sy = syf;
        
        // �������ݵ㣨��ѡ��
        GUI_DrawCircle(sxf, syf, 2);
    }
}

DataPoint *head=NULL;
DataPoint *DP_create(float x,float y)
{
	DataPoint *node=(DataPoint *)malloc(sizeof(DataPoint));
	node->x=x;
	node->y=y;
	node->next=NULL;
//	node->prev=NULL;
	return node;
}


void AddPoint(DataPoint **DP,float x,float y)
{
	DataPoint *NewNode=DP_create(x,y);
	if(*DP == NULL)
	{
		*DP=NewNode;
		return;
	}
	DataPoint *temp=*DP;
	while(temp->next!=NULL)
	{
	  temp = temp->next;
	}
	NewNode->next=temp->next;
	temp->next=NewNode;
	
////	temp->next=NewNode;
////	NewNode->next=NULL;
//	NewNode->prev=temp;
}

void Destory_Point(DataPoint **DP)
{
	if(*DP==NULL)
	{return;}
	DataPoint *temp=*DP;
	DataPoint *del=NULL;
	while(temp!=NULL)
	{
		del=temp->next;
		free(temp);
		temp=del;
	}
	*DP=NULL;
}

void TEXT_Point(int X,float Y)
{
	AddPoint(&head,X,Y);
//	AddPoint(&head,200,0.2);
//	AddPoint(&head,500,1.2);
//	AddPoint(&head,800,1.4);
//	AddPoint(&head,1200,1.8);
//	AddPoint(&head,1500,2.3);
//	AddPoint(&head,2000,2.4);
//	AddPoint(&head,2200,2.8);
//	AddPoint(&head,2700,3.15);
	
}

void DrawPoints(DataPoint *value )
{
	float prev_sx, prev_sy;
	int num=0;
	while(value)
		{
			float sxf, syf;
        DataToScreenf(value->x, value->y, &sxf, &syf);
        
       if(num>0 && value->x)
			 {
					WM_SelectWindow(MONV.Monitor);
						GUI_SetPenSize(2);
		    GUI_SetColor(GUI_BLACK);
				GUI_SetLineStyle(GUI_LS_SOLID);
            // ����ǰһ�㵽��ǰ��
            GUI_DrawLine(prev_sx, prev_sy, sxf, syf);
			 }
        num++;
        prev_sx = sxf;
        prev_sy = syf;
        value=value->next;
        // �������ݵ㣨��ѡ��
			 
        GUI_DrawCircle(sxf, syf, 2);
			 
   	}
}

/*XY��̶��߻���*/
void XYBlock_Init(void)
{
	MONV.X_Block=4;
	MONV.Y_Block=5.0;
	
	DISPV.X_Block=3;
	DISPV.Y_Block=5.0;
}

/*��������λ��*/
void Move_Figure(WM_HWIN hItem,WM_HWIN Icon,int flag,float value)
{
	float sxf, syf;
	//Deg
	if(flag==0)
	{
		GUI_RECT RectD;
    DataToScreenf(value, 0, &sxf, &syf);
		WM_GetWindowRectEx(hItem,&RectD);
//		int Y=WM_GetWindowSizeY(hItem);
		
				if(hItem==DISPV.DegTarget)
		{
			WM_MoveChildTo(hItem,sxf+5,RectD.y0);
		WM_MoveChildTo(Icon,sxf-3,RectD.y0);
		}
		else{
		WM_MoveChildTo(hItem,sxf-3,RectD.y0);
		WM_MoveChildTo(Icon,sxf-10,RectD.y0);	
		}
	}
	//Nm
	else if(flag==1)
	{
		GUI_RECT RectN;
		DataToScreenf(0, value, &sxf, &syf);
		WM_GetWindowRectEx(hItem,&RectN);
//		int X=WM_GetWindowSizeX(hItem);
//		WM_MoveChildTo(hItem,X,syf);
		WM_MoveChildTo(hItem,RectN.x0,syf-5);
		WM_MoveChildTo(Icon,RectN.x0-8,syf-5);
		}
	
	
}

void Move_Line(WM_HWIN hItem,int flag,float value)
{
	float sxf, syf;
	//Deg
	if(flag==0)
	{
		GUI_RECT RectD;
    DataToScreenf(value, 0, &sxf, &syf);
		WM_GetWindowRectEx(hItem,&RectD);
//		int Y=WM_GetWindowSizeY(hItem);
		GUI_SetPenSize(1);
		GUI_SetLineStyle(GUI_LS_DASHDOT);
		GUI_SetColor(0x00FF0000);
		GUI_DrawLine(sxf, CHART_HEIGHT - MARGIN_BOTTOM, sxf, MARGIN_TOP+65);		
	}
	//Nm
	else if(flag==1)
	{
		GUI_RECT RectN;
		DataToScreenf(0, value, &sxf, &syf);
		WM_GetWindowRectEx(hItem,&RectN);
//		int X=WM_GetWindowSizeX(hItem);
//		WM_MoveChildTo(hItem,X,syf);
		GUI_SetPenSize(1);
		GUI_SetLineStyle(GUI_LS_DASHDOT);
		GUI_SetColor(GUI_MAKE_COLOR(0x00FF0080));
		GUI_DrawLine(CHART_WIDTH - MARGIN_RIGHT-50, syf, MARGIN_LEFT, syf);
	}
}

/*��ͷ*/



// �����ͷ��������
 void CalculateArrowPoints(
    int x0, int y0,   // �˵�����
    int x1, int y1,   // ���ڵ�����
    ArrowConfig cfg,  // ��ͷ����
    int points[3][2]  // �����������
) {
    float dx = x1 - x0;
    float dy = y1 - y0;
    float len = sqrt(dx*dx + dy*dy);
    
    // ����������һ��
    float ux = dx / len;
    float uy = dy / len;

    // �����ͷ��������
    float angleSin = sin(cfg.angle);
    float angleCos = cos(cfg.angle);
    
    // ��ͷ����
    points[0][0] = x0;
    points[0][1] = y0;
    
    points[1][0] = x0 + cfg.length*(angleCos*ux - angleSin*uy);
    points[1][1] = y0 + cfg.length*(angleSin*ux + angleCos*uy);
    
    // ��ͷ�Ҳ��
    points[2][0] = x0 + cfg.length*(angleCos*ux + angleSin*uy);
    points[2][1] = y0 + cfg.length*(-angleSin*ux + angleCos*uy);
}

// ���Ƶ�����ͷ
void GUI_DrawArrow(int x0, int y0, int x1, int y1, ArrowConfig cfg) {
    int arrowPoints[3][2];
    CalculateArrowPoints(x0, y0, x1, y1, cfg, arrowPoints);
    
//    if(cfg.fill) {
//        GUI_FillPolygon(arrowPoints[0], 3, 0, 0);
//    } else {
//        GUI_DrawPolygon(arrowPoints[0], 3, 0, 0);
//    }
}

// ��˫��ͷ���߶λ��ƺ���
void GUI_DrawLineWithArrows(int x0, int y0, int x1, int y1, ArrowConfig cfg) {
    // ����ԭʼ�߶�
    GUI_DrawLine(x0, y0, x1, y1);
    
    // ��������ͷ����Ҫ������㣩
    if(x0 != x1 || y0 != y1) {
        GUI_DrawArrow(x0, y0, x0 + (x0-x1), y0 + (y0-y1), cfg);
    }
    
    // �����յ��ͷ
    GUI_DrawArrow(x1, y1, x0, y0, cfg);
}

/*��������*/
_List* Head_Task=NULL;
_List* List_Create(struct Disposition_Value val)
{
	_List* head = (_List*)malloc(sizeof(_List));
	strcpy(head->Nm_Max,val.Nm_Max);
	strcpy(head->Nm_Min,val.Nm_Min);
	strcpy(head->Nm_Target,val.Nm_Target);
	strcpy(head->Deg_Max,val.Deg_Max);
	strcpy(head->Deg_Min,val.Deg_Min);
	strcpy(head->Deg_Target,val.Deg_Target);
	head->Task_Num=val.Task_Sel;
//	head->Task_Sort=(0x01<<(head->Task_Num));
//	head->Task_Sort=head->Task_Num;
	
	head->prev=NULL;
	head->next=NULL;
	return head;
	
}

void Insert_Task(_List** TL,struct Disposition_Value val)
{
	_List *NewNode=List_Create(val);
	if(NewNode==NULL)
	{
		printf("�ڴ����ʧ�ܣ�\n");
	}
	if(*TL == NULL)
	{
		*TL=NewNode;
		return;
	}
	_List *temp=*TL;
	while(temp->next!=NULL)
	{
	  temp = temp->next;
	}
	NewNode->next=temp->next;
	temp->next=NewNode;
	NewNode->prev=temp;
}
void Inset2(_List** TL,struct Disposition_Value val)
{
	_List *NewNode=List_Create(val);
	_List *temp=*TL;
	while(temp->next!=NULL)
	{
		temp=temp->next;
	}
	NewNode->prev=temp;
	temp->next=NULL;
}

void Add_Task(struct Disposition_Value val)
{
	int count = 0;
    _List* temp = Head_Task;
    while (temp != NULL) {
        count++;
        temp = temp->next;
    }
    
    // �������������������20��
    
		printf("��������:%d\n",count);
        
    
//	Inset2(&Head_Task,val);
	Insert_Task(&Head_Task,val);
//  DISPV.Task_Sel = count + 1;  // �����������
	ShowList(&Head_Task);
}

void Destory_Task(_List **TL,int pos)
{
	_List *temp=*TL;
	_List *del=NULL;
	while(temp->next!=NULL)
	{
		
		del=temp->next;
		free(temp);
		temp=del;
		
	}
	temp->next=*TL;
}

_List*  Find_Del_Task(_List* TL,int pos)
{
	_List *temp=TL;
	while(temp)
	{
		if(temp->Task_Num==pos)
		{
			if(temp->prev==NULL)
			{
				temp=temp->next;
				if(TL)
				{
					temp->prev=NULL;
					temp->next=NULL;
				}
				free(temp);
				return temp;
			}
			if(temp->prev && temp->next)
			{
				temp->next->prev=temp->prev;
				temp->prev->next=temp->next;
				free(temp);
				return temp;
			}
			if(temp->next==NULL)
			{
				temp->prev->next=NULL;
				temp->prev=NULL;
				free(temp);
				return temp;
			}
		}
		temp=temp->next;
	}
	return NULL;
}

_List* Find_Task(_List* TL,int pos)
{
	_List *temp=TL;
	while(temp!=NULL)
	{
		if(temp->Task_Num==pos)
//			if(temp->Task_Sort & (0x01<<pos))
		{
//			if(temp->prev==NULL)
//			{
//				temp=temp->next;
//				if(TL)
//				{
//					temp->prev=NULL;
//					temp->next=NULL;
//				}
//				free(temp);
//				return temp;
//			}
//			if(temp->prev && temp->next)
//			{
//				temp->next->prev=temp->prev;
//				temp->prev->next=temp->next;
////				free(temp);
			printf("find\n");
			printf("Deg_Min:%f Deg_Max:%f Deg_Target:%f Nm_Min:%f Nm_Ma:%f Nm_Target:%f \n ",
		atof(temp->Deg_Min),atof(temp->Deg_Max),atof(temp->Deg_Target),atof(temp->Nm_Min),atof(temp->Nm_Max),atof(temp->Nm_Target));
				return temp;
//			}
//			if(temp->next==NULL)
//			{
//				temp->prev->next=NULL;
//				temp->prev=NULL;
////				free(temp);
//				return temp;
//			}
		}
		temp=temp->next;
	}
	return NULL;
}

void Test_del(int pos)
{
	_List* node=Find_Del_Task(Head_Task,pos);
//	Del_Task(&Head_Task,node);
	ShowList(&Head_Task);
}

_List* Task_Seek(int pos)
{
	_List* node=Find_Task(Head_Task,pos);
	return node;
}

void Task_Update(struct Disposition_Value *val,int pos,int flag)
{
	
	_List* node=Task_Seek(pos);
	if(flag == 1 && node !=NULL)
	{
//	_List* node=Task_Prev(pos);
	strcpy(val->Nm_Max,node->Nm_Max);
	strcpy(val->Nm_Min,node->Nm_Min);
	strcpy(val->Nm_Target,node->Nm_Target);
	strcpy(val->Deg_Max,node->Deg_Max);
	strcpy(val->Deg_Min,node->Deg_Min);
	strcpy(val->Deg_Target,node->Deg_Target);
	val->Task_Sel=node->Task_Num;
	}
	
	if(flag == 0 && node !=NULL)
	{
//	_List* node=Task_Prev(pos);
	strcpy(val->Nm_Max,node->Nm_Max);
	strcpy(val->Nm_Min,node->Nm_Min);
	strcpy(val->Nm_Target,node->Nm_Target);
	strcpy(val->Deg_Max,node->Deg_Max);
	strcpy(val->Deg_Min,node->Deg_Min);
	strcpy(val->Deg_Target,node->Deg_Target);
	val->Task_Sel=node->Task_Num;
	}
}

void Free_List(_List* TL)
{
	_List* temp=TL;
	
//	_List* node=Find_Task(Head_Task,);
//	while(temp)
//	{
//		TL=TL->next;
//		free(temp);
//		temp=TL;
//	}
	
}

void Task_Free(void)
{
	Free_List(Head_Task);
	printf("Free \n");
	printf("Deg_Min:%f Deg_Max:%f Deg_Target:%f Nm_Min:%f Nm_Ma:%f Nm_Target:%f \n ",
		atof(Head_Task->Deg_Min),atof(Head_Task->Deg_Max),atof(Head_Task->Deg_Target),atof(Head_Task->Nm_Min),atof(Head_Task->Nm_Max),atof(Head_Task->Nm_Target));
}

void ShowList(_List** TL)
{
	_List* temp=*TL;
	while(temp!=NULL)
	{
		printf("Deg_Min:%f Deg_Max:%f Deg_Target:%f Nm_Min:%f Nm_Ma:%f Nm_Target:%f \n ",
		atof(temp->Deg_Min),atof(temp->Deg_Max),atof(temp->Deg_Target),atof(temp->Nm_Min),atof(temp->Nm_Max),atof(temp->Nm_Target));
		temp=temp->next;
	}
}