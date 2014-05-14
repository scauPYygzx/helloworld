import java.awt.*;
import javax.swing.*;
import java.awt.event.*;
import java.io.*;
import java.util.*;

public class MyFive extends JFrame {
	public final int N = 15;

	public static void main(String[] args) {
		MyFive frame = new MyFive();
	}

	public MyFive() {
		setLayout(new BorderLayout());
		getContentPane().add(new Board());

		setTitle("MyFive");
		setSize(520, 620);
		setResizable(false);
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		//setLocationRelativeTo(null);
		setLocation(10,10);
		setVisible(true);
	}
}

class Board extends JPanel implements ActionListener {
	private int INF = 1 << 30;
	private int startX = 50;
	private int startY = 50;
	boolean isBlack = true;
	private PointInt nextPoint;
	private Analyze ana = new Analyze();
	private int[] grade = {0,10,100,1000,10000,1000000};
	public ArrayList<PointInt> pointList = new ArrayList<PointInt>();
	public int count;
	public int[][] status = new int[15][15];
	private JButton jbtReset = new JButton("Reset");
	private JButton jbtExit = new JButton("EXIT");
	private Random rand = new Random();

	public Board() {
		addMouseListener(new MouseHandler());
		add(jbtReset);
		add(jbtExit);
		jbtReset.addActionListener(this);
		jbtExit.addActionListener(this);

		for(int i = 0;i < 15;i++)
			for(int j = 0;j < 15;j++)
				status[i][j] = 0;
		count = 0;
	}

	public void actionPerformed(ActionEvent e) {
		JButton button = (JButton)e.getSource();
		if(button.equals(jbtReset)) {
			newGame();
		}
		else if(button.equals(jbtExit)) {
			System.exit(0);
		}
	}

	protected void paintComponent(Graphics g) {
		super.paintComponent(g);
		
		g.setColor(Color.YELLOW);
		g.fillRect(startX,startY,420,420);
		
		g.setColor(Color.BLACK);
		g.fillRect(startX + 7 * 30 - 2,startY + 7 * 30 - 2,5,4);
		for(int i = 0;i < 15;i++) {
			g.drawString((i + 0) + "",startX + i * 30,44);
			g.drawString((i + 0) + "",36,startY + i * 30);
		}
		for(int i = 0;i < 15;i++) {
			g.drawLine(startX,startY + i * 30,startX + 420,startY + i *30);
			g.drawLine(startX + i * 30,startY,startX + i * 30,startY + 420);
		}

		for(int i = 0;i < pointList.size();i++) {
			int x = pointList.get(i).getX();
			int y = pointList.get(i).getY();
			if((i & 1) == 0) {  
				g.setColor(Color.BLACK);
			}
			else {
				g.setColor(Color.WHITE);
			}
			g.fillOval(startX + x * 30 - 14,startY + y * 30 - 14,28,28);
			g.setColor(Color.BLACK);
			g.drawOval(startX + x * 30 - 14,startY + y * 30 - 14,28,28);
		}
	}

	public boolean isWin(int[][] status,int x,int y,int isBlack) {
		int minX,minY,maxX,maxY;

		minX = (x - 4) < 0? 0 : (x - 4);
		minY = (y - 4) < 0? 0 : (y - 4);
		maxX = (x + 4) > 14? 14 : (x + 4);
		maxY = (y + 4) > 14? 14 : (y + 4);

		for(int i = minX;i <= x;i++) {
			if(i + 4 > 14) break;
			else if((status[i][y] == isBlack) && (status[i + 1][y] == isBlack) && (status[i + 2][y] == isBlack) && (status[i + 3][y] == isBlack) && (status[i + 4][y] == isBlack)) return true;
		}

		for(int i = minY;i <= y;i++) {
			if(i + 4 > 14) break;
			else if((status[x][i] == isBlack) && (status[x][i + 1] == isBlack) && (status[x][i + 2] == isBlack) && (status[x][i + 3] == isBlack) && (status[x][i + 4] == isBlack)) return true;
		}

		for(int i = x,j = y;i >= minX;i--,j--) {
			if((i + 4) > 14 || (j + 4) > 14) continue;
			if(i < 0 || j < 0) break;
			else if((status[i][j] == isBlack) && (status[i + 1][j + 1] == isBlack) && (status[i + 2][j + 2] == isBlack) && (status[i + 3][j + 3] == isBlack) && (status[i + 4][j + 4] == isBlack)) return true;
		}

		for(int i = x,j = y;i <= maxX;i++,j--) {
			if((i - 4) < 0 || (j + 4) > 14) continue;
			if(j < 0) break;
			else if((status[i][j] == isBlack) && (status[i - 1][j + 1] == isBlack) && (status[i - 2][j + 2] == isBlack) && (status[i - 3][j + 3] == isBlack) && (status[i - 4][j + 4] == isBlack)) return true;
		}
		return false;
	}

	public void newGame() {
			isBlack = true;
			pointList.clear();
			for(int i = 0;i < 15;i++)
				for(int j = 0;j < 15;j++)
					status[i][j] = 0;
			repaint();
	}

	boolean isExists(int x,int y) {
		for(int i = 0;i < pointList.size();i++) {
			if((pointList.get(i).getX() == x) && (pointList.get(i).getY() == y)) return true;
		}
		return false;
	}

	public int calculatePoint(int[][] status,int type) {
		int ans = 0;

		for(int i = 0;i < 15;i++) {
			for(int j = 0;j < 15;j++) {
				if(status[i][j] == type) {
					int k,count = 0;
					for(k = j;k < 15;k++) {
						if(status[i][k] == type) count++;
						else break;
					}
					j = k;
					if(count > 5) count = 5;
					ans += grade[count];
				}
			}
		}

		for(int i = 0;i < 15;i++)
			for(int j = 0;j < 15;j++) {
				if(status[j][i] == type) {
					int k,count = 0;
					for(k = j;k < 15;k++) {
						if(status[k][i] == type) count++;
						else break;
					}
					j = k;
					if(count > 5) count = 5;
					ans += grade[count];
				}
			}

		for(int j = 0;j < 15;j++) {
			int k,count = 0;
			for(k = j;k < 15;k++) {
				if(status[k][k] == type) count++;
				else break;
			}
			j = k;
			if(count > 5) count = 5;
			ans += grade[count];
		}
		for(int i = 1;i < 11;i++) {
			for(int h = 0;h < 15 - i;h++) {
				int k,l,count = 0;
				for(k = i + h,l = h;k < 15;k++,l++)
					if(status[k][l] == type) count++;
					else break;
				h = l;
				if(count > 5) count = 5;
				ans += grade[count];
			}
			for(int h = 0;h < 15 - i;h++) {
				int k,l,count = 0;
				for(k = i + h,l = h;k < 15;k++,l++)
					if(status[l][k] == type) count++;
					else break;
				h = l;
				if(count > 5) count = 5;
				ans += grade[count];
			}
		}

		for(int i = 0;i < 15;i++) {
			int j,count = 0;
			for(j = i;j < 15;j++) {//System.out.println(j + "  *  " + (14 - j));
				if(status[j][14 - j] == type) count++;
				else break;
			}
			i = j;
			if(count > 5) count = 5;
			ans += grade[count];
		}
		for(int i = 4;i < 14;i++) {
			for(int j = 0;j < i;j++) {
				int k,l,count = 0;
				for(k = i,l = j;k >= 0;l++,k--)
					if(status[k][l] == type) count ++;
					else break;
				j = l;
				if(count > 5) count = 5;
				ans += grade[count];
			}
			for(int j = 0;j < i;j++) {
				int k,l,count = 0;
				for(k = i,l = j;k >= 0;l++,k--)
					if(status[l][k] == type) count ++;
					else break;
				j = l;
				if(count > 5) count = 5;
				ans += grade[count];
			}
		}
		return ans;
	}

//	public PointInt PVC(int[][] status,int dep) {
//		int maxPoint = -INF,tp1= 0,tp2 = 0;
//		int tmpX = 0,tmpY = 0;
//
//		for(int i = 0;i < 15;i++) {
//			for(int j = 0;j < 15;j++) {
//				if(status[i][j] != 0) continue;
//				status[i][j] = dep % 2 == 0? 1 : 2;
//				int p1 = ana.analyzeAll(status,i,j) + 1;
//				status[i][j] = dep % 2 == 0? 2 : 1;
//				int p2 = ana.analyzeAll(status,i,j);
//				status[i][j] = 0;
//				int p = Math.max(p1,p2);//System.out.print((int)(p/10) + " ");
//				if(p > maxPoint) {
//					maxPoint = p;
//					tmpX = i;
//					tmpY = j;
//					tp1 = p1;
//					tp2 = p2;
//				}
//			}//System.out.println("");
//		}//System.out.println("-----------");
//		System.out.println(tmpX + "  " + tmpY + "  " + tp1 + "  " + tp2 + "  " + maxPoint);
//		return new PointInt(tmpX,tmpY);
//	}
	
	//白色: 2, 黑色: 1
	public PointInt PVC(int[][] status, int dep) {
		if(pointList.size() == 1) {
			int x = pointList.get(0).x;
			int y = pointList.get(0).y;
			if(x <= 7) x++;
			else x--;
			return new PointInt(x,y);
		}
		
		int maxValue = -INF;
		ArrayList<PointInt> pl = new ArrayList<PointInt>();
		for(int i = 0;i < 15; i++) {
			for(int j=0 ;j < 15; j++) {
				if(status[i][j] != 0) continue;
				status[i][j] = 2;
				int type = ana.analyzeAll(status, i, j);
				if(type == 100) return new PointInt(i,j);
				int value = miniMax(status, maxValue, INF, 0);
				value += ana.analyzeAll(status, i, j);
				if(value >= maxValue) {
					if(value > maxValue) pl.clear();
					maxValue = value;
					PointInt tmpPoint = new PointInt(i,j);
					pl.add(tmpPoint);
				}
				status[i][j] = 0;
			}
		}
		System.out.println(maxValue+" : "+pl.size());
		return pl.get(0);
	}
	
	public int miniMax(int[][] status, int alpha, int beta, int dep){
		if(dep == 0) {//find min
			int minValue = beta;
			for(int i = 0;i < 15; i++) {
				for(int j=0 ;j < 15; j++) {
					if(status[i][j] != 0) continue;
					status[i][j] = 1;
					int tmp = ana.analyzeAll(status, i, j);
					if(tmp == 100) return -INF-10000;
					int value = miniMax(status, alpha, minValue, dep + 1);
					value -= ana.analyzeAll(status, i, j);
					status[i][j] = 2;
					status[i][j] = 0;
					if(value < minValue) 
						minValue = value;
					if(minValue <= alpha)
						return minValue;
				}
			}
			return minValue;
		}
		else if(dep == 1){ //find max
			int maxValue = alpha;
			for(int i = 0;i < 15; i++) {
				for(int j=0 ;j < 15; j++) {
					if(status[i][j] != 0) continue;
					status[i][j] = 2;
					int value = ana.analyzeAll(status, i, j);
					status[i][j] = 0;
					if(value == 100) return INF;
					if(value > maxValue) 
						maxValue = value;
					if(maxValue >= beta)
						return maxValue;
				}
			}
			return maxValue;
		}
		return 0;
	}
	
	public void print() {
		for(int i=0;i<15;i++){
			for(int j=0;j<15;j++){
				System.out.print(status[i][j]+" ");
			}
			System.out.println();
		}
		System.out.println();
	}
	
	public boolean isContinueToPlay() {
		int isContinue = 1;
		if(isBlack) isContinue = JOptionPane.showConfirmDialog(null,"Black Player has won!! Continue??");
		else isContinue = JOptionPane.showConfirmDialog(null,"White Player has won!! Continue??");
		if(isContinue == 0) {newGame();return true;}
		else System.exit(0);
		return false;
	}

	class MouseHandler extends MouseAdapter {
		public void mousePressed(MouseEvent e) {
			if(e.getX() < 35 || e.getY() < 35) return ;
			int x = ((e.getX() - 50) + 15) / 30;
			int y = ((e.getY() - 50) + 15) / 30;
			PointInt tmp = new PointInt(0,0);
			if((x >= 0) && (y >= 0) && (x < 15) && (y < 15) && !isExists(x,y)) {
				tmp.setX(x);
				tmp.setY(y);
				pointList.add(tmp);
				if(isBlack) status[x][y] = 1;
				else status[x][y] = 2;
				if(isWin(status,x,y,isBlack? 1 : 2)) {
					if(isContinueToPlay()) return ;
				}
				isBlack = !isBlack;
				repaint();

				tmp = PVC(status,1);
				x = tmp.getX();
				y = tmp.getY();
				pointList.add(tmp);
				if(isBlack) status[x][y] = 1;
				else status[x][y] = 2;
				if(isWin(status,x,y,isBlack? 1 : 2)) {
					if(isContinueToPlay()) return ;
				}
				isBlack = !isBlack;
				repaint();


				//Analyze ana = new Analyze(status,x,y);
				//int temp1 = ana.analyzeLF();
				//int temp2 = ana.analyzeUD();
				//int temp3 = ana.analyzeLURD();
				//int temp4 = ana.analyzeLDRU();
				//System.out.println(temp1 + "  " + temp2 + "  " + temp3 + "  " + temp4);
				/*
				System.out.println(ana.analyzeAll(status,x,y));
				int temp1 = ana.analyzeLF();
				int temp2 = ana.analyzeUD();
				int temp3 = ana.analyzeLURD();
				int temp4 = ana.analyzeLDRU();
				System.out.println(temp1 + "  " + temp2 + "  " + temp3 + "  " + temp4);
				*/
			}
		}
	}
}

class Analyze {
	private int[][] status = new int[17][17];
	public int type = 0,x = 0,y = 0;
	private int finalGrade = 0;
					            //0,1 ,2 ,3 ,4 ,5 ,6 ,7 ,8 ,9 ,10
	//private static int[] grade = {0,10,20,30,40,50,60,70,80,90,100};

	public Analyze() {
	}

	public Analyze(int[][] status,int x,int y) {
		for(int i = 0;i < 17;i++)
			for(int j = 0;j < 17;j++)
				if(i == 0 || j == 0 || i == 16 || j == 16) this.status[i][j] = -1;
				else this.status[i][j] = status[i - 1][j - 1];
		this.x = x + 1;
		this.y = y + 1;
		this.type = status[x][y];
	}

	private int pow(int a,int b) {
		int ans = 1;
		for(int i = 0;i< b;i++) ans *= a;
		return ans;
	}

	public void setStatus(int[][] status) {
		for(int i = 0;i < 17;i++)
			for(int j = 0;j < 17;j++)
				if(i == 0 || j == 0 || i == 16 || j == 16) this.status[i][j] = -1;
				else this.status[i][j] = status[i - 1][j - 1];
	}

	public void setX(int x) {
		this.x = x;
	}

	public void setY(int y) {
		this.y = y;
	}

	private int getCount(int count,boolean flag1,boolean flag2) {
		count = pow(5,count) ;
		if(flag1 && flag2) count *= 3;
		else if(flag1 || flag2) count *= 2;
		switch(count) {
			/**
			 * 单子:0
			 * 死二:10
			 * 活二:20
			 * 死三:30
			 * 双 活二:40
			 * 活三:50
			 * 死四:60
			 * 死三活三:70
			 * 双 活三:80
			 * 活四 双死四 死四活三:90
			 * 成五:100
			 */
			case 5:count = 0;break;
			case 10:count = 0;break;
			case 15:count = 0;break;
			case 50:count = 10;break;
			case 75:count = 20;break;
			case 250:count = 30;break;
			case 375:count = 50;break;
			case 1250:count = 60;break;
			case 1875:count = 90;break;
			case 3125:count = 100;break;
			case 6250:count = 100;break;
			case 9375:count = 100;break;
			default:count = 0;
		}
		return count;
	}

	public int analyzeAll(int[][] ts,int x,int y) {
		int ans = 0;
		this.x = x + 1;
		this.y = y + 1;
		int[] temp = new int[5];

		setStatus(ts);
		type = status[this.x][this.y];
		temp[0] = analyzeLF();
		temp[1] = analyzeUD();
		temp[2] = analyzeLURD();
		temp[3] = analyzeLDRU();
		for(int i = 0;i < 3;i++)
			for(int j = i + 1;j < 4;j++)
				if(temp[i] < temp[j]) {
					temp[4] = temp[i];
					temp[i] = temp[j];
					temp[j] = temp[4];
				}
//		if(temp[0] >= 90) ans = temp[0];
//		else if(temp[0] == 60 && temp[1] == 60) ans = 90;
//		else if(temp[0] == 60 && temp[1] == 50) ans = 90;
//		else if(temp[0] == 50 && temp[1] == 50) ans = 80;
//		else if(temp[0] == 50 && temp[1] == 30) ans = 70;
//		else if(temp[0] == 20 && temp[1] == 20) ans = 40;
//		else ans = temp[0];
		if(temp[0] >= 90) ans = temp[0];
		else if(temp[0] == 60 && temp[1] == 60) ans = 90;
		else if(temp[0] == 60 && temp[1] == 50) ans = 90;
		else if(temp[0] == 50 && temp[1] == 50) ans = 80;
		else if(temp[0] == 50 && temp[1] == 30) ans = 70;
		else if(temp[0] == 20 && temp[1] == 20) ans = 40;
		else ans = temp[0];
		return ans;
	}

	public int analyzeLF() {
		int count = 0,tx = x,ty = y;
		boolean leftFlag = false,rightFlag = false;

		if(type == 0) return 0;
		else count = 1;
		do {
			tx--;
			if(status[tx][ty] == type) count++;
			else if(status[tx][ty] == 0) leftFlag = true;
			else leftFlag = false;
		}while(status[tx][ty] == type);

		tx = x;
		ty = y;
		do {
			tx++;
			if(status[tx][ty] == type) count++;
			else if(status[tx][ty] == 0) rightFlag = true;
			else rightFlag = false;
		}while(status[tx][ty] ==type);
		count = getCount(count,leftFlag,rightFlag);
		return count;
	}

	public int analyzeUD() {
		int count = 0,tx = x,ty = y;
		boolean upFlag = false,downFlag = false;

		if(type == 0) return 0;
		else count = 1;
		do {
			ty--;
			if(status[tx][ty] == type) count++;
			else if(status[tx][ty] == 0) upFlag = true;
			else upFlag = false;
		}while(status[tx][ty] == type);

		tx = x;
		ty = y;
		do {
			ty++;
			if(status[tx][ty] == type) count++;
			else if(status[tx][ty] == 0) downFlag = true;
			else downFlag = false;
		}while(status[tx][ty] == type);
		count = getCount(count,upFlag,downFlag);
		return count;
	}

	public int analyzeLURD() {
		int count = 0,tx = x,ty = y;
		boolean luFlag = false,rdFlag = false;

		if(type == 0) return 0;
		else count = 1;
		do {
			tx--;ty--;
			if(status[tx][ty] == type) count++;
			else if(status[tx][ty] == 0) luFlag = true;
			else luFlag = false;
		}while(status[tx][ty] == type);

		tx = x;
		ty = y;
		do {
			tx++;ty++;
			if(status[tx][ty] == type) count++;
			else if(status[tx][ty] == 0) rdFlag = true;
			else rdFlag = false;
		}while(status[tx][ty] == type);
		count = getCount(count,luFlag,rdFlag);
		return count;
	}

	public int analyzeLDRU() {
		int count = 0,tx = x,ty = y;
		boolean ldFlag = false,ruFlag = false;

		if(type == 0) return 0;
		else count = 1;
		do {
			tx--;ty++;
			if(status[tx][ty] == type) count++;
			else if(status[tx][ty] == 0) ldFlag = true;
			else ldFlag = false;
		}while(status[tx][ty] == type);

		tx = x;
		ty = y;
		do {
			tx++;ty--;
			if(status[tx][ty] == type) count++;
			else if(status[tx][ty] == 0) ruFlag = true;
			else ruFlag = false;
		}while(status[tx][ty] == type);
		count = getCount(count,ldFlag,ruFlag);
		return count;
	}

}
