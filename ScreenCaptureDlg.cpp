
// ScreenCaptureDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ScreenCapture.h"
#include "ScreenCaptureDlg.h"
#include "afxdialogex.h"
#include <gdiplus.h>
#include <vector>

//------------标志状态的变量
int have_magnifier=0;
bool is_redraw=true;
int draw_mode=-1;
bool lbd=false;
CPoint op;//存储了鼠标点击的位置
CPoint np;//用于线程存储鼠标的位置

bool set_re=false;
int c_p=0;
int m_cx=0,m_cy=0;

int rectTop;
int rectBottom;
int rectLeft;
int rectRight;



vector <CRect> data_rect;
CRect main_rect=NULL;//自动选框





#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CDC mdc;//选框外
CDC omdc;//选框内
CDC t_mdc;//放大镜部分
CBitmap mdc_bitmap;
CBitmap omdc_bitmap;
CBitmap t_mdc_bitmap;


//--------------------------------------判断鼠标点击的位置------------------------------------
int judge_click_p( CPoint pt,CRect re,int range)
{   
	if (abs(pt.x-re.left)<range && abs(pt.y-re.top)<range)
	     return 1;//鼠标点中左上角

	else if (abs(pt.x-re.right)<range && abs(pt.y-re.top)<range)
		 return 3;//鼠标点中右上角

	else if (abs(pt.x-re.right)<range && abs(pt.y-re.bottom)<range)
		 return 5;//鼠标点中右下角

	else if (abs(pt.x-re.left)<range && abs(pt.y-re.bottom)<range)
		 return 7;//鼠标点中左下角
	
	else if ( abs(pt.x-re.left)<range )
         return 8; //鼠标点中左边

	else if(abs(pt.x-re.right)<range)
		 return 4;//鼠标点中右边
		
	else if(abs(pt.y-re.top)<range)
		 return 2;//鼠标点中上边

	else if(abs(pt.y-re.bottom)<range)
		 return 6;//鼠标点中下边
	else if (re.PtInRect(pt))
	     return 0;//鼠标在框内

	else return -1;//鼠标在框外
	
}


//--------------------------------设置选框----------------------------------
bool set_rect_d(int c_p,CRect *rect)
{
	CPoint pt;
	GetCursorPos(&pt);

	//int rectTop=rect->top;
	//int rectBottom=rect->bottom;


	if (c_p==-1)
		return false;

	else if(c_p==1)
	{
		if(pt.y>=rectBottom||pt.x>=rectRight)
		{
			if(pt.y>=rectBottom)
			{
				rect->top=rectBottom;
				rect->left=pt.x;
				rect->bottom=pt.y;
			}
			if(pt.x>=rectRight)
			{
				rect->left=rectRight;
				rect->right=pt.x;
				rect->top=pt.y;
			}
			if(pt.y>=rectBottom&&pt.x>=rectRight)
			{
				rect->left=rectRight;
				rect->top=rectBottom;
				rect->right=pt.x;
				rect->bottom=pt.y;
			}
			if(pt.x<=rectRight)
			{
				rect->right=rectRight;
				rect->left =pt.x;
			}
			if(pt.y<=rectBottom)
			{
				rect->bottom=rectBottom;
				rect->top=pt.y;
			}
		}
		else
		{
			rect->bottom=rectBottom;
			rect->right=rectRight;
			rect->left=pt.x;
			rect->top=pt.y;
		}
		return true;
	}

	else if (c_p==2)
	{
		if(pt.y>=rectBottom)
		{
			rect->top=rectBottom;
			rect->bottom=pt.y;
		}
		else
		{
			rect->bottom=rectBottom;
			rect->top=pt.y;
		}
		return true;
	}

	else if (c_p==3)
	{
		if(pt.x<=rectLeft||pt.y>=rectBottom)
		{
			if(pt.x<=rectLeft)
			{
				rect->right=rectLeft;
				rect->left=pt.x;
				rect->top=pt.y;
			}
			if(pt.y>=rectBottom)
			{
				rect->top=rectBottom;
				rect->bottom=pt.y;
				rect->right=pt.x;
			}
			if(pt.x<=rectLeft&&pt.y>=rectBottom)
			{
				rect->right=rectLeft;
				rect->top=rectBottom;
				rect->left=pt.x;
				rect->bottom=pt.y;
			}
			if(pt.x>=rectLeft)
			{
				rect->left=rectLeft;
				rect->right=pt.x;
			}
			if(pt.y<=rectBottom)
			{
				rect->bottom=rectBottom;
				rect->top=pt.y;
			}
		}
		else
		{
			rect->left =rectLeft;
			rect->bottom=rectBottom;
			rect->right=pt.x;
			rect->top=pt.y;
		}
		return true;
	}

	else if (c_p==4)
	{
		if(pt.x<=rectLeft)
		{
			rect->right=rectLeft;
			rect->left=pt.x;
		}
		else
		{
			rect->left=rectLeft;
			rect->right=pt.x;
		}
		return true;
	}

	else if (c_p==5)
	{
		if(pt.x<=rectLeft||pt.y<=rectTop)
		{
			if(pt.x<=rectLeft)
			{
				rect->right=rectLeft;
				rect->left=pt.x;
				rect->bottom=pt.y;
			}
			if(pt.y<=rectTop)
			{
				rect->bottom=rectTop;
				rect->top=pt.y;
				rect->right=pt.x;
			}
			if( pt.x<=rectLeft&&pt.y<=rectTop )
			{
				rect->bottom=rectTop;
				rect->right=rectLeft;
				rect->top=pt.y;
				rect->left=pt.x;
			}
			if(pt.x>=rectLeft)
			{
				rect->left=rectLeft;
				rect->right=pt.x;
			}
			if(pt.y>=rectTop)
			{
				rect->top=rectTop;
				rect->bottom=pt.y;
			}
		}
		else
		{
			rect->top=rectTop;
			rect->left=rectLeft;
			rect->right=pt.x;
			rect->bottom=pt.y;
		}
		return true;
	}

	else if (c_p==6)
	{
		if(pt.y<=rect->top)
		{
			rect->bottom=rectTop;
			rect->top=pt.y;
		}
		else
		{
			rect->top=rectTop;
			rect->bottom=pt.y;
		}
			return true;
	}

	else if (c_p==7)
	{
		if(pt.x>=rectRight||pt.y<=rectTop)
		{
			if(pt.x>=rectRight)
			{
				rect->left=rectRight;
				rect->right=pt.x;
				rect->bottom=pt.y;
			}
			if(pt.y<=rectTop)
			{
				rect->bottom=rectTop;
				rect->left=pt.x;
				rect->top=pt.y;
			}
			if(pt.x>=rectRight&&pt.y<=rectTop)
			{
				rect->left=rectRight;
				rect->bottom=rectTop;
				rect->right=pt.x;
				rect->top=pt.y;
			}
			if(pt.x<=rectRight)
			{
				rect->right=rectRight;
				rect->left=pt.x;
			}
			if(pt.y>=rectTop)
			{
				rect->top=rectTop;
				rect->bottom=pt.y;
			}
		}
		else
		{
			rect->top=rectTop;
			rect->right=rectRight;
			rect->left=pt.x;
			rect->bottom=pt.y;
		}
		return true;
	}

	else if (c_p==8)
	{
		if(pt.x>=rectRight)
		{
			rect->left=rectRight;
			rect->right=pt.x;
		}
		else
		{
			rect->right=rectRight;
			rect->left=pt.x;
		}
		return true;
	}

	else if (c_p==0)
	{
		int width=rect->Width(),height=rect->Height();
		rect->left=pt.x-m_cx;
		rect->top=pt.y-m_cy;
		rect->right=rect->left+width;
		rect->bottom=rect->top+height;
		return true;
	}

	else if (c_p==-2)
	{  
		if (is_redraw==false)
		{
			draw_mode=-3;
			return true;
		}

		//当还没有自动选框时，main_rect为空，这是模式为-2，且向main_rect赋予初值，op为鼠标点击位置，np为鼠标移动到的位置
		main_rect.left=op.x;	
		main_rect.top=op.y;
		main_rect.right=np.x;
		main_rect.bottom=np.y;

		return true;
	}
}

void ScreenCapture()
{
	//------------------------mdc初始化---------------------------------------------
	CDC *dc=CDC::FromHandle(::GetDC(NULL));//获取当前整个屏幕DC
	mdc.CreateCompatibleDC(dc);   //创建一个与指定设备兼容的内存设备上下文环境（DC）,即创建选框外内存DC
	omdc.CreateCompatibleDC(dc);//选框内内存DC
	t_mdc.CreateCompatibleDC(dc);//坐标内存DC
	
	//创建与指定的设备环境相关的设备兼容的位图
	mdc_bitmap.CreateCompatibleBitmap(dc,GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN));
	omdc_bitmap.CreateCompatibleBitmap(dc,GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN));
	t_mdc_bitmap.CreateCompatibleBitmap(dc,GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN));


	//选择一对象到指定的设备上下文环境中，该新对象替换先前的相同类型的对象。
	mdc.SelectObject(&mdc_bitmap);//内存DC（mdc）的图像输出画到位图（mdc_bitmap）上
	omdc.SelectObject(&omdc_bitmap);//选框内
	t_mdc.SelectObject(t_mdc_bitmap);

	t_mdc.SetTextColor(RGB(255,255,255)); //指定设备环境(HDC)的字体颜色，坐标的字体颜色
	t_mdc.SetBkMode(TRANSPARENT);//使用透明输出，使坐标背景不变

	mdc.BitBlt(0,0,GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN),dc,0,0,SRCCOPY);//从原设备（dc）中复制一个矩形到目标设备，选框外的DC
	omdc.BitBlt(0,0,GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN),dc,0,0,SRCCOPY);//选框DC

	//----------------使用到的画笔等--------------------------------------
	SolidBrush bk(Color(130,0,0,0));//单色画笔
	Graphics mdc_g(mdc.m_hDC);//创建一个与DC（mdc）有关联的Graphics对象

	mdc_g.FillRectangle(&bk,0,0,GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN));//用单色画笔填充整个屏幕，就是给选框外添加透明灰
	::ReleaseDC(NULL,dc->m_hDC);
	dc->DeleteDC();


	t_mdc.BitBlt(0,0,GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN),&mdc,0,0,SRCCOPY);
	
}


//---------------------用于检测窗口矩形是否存在----------------------------------
bool rect_is_exist(CRect re,vector <CRect> mre)
{
	if (re.left<=0 && re.top<=0 && re.Width()>=GetSystemMetrics(SM_CXSCREEN) && re.Height()>=GetSystemMetrics(SM_CYSCREEN))
	return true;

	for (int i=0;i<mre.size();i++)
	{
		if (re==mre[i])
		{
			return true;
		}
	}

	return false;
}

//-----------------------传递鼠标所在自选框的信息到re------------------------------------
bool find_pir(CRect *re,vector <CRect> mre,POINT tp)
{
	CRect t1=NULL;//暂存自动选框的信息
	int p=0;//如果鼠标在自动选框内，使P=1
	int t2=0;//如果鼠标在自动选框内，使t2=1
	vector <CRect> temp_re;
	for (int i=0;i<mre.size();i++)
	{
		if(mre[i].PtInRect(tp))   //判断参数中给出的点是否在矩形区域内
		{
			t2=1;
			if (p==0)
			{
				t1=mre[i];
				p=1;
			}
			else
			{
				if(mre[i].left>t1.left | mre[i].top>t1.top |mre[i].Width()<t1.Width() | mre[i].Height()<t1.Height())//选定更小的选框
				{
					t1=mre[i];
				}
			}

		}


	}

	if (t2=1)
	{
		*re=t1;
		return true;
	}
	else
		false;
}

UINT __cdecl ThreadProc(LPVOID lpParameter)
{
	CDC *dc=CDC::FromHandle(GetDC((HWND)lpParameter));//为窗口对象建立设备上下文



	//定义瞄准器使用的变量
	CRect magnifier_rect;   //放大镜矩形
	CString pixel_inform;	//像素信息
	int b_w=1;    //放大镜窗口与放大镜显示区域的边距宽
	int b_h=1; //放大镜窗口与放大镜显示区域的边距高
	int c_w=120;  //放大后的显示区域宽
	int c_h=100; //放大后的显示区域高
	int	magn_x=4;//放大倍数,2以下缩小模式,2为不放大,2以上为放大模式(取整数,不能为0)
	int fm_width=30; //距鼠标距离
	int fm_height=30;//距鼠标距离
	int magnifier_width=122; //放大镜窗口大小宽
	int magnifier_height=122;//放大镜窗口的高

	CRect temp_re;//创建对象时左上右下全默认为0

	POINT p;
	p.x=0;
	p.y=0;



	Graphics magn_g(dc->m_hDC);	//用于在窗口DC上绘图
	Graphics main_g(t_mdc.m_hDC);	//用于坐标位置上

	Pen bp(Color(255,255,0,0),2);//用于绘制直线和曲线的对象
	Pen bp2(Color(255,10,200,255),1);//浅蓝，用于选框边线
	SolidBrush b3(Color(150,10,200,255));//画刷类
	SolidBrush b(Color(200,0,0,0));
	SolidBrush b2(Color(255,10,200,255));
	SolidBrush w(Color(255,255,255,255));
	bp.SetAlignment(PenAlignmentInset);//画笔对齐方式为：线条之内
	dc->SetStretchBltMode(STRETCH_DELETESCANS);//设置指定设备环境dc中的位图拉伸模式为删除像素模式。
	COLORREF mycolor;







	while (1)
	{ 
		GetCursorPos(&np);//获取鼠标的位置，存到np


		if (lbd==true)//鼠标是否按下
		{
			if(draw_mode==-1)
			{
				if (main_rect.PtInRect(np))//判断鼠标np是否在矩形main_rect中
				{
					if (np==op)
						draw_mode=-1;

					else 
						draw_mode=-2;	
				}

				else 
					draw_mode=-2;

			}

			if (draw_mode==-2 ||(draw_mode<=8 && draw_mode>=0))
				set_rect_d(draw_mode,&main_rect);	
		}







		//----------------根据绘图模式,来刷新-------------------------------------------				
		//dc->BitBlt(temp_re.left-3,temp_re.top-3,temp_re.Width()+6,temp_re.Height()+6,&mdc,temp_re.left-3,temp_re.top-3,SRCCOPY);	
		t_mdc.BitBlt(temp_re.left-3,temp_re.top-3,temp_re.Width()+6,temp_re.Height()+6,&mdc,temp_re.left-3,temp_re.top-3,SRCCOPY);

		//-----------------------------放大镜-------------------------------------------
		if (draw_mode!=0)
		{

			if (np.x<magnifier_width+20 || np.y<magnifier_height+fm_height)
			{
				if (np.x>GetSystemMetrics(SM_CXSCREEN)-magnifier_width-fm_width)
				{
					magnifier_rect.top=np.y+fm_height;
					magnifier_rect.left=np.x-magnifier_width-fm_width;
					magnifier_rect.bottom=magnifier_rect.top+magnifier_height;
					magnifier_rect.right=magnifier_rect.left+magnifier_width;
				}
				else if (np.y>GetSystemMetrics(SM_CYSCREEN)-magnifier_height-fm_height)
				{
					magnifier_rect.top=np.y-magnifier_height-fm_height;
					magnifier_rect.left=np.x+fm_width;
					magnifier_rect.bottom=magnifier_rect.top+magnifier_height;
					magnifier_rect.right=magnifier_rect.left+magnifier_width;
				}
				else 
				{
					magnifier_rect.top=np.y+fm_height;
					magnifier_rect.left=np.x+fm_width;
					magnifier_rect.bottom=magnifier_rect.top+magnifier_height;
					magnifier_rect.right=magnifier_rect.left+magnifier_width;
				}

			}

			else
			{
				magnifier_rect.top=np.y-magnifier_height-fm_height;
				magnifier_rect.left=np.x-magnifier_width-fm_width;
				magnifier_rect.bottom=magnifier_rect.top+magnifier_height;
				magnifier_rect.right=magnifier_rect.left+magnifier_width;
			}
		}
		//------------------------------------------------------------------------------

		//--------------------------自动框选模式----------------------------------------

		if (draw_mode==-1)
		{
			if(find_pir(&main_rect,data_rect,np))//找到鼠标所在的窗口矩形，传递给main_rect
			{
				{ 
					temp_re.UnionRect(main_rect,magnifier_rect);
					t_mdc.BitBlt(main_rect.left,main_rect.top,main_rect.Width(),main_rect.Height(),&omdc,main_rect.left,main_rect.top,SRCCOPY);
					main_g.DrawRectangle(&bp,main_rect.left,main_rect.top,main_rect.Width(),main_rect.Height());//为选框画线

					main_g.FillRectangle(&b,magnifier_rect.left,magnifier_rect.top,magnifier_rect.Width(),magnifier_rect.Height());
					t_mdc.StretchBlt(magnifier_rect.left+b_w,magnifier_rect.top+b_h,c_w,c_h,&omdc,np.x-c_w/magn_x,np.y-c_h/magn_x,c_w/magn_x*2,c_h/magn_x*2,SRCCOPY);//从源矩形中复制一个位图到目标矩形，必要时按目标设备设置的模式进行图像的拉伸或压缩。
					mycolor=omdc.GetPixel(np);//检索指定坐标点的像素的RGB颜色值。
					pixel_inform.Format(_T("%d %d"),np.x,np.y);
					t_mdc.TextOut(magnifier_rect.left+5,magnifier_rect.top+105,pixel_inform);//在放大镜下面输出坐标的信息
					main_g.DrawLine(&bp,magnifier_rect.left+45,magnifier_rect.top+51,magnifier_rect.left+45+30,magnifier_rect.top+51);//瞄准器的实现
					main_g.DrawLine(&bp,magnifier_rect.left+61,magnifier_rect.top+38,magnifier_rect.left+61,magnifier_rect.top+63);//瞄准器的实现


					dc->BitBlt(0,0,GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN),&t_mdc,0,0,SRCCOPY);

				}
			}
		}

		//--------------------------鼠标拖拽模式----------------------------------------
		else if(draw_mode==-2)
		{
			main_rect.NormalizeRect();

			t_mdc.BitBlt(main_rect.left,main_rect.top,main_rect.Width(),main_rect.Height(),&omdc,main_rect.left,main_rect.top,SRCCOPY);	 //高亮选区
			main_g.DrawRectangle(&bp2,main_rect.left,main_rect.top,main_rect.Width()-1,main_rect.Height()-1);
			//八点
			main_g.FillRectangle(&b2,main_rect.left-2,main_rect.top-2,5,5);
			main_g.FillRectangle(&b2,main_rect.left-2,main_rect.bottom-3,5,5);
			main_g.FillRectangle(&b2,main_rect.right-3,main_rect.top-2,5,5);
			main_g.FillRectangle(&b2,main_rect.right-3,main_rect.bottom-3,5,5);
			main_g.FillRectangle(&b2,main_rect.right-main_rect.Width()/2-3,main_rect.top-2,5,5);
			main_g.FillRectangle(&b2,main_rect.right-main_rect.Width()/2-3,main_rect.bottom-3,5,5);
			main_g.FillRectangle(&b2,main_rect.right-3,main_rect.bottom-main_rect.Height()/2-3,5,5);
			main_g.FillRectangle(&b2,main_rect.left-2,main_rect.bottom-main_rect.Height()/2-3,5,5);
			//放大镜
			main_g.FillRectangle(&b,magnifier_rect.left,magnifier_rect.top,magnifier_rect.Width(),magnifier_rect.Height());//为放大镜填充背景色
			t_mdc.StretchBlt(magnifier_rect.left+b_w,magnifier_rect.top+b_h,c_w,c_h,&omdc,np.x-c_w/magn_x,np.y-c_h/magn_x,c_w/magn_x*2,c_h/magn_x*2,SRCCOPY);//将要放大的图放到放大镜上
			mycolor=omdc.GetPixel(np);
			pixel_inform.Format(_T("%d %d"),np.x,np.y);
			t_mdc.TextOut(magnifier_rect.left+5,magnifier_rect.top+105,pixel_inform);
			main_g.DrawLine(&bp,magnifier_rect.left+45,magnifier_rect.top+51,magnifier_rect.left+45+30,magnifier_rect.top+51);
			main_g.DrawLine(&bp,magnifier_rect.left+61,magnifier_rect.top+38,magnifier_rect.left+61,magnifier_rect.top+63);
			//联合两块rect
			temp_re.UnionRect(main_rect,magnifier_rect);
			//绘制内容
			dc->BitBlt(0,0,GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN),&t_mdc,0,0,SRCCOPY);
		}
		//--------------------------调整选框模式----------------------------------------
		else if (draw_mode<=8 && draw_mode>=0)
		{
			//限制选框的选取区域，用于拖动选择区域，而不是调整区域
			if (main_rect.left<0)
			{int x=main_rect.Width();
			main_rect.left=0;
			main_rect.right=x;
			m_cx=np.x-main_rect.left;
			}

			if (main_rect.top<0)
			{int y=main_rect.Height();
			main_rect.top=0;
			main_rect.bottom=y;
			m_cy=np.y-main_rect.top;
			}

			if (main_rect.right>GetSystemMetrics(SM_CXSCREEN))
			{int x=main_rect.Width();
			main_rect.right=GetSystemMetrics(SM_CXSCREEN);
			main_rect.left=GetSystemMetrics(SM_CXSCREEN)-x;
			m_cx=np.x-main_rect.left;
			}

			if (main_rect.bottom>GetSystemMetrics(SM_CYSCREEN))
			{int y=main_rect.Height();
			main_rect.bottom=GetSystemMetrics(SM_CYSCREEN);
			main_rect.top=GetSystemMetrics(SM_CYSCREEN)-y;
			m_cy=np.y-main_rect.top;
			}

			//------------------------------------------------------------------
			main_rect.NormalizeRect();

			t_mdc.BitBlt(main_rect.left,main_rect.top,main_rect.Width(),main_rect.Height(),&omdc,main_rect.left,main_rect.top,SRCCOPY);
			main_g.DrawRectangle(&bp2,main_rect.left,main_rect.top,main_rect.Width()-1,main_rect.Height()-1);//选框时的边框
			//八点
			main_g.FillRectangle(&b2,main_rect.left-2,main_rect.top-2,5,5);
			main_g.FillRectangle(&b2,main_rect.left-2,main_rect.bottom-3,5,5);
			main_g.FillRectangle(&b2,main_rect.right-3,main_rect.top-2,5,5);
			main_g.FillRectangle(&b2,main_rect.right-3,main_rect.bottom-3,5,5);
			main_g.FillRectangle(&b2,main_rect.right-main_rect.Width()/2-3,main_rect.top-2,5,5);
			main_g.FillRectangle(&b2,main_rect.right-main_rect.Width()/2-3,main_rect.bottom-3,5,5);
			main_g.FillRectangle(&b2,main_rect.right-3,main_rect.bottom-main_rect.Height()/2-3,5,5);
			main_g.FillRectangle(&b2,main_rect.left-2,main_rect.bottom-main_rect.Height()/2-3,5,5);

			//选框调整时放大镜
			if (draw_mode!=0)
			{
				main_g.FillRectangle(&b,magnifier_rect.left,magnifier_rect.top,magnifier_rect.Width(),magnifier_rect.Height());
				t_mdc.StretchBlt(magnifier_rect.left+b_w,magnifier_rect.top+b_h,c_w,c_h,&omdc,np.x-c_w/magn_x,np.y-c_h/magn_x,c_w/magn_x*2,c_h/magn_x*2,SRCCOPY);
				mycolor=omdc.GetPixel(np);
				pixel_inform.Format(_T("%d %d"),np.x,np.y);
				t_mdc.TextOut(magnifier_rect.left+5,magnifier_rect.top+105,pixel_inform);
				main_g.DrawLine(&bp,magnifier_rect.left+45,magnifier_rect.top+51,magnifier_rect.left+45+30,magnifier_rect.top+51);
				main_g.DrawLine(&bp,magnifier_rect.left+61,magnifier_rect.top+38,magnifier_rect.left+61,magnifier_rect.top+63);
			}
			temp_re.UnionRect(main_rect,magnifier_rect);
			dc->BitBlt(0,0,GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN),&t_mdc,0,0,SRCCOPY);
		}
		//--------------------普通模式--------------------------------------------------		
		else
		{

			t_mdc.BitBlt(main_rect.left,main_rect.top,main_rect.Width(),main_rect.Height(),&omdc,main_rect.left,main_rect.top,SRCCOPY);	 //高亮选区
			main_g.DrawRectangle(&bp2,main_rect.left,main_rect.top,main_rect.Width()-1,main_rect.Height()-1);


			//八点
			main_g.FillRectangle(&b2,main_rect.left-2,main_rect.top-2,5,5);
			main_g.FillRectangle(&b2,main_rect.left-2,main_rect.bottom-3,5,5);
			main_g.FillRectangle(&b2,main_rect.right-3,main_rect.top-2,5,5);
			main_g.FillRectangle(&b2,main_rect.right-3,main_rect.bottom-3,5,5);
			main_g.FillRectangle(&b2,main_rect.right-main_rect.Width()/2-3,main_rect.top-2,5,5);
			main_g.FillRectangle(&b2,main_rect.right-main_rect.Width()/2-3,main_rect.bottom-3,5,5);
			main_g.FillRectangle(&b2,main_rect.right-3,main_rect.bottom-main_rect.Height()/2-3,5,5);
			main_g.FillRectangle(&b2,main_rect.left-2,main_rect.bottom-main_rect.Height()/2-3,5,5);






			//绘制内容
			temp_re.UnionRect(main_rect,magnifier_rect);
			dc->BitBlt(0,0,GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN),&t_mdc,0,0,SRCCOPY);
		}




		Sleep(50);
	}

	return 0;
}



	



// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CScreenCaptureDlg 对话框




CScreenCaptureDlg::CScreenCaptureDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CScreenCaptureDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CScreenCaptureDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CScreenCaptureDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CScreenCaptureDlg 消息处理程序

BOOL CScreenCaptureDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	
	

	/*---------------------------截取当前屏幕----------------------------------*/
	
	ScreenCapture();

	/*------------------------扫描屏幕的所有窗口区域---------------------------*/
	HWND t_wnd;
	POINT t_point;
	CRect t_rect1;
	for (int i1=0;i1<GetSystemMetrics(SM_CXSCREEN);i1+=40)
	{
		for (int i2=0;i2<GetSystemMetrics(SM_CYSCREEN);i2+=10)
		{
			t_point.x=i1;
			t_point.y=i2;
			t_wnd=::WindowFromPoint(t_point);
			::GetWindowRect(t_wnd,t_rect1);
			if (data_rect.size()==0)
			  {if(t_rect1.Width()<GetSystemMetrics(SM_CXSCREEN) && t_rect1.Height()<GetSystemMetrics(SM_CYSCREEN))	
				  data_rect.push_back(t_rect1);//在后面保存窗口矩形信息
			  }
	     else if(rect_is_exist(t_rect1,data_rect)!=true)//如果窗口矩形信息不存在，则保存到data_rect
			     data_rect.push_back(t_rect1);	
        }
	}

	/*---------------------------将当前窗口全屏置顶----------------------------*/
	SetWindowPos(NULL,0,0,GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN),NULL);
	
	/*-------------------------创建用于维护界面的线程--------------------------*/
    AfxBeginThread(ThreadProc, (LPVOID)GetSafeHwnd());//将参数GetSafeHwnd()的参数传递给函数ThreadProc()的参数
	

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}



void CScreenCaptureDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CScreenCaptureDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDC *odc=GetDC();
		odc->BitBlt(0,0,GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN),&mdc,0,0,SRCCOPY);
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CScreenCaptureDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}






void CScreenCaptureDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	lbd=true;
	op=point;
	if (draw_mode!=-1)
	{
		rectTop=main_rect.top;
		rectBottom=main_rect.bottom;
		rectLeft=main_rect.left;
		rectRight=main_rect.right;
		draw_mode=judge_click_p(point,main_rect,4);
		if (draw_mode==0)//调整模式
		{
			m_cx=point.x-main_rect.left;
			m_cy=point.y-main_rect.top;

		}
	}
	

	
	//	AfxBeginThread(judge_point, (LPVOID)12);



	
	

	
	CDialogEx::OnLButtonDown(nFlags, point);
}


void CScreenCaptureDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	
	
	is_redraw=false;
	draw_mode=-3;
	lbd=false;
	CDialogEx::OnLButtonUp(nFlags, point);
}


void CScreenCaptureDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (main_rect.PtInRect(point))
	{ 
		CDC t_dc;
		CBitmap t_bitmap;
		t_dc.CreateCompatibleDC(&omdc);
		t_bitmap.CreateCompatibleBitmap(&omdc,main_rect.Width(),main_rect.Height());
		t_dc.SelectObject(t_bitmap);
		t_dc.BitBlt(0,0,main_rect.Width(),main_rect.Height(),&omdc,main_rect.left,main_rect.top,SRCCOPY);

		OpenClipboard();
		EmptyClipboard();
		SetClipboardData(CF_BITMAP,t_bitmap);
		CloseClipboard();
		SendMessage(WM_CLOSE,0,0);
	}

	CDialogEx::OnLButtonDblClk(nFlags, point);
}


void CScreenCaptureDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	MessageBox(NULL,NULL,NULL);
	KillTimer(1);
	CDialogEx::OnTimer(nIDEvent);
}
