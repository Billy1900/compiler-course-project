int a,b,c;//可改错误1：缺少分号
float m,n;
char c1,c2;//增加char类型
char a[10];//增加数组的定义
int fibo(int a)
{/*注释部分自动去掉*/
	int i;
	if(a == 1 || a == 2){
	  return 1;
        }
	for(i<15){//增加了for语句循环
          i++;
	}
	return fibo(a-1)+fibo(a-2);
}
int main()//注释部分自动去掉
{
	int m,n,i;
	char c;
	float ar[20];
	m=read();
	i=1;
	i++;
	--i;//加了自增和自减
	m+=i+15;//加了复合赋值运算
	while(i <= m){
		n=fibo(i);
		write(n);
		i=i+1;
	}
	return 1;
}

