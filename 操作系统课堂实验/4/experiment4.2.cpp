#include <cstdio>
#include <unistd.h>
void mem_addr_virtophy(unsigned long vaddr,unsigned long *paddr);
int main()
{
    int vaddr = 10;
    unsigned long paddr = 0;
    mem_addr_virtophy((unsigned long)&vaddr,&paddr);
    printf("vaddr=%x,paddr=%x\n",&vaddr,paddr);
}
void mem_addr_virtophy(unsigned long vaddr,unsigned long *paddr)
{
    int pageSize = getpagesize();//系统设定的页面大小

    //虚拟地址相对于0X0经过的页面数
    unsigned long v_pageIndex = vaddr / pageSize;
    //在pagemap文件中的偏移量
    unsigned long pfn_item_offset = v_pageIndex * sizeof(uint64_t);

    //页内偏移量
    unsigned long page_offset = vaddr % pageSize;
    uint64_t item = 0;//存对应项的值

    //以只读方式打开pagemap文件
    int fd = open("/proc/self/pagemap",O_RDONLY);
    if(fd < 0)
    {
        printf("open failed\n");
        return;
    }

    //将游标移到对应项的起始位置
    if(lseek(fd,pfn_item_offset,SEEK_SET) < 0)
    {
        printf("lseek failed\n");
        return;
    }

    //读取对应项的值并判断读取位数
    if(read(fd, &item, sizeof(uint64_t)) != sizeof(uint64_t))
    {
        printf("read failed\n");
        return;
    }

    //判断物理页是否在内存上
    if (0==(item & (((uint64_t)1)<<63)))
    {
        printf("page is not present");
        return;
    }

    //如果在内存上，物理页号加上偏移地址就是物理地址
    //对应项bit0-54位表示物理页号
    uint64_t phy_pageIndex = ((((uint64_t)1)<<55)-1) & item;
    *paddr = phy_pageIndex * pageSize + page_offset;
}
