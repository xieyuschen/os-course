#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>
typedef struct page{
    int index;
    bool in_block;
    int times_be_call;
    int times_stay;
    page* next;
} page;
page Max(page a, page b);
//�������ģ���ڴ�Ĵ���
int times = 500;
//�����������ڴ���������г���
const int seq_len = 50;
//���岢��ʼ���ڴ�����
int block[4]={-1,-1,-1,-1};
int visit_seq [seq_len];
int page_num = 32;
page *first,*node, *head, *end;

struct page all[32];
//ҳ����̭�㷨����.
void FIFO();
void LRU();
void OPT();


//����һ������ģ���ڴ�
page* generate(int n)
{
    head = (page*)malloc(sizeof(page));
    head->index=0;
    head->in_block = false;
    head->times_stay = 0;
    end = head;
    all[0] = *head;
  for(int i = 0 ; i < n-1 ; i ++)
  {
      node = (page*)malloc(sizeof(page));
      node->times_stay = 0;
      node->in_block = false;
      node->index = i+1;
      end->next = node;
      end = node;
      all[i+1] = *node;
  }
  end->next = NULL;
  return head;
}
int main(void)
{
    srand((unsigned int)(time(NULL)));
    //����ҳ��
    first = generate(page_num);
    //���ɷ�������

    printf("���̷���ҳ��˳��Ϊ: ");
    for(int i =0;i< seq_len;i++)
    {
        int n = rand()%32;
        visit_seq[i] = n;
        printf("%d ", n);
    }
    printf("\n");

    OPT();
    FIFO();
    LRU();
}
void OPT()
{
    //δ���д���
    double miss = 0;
    //������һ��ҳ������Զ�Ľ���������.

    int num=0;
    for(int i = 0 ; i < seq_len; i++)
    {
        int check[32]={0};
        //���ҳ����û�з���������Ҫ���ʵ�ҳ
        if(block[0]!= visit_seq[i] && block[1]!= visit_seq[i] && block[2]!= visit_seq[i] && block[3]!= visit_seq[i] )
        {
            //ȱҳ������һ
            miss++;
            if(block[0]==-1||block[1]==-1||block[2]==-1||block[3]==-1)
            {

                block[num] = visit_seq[i];
                num++;
                printf("ҳ%d����block\n", block[i]);
                if(num > 4)
                    printf("error! this is the block[%d]\n", i);
            }
            //Ѱ����Զ�����ʵ�ҳ��.���ȱ���Ŀǰδ���ʵ�����.
            else {
                for(int j = i ; j < seq_len; j++)
            {
                //֮ǰ����һ������,�����洢��ǰ�������������ҳ�ķ��ʴ���.
                check[visit_seq[j]]++;
                //�����ʱһ��ҳ��Ϊ�ı����ʴ���Ϊ0,�������ı����ʴ�������Ϊ0˵����������Զ�����ʵ�ҳ��.
                if(check[block[0]]==0 && check[block[1]]!=0 && check[block[2]]!=0 && check[block[3]]!=0)
                {
                    printf("ҳ%d�뿪block,ҳ%d����block\n",block[0], visit_seq[i]);
                    block[0] = visit_seq[i];
                    break;
                }
                else if(check[block[0]]!=0 && check[block[1]]==0 && check[block[2]]!=0 && check[block[3]]!=0)
                {
                    printf("ҳ%d�뿪block,ҳ%d����block\n",block[1], visit_seq[i]);
                    block[1] = visit_seq[i];
                    break;
                }
                else if(check[block[0]]!=0 && check[block[1]]!=0 && check[block[2]]==0 && check[block[3]]!=0)
                {
                    printf("ҳ%d�뿪block,ҳ%d����block\n",block[2], visit_seq[i]);
                    block[2] = visit_seq[i];
                    break;
                }
                else if(check[block[0]]!=0 && check[block[1]]!=0 && check[block[2]]!=0 && check[block[3]]==0)
                {
                    printf("ҳ%d�뿪block,ҳ%d����block\n",block[3], visit_seq[i]);
                    block[3] = visit_seq[i];
                    break;
                }
                //��block�ж���1��Ԫ���ڽ������ᱻ����ʱ,���ѡȡһ��Ԫ���滻
            }
            if(block[0]!=visit_seq[i] && block[1]!=visit_seq[i] && block[2]!=visit_seq[i] && block[3]!=visit_seq[i])
            {
                if(check[block[0]]==0)
                {
                    printf("ҳ%d�뿪block,ҳ%d����block(���ڶ����Զ����Ԫ��)\n",block[0], visit_seq[i]);
                    block[0] = visit_seq[i];
                }
                else if(check[block[1]]==0)
                {
                    printf("ҳ%d�뿪block,ҳ%d����block(���ڶ����Զ����Ԫ��)\n",block[1], visit_seq[i]);
                    block[1] = visit_seq[i];
                }
                else if(check[block[2]]==0)
                {
                    printf("ҳ%d�뿪block,ҳ%d����block(���ڶ����Զ����Ԫ��)\n",block[2], visit_seq[i]);
                    block[2] = visit_seq[i];
                }
                else if(check[block[3]]==0)
                {
                    printf("ҳ%d�뿪block,ҳ%d����block(���ڶ����Զ����Ԫ��)\n",block[3], visit_seq[i]);
                    block[3] = visit_seq[i];
                }
            }
            }


        }
        else {
            printf("λ��block���е�ҳ%d������\n",visit_seq[i]);
        }
    }
    printf("ȱҳ����Ϊ: %f\n", miss);
    printf("ȱҳ��Ϊ: %f\n", miss/seq_len);
}

void FIFO()
{
    double miss=0;
    for(int i = 0; i < seq_len ; i++)
    {
        if(block[0] != visit_seq[i] && block[1] != visit_seq[i] && block[2] != visit_seq[i] && block[3] != visit_seq[i])
        {
            miss++;
            printf("ҳ%d��block���뿪,ҳ%d����block\n", block[0], visit_seq[i]);
            block[0] = block[1];
            block[1] = block[2];
            block[2] = block[3];
            block[3] = visit_seq[i];

        }
        else continue;
    }
    printf("ȱҳ����Ϊ: %f\n", miss);
    printf("ȱҳ��Ϊ: %f\n", miss/(double)seq_len);
}

void LRU()
{
    double miss=0;
    page* node = head;
    for(int i = 0; i < seq_len ; i++)
    {
        //���ڴ滹�п���ʱ,�����ڴ�
        if(block[0]==-1||block[1]==-1||block[2]==-1||block[3]==-1)
        {
            block[i] = visit_seq[i];
            printf("ҳ%d����block\n", block[i]);
            all[block[i]].times_stay=1;
            int k=i;
            while(k--!=0)
                all[block[k]].times_stay++;
        }
        else {
            //���ڴ���û��Ҫ���ʵ�ҳ��ʱ
        if(block[0] != visit_seq[i] && block[1] != visit_seq[i] && block[2] != visit_seq[i] && block[3] != visit_seq[i])
        {
            miss++;
            //�ҵ�����block��ʱ�����ҳ���index
            int max = Max(all[block[0]], Max(all[block[1]], Max(all[block[2]], all[block[3]]))).index;
            //�ҵ���index֮���block�е�ÿһ��Ԫ�ؽ��м��,����������滻���·��ʵ�ҳ,����block������Ԫ�ص�ʱ������.
            if(block[0] == max)
            {
                printf("ҳ%d�뿪block,ҳ%d����block\n",max, visit_seq[i]);
                all[block[0]].times_stay=0;
                block[0] = visit_seq[i];
                all[visit_seq[i]].times_stay=1;
                all[block[1]].times_stay++;
                all[block[2]].times_stay++;
                all[block[3]].times_stay++;
            }
            else if(block[1] == max)
            {
                printf("ҳ%d�뿪block,ҳ%d����block\n",max, visit_seq[i]);
                all[block[1]].times_stay=0;
                block[1] = visit_seq[i];
                all[visit_seq[i]].times_stay=1;
                all[block[0]].times_stay++;
                all[block[2]].times_stay++;
                all[block[3]].times_stay++;
            }
            else if(block[2] == max)
            {
                printf("ҳ%d�뿪block,ҳ%d����block\n",max, visit_seq[i]);
                all[block[2]].times_stay=0;
                block[2] = visit_seq[i];
                all[visit_seq[i]].times_stay=1;
                all[block[0]].times_stay++;
                all[block[1]].times_stay++;
                all[block[3]].times_stay++;
            }
            else if(block[3] == max)
            {
                printf("ҳ%d�뿪block,ҳ%d����block\n",max, visit_seq[i]);
                all[block[3]].times_stay=0;
                block[3] = visit_seq[i];
                all[visit_seq[i]].times_stay=1;
                all[block[0]].times_stay++;
                all[block[1]].times_stay++;
                all[block[2]].times_stay++;
            }

        }
        //����ڴ�����Ҫ���ʵ�ҳ��.
        else
        {
                //����all�����ж�Ӧҳ��block�е�ʱ��.
                if(block[0] == visit_seq[i])
                {
                    printf("λ��block�е�ҳ%d������\n",visit_seq[i]);
                    all[block[0]].times_stay=1;
                    all[block[1]].times_stay++;
                    all[block[2]].times_stay++;
                    all[block[3]].times_stay++;
                }
                else if  (block[1] == visit_seq[i])
                {
                    printf("λ��block�е�ҳ%d������\n",visit_seq[i]);
                    all[block[0]].times_stay++;
                    all[block[1]].times_stay=1;
                    all[block[2]].times_stay++;
                    all[block[3]].times_stay++;
                }
                else if  (block[2] == visit_seq[i])
                {
                    printf("λ��block�е�ҳ%d������\n",visit_seq[i]);
                    all[block[0]].times_stay++;
                    all[block[1]].times_stay++;
                    all[block[2]].times_stay=1;
                    all[block[3]].times_stay++;
                }
                else if  (block[3] == visit_seq[i])
                {
                    printf("λ��block�е�ҳ%d������\n",visit_seq[i]);
                    all[block[0]].times_stay++;
                    all[block[1]].times_stay++;
                    all[block[2]].times_stay++;
                    all[block[3]].times_stay=1;
                }
                else printf("%d, error!\n", visit_seq[i]);
                }}
    }
    printf("�ܹ�ȱҳ����: %f, ȱҳ��: %f\n", miss, miss/(double) seq_len);
}
page Max(page a, page b)
{
    if (a.times_stay> b.times_stay)
        return a;
    else return b;
}