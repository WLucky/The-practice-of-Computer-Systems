#include <assert.h>
#include <stdio.h>

typedef enum {NEG, ZERO, POS, OTHER} range_t;

range_t find_range(float x)
{
    __asm__(
        "vxorps %xmm1, %xmm1, %xmm1\n\t"
        "vucomiss %xmm1, %xmm0\n\t"
        "jp .P\n\t"
        "ja .A\n\t"
        "jb .B\n\t"
        "je .E\n\t"
        ".A:\n\t"
        "movl $2, %eax\n\t"
        "jmp .Done\n\t"
        ".B:\n\t"
        "movl $0, %eax\n\t"
        "jmp .Done\n\t"
        ".E:\n\t"
        "movl $1, %eax\n\t"
        "jmp .Done\n\t"
        ".P:\n\t"
        "movl $3,%eax\n\t"
        ".Done:\n\t"
    );
}

range_t find_range_C(float x)
{
    range_t result;
    if(x < 0)
    {
        result = NEG;
    }
    else if (x == 0)
    {
        result = ZERO;
    }else if (x > 0)
    {
        result = POS;
    }else
    {
        result = OTHER;
    }
    
    return result;
}

int main(int argc, char *argv[])
{
    long long count = 0;
    for(unsigned i = 0; i < (unsigned)(-1); i++)
    {
        count++;
        float f = *(float*)&i;
        if(find_range(f) != find_range_C(f))
		{
			printf("err float %f first %d second %d\n", f, find_range(f), find_range_C(f));
			break;
		}
    }

    printf("test count %lld sizeof long %lu\n", count, sizeof(long long));
    return 0;
}
