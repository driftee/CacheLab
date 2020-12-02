// Li Guoxiang lgx 2018202135
#include "cachelab.h"
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

int Pow(int a, int b)
{ // a simple function returns a^b
    int result = 1;
    for (int i = 0; i < b; i++)
    {
        result *= a;
    }
    return result;
}

typedef unsigned long long Address;

typedef struct
{
    int s;
    int b;
    int E;
    int t;
    int hits;
    int misses;
    int evicts;
} cache_ctrl;

typedef struct
{ // a cacheline
    int time_stamp;
    int valid;
    int tag;
} cache_line;

typedef struct
{ // a cache set has many lines
    cache_line *lines;
} cache_set;

typedef struct
{ // a cache has many sets
    cache_set *sets;
} cache;

cache Init(int number_of_sets, int number_of_lines)
{ // this function initializes a cache and return it

    cache MyCache;
    cache_set MySet;
    cache_line MyLine;

    MyCache.sets = (cache_set *)malloc(sizeof(cache_set) * number_of_sets);
    for (int i = 0; i < number_of_sets; i++)
    {
        MySet.lines = (cache_line *)malloc(sizeof(cache_line) * number_of_lines);
        MyCache.sets[i] = MySet;
        for (int j = 0; j < number_of_lines; j++)
        {
            MyLine.time_stamp = 0;
            MyLine.valid = 0;
            MyLine.tag = 0;
            MySet.lines[j] = MyLine;
        }
    }

    return MyCache;
}

void HelpInfo()
{ // to be finished
    return;
}

void Address_Parser(cache_ctrl Panel, Address address, Address *tag, Address *set_index)
{ // this function parses an address into tag and index of set(offset in not needed here)
    Address temp = address << Panel.t;
    *tag = address >> (Panel.s + Panel.b);
    *set_index = temp >> (Panel.t + Panel.b);
}

void Cache_Read(cache MyCache, cache_ctrl *Panel, Address address)
{
    Address tag, set_index;
    Address_Parser(*Panel, address, &tag, &set_index);

    int line_empty = 0;
    int if_hit = 0;
    cache_set Set = MyCache.sets[set_index];

    int empty_line_index = 0;
    for (int i = 0; i < Panel->E; i++)
    { // searching in the according set's lines
        if (Set.lines[i].valid)
        { 
            if (Set.lines[i].tag == tag)
            { // hit !
                if_hit = 1;
                Panel->hits++;
                Set.lines[i].time_stamp++;
            }
        }
        else if (!Set.lines[i].valid && !line_empty)
        { // find an empty line an miss
            empty_line_index = i;
            line_empty = 1;
        }
    }
    if (if_hit)
    {
        return;
    }
    else
    { // miss !
        Panel->misses++;
    }
    int least_used = Set.lines[0].time_stamp;
    int most_used = Set.lines[0].time_stamp;
    int evict_line = 0;
    for (int i = 1; i < Panel->E; i++)
    {
        if (least_used > Set.lines[i].time_stamp)
        { // update the least used cache line's index
            least_used = Set.lines[i].time_stamp;
            evict_line = i;
        }
        if (most_used < Set.lines[i].time_stamp)
        {
            most_used = Set.lines[i].time_stamp;
        }
    }
    if (!line_empty)
    { // evict !
        Panel->evicts++;
        Set.lines[evict_line].tag = tag;
        Set.lines[evict_line].time_stamp = most_used + 1;
    }
    else
    {
        Set.lines[empty_line_index].tag = tag;
        Set.lines[empty_line_index].valid = 1;
        Set.lines[empty_line_index].time_stamp = most_used + 1;
    }
    return;
}

void Free_Cache(cache *MyCache, cache_ctrl Panel)
{ // free the memory allocated
    int number_of_sets = Pow(2, Panel.s);

    for (int i = 0; i < number_of_sets; i++)
    {
        cache_set set = MyCache->sets[i];
        if (set.lines != NULL)
        {
            free(set.lines);
        }
    }
    if (MyCache->sets != NULL)
    {
        free(MyCache->sets);
    }
}

int main(int argc, char **argv)
{
    cache MyCache;

    cache_ctrl Panel;
    Panel.evicts = 0;
    Panel.hits = 0;
    Panel.misses = 0;
    int number_of_sets;

    char c;

    char *filename;

    while ((c = getopt(argc, argv, "hvs:E:b:t:")) != -1)
    {
        switch (c)
        {
        case 'h':
            HelpInfo();
            exit(0);
        case 'v':
            exit(0); // to be finished
        case 's':
            Panel.s = atoi(optarg);
            break;
        case 'E':
            Panel.E = atoi(optarg);
            break;
        case 'b':
            Panel.b = atoi(optarg);
            break;
        case 't':
            filename = optarg;
            break;
        default:
            exit(1);
        }
    }
    
    Panel.t = 64 - Panel.s - Panel.b;
    number_of_sets = Pow(2, Panel.s);

    MyCache = Init(number_of_sets, Panel.E);

    char instruction;
    Address address;
    int size;

    FILE *file = fopen(filename, "r");
    if (file != NULL)
    {
        while (fscanf(file, " %c %llx,%d", &instruction, &address, &size) == 3)
        {
            switch (instruction)
            {
            case 'I':
                break;
            case 'L':
            case 'S':
                Cache_Read(MyCache, &Panel, address);
                break;
            case 'M':
                Cache_Read(MyCache, &Panel, address);
                Cache_Read(MyCache, &Panel, address);
                break;
            default:
                break;
            }
        }
    }
    printSummary(Panel.hits, Panel.misses, Panel.evicts);
    Free_Cache(&MyCache, Panel);
    return 0;
}
