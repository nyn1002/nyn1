#include<stdio.h>
#include<string.h>
#include<stdlib.h>


typedef struct
{
	int c_custkey;    	   //顾客编号
	char c_mkgsegment[20]; //对应的某个市场部门
}customer;				   //顾客结构体

typedef struct
{
	int o_orderkey;    	 //订单号
	int o_custkey;    	 //顾客编号
	char o_orderdate[10];//订货日期
}orders;				 //订单

typedef struct
{
	int l_orderkey;//订单号
	double l_extendedprice;//额外价格
	char l_shipdate[10];//发货日期
}lineitem; //商品信息

typedef struct
{
	int l_orderkey;//订单号
	char o_orderdate[10];//订货日期
	double l_extendedprice;//额外价格
}select_result;


customer * read_customer_txt() //读取customer。txt内容
{
	FILE * fp;
	customer *a=NULL;
	a = (customer *)malloc(101*sizeof(customer));
	int i=0;
	char b;
	fp = fopen("customer.txt","r");
	if(NULL==fp)
	{
		printf("cannot open customer.txt!");
		return NULL;
	}
	while(!feof(fp))
	{

		fscanf(fp,"%d%c%s",&a[i].c_custkey,&b,&a[i].c_mkgsegment);

		i++;
	}
	fclose(fp);
	return a;
}
orders * read_orders_txt()//读取orders.txt内容
{
	int i =0;
	orders * a=NULL;
	a = (orders * )malloc(4001*sizeof(orders));
	char b,c;
	long long d;
	FILE *fp;
	fp = fopen("orders.txt","r");
	if(fp == NULL)
	{
		printf("cannot open orders.txt!");
		return NULL;
	}
	while(!feof(fp))
	{

		fscanf(fp,"%d%c%lld%c%s",&a[i].o_orderkey,&b,&d,&c,&a[i].o_orderdate);
		a[i].o_custkey=d%100;

		i++;
	}
	fclose(fp);
	return a;
}

lineitem * read_lineitem_txt()//读取lineitem.txt内容
{
	FILE * fp;
	lineitem * l=NULL;
	l = (lineitem *)malloc(1001*sizeof(lineitem));
	int i=0;
	char b,c;
	fp = fopen("lineitem.txt","r");
	if(fp==NULL)
	{
		printf("cannot open lineitem.txt!");
		return NULL;
	}
	while(!feof(fp))
	{

		fscanf(fp,"%d%c%lf%c%s",&l[i].l_orderkey,&c,&l[i].l_extendedprice,&b,&l[i].l_shipdate);

		i++;
	}
	fclose(fp);
	return l;
}

select_result * Select(customer * cus,orders * ord,lineitem * item,char * order_date,char * ship_date,char * mktsegment)//进行选择
{
    int a, b, c;  //for cycle
    int sindex=0;  //index of slct_res
    double tmp; //for sort
    select_result * slct_res = NULL;

    slct_res = (select_result *)malloc(1001*sizeof(select_result));

    //create slct_res
    for (a=0; a<100; a++){
        if (strcmp(cus[a].c_mkgsegment, mktsegment)==0){
            for (b=0; b<4000; b++){
                if (cus[a].c_custkey==ord[b].o_custkey && strcmp(ord[b].o_orderdate, order_date)<0){
                    for (c=0; c<1000; c++){
                        if (ord[b].o_orderkey==item[c].l_orderkey && strcmp(item[c].l_shipdate, ship_date)>0){
                            slct_res[sindex].l_extendedprice = item[c].l_extendedprice;
                            slct_res[sindex].l_orderkey = item[c].l_orderkey;
                            strcpy(slct_res[sindex].o_orderdate, ord[b].o_orderdate);
                            sindex++;
                        }
                    }
                }
            }
        }
    }

    //now, sindex is length of slct_res

	//remove repetitive elements
	for (a=0; a<sindex-1; a++){
		for (b=a+1; b<sindex; b++){
			if (slct_res[a].l_orderkey==slct_res[b].l_orderkey && strcmp(slct_res[a].o_orderdate, slct_res[b].o_orderdate)==0){
				slct_res[a].l_extendedprice = slct_res[a].l_extendedprice + slct_res[b].l_extendedprice;
				slct_res[b].l_extendedprice = 0;
			}
		}
	}

    //Bubble_Sort on the basis of l_extendedprice
    for (a=0; a<sindex-1; a++){
        for (b=0; b<sindex-1; b++){
            if (slct_res[b].l_extendedprice < slct_res[b+1].l_extendedprice){
                tmp = slct_res[b+1].l_extendedprice;
                slct_res[b+1].l_extendedprice = slct_res[b].l_extendedprice;
                slct_res[b].l_extendedprice = tmp;
            }
        }
    }

    return slct_res;
}

int change_argv_to_number(char s[])//将命令行里读入的数字字符串转化为整形数字
{
	int i=0;
	int number=0;
	while(s[i]!='\0')
	{
		if(i==0)
			number = (s[i]-48);
		else
			number = number*10 + (s[i]-48);

		i++;
	}
	return number;
}

int main(int argc,char * argv[])//argc表示输入内容的总个数，argv[]内保存着输入的内容
{
	int i,j;
	int num;

	int limit=3;
	char order_date[15];
	char ship_date[10];
	char mktsegment[20];
	select_result *result=NULL;
	customer * cus = NULL;//指向客户表的指针
	orders * ord = NULL;//指向订单表的指针
	lineitem * item = NULL;//指向 产品表的指针
	cus = read_customer_txt();//读取customer.txt的内容 ，导入客户表
	ord = read_orders_txt();//读取orders.txt的内容 ，导入订单表
	item = read_lineitem_txt();//读取lineitem.txt的内容 ，导入产品表
	num = change_argv_to_number(argv[4]);//总共计算的次数

	for(i=0;num>0;num--,i=i+4)
	{
		strcpy(mktsegment,argv[5+i]);
		strcpy(ship_date,argv[7+i]);
		strcpy(order_date,argv[6+i]);
		limit = change_argv_to_number(argv[8+i]);
//        printf("%d	mktsegment:%s	order_date:%s	ship_date:%s	limit:%d\n",num,mktsegment,order_date,ship_date,limit);
		result=Select(cus,ord,item,order_date,ship_date,mktsegment);
		printf("l_orderkey|o_orderdate|revenue\n");
		for(j=0;j<limit;j++)
		{
			if(result[j].l_extendedprice==0)
				printf("null      |null       |null   \n");
			else
				printf("%-10d|%-11s|%-20.2lf\n",result[j].l_orderkey,result[j].o_orderdate,result[j].l_extendedprice);
		}
		free(result);
		result = NULL;
	}


	return 0;
}
