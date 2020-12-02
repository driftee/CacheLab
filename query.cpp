#include <cstdio>
#include "dataload.h"
#define limit_orderdate 19950630

const char lineorder_name[] = "/home/optdata/lineorder.tbl";

static __inline__ uint64_t curtick()
{
	uint64_t tick;
	unsigned long lo, hi;
	__asm__ __volatile__("rdtsc"
						 : "=a"(lo), "=d"(hi));
	tick = (uint64_t)hi << 32 | lo;
	return tick;
}

static __inline__ void startTimer(uint64_t *t)
{
	(*t) = curtick();
}

static __inline__ void stopTimer(uint64_t *t)
{
	(*t) = curtick() - *t;
}

int main()
{
	table_info lineorder_table_info;
	FILE *lineorder_file;

	//load lineorder table from file
	lineorder_file = fopen(lineorder_name, "r");
	loadTable(lineorder_file, &lineorder_table_info);

	unsigned int quantity_sum = 0;
	double discount_total_price = 0;
	double tax_discount_total_price = 0;
	unsigned int quantity_sum_with_condition = 0;
	double discount_total_price_with_condition = 0;
	double tax_discount_total_price_with_condition = 0;

	uint64_t beg;
	startTimer(&beg);

	//you should editor the following the part to accelerate the calculation
	/*--------------------------------*/

	long long rows = lineorder_table_info.rows;
	bool le_0;
	bool le_1;
	bool le_2;
	double extended_price;
	double rate_1_0;
	double rate_1_1;
	double rate_1_2;
	int i;

	for (i = 0; i + 3< rows; i += 3)
	{
		le_0 = lineorder_table_info.table->lo_orderdate[i] <= limit_orderdate;
		le_1 = lineorder_table_info.table->lo_orderdate[i + 1] <= limit_orderdate;
		le_2 = lineorder_table_info.table->lo_orderdate[i + 2] <= limit_orderdate;

		quantity_sum += (lineorder_table_info.table->lo_quantity[i] + lineorder_table_info.table->lo_quantity[i + 1]) + lineorder_table_info.table->lo_quantity[i + 2];
		quantity_sum_with_condition += (lineorder_table_info.table->lo_quantity[i] * le_0 + lineorder_table_info.table->lo_quantity[i + 1] * le_1) + lineorder_table_info.table->lo_quantity[i + 2] * le_2;


		rate_1_0 = 1 - lineorder_table_info.table->lo_discount[i];
		rate_1_1 = 1 - lineorder_table_info.table->lo_discount[i + 1];
		rate_1_2 = 1 - lineorder_table_info.table->lo_discount[i + 2];

		rate_1_0 *= lineorder_table_info.table->lo_extendedprice[i];
		rate_1_1 *= lineorder_table_info.table->lo_extendedprice[i + 1];
		rate_1_2 *= lineorder_table_info.table->lo_extendedprice[i + 2];

		discount_total_price += rate_1_0;
		discount_total_price += rate_1_1;
		discount_total_price += rate_1_2;

		discount_total_price_with_condition += rate_1_0 * le_0;
		discount_total_price_with_condition += rate_1_1 * le_1;
		discount_total_price_with_condition += rate_1_2 * le_2;

		rate_1_0 *= 1 + lineorder_table_info.table->lo_tax[i];
		rate_1_1 *= 1 + lineorder_table_info.table->lo_tax[i + 1];
		rate_1_2 *= 1 + lineorder_table_info.table->lo_tax[i + 2];

		tax_discount_total_price += rate_1_0;
		tax_discount_total_price += rate_1_1;
		tax_discount_total_price += rate_1_2;

		tax_discount_total_price_with_condition += rate_1_0 * le_0;
		tax_discount_total_price_with_condition += rate_1_1 * le_1;
		tax_discount_total_price_with_condition += rate_1_2 * le_2;
	}

	for (; i < rows; i++)
	{
		le_0 = lineorder_table_info.table->lo_orderdate[i] <= limit_orderdate;

		quantity_sum += lineorder_table_info.table->lo_quantity[i];
		quantity_sum_with_condition += lineorder_table_info.table->lo_quantity[i] * le_0;

		rate_1_0 = lineorder_table_info.table->lo_extendedprice[i];


		rate_1_0 *= 1 - lineorder_table_info.table->lo_discount[i];

		discount_total_price = discount_total_price + rate_1_0;

		discount_total_price_with_condition += rate_1_0 * le_0;

		rate_1_0 *= 1 + lineorder_table_info.table->lo_tax[i];

		tax_discount_total_price += rate_1_0;

		tax_discount_total_price_with_condition += rate_1_0 * le_0;
	}
	/*--------------------------------*/

	stopTimer(&beg);

	//output
	printf("%d\n", quantity_sum);
	printf("%0.6lf\n", discount_total_price);
	printf("%0.6lf\n", tax_discount_total_price);
	printf("%d\n", quantity_sum_with_condition);
	printf("%0.6lf\n", discount_total_price_with_condition);
	printf("%0.6lf\n", tax_discount_total_price_with_condition);
	printf("running time is %ld\n", (long)(beg));
}
